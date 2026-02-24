# petalinux-configs

1. Configs &amp; Syntax, MemoryMaps for PetaLinux.

2. Also have ***Docker container composer*** for Petalinux IDE.

- Can be used in the linux distro which is not supporting APT.

3. Included python program that Auto Generating ***"platform-top.h"***.

- ./gen/gen.py

* * *

## 1. Building enviroments

<details><summary>1.1. First initialize building enviroment</summary>

### for APT (Ubuntu 18.04)

``` bash
sudo apt install libtinfo5 libncurses5
sudo ln -s /usr/bin/make /usr/bin/gmake
```

``` bash
sudo dpkg --add-architecture i386 && sudo apt-get update
sudo apt install tofrodos gawk xvfb git libncurses5-dev tftpd zlib1g-dev zlib1g-dev:i386 libssl-dev flex bison chrpath socat autoconf libtool texinfo gcc-multilib libsdl1.2-dev libglib2.0-dev screen pax xterm bc build-essential dnsutils

./petalinux-<petalinux-version>-installer.run -d ./petalinux-build/
source ./petalinux-build/settings.sh

petalinux-create --type project --template <template, ex)zynqMP> --name <user-project-name>
```

### for PACMAN (Arch distro)

``` bash
sudo pacman -S --needed cpio inetutils libxcrypt-compat xorg-xlsclients
yay -S --needed libpng12 ncurses5-compat-libs fxload digilent.adept.runtime digilent.adept.utilities
sudo ln -s /usr/bin/make /usr/bin/gmake
```

``` bash
# For Vitis execution
mv ${VITIS_PATH}/lib/lnx64.o/Default/libstdc++.so.6 ${VITIS_PATH}/lib/lnx64.o/Default/libstdc++.so.6.old
ln -s /usr/lib/libstdc++.so.6 ${VITIS_PATH}/lib/lnx64.o/Default/libstdc++.so.6
ln -sf ${VITIS_PATH}/tps/lnx64/cmake-3.24.2/libs/Rhel/9/* /usr/lib
```

* * *

</details>

<details><summary>1.2. Initialize building enviroment</summary>

``` bash
source ./petalinux-build/settings.sh
cd <user-project-root>
```

* * *

</details>

## 2. Building orders

<details><summary>2.1. First build order</summary>

### HW Menuconfig

``` bash
petalinux-config --get-hw-description <user-fpga-directory>
```

1. Subsystem AUTO Hardware Settings → Flash Settings

<div align="middle">

