# tobante's (crappy) compiler collection

## Status

|                                                            License                                                             |                                                       Linux/macOS                                                       |                                                                   Windows                                                                    |                                                            Coverage                                                            |                                    Issues                                     |
| :----------------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------: | :------------------------------------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------: |
| [![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/tobiashienzsch/tcc/blob/master/LICENSE) | [![Build Status](https://travis-ci.org/tobiashienzsch/tcc.svg?branch=master)](https://travis-ci.org/tobiashienzsch/tcc) | [![Build status](https://ci.appveyor.com/api/projects/status/ggt3ifgoxamvfe53?svg=true)](https://ci.appveyor.com/project/tobiashienzsch/tcc) | [![codecov](https://codecov.io/gh/tobiashienzsch/tcc/branch/master/graph/badge.svg)](https://codecov.io/gh/tobiashienzsch/tcc) | ![GitHub issues](https://img.shields.io/github/issues/tobiashienzsch/tcc.svg) |

## Features

Everything is in development. See test files for details.

### Compiler

Source code is parsed using the `Boost Spirit.Qi` library. It creates an intermediate representation which generates the assembly.

### Virtual Machine

Current implementation is a stack only based virtual machine.

## Quick Start

### Dependencies

- C++17
- CMake 3.12
- Boost 1.69
  - Filesystem
  - Program Options
  - System
  - Spirit.Qi

### Clone

```sh
git clone --recursive https://github.com/tobiashienzsch/tcc.git
```

### Build

```sh
# unix
CONFIG=Debug make
# same as
CONFIG=Debug make config build test

# visual studio (you may need to edit the boost path in the makefile)
CONFIG=Debug make win
# same as
CONFIG=Debug make config-vs build test
```

### clang-tidy

```sh
cd $BUILD_DIR
python ../scripts/run-clang-tidy.py -header-filter="(tcc|tcvm|tcsl)" -quiet -fix -p . ..
```

## Project Structure

```sh
$PROJECT_ROOT
  3rd_party     # external code
  benchmark     # benchmarks using google bm
  cmake         # cmake modules
  docs          # documentation
  tcc           # compiler
  tcvm          # virtual machine
  test_data     # files for testing
  tcsl           # shared code
```

## Resources

- **Parsing**
  - [Boost Spirit X3](https://ciere.com/cppnow15/using_x3.pdf)
- **IR**
  - [Three-adrdress code](https://en.wikipedia.org/wiki/Three-address_code)
  - [SSA](https://www.cs.cmu.edu/~fp/courses/15411-f08/lectures/09-ssa.pdf)
- **YouTube**
  - [Compilation:](https://www.youtube.com/watch?v=0A079QpDAzY&list=PLTd6ceoshpreZuklA7RBMubSmhE0OHWh_)
    - [Compilation - Part Four: Intermediate Code](https://www.youtube.com/watch?v=xtouovp9kvQ)
  - [Terence Parr: Build a VM (stack machine)](https://www.youtube.com/watch?v=OjaAToVkoTw&list=LLss9ftQbmdAAgeOgJ0nSoWA&index=3&t=5344s)
  - [Bisqwit: Compiler series](https://www.youtube.com/watch?v=KwpcOYKfXZc&list=PLzLzYGEbdY5n9ITKUqOuRjXkRU5tMW2Sd)
    - [GitHub](https://github.com/bisqwit/compiler_series/tree/master/ep1)
  - [GopherCon 2017: Keith Randall - Generating Better Machine Code with SSA](https://www.youtube.com/watch?v=uTMvKVma5ms)
  - [GopherCon 2015: Ben Johnson - Static Code Analysis Using SSA](https://www.youtube.com/watch?v=D2-gaMvWfQY)
