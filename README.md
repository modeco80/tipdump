## TIPDump

A texture dumper for the Namco Museum PS1 games (more specifically, the `.tip` files). Written in C++20, as per usual.

Also provides a standalone library for reading .TIP files (td::tip/libtip).

## Usage

```
Usage: tipdump [Path to .TIP file] [OPTIONAL: start address of TIP in hexadecimal];
Start address option is useful for extracting from Vol. 3 .DAT files, for instance..
```

## Building

### Requirements

 - A C++20 compiler
 - CMake

To build:

```
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build
...
Profit?
```

## Known issues

- Vol. 1 `MUSEUM/LOB.TIP` and some stuff in `/DISPLAY` is wonky with libtip

  This is due to the CLUT order being very screwy in particularly those TIP file only, and the first image not even having a CLUT in the first place..
