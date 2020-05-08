# Overview
TUYA_IPC_SDK is an integrated library&APIs, primarily for developing IP camera products.

* DemoResource:   resources for sdk quick demo: h264 video/pcm audio/jpeg snapshot etc.
* Stable: 4.7.8   suggested version.
* Lastest: 4.8.1  version with new supported feature, tested and varified only on certain hardware platform.
	- Audio/video message: leave audio/video message to message center
	- AP mode streaming: when IPC is working on AP mode without WAN, TUYA APP is able to preview via LAN. 
	- Direct QR scanning pairing: for IPC with 4G network, use TUYA APP to scan the QR code on IPC for pairing.

# Feature highlights
* Register to Tuya Cloud platform
* Realtime preview via Tuya Smart APP
* Realtime preview via Amazon Echo products and Goolgle chromecast
* Relatime preview via Web
* Two-way audio via Tuya APP and Amazon Echo
* Local storage(SD card e.g.) and playback
* Cloud storage and playback
* Multiple types of event notification
* AI detect(MD/HD/FD/Pet etc.) via Tuya cloud
* Edge AI detect(FD e.g.) managerment
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
3. extract DemoResource/resource.tar.gz and put together with build result bin "tuya_ipc_demo_tuya_ipc" under /tmp, then run as below (check detail in <Tuya_IPC_SDK_Development_Manual.doc>)
```bash
$ /tmp/tuya_ipc_demo_tuya_ipc -m 2 -r "./" -t "A token you can get from TUYA APP"
```

## Preview on Web
Login on https://ipc.ismartlife.me/login with Chrome, scan QR code with Tuya Smart App.
(works only with SDK version >= 4.7.2 and NOT low-power type)

## Full SDK manual and API description
[TUYA Docs center]( https://docs.tuya.com/zh/iot/device-development/access-mode-product-solution/product-solution-ipc/sdk )

# Open Source List
* librtmp v2.3
* libuv v1.23.2
* Mbed TLS v2.9.0
* zlib v1.2.11

# Supported Platforms

## ubuntu x64 laptop
* linux-ubuntu-6.2.0_64Bit

## Hisilicon Linux
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
* liteos-3.2.3(use arm-himix100-linux)

## REALTEK
* mips-linux-uclibc-gcc-4.8.5_rts3903 for rts3903
* rsdk-6.4.1 for rts3905
* rsdk-6.4.1-linux for rts3906
* rsdk-6.5.0 for rts3903/3906
* msdk-4.4.7 for rtl819x

## MStar
* gcc-linaro-arm-linux-gnueabihf-4.8 for msc313e
* arm-buildroot-linux-uclibcgnueabihf-313e for msc313e/ssc323e
* arm-buildroot-linux-uclibcgnueabihf-msc316dc for msc316dc

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

## RockChip
* aarch64-buildroot-linux-gnu for RK3399
* aarch64-linux-gnu for RK3326

## 安凯
* arm-none-linux-4.4.1 for 3918/3916 V100
* arm-anykav200-linux-uclibc-4.8.5 for 3918/3916 V200

## 君正 T10 T20 T30A T31
* mips-linux-4.7.2_32Bit
* mips-linux-4.7.2_64Bit
* mips-linux-uclibc-4.7.2_32Bit
* mips-linux-uclibc-4.7.2_64Bit
* mips-gcc472-glibc216-32bit-r2.3.3
* mips-gcc540-glibc222-64bit-r3.3.0

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

## 数字动力
* gcc-linaro-5.3.1-2016.05-x86_64 for X1H
* arm-linux-gnueabihf_x9 for X9

## 安霸 AMBARELLA S5L
* linaro-aarch64-2017.08-gcc7.1

## 多方科技augentix
* arm-augentix-linux-gnueabi

## Others
* dh_alpsx_4.8.3
* android_arm-linux-androideabi for standard 32Bit android
