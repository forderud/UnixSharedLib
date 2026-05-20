#pragma once
#include <string_view>

extern "C" // avoid C++ name mangling
std::string_view print_embedded_file (const char* section_name);
