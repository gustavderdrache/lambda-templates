lambda-templates
================

An abusive lambda calculus evaluator written entirely in C++ templates

USAGE
-----

1. Drop `#include "lambda-calculus.hh"` into a file.
2. Abuse templates.

NOTE: Requires C++11 features (variadic templates are used).

NOTES
-----

The `factorial.cc` file, included as an example, should not go above computing
the factorial of 4. Especially not with g++.
