// iOS UIKit host for MobileDlopen.
//
// Displays every log line written via the printf shim in Main.cpp inside a
// full-screen scrollable UITextView, and kicks off the embedded-library demo
// (RunEmbeddedDemo_C) from a background queue once the UI is visible.

import UIKit

// RunEmbeddedDemo_C is defined in Main.cpp with C linkage.
@_silgen_name("RunEmbeddedDemo_C")
private func RunEmbeddedDemo_C()

// MARK: - Log sink

private var g_logView: UITextView?         // set once the view is loaded
private var g_pendingLog: NSMutableString? // buffers messages logged before
                                           // the view is available

@_cdecl("iOSLogAppend")
public func iOSLogAppend(_ msg: UnsafePointer<CChar>?) {
    guard let msg = msg, msg.pointee != 0 else { return }
    guard let str = String(validatingUTF8: msg) else { return }
    DispatchQueue.main.async {
        if let tv = g_logView {
            tv.text = (tv.text ?? "") + str
            let end = NSRange(location: (tv.text as NSString).length, length: 0)
            tv.scrollRangeToVisible(end)
        } else {
            if g_pendingLog == nil {
                g_pendingLog = NSMutableString()
            }
            g_pendingLog?.append(str)
        }
    }
}

// MARK: - View controller

final class LogViewController: UIViewController {
    override func viewDidLoad() {
        super.viewDidLoad()
        if #available(iOS 13.0, *) {
            view.backgroundColor = .systemBackground
        } else {
            view.backgroundColor = .white
        }

        let tv = UITextView(frame: view.bounds)
        tv.autoresizingMask = [.flexibleWidth, .flexibleHeight]
        tv.isEditable = false
        tv.font = UIFont(name: "Menlo", size: 12) ?? .systemFont(ofSize: 12)
        tv.text = (g_pendingLog as String?) ?? ""
        g_pendingLog = nil
        view.addSubview(tv)
        g_logView = tv

        // Run the demo off the main thread so the UI stays responsive.
        DispatchQueue.global(qos: .userInitiated).async {
            RunEmbeddedDemo_C()
        }
    }
}

// MARK: - App delegate

@main
final class AppDelegate: UIResponder, UIApplicationDelegate {
    var window: UIWindow?

    func application(_ application: UIApplication,
                     didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        let window = UIWindow(frame: UIScreen.main.bounds)
        window.rootViewController = LogViewController()
        window.makeKeyAndVisible()
        self.window = window
        return true
    }
}
