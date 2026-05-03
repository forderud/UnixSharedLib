#pragma once

extern "C" // to avoid name mangling
__attribute__((visibility("default"))) // export function
int compute_sum (int a, int b);

extern "C" // to avoid name mangling
__attribute__((visibility("default"))) // export function
void test_internal_exception ();
