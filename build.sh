#!/bin/bash
if [ ! -e $(command -v bash) ]; then
  echo "checking for bash... no"
  echo "build.sh (1) install bash!"
  exit 1
else
  echo "checking for bash... ok"
  continue
fi
if [ ! -e $(command -v gcc) ]; then
  echo "checking for gcc... no"
  if [ ! -e $(command -v clang) ]; then
      echo "checking for clang... no"
      echo "build.sh (2) install gcc or clang!"
      exit 1
  else
      echo "checking for clang... ok"
      export COMPILER=clang
      continue
  fi
else
  echo "checking for gcc... yes"
  export COMPILER=gcc
  continue
fi
echo "configuring CFLAGS..."
LOCAL_CFLAGS="-I./Include -I./Config -Os -Wall -pedantic -o"
TARGET="pkgmicro"
SOURCE="pkg.c"
read -p "If you want to make any target changes, edit Config/config.h, ready to build? y/N " tgtyesno
case $tgtyesno in
  [Yy])
    echo "Starting build..."
    $COMPILER $SOURCE $LOCAL_CFLAGS $TARGET
    echo "Complete!"
    exit 0
    ;;
  [Nn])
    echo "Please add any config changes and start build again."
    exit 0
    ;;
  *)
    echo "Input not in range."
    exit 1
    ;;
esac
