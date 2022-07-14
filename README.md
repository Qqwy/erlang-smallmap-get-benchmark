# Benchmarking Erlang map element internals

This is related to [issue #6139](https://github.com/erlang/otp/issues/6139) on the Erlang/OTP repository.

It is an exploratory benchmark to compare different ways of iterating over a 'flatmap', which is a collection
of at most 32 keys (and 32 values).

The challenge is to look up the value for a key that matches a given input key.


# Building and running

Prerequisites: gcc, g++, clang, clang++, make.

This project uses the 'picobenchmark' microbenchmarking library, which is included as single-header in the repo.

After cloning the repository, run `make` to create four executables:
- clangO3
- clangO2
- gccO3
- gccO2

These reflect the two C compilers with related optimization options.

Running each of these executables will print a benchmark results table.

# About the microbenchmark

Take the results with a huge grain of salt! 
In an actual Erlang program there are thousands of other things going on,
and cache lines will be filled with other stuff.

This benchmark only tests what happens if a program does nothing else than look up many keys in tiny maps.

The particular benchmark will, for each map size between 1 and 32, run 1000 iterations in which a random key in _twice_ this range is picked.
This means there is a 50% chance of finding the key.

This procedure is repeated for each of the potential map retrieval function implementations.

By default, two implementations which I came up with which turn out to be quite a bit slower on my machine than the original implementation
are commented in the source code (in main.cpp), but feel free to experiment.
