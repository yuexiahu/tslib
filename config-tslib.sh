#!/bin/bash - 

export CC=/home/hzh/Develop/toolchain/imx6/gcc-linaro-4.9.4/bin/arm-linux-gnueabi-gcc
export CXX=/home/hzh/Develop/toolchain/imx6/gcc-linaro-4.9.4/bin/arm-linux-gnueabi-g++

./configure --prefix=/opt/libs-imx6 \
    --host=arm-linux \
    ac_cv_func_malloc_0_nonnull=yes

