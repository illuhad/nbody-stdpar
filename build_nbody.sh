#!/usr/bin/env bash

ORIGIN_DIR=`pwd`
if [ -d "build" ]; then 
  rm -rf "build"
fi
mkdir -p "install"
mkdir "build" ; cd "build"
cmake ..\
    -DCMAKE_INSTALL_PREFIX:FILEPATH=${ORIGIN_DIR}/install \
    -DCMAKE_BUILD_TYPE:STRING=Release \
    -DENABLE_NVIDIA_GPU:BOOL=ON
make VERBOSE=1 install
cd ${ORIGIN_DIR}
