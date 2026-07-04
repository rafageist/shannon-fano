# Shannon-Fano coding

This repository contains a modern, portable C++17 implementation of Shannon-Fano coding that can be compiled with GCC, Clang, MinGW, or MSVC.

The project started as a Borland C++ exercise written in 2007. That original implementation is intentionally not kept in the active source tree; Git history preserves it. The current tree focuses on a small, standard, cross-platform version suitable for learning and experimentation.

## Files

- `src/shannon_fano.cpp` — portable C++17 implementation.
- `CMakeLists.txt` — optional CMake build file.

## Build with GCC or MinGW

Linux/macOS:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/shannon_fano.cpp -o shannon_fano
./shannon_fano 0.4 0.3 0.2 0.1
```

Windows with MinGW:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/shannon_fano.cpp -o shannon_fano.exe
shannon_fano.exe 0.4 0.3 0.2 0.1
```

## Build with CMake

```bash
cmake -S . -B build
cmake --build build
```

Run it after building:

```bash
./build/shannon_fano 0.4 0.3 0.2 0.1
```

On Windows, the executable location depends on the generator. With Visual Studio it is commonly under `build\Debug\shannon_fano.exe` or `build\Release\shannon_fano.exe`.

## Usage

Pass probabilities or weights as command-line arguments:

```bash
./shannon_fano 0.4 0.3 0.2 0.1
```

The program also works interactively if no arguments are provided:

```bash
./shannon_fano
```

Input values are normalized internally, so both of these are equivalent:

```bash
./shannon_fano 0.4 0.3 0.2 0.1
./shannon_fano 4 3 2 1
```

Example output:

```text
Shannon-Fano coding
-------------------
Symbol     Probability        Code    Length
S1            0.400000           1         1
S2            0.300000          01         2
S3            0.200000         001         3
S4            0.100000         000         3

Entropy:              1.846439 bits/symbol
Expected code length: 1.900000 bits/symbol
```

## Algorithm note

Fano's Shannon-Fano method sorts symbols by probability, splits them into two groups whose total probabilities are as close as possible, assigns one bit to each group, and repeats the same operation until each group contains a single symbol.

This is useful for learning prefix coding and divide-and-conquer algorithms, but it is not always optimal. Huffman coding can produce shorter expected code lengths for some distributions.

## Modernization points

The modern version intentionally keeps the algorithm small and readable. The main changes from the original version are:

- Standard C++ headers instead of Borland-specific headers such as `iostream.h` and `conio.h`.
- No `clrscr`, `textcolor`, `cprintf`, or `getch`.
- `std::vector` and `std::string` instead of manual `new[]` arrays.
- Explicit pending intervals instead of indirect arrays of cuts and termination flags.
- Half-open intervals `[begin, end)` to avoid ambiguous inclusive boundary handling.
- Separate functions for input parsing, symbol preparation, splitting, encoding, and output.
