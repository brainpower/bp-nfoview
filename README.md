[![CI](https://github.com/brainpower/bp-nfoview/actions/workflows/main.yml/badge.svg)](https://github.com/brainpower/bp-nfoview/actions/workflows/main.yml)

# bp-nfoview

A Simple NFO-Viewer

## Installation

### Arch Linux

There is a package in the AUR for this: [bp-nfoview](https://aur.archlinux.org/packages/bp-nfoview).
You can use your favourite AUR-Helper to install it. For example:

    pacaur -S bp-nfoview

You can also use the PKGBUILD in arch/ to create a package from git master.

### Others

... will have to compile from source.

#### Using dpkg on debian based systems

However, for dpkg based distributions you can use
the included files in the debian/ folder to create
a .deb package.
First update the version in the changelog using your favorite editor:

    vim debian/changelog

Then run:

    dpkg-buildpackage


#### Using CMake

So, to compile this app, make sure you installed the Qt SDK,
then just run

    mkdir build
    cd build
    cmake ..
    make

To install it then, run (may need root permissions)

    make install

#### Using meson

Building against Qt4 is not supported with my meson.build, use CMake if you need that.

To build, run:

    meson builddir
    cd builddir
    ninja
    ninja install

You may need root permissions for the install step.



Also, feel free to contact me
if you found bugs or you want to make a feature suggestion, I'll try my best then ;)
