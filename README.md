# Application Security - Assignment 1

[![Build Status](https://travis-ci.org/bgottlob/app-sec-assignment1.svg?branch=master)](https://travis-ci.org/bgottlob/app-sec-assignment1)

This assignment implements a simple spell checker in C.

## Source Tree

* `spell.c` - contains the implementation of the assignment prompt
* `spell_check.c` - contains a `main` that demonstrates the functionality of `spell.c`
* `dictionary.h` and `dictionary.c` - code provided by the assignment prompt
* `wordlist.txt` - output of `aspell` used as a dictionary
* `tests` - contains source code for unit tests
* `tests/samples` - contains sample corpus files that can be checked for misspellings.
* `afl-findings`, `afl-findings-after-fixes`, and `afl-findings-dictionary` - results of running the [AFL](https://lcamtuf.coredump.cx/afl/) fuzzer; for more details see [the report](gottlob_brandon_report1.md)

## Running the Spell Checker

Run `make spell_check` to create the `spell_check` executable.
Next, run `./spell_check <corpus_file> <dictionary_file>`

## Running Tests

Ensure you have installed the [Check](https://libcheck.github.io/check/) library.
It is available as the `check` package on many Linux distributions.

Run `make test` to create an executable that runs the unit tests.
Then, run `./test` to run the tests.

The tests must run from the root directory of this repository, since the file paths in test code are relative.

## Cleaning Up
Run `make clean` to delete all exectuables and object files
