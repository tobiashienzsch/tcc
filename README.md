# tobante's crappy compiler

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
