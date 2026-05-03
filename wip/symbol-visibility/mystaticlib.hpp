#pragma once
#include <string>
#include <stdexcept>


extern "C" // to avoid name mangling
int compute_sum_static (int a, int b);

extern "C" // to avoid name mangling
void throw_standard_exception (const char* message);

/** Custom exception type hierarchy. */
namespace dcm {
    struct exception : public std::runtime_error {
        exception (const std::string& msg) : std::runtime_error(msg.c_str()) { }
    };

    struct incompatible_data : public dcm::exception {
        incompatible_data (const std::string& msg) : exception(msg) {}
    };

    struct bad_type : public dcm::incompatible_data {
        bad_type(const std::string& msg) : incompatible_data(msg) {}
    };
}

extern "C" // to avoid name mangling
void throw_custom_exception (const char* message);
