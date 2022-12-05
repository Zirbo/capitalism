#! /bin/bash

version=$1

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

./capitalism_simulator
xmgrace -nxy results.dat
