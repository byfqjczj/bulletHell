# tbh

## Building

### Prerequisites
You will need CMake (>= 3.16), Ninja, and a C++ compiler (tested on GCC 12.2.1), but Clang will also work.

Configure the build directory:
```
cmake -Bbuild -GNinja
```

Build and run the project:
```
ninja -Cbuild
./build/src/tbh
```
