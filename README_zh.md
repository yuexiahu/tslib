# 中文使用说明

1. 安装工具

   ```bash
   sudo apt-get install autoconf autogen libtool
   ```

2. 执行`autogen.sh`

   ```bash
   ./autogen.sh
   ```

3. 执行`configure`

   ```bash
   export CC=/home/hzh/Develop/toolchain/imx6/gcc-linaro-4.9.4/bin/arm-linux-gnueabi-gcc
   ./configure --prefix=/opt/libs-imx6/tslib \
       --host=arm-linux \
       ac_cv_func_malloc_0_nonnull=yes
   ```

4. 编译安装

   ```bash
   make && make install
   ```

5. 复制到arm板子中，设置环境变量

   ```bash
   #!/bin/sh
   
   TSLIB_ROOT=/opt/tslib
   
   export TSLIB_TSDEVICE=/dev/ctp
   export TSLIB_CALIBFILE=$TSLIB_ROOT/etc/pointercal
   export TSLIB_CONFFILE=$TSLIB_ROOT/etc/ts.conf
   export TSLIB_PLUGINDIR=$TSLIB_ROOT/lib/ts
   export TSLIB_FBDEVICE=/dev/fb0
   export TSLIB_TSLIB_CONSOLEDEVICE=/dev/tty
   
   # enable tslib
   export QT_QPA_EGLFS_TSLIB=1
   # set input device
   export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=/dev/ctp
   ```

6. 配置插件，修改`$TSLIB_ROOT/etc/ts.conf`

   ```bash
   #module_raw input 修改原始数据读取插件为ctp
   module_raw ctp
   ...
   #其他配置按需设置
   ```

7. 其他设置请见[README](README.md)

