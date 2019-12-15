# tobante's crappy compiler

## Status

|                                                            License                                                             |                                                       Linux/macOS                                                       |                                                            Coverage                                                            |                                    Issues                                     |
| :----------------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------: |
| [![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/tobiashienzsch/tcc/blob/master/LICENSE) | [![Build Status](https://travis-ci.org/tobiashienzsch/tcc.svg?branch=master)](https://travis-ci.org/tobiashienzsch/tcc) | [![codecov](https://codecov.io/gh/tobiashienzsch/tcc/branch/master/graph/badge.svg)](https://codecov.io/gh/tobiashienzsch/tcc) | ![GitHub issues](https://img.shields.io/github/issues/tobiashienzsch/tcc.svg) |

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

## Resources

- **YouTube**
  - [Compilation:](https://www.youtube.com/watch?v=0A079QpDAzY&list=PLTd6ceoshpreZuklA7RBMubSmhE0OHWh_)
    - [Compilation - Part Four: Intermediate Code](https://www.youtube.com/watch?v=xtouovp9kvQ)
  - [Terence Parr: Build a VM (stack machine)](https://www.youtube.com/watch?v=OjaAToVkoTw&list=LLss9ftQbmdAAgeOgJ0nSoWA&index=3&t=5344s)
  - [Bisqwit: Compiler series](https://www.youtube.com/watch?v=KwpcOYKfXZc&list=PLzLzYGEbdY5n9ITKUqOuRjXkRU5tMW2Sd)
    - [GitHub](https://github.com/bisqwit/compiler_series/tree/master/ep1)
  - [GopherCon 2017: Keith Randall - Generating Better Machine Code with SSA](https://www.youtube.com/watch?v=uTMvKVma5ms)
  - [GopherCon 2015: Ben Johnson - Static Code Analysis Using SSA](https://www.youtube.com/watch?v=D2-gaMvWfQY)
