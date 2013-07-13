abstract_factory
================

A c++ abstract factory implementation.
This implementation based on templates so it contains only a header to include.

How to build the unit tests:

Requirements:
c++ compiler with c++11 support (tested on Mac OS X with Apple LLVM version 4.2)
cmake >= 2.8
make tool(GNU Make, Ninja, ...)
boost(log, unit_test_framework)

create a build directory and change to it
> mdkir build && cd build
> cmake 'path to abstract_factory root dir' -G "[Unix Makefiles|Ninja|..."
> make|ninja|...
> make test | ninja test | ... test # executes the unit tests
