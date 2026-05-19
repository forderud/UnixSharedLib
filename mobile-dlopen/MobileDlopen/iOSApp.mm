// iOS UIKit host for MobileDlopen.
//
// Displays every log line written via the printf shim in Main.cpp inside a
// full-screen scrollable UITextView, and kicks off the embedded-library demo
// (RunEmbeddedDemo_C) from a background queue once the UI is visible.

#import <UIKit/UIKit.h>

extern "C" void RunEmbeddedDemo_C(void); // defined in Main.cpp

// ---------------------------------------------------------------------------
// Log sink
// ---------------------------------------------------------------------------

static UITextView*      g_logView    = nil;   // set once the view is loaded
static NSMutableString* g_pendingLog = nil;   // buffers messages logged before
                                              // the view is available

extern "C" void iOSLogAppend(const char* msg) {
    if (!msg || !*msg)
        return;
    NSString* str = [[NSString alloc] initWithUTF8String:msg];
    if (!str)
        return;
    dispatch_async(dispatch_get_main_queue(), ^{
        if (g_logView) {
            g_logView.text = [g_logView.text stringByAppendingString:str];
            NSRange end = NSMakeRange(g_logView.text.length, 0);
            [g_logView scrollRangeToVisible:end];
        } else {
            if (!g_pendingLog)
                g_pendingLog = [NSMutableString string];
            [g_pendingLog appendString:str];
        }
    });
}

// ---------------------------------------------------------------------------
// View controller
// ---------------------------------------------------------------------------

@interface LogViewController : UIViewController
@end

@implementation LogViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    if (@available(iOS 13.0, *)) {
        self.view.backgroundColor = UIColor.systemBackgroundColor;
    } else {
        self.view.backgroundColor = UIColor.whiteColor;
    }

    UITextView* tv = [[UITextView alloc] initWithFrame:self.view.bounds];
    tv.autoresizingMask = UIViewAutoresizingFlexibleWidth |
                          UIViewAutoresizingFlexibleHeight;
    tv.editable     = NO;
    tv.font         = [UIFont fontWithName:@"Menlo" size:12]
                   ?: [UIFont systemFontOfSize:12];
    tv.text         = g_pendingLog ?: @"";
    g_pendingLog    = nil;
    [self.view addSubview:tv];
    g_logView = tv;

    // Run the demo off the main thread so the UI stays responsive.
    dispatch_async(
        dispatch_get_global_queue(QOS_CLASS_USER_INITIATED, 0), ^{
            RunEmbeddedDemo_C();
        });
}
@end

// ---------------------------------------------------------------------------
// App delegate
// ---------------------------------------------------------------------------

@interface AppDelegate : UIResponder <UIApplicationDelegate>
@property (nonatomic, strong) UIWindow* window;
@end

@implementation AppDelegate
- (BOOL)application:(UIApplication*)application
    didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {
    self.window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    self.window.rootViewController = [LogViewController new];
    [self.window makeKeyAndVisible];
    return YES;
}
@end

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil,
                                 NSStringFromClass([AppDelegate class]));
    }
}
