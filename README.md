[![CI](https://github.com/brainpower/bp-nfoview/actions/workflows/main.yml/badge.svg)](https://github.com/brainpower/bp-nfoview/actions/workflows/main.yml)

# bp-nfoview

A Simple NFO-Viewer

## Installation

### Arch Linux

There is a package in the AUR for this: [bp-nfoview](https://aur.archlinux.org/packages/bp-nfoview).
You can use your favourite AUR-Helper to install it. For example:

```bash
pacaur -S bp-nfoview
```

You can also use the PKGBUILD in arch/ to create a package from git master.

### CPack

You can use CPack to generate a package for your systems package manager,
if it uses either a dpkg or rpm based package manager.

#### Debian based (Debian, Ubuntu, Mint, ...)

```bash
mkdir build
cmake -GNinja -B build . -DCPACK_BINARY_DEB=ON
ninja -C build package
```

#### RPM based (openSUSE, Fedora, ...)

```bash
mkdir build
cmake -GNinja -B build . -DCPACK_BINARY_RPM=ON
ninja -C build package
```

### Others

... will have to compile "manually".

#### Using CMake

So, to compile this app, make sure you installed the Qt SDK,
then just run for example:

```bash
mkdir build
cmake -GNinja -B build .
ninja -C build
```

To install it then, run (may need root permissions)

```bash
ninja -C build install
```

#### Using meson

meson build support is considered experimental, CMake is the fully supported build solution.
Expect trouble, expecially when cross compiling.
If you have trouble with meson, try cmake.

To build and install, run for example:

```bash
meson build
ninja -C build
ninja -C build install
```

You may need root permissions for the install step.
