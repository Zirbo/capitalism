#! /bin/bash

version=$1
latest_version=v2

if [ "$1" = "" ]; then
  echo "assuming latest version"
  version="$latest_version"
fi

case $version in
v1|v2)
  ;;
*)
  echo "invalid version"
  exit 1
  ;;
esac

mkdir -p bld
pushd bld
  pwd
  cmake ../src/$version/
  make -j8
  cp capitalism_simulator ..
popd
