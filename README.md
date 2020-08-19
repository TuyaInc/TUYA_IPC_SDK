# Overview
TUYA_IPC_SDK is an integrated library&APIs, primarily for developing IP camera products.

* DemoResource:   resources for sdk quick demo: h264 video/pcm audio/jpeg snapshot etc.
* Stable: 4.7.14   suggested version. [release note](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/release_note.txt)
* Lastest: 4.8.4  version with new supported feature, tested and varified only on certain hardware platform.
	- Audio/video message: leave audio/video message to message center
	- AP mode streaming: when IPC is working on AP mode without WAN, TUYA APP is able to preview via LAN. 
	- Direct QR scanning pairing: for IPC with 4G network, use TUYA APP to scan the QR code on IPC for pairing.
	[release note](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Latest/release_note.txt)

# Feature highlights
* Register to Tuya Cloud platform
* Multiple device pairing method(EZ/QR/AP/Wired/4G e.g.)
* Realtime preview via Tuya Smart APP
* Realtime preview via Amazon Echo products and Goolgle chromecast
* Relatime preview via Web(webrtc)
* Two-way audio via Tuya APP and Amazon Echo(webrtc)
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
* [linux-ubuntu-6.2.0_64Bit](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/linux-ubuntu-6.2.0_64Bit)

## Android
* [aarch64-linux-android-4.9-ndk-r18b for 64bit](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/aarch64-linux-android-4.9-ndk-r18b)
* [arm-linux-androideabi-4.9-ndk-r18b for 32bit](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-linux-androideabi-4.9-ndk-r18b)

## 海思 Hisilicon Linux
* [arm-hisiv100-linux-uclibc-4.4.1](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-hisiv100-linux-uclibc-4.4.1)
* [arm-hisiv300-linux-uclibc-4.8.3](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-hisiv300-linux-uclibc-4.8.3)
* [arm-hisiv500-linux-4.9.4](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-hisiv500-linux-4.9.4)
* [arm-hisiv510-linux](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-hisiv510-linux)
* [arm-hisiv600-linux](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-hisiv600-linux)
* [arm-himix100-linux](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-himix100-linux)
* [arm-himix200-linux](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-himix200-linux)

## 海思 Hisilicon LiteOS
* [liteos-1.3.2](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/liteos-1.3.2)
* [liteos-1.4.5](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/liteos-1.4.5)
* [liteos-1.4.6](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/liteos-1.4.6)
* [liteos-3.2.3](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/liteos-3.2.3)

## 瑞昱 Realtek
* [mips-linux-uclibc-gcc-4.8.5 for rts3903](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/mips-linux-uclibc-gcc-4.8.5_rts3903)
* [rsdk-6.4.1 for rts3905 rtos](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/rsdk-6.4.1)
* [rsdk-6.4.1-linux for rts3906](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/rsdk-6.4.1-linux)
* [rsdk-6.5.0 for rts3903/3906](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/rsdk-6.5.0)
* [msdk-4.4.7 for rtl819x](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/msdk-4.4.7)

## 晨星 MStar/SigmaStar
* [gcc-linaro-arm-linux-gnueabihf-4.8 for msc313e](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/gcc-linaro-arm-linux-gnueabihf-4.8)
* [arm-buildroot-linux-uclibcgnueabihf for msc313e](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-buildroot-linux-uclibcgnueabihf-313e)
* [arm-buildroot-linux-uclibcgnueabihf for ssc323/333](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-buildroot-linux-uclibcgnueabihf-ssc323)
* [arm-buildroot-linux-uclibcgnueabihf for msc316dc](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-buildroot-linux-uclibcgnueabihf-msc316dc)
* [arm-buildroot-linux-uclibggnueabinhf-gcc-4.7.3](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-buildroot-linux-uclibggnueabinhf-gcc-4.7.3)

## 安凯 Anyka
* [arm-none-linux-4.4.1 for 3918/3916 V100](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-none-linux-4.4.1 )
* [arm-anykav200-linux-uclibc-4.8.5 for 3918/3916 v200](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-anykav200-linux-uclibc-4.8.5)
* [arm-anykav500-linux-uclibcgnueabi-gcc-4.9.4 for v500](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-anykav500-linux-uclibcgnueabi-gcc-4.9.4)

