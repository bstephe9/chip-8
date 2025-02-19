# CHIP-8 Interpreter

## How to Run

### Local Build

Build the standalone CHIP-8 interpreter and pass in a `.ch8` rom:

```bash
make
./chip8 <path/to/rom.ch8>
```

### Local Build for Web

Build the CHIP-8 interpreter with `emcc` and run a local server with `emrun`:

```bash
make web
emrun .
```

## Controls

### CHIP-8 Keypad

#### Original Keypad

| 1 | 2 | 3 | C |
| - | - | - | - |
| __4__ | __5__ | __6__ | __D__ |
| __7__ | __8__ | __9__ | __E__ |
| __A__ | __0__ | __B__ | __F__ |

#### Keyboard

| 1 | 2 | 3 | 4 |
| - | - | - | - |
| __q__ | __w__ | __e__ | __r__ |
| __a__ | __s__ | __d__ | __f__ |
| __z__ | __x__ | __c__ | __v__ |

## Makefile Commands

### Build with gcc

```bash
make
```

### Web build with emcc

Generates the .js and .wasm glue code needed to run the program locally.

**Note**: Requires [emcc from Emscripten toolchain](https://github.com/emscripten-core/emscripten).

```bash
make web
```

### Build and Run Tests

Tests are run using the [Unity Testing Framework](https://github.com/ThrowTheSwitch/Unity).

```bash
make tests
```

### Remove Build Output Files

```bash
make clean
```
