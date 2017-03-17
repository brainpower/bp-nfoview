[![Build Status](https://travis-ci.org/brainpower/bp-nfoview.svg?branch=master)](https://travis-ci.org/brainpower/bp-nfoview)

# bp-nfoview

A Simple NFO-Viewer by brainpower

## Installation

### Arch Linux

There is a package in the AUR for this: [bp-nfoview](https://aur.archlinux.org/packages/bp-nfoview).
You can use your favourite AUR-Helper to install it. For example:

    pacaur -S bp-nfoview

You can also use the PKGBUILD in arch/ to create a package from git master.

### Gentoo

You may use the ebuild at [1] as an example to create a `bp-nfoview-2.0.ebuild`
which can be used for installing bp-nfoview by creating an local overlay.<br>
See [2] on how to do that.

After that you can use emerge to install:

    emerge --sync
    emerge --ask --oneshot app-text/bp-nfoview


[1] https://github.com/fhede/Gentoo-misc/tree/master/ebuilds/bp-nfoview-qt5 <br>
[2] https://wiki.gentoo.org/wiki/Custom_repository

### Others

... will have to compile from source.

However, for dpkg based distributions you can use the included files in the debian/ folder to create a .deb package by running:

    dpkg-buildpackage


So, to compile this app, make sure you installed the Qt SDK,
then just run

    cd bp-nfoview
    mkdir build
    cd build
    cmake ..
    make

To install it then, run (may need root permissions)

    make install

Also, feel free to contact me
if you found bugs or you want to make a feature suggestion, I'll try my best then ;)
