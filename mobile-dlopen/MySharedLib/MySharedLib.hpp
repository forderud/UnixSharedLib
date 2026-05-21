#pragma once
#include <string_view>

extern "C" // avoid C++ name mangling
std::string_view LibraryFunction (const char* section_name);
