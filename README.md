# SystemTheme++

This is a C++ library that retrieves system theme information from both Window, Linux and in the future also Mac OS.

## Setup

If examples are enabled submodules should be initialized as well:

```bash
git submodules update --init --recursive
```

### Windows

On Windows C++WinRT is needed. It can be installed using [vcpkg](https://vcpkg.io):

```PowerShell
vcpkg install cppwinrt
```

### Configuring and building

To configure the project run:

```bash
cmake -B build
```

For static linking set `-DSHARED_LIBRARY=OFF`. If examples should not be build set `-DBUILD_EXAMPLES=OFF`.

Build with:

```bash
cmake --build build [-j $(nproc)]
```

## Install

Currently no install target is made. This however is coming.