## 1. Requirements

- Ultra96-V2 - PetaLinux 2019.2 BSP : <http://zedboard.org/support/design/28476/181>
- PetaLinux 2019.2 tool Installer : <https://japan.xilinx.com/support/download/index.html/content/xilinx/ja/downloadNav/embedded-design-tools/2019-2.html> 

## 2. Docker Login

```
$ docker build -t ultra96 .
$ docker volume create peta2019.2
$ docker run -it -v peta2019.2:/build --device=/dev/<SDCARD PARTITION>:/dev/sdcard  ultra96 /bin/bash
```

## 3. Install Petalinux

```
$ sudo mv ultra96v2_oob_2019_2.zip /build
$ sudo chmod -R 777 /build
$ mkdir -p /build/Xilinx/peta2019.2
$ cd /build
$ ../petalinux-v2019.2-final-installer.run /build/Xilinx/peta2019.2/

( License agreement )
```

### Fix out-of-date library

read the text. <https://www.hackster.io/whitney-knitter/petalinux-2019-2-on-ultra96-using-bsp-recipe-patch-6e0920>

### Disable recipes-openamp

```
ERROR: Task (/build/Xilinx/peta2019.2/components/yocto/source/aarch64/layers/meta-xilinx-tools/recipes-openamp/examples/openamp-fw-rpc-demo_git.bb:do_configure) failed with exit code '1'

...

Summary: 3 tasks failed:
  /build/Xilinx/peta2019.2/components/yocto/source/aarch64/layers/meta-xilinx-tools/recipes-openamp/examples/openamp-fw-rpc-demo_git.bb:do_configure
  /build/Xilinx/peta2019.2/components/yocto/source/aarch64/layers/meta-xilinx-tools/recipes-openamp/examples/openamp-fw-mat-muld_git.bb:do_configure
  /build/Xilinx/peta2019.2/components/yocto/source/aarch64/layers/meta-xilinx-tools/recipes-openamp/examples/openamp-fw-echo-testd_git.bb:do_configure
Summary: There were 3 ERROR messages shown, returning a non-zero exit code.
ERROR: Failed to build project
```

```
$ petalinux-config -c rootfs

Filesystem Packages --->
  misc --->
    openamp-fw-echo-testd --->
      [ ] openamp-fw-echo-testd
    openamp-fw-mat-muld --->
      [ ] openamp-fw-mat-muld
    openamp-fw-rpc-demo --->
      [ ] openamp-fw-rpc-demo
```

### Able to generate ext4 rootfs

```
$ petalinux-config

```

## 4. Build Ultra96 Linux Kernel

```
$ source Xilinx/peta2019.2/settings.sh
$ unzip ultra96v2_oob_2019_2.zip
$ petalinux-create -t project -s ultra96v2_oob_2019_2.bsp
$ cd ultra96v2_oob_2019_2
$ petalinux-build 
```

## 5. Package Linux

```
$ petalinux-package --boot --fsbl ./images/linux/zynqmp_fsbl.elf --fpga ./images/linux/system.bit --pmufw ./images/linux/pmufw.elf --u-boot --force
```

## 6. Write

1. Create 1G vfat and 4G ext4 partitions. (I used cgdisk and mkfs)
2. 

```
$ docker cp 0aa21593402b:/build/ultra96v2_oob_2019_2/images/linux/rootfs.ext4 .                  
$ docker cp 0aa21593402b:/build/ultra96v2_oob_2019_2/images/linux/BOOT.BIN .
$ docker cp 0aa21593402b:/build/ultra96v2_oob_2019_2/images/linux/image.ub .
$ sudo mount /dev/sdc1 /mnt
$ sudo cp BOOT.BIN image.ub /mnt
$ sudo dd if=rootfs.ext4 of=/dev/sdc2 bs=1M status=progress 
```