![4](https://github.com/aruyu/petalinux-configs/blob/master/ref/memorymap/1.png)
</div>

2. u-boot Configuration → u-boot script configuration → QSPI/OSPI image offsets

<div align="middle">

![4](https://github.com/aruyu/petalinux-configs/blob/master/ref/memorymap/2.png)
</div>

* ext 1-1. Image Packaging Configuration → (INITRD or JFFS2, if=JFFS2 then=set erase_block)

* ext 1-2. Image Packaging Configuration → INITRAMFS/INITRD Image name
  (petalinux-image-minimal)
  (petalinux-initramfs-image)

* ext 2. Firmware Version Configuration

* ext 3-1. Yocto Settings → Add pre-mirror url (/downloads)

* ext 3-2. Yocto Settings → Local sstate feeds settings (/aarch64)
  (file://.*/.*)

### U-Boot Menuconfig

``` bash
petalinux-config -c u-boot
```

1. ARM architecture: Boot script offset

<div align="middle">

![4](https://github.com/aruyu/petalinux-configs/blob/master/ref/memorymap/3.png)
</div>

2. Environment: Environment...

3. Environment: Environment offset

<div align="middle">

![4](https://github.com/aruyu/petalinux-configs/blob/master/ref/memorymap/4.png)
</div>

* ext. Command line interface: Shell prompt

### Kernel Menuconfig

``` bash
petalinux-config -c kernel
```

1. File systems → Miscellaneous filesystems → [ ] JFFS2 summary support (disable)

2. File systems → Miscellaneous filesystems → [ ] JFFS2 XATTR support (disable)

### Rootfs Menuconfig

``` bash
petalinux-config -c rootfs
```

``` bash
cat >> ./project-spec/meta-user/conf/petalinuxbsp.conf <<-EOF
	INIT_MANAGER_DEFAULT:forcevariable = "sysvinit"
EOF

cat >> ./project-spec/meta-user/conf/user-rootfsconfig <<-EOF
	CONFIG_devmem2
	CONFIG_libubootenv
	CONFIG_libubootenv-bin
	CONFIG_updatetools
EOF
```

1. Image Features -> Init-manager (sysvinit or systemd)
2. user packages -> (libubootenv, libubootenv-bin)
* ext. PetaLinux RootFS Settings (enable busybox)

### Busybox Menuconfig

``` bash
petalinux-config -c busybox
```

* ext. PetaLinux RootFS busybox Settings (flashcp)

### Build

``` bash
#if need
vi ./project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi
vi ./project-spec/meta-user/recipes-bsp/u-boot/files/platform-top.h
```

``` bash
petalinux-build
```

* * *

</details>

<details><summary>2.2. Build order</summary>

``` bash
petalinux-config --get-hw-description <user-fpga-directory>
petalinux-build

petalinux-config -c <component>
petalinux-build -c <component>
```

* * *

</details>

<details><summary>2.3. Customize rootfs</summary>

### ramdisk

``` bash
dd bs=64 skip=1 if=rootfs.cpio.gz.u-boot of=rootfs.cpio.gz
gunzip rootfs.cpio.gz

mkdir rootfs && cd rootfs

cpio -i -F ../rootfs.cpio
```

<CUSTOM ROOTFS>

``` bash
sudo su <<-EOF
	chown -R root:root *
	chown -R user:user home/kvim
	find . | cpio -o -H newc | gzip -9 > ../rootfs_new.cpio.gz
EOF
cd ../

mkimage -A arm -T ramdisk -C gzip -d rootfs_new.cpio.gz rootfs_new.cpio.gz.u-boot
```

### JFFS2


* * *

</details>

<details><summary>2.4. Building u-boot env</summary>

### Use mkenvimage

``` bash
vi env.txt
```

> The input file is in format:
>
> key1=value1
>
> key2=value2
>
> ...
>
> Empty lines are skipped, and lines with a # in the first
>
> column are treated as comments (also skipped).

``` bash
mkenvimage -s <env-size> -o <env-name> env.txt
```

### Use U-Boot tftpput

``` bash
saveenv
sf probe; sf read 0x100000 <env-offset> <env-size>; tftpput 0x100000 <env-size> ${serverip}:<env-name>
```

* * *

</details>

<details><summary>2.5. Packaging</summary>

### U-Boot only (No fpga, env)

``` bash
petalinux-package --boot --force --format BIN --u-boot -o uboot.bin
```

### U-Boot only (No env)

``` bash
petalinux-package --boot --force --format BIN --fpga --u-boot -o uboot.bin
```

### U-Boot only (With env)

``` bash
petalinux-package --boot --force --format BIN --fpga --u-boot --add uboot.env --offset <env-offset> -o uboot.bin
```

### Booting using Fit image

``` bash
petalinux-package --boot --force --format BIN --fpga --u-boot --kernel image.ub --offset <kernel-offset> --boot-script --offset <bootsrc-offset>
```

### Booting using Separate images (NOT WORKING)

``` bash
# Have to use u-boot tftpboot
petalinux-package --boot --force --format BIN --fpga --u-boot --kernel Image.gz --offset <kernel-offset> --boot-script --offset <bootsrc-offset> --add rootfs.cpio.gz.u-boot --offset <rootfs-offset>
```

* * *

</details>

## 3. Building kernel driver

<details><summary>3.1. Initialize building kernel driver enviroment</summary>

``` bash
cd <user-project-root>
petalinux-create --type modules --enable --name <user-module-name>

cat >> ./project-spec/meta-user/conf/petalinuxbsp.conf <<-EOF
	RM_WORK_EXCLUDE += "<user-module-name>"
EOF

cd ./project-spec/meta-user/recipes-modules/<user-module-name>
vi ./files/<user-module-name>.c
```

* * *

</details>

<details><summary>3.2. Build kernel driver (module)</summary>

### Command List

``` bash
petalinux-build -c <user-module-name> -x listtasks
```

### Build

``` bash
petalinux-build -c <user-module-name>
```

### Rebuild

``` bash
petalinux-build -c <user-module-name> -x do_clean
petalinux-build -c <user-module-name>
```

### Install

``` bash
petalinux-build -c <user-module-name> -x do_install
```

### Output Directory

``` bash
<user-project-root>/build/tmp/work/<machine-name>-xilinx-linux/<user-module-name>/1.0-r0/sysroot-destdir/lib/modules/<petalinux-version>/extra/<user-module-name>.ko
```

* * *