## 君正 Ingenic
* [mips-linux-4.7.2_32Bit for T10/T20/T21](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/mips-linux-4.7.2_32Bit)
* [mips-linux-4.7.2_64Bit](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/mips-linux-4.7.2_64Bit)
* [mips-linux-uclibc-4.7.2_32Bit](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/mips-linux-uclibc-4.7.2_32Bit)
* [mips-linux-uclibc-4.7.2_64Bit](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/mips-linux-uclibc-4.7.2_64Bit)
* [mips-gcc472-glibc216-32bit-r2.3.3 for T30A/T31](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/mips-gcc472-glibc216-32bit-r2.3.3)
* [mips-gcc540-glibc222-64bit-r3.3.0](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/mips-gcc540-glibc222-64bit-r3.3.0)

## 国科 Goke
* [arm-goke-linux-glibc-4.6.1 for GK7101/7102](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-goke-linux-glibc-4.6.1)
* [arm-goke-linux-uclibc-4.6.1](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-goke-linux-uclibc-4.6.1)
* [arm-linux-gnueabihf-4.8.3](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-linux-gnueabihf-4.8.3)

## 全志 Allwinner
* [arm-openwrt-linux-muslgnueabi for R16](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-openwrt-linux-muslgnueabi)
* [arm-openwrt-linux-gnueabi-gcc-6.4.1 MR137](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-openwrt-linux-gnueabi-gcc-6.4.1)

## 瑞芯微 RockChip
* [aarch64-buildroot-linux-gnu for RK3399](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/aarch64-buildroot-linux-gnu )
* [aarch64-linux-gnu for RK3326](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/aarch64-linux-gnu )
* [arm-buildroot-linux-gnueabihf for RV1108](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-buildroot-linux-gnueabihf-rv1108)

## 联发科 MediaTek
* [mipsel-openwrt-linux-uclibc for MT7628](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/mipsel-openwrt-linux-uclibc)
* [mipsel-openwrt-linux for mt7688](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/mipsel-openwrt-linux-mt7688)

## 联咏 NOVATEK
* [mipsel-24kec-linux-glibc-4.9 for NT9851X](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/mipsel-24kec-linux-glibc-4.9)
* [mipsel-24kec-linux-uclibc-4.9](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/mipsel-24kec-linux-uclibc-4.9)

## 智源 Grain
* [arm-unknown-linux-uclibc-4.4.0 for GM8136](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/rm-unknown-linux-uclibc-4.4.0)

## 雄迈 XMsilicon
* [arm-xm-linux for XM530AI](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-xm-linux)
* [arm-eabi-uclibc for XM510](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-eabi-uclibc)

## 新唐 Nuvoton
* [arm-nuvoton-linux-uclibceabi for N32926](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/rm-nuvoton-linux-uclibceabi)

## 富瀚 Fullhan
* [arm-fullhan-linux-uclibc-4.3.2 for FH8812](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-fullhan-linux-uclibc-4.3.2)
* [arm-fullhan-linux-uclibcgnueabi for FH8632](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-fullhan-linux-uclibcgnueabi)

## 睿致 Vatics
* [arm-buildroot-linux-uclibcgnueabihf for M5S](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-buildroot-linux-uclibcgnueabihf)

## 松瀚 Sonix
* [arm-unknown-linux-uclibcgnueabi for SN98660](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-unknown-linux-uclibcgnueabi )

## 酷芯 Artosyn
* [gcc-linaro-5.4.1-2017.05-x86_64 for AR9201](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/gcc-linaro-5.4.1-2017.05-x86_64)

## 数字动力 D-power
* [gcc-linaro-5.3.1-2016.05-x86_64 for X1H](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/gcc-linaro-5.3.1-2016.05-x86_64)
* [arm-linux-gnueabihf for X9](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-linux-gnueabihf_x9)

## 安霸 AMBARELLA
* [linaro-aarch64-2017.08-gcc7.1 for S5L](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/linaro-aarch64-2017.08-gcc7.1)

## 多方科技 augentix
* [arm-augentix-linux-gnueabi](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-augentix-linux-gnueabi)

## 豪威 OV
* [arm-ov-linux-uclibcgnueabihf for OA7000](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/arm-ov-linux-uclibcgnueabihf)

## Others
* [dh_alpsx_4.8.3](https://github.com/TuyaInc/TUYA_IPC_SDK/tree/master/Stable/dh_alpsx_4.8.3)
