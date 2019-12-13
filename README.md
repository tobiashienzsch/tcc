# tobante's crappy compiler

## Status

|                                                License                                                |                                                       Linux/macOS                                                       |                                    Issues                                     |
| :---------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------: |
| [![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/) | [![Build Status](https://travis-ci.org/tobiashienzsch/tcc.svg?branch=master)](https://travis-ci.org/tobiashienzsch/tcc) | ![GitHub issues](https://img.shields.io/github/issues/tobiashienzsch/tcc.svg) |

## Features

Everything is in development. See test files for details.

- Source code lexer
- AST to bytecode
- Bytecode virtual stack machine

## Quick Start

### Dependencies

- C++17
- CMake 3.12

### Clone

```sh
git clone --recursive https://github.com/tobiashienzsch/tcc.git
```

### Build

```sh
CONFIG=Debug make
# same as
CONFIG=Debug make config build test
# visual studio (git-bash)
CONFIG=Debug CM_GENERATOR="Visual Studio 16 2019" make
```
