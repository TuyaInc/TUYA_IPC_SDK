# Overview
TUYA_IPC_SDK is an integrated library&APIs, primarily for developing IP camera products.

* Stable: 4.5.14   suggested version with all basic features
* Lastest: 
    - 4.7.2: New supported feature: edge AI detection(MD/HD/FD/Pet etc.) realtime preview/two-way audio via both WebRTC and standard tuya media relay server.
	- 4.8.0: New supported feature: Leave Audio/Video message
	NOTE: By default, higher version SDK supports all features listed in lower ones.

# Feature highlights
* Register to Tuya Cloud platform
* Realtime preview via Tuya Smart APP
* Realtime preview via Amazon Echo products and Goolgle chromecast
* Relatime preview via Web
* Two-way audio
* Local storage(SD card e.g.) and playback
* Cloud storage and playback
* Multiple types of event notification
* AI detect(MD/HD/FD/Pet etc.) via Tuya Cloud
* PTZ control and preset cruise
* Image processing algorithm to enhance QR code recognition
* Image processing algorithm for motion detection and tracking

# How to

## Run a quick demo
1. edit Makefile: set COMPILE_PREX ?= to absolute path of your toolchain gcc.
```bash
$ vi Makefile
```
2. edit demo_tuya_ipc/user/user_main.c, input right IPC_APP_PID/IPC_APP_UUID/IPC_APP_AUTHKEY and then build an application
```bash
$ sh build_app.sh demo_tuya_ipc
```
3. put build result bin "tuya_ipc_demo_tuya_ipc" and "demo_tuya_ipc/resource" dir onto board/device /tmp,  and run as below (check detail in <Tuya_IPC_SDK_Development_Manual.doc>)
```bash
$ cd  /tmp
$ ./tuya_ipc_demo_tuya_ipc -m 2 -r "./" -t "A token you can get from TUYA APP"
```
## Full SDK manual and API description
[TUYA Docs center](https://docs.tuya.com/zh/iot/device-development/access-mode-product-solution/product-solution-ipc/sdk)

# Open Source List
* librtmp v2.3
* libuv v1.23.2
* Mbed TLS v2.9.0
* zlib v1.2.11

# Supported Platforms

## ubuntu x64 laptop
* linux-ubuntu-6.2.0_64Bit

## Hisilicon
* arm-hisiv100-linux-uclibc-4.4.1
* arm-hisiv300-linux-uclibc-4.8.3
* arm-hisiv500-linux-4.9.4
* arm-hisiv510-linux
* arm-hisiv600-linux
* arm-himix100-linux
* arm-himix200-linux

## Hisilicon LiteOS
* liteos-1.3.2
* liteos-1.4.5
* liteos-1.4.6
* liteos-3.2.3

## REALTEK
* mips-linux-uclibc-gcc-4.8.5_rts3903 for rts3903
* rsdk-6.4.1-linux for rts3906

## REALTEK rtl819x
* msdk-4.4.7

## MStar
* gcc-linaro-arm-linux-gnueabihf-4.8 for msc313e
* arm-buildroot-linux-uclibcgnueabihf-313e for msc313e/ssc323e
* arm-buildroot-linux-uclibcgnueabihf-msc316dc for msc316dc

## andriod 64bit Linux
* andriod-arm-aarch64-linux-gnu

## andriod 32bit Linux
* android_arm-linux-androideabi

## MTK
* mipsel-openwrt-linux-uclibc for MT7628
* mipsel-openwrt-linux-mt7688 for mt7688

## Multitek HC1772
* arm-multitek-linux-gnueabi

## NOVATEK NT9851X
* mipsel-24kec-linux-glibc-4.9
* mipsel-24kec-linux-uclibc-4.9

## xmsilicon
* arm-xm-linux for XM530AI
* arm-eabi-uclibc for xm510

## RockChip RK3399
* aarch64-buildroot-linux-gnu

## 安凯
* arm-none-linux-4.4.1 for 3918/3916 V100
* arm-anykav200-linux-uclibc-4.8.5 for 3918/3916 V200

## 君正
* mips-linux-4.7.2_32Bit
* mips-linux-4.7.2_64Bit
* mips-linux-uclibc-4.7.2_32Bit
* mips-linux-uclibc-4.7.2_64Bit

## 全志R16
* arm-openwrt-linux-muslgnueabi

## 新唐N32926
* arm-nuvoton-linux-uclibceabi

## 智源GM8136
* arm-unknown-linux-uclibc-4.4.0

## 国科GK7101/7102
* arm-goke-linux-uclibc-4.6.1
* arm-linux-gnueabihf-4.8.3

## 富瀚
* arm-fullhan-linux-uclibc-4.3.2 for FH8812
* arm-fullhan-linux-uclibcgnueabi for FH8632

## 睿致M5S
* arm-buildroot-linux-uclibcgnueabihf

## 松瀚 SN98660
* arm-unknown-linux-uclibcgnueabi

## 酷芯 AR9201
* gcc-linaro-5.4.1-2017.05-x86_64

## 数字动力 X1H
* gcc-linaro-5.3.1-2016.05-x86_64

## Others
* dh_alpsx_4.8.3
