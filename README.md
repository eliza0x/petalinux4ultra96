## 1. Requirements

- Ultra96-V2 - PetaLinux 2019.2 BSP : <http://zedboard.org/support/design/28476/181>
- PetaLinux 2019.2 tool Installer : <https://japan.xilinx.com/support/download/index.html/content/xilinx/ja/downloadNav/embedded-design-tools/2019-2.html> 

## 2. Docker Login

```
$ docker build -t ultra96 .
$ docker volume create peta2019.2
$ docker run -it -v peta2019.2:/build ultra96 /bin/bash
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

## 4. Build Ultra96 Linux Kernel

```
$ source Xilinx/peta2019.2/settings.sh
$ unzip ultra96v2_oob_2019_2.zip
$ petalinux-create -t project -s ultra96v2_oob_2019_2.bsp
$ cd ultra96v2_oob_2019_2
$ petalinux-build 
```
### UNSOLVED: recipes-openamp build failed

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

- ~`/build/ultra96v2_oob_2019_2/project-spec/meta-plnx-generated/recipes-core/images/petalinux-user-image.bb`~
- ~`Xilinx/peta2019.2/components/yocto/source/aarch64/layers/meta-xilinx-tools/recipes-openamp/examples/openamp-fw-mat-muld_git.bb`~
