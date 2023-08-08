#!/bin/bash

# SPDX-FileCopyrightText: 2023 brainpower <brainpower@mailbox.org>
#
# SPDX-License-Identifier: MIT

QTV=${QT_MAJOR_VERSION:-6}

# basic preparations
pacman -Sy && \
pacman -S archlinux-keyring --noconfirm --needed && \
pacman -Su --noconfirm && \
pacman-key --init && \
pacman-key --populate

# some basic utils
pacman -S --noconfirm --needed make ninja git cmake binutils patch base-devel unzip python rsync zip

# mingw builds
echo "[ownstuff]" >> /etc/pacman.conf && \
echo "Server = https://ftp.f3l.de/~martchus/\$repo/os/\$arch " >> /etc/pacman.conf && \
echo "Server = https://martchus.no-ip.biz/repo/arch/\$repo/os/\$arch " >> /etc/pacman.conf && \
pacman-key --recv-keys B9E36A7275FC61B464B67907E06FE8F53CDC6A4C && \
pacman-key --finger    B9E36A7275FC61B464B67907E06FE8F53CDC6A4C && \
pacman-key --lsign-key B9E36A7275FC61B464B67907E06FE8F53CDC6A4C && \
pacman -Sy

# Install core MingW packages
pacman -S --noconfirm \
  mingw-w64-binutils \
  mingw-w64-crt \
  mingw-w64-gcc \
  mingw-w64-configure \
  mingw-w64-headers \
  mingw-w64-winpthreads \
  mingw-w64-cmake \
  mingw-w64-cmake-static \
  mingw-w64-extra-cmake-modules \
  mingw-w64-fontconfig \
  mingw-w64-freeglut \
  mingw-w64-freetype2 \
  mingw-w64-gettext \
  nsis \
  mingw-w64-pkg-config \
  mingw-w64-tools \
  mingw-w64-vulkan-headers

# Install MingW Qt5 packages
pacman -S --noconfirm \
  qt"${QTV}"-base \
  mingw-w64-qt"${QTV}"-base-static \
  mingw-w64-qt"${QTV}"-imageformats-static \
  mingw-w64-qt"${QTV}"-svg-static \
  mingw-w64-qt"${QTV}"-tools-static

if [[ "$QTV" == "6" ]]; then
  pacman -S --noconfirm mingw-w64-qt"${QTV}"-5compat-static
fi
