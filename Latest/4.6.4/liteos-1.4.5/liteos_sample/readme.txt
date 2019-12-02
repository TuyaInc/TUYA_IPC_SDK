使用原生海思原生SDK，该sample对应版本是 Hi3518E_SDK_V5.0.5.3.tgz
1. cd osdrv/opensource/liteos/liteos，make
2. 将tuya_project.tar.gz解压到海思SDK根目录，即与osdrv同级
3. cd tuya_project, make
4. 生成的产物为tuya_project/src/media/tuya_process.bin
5. uboot阶段 tftp 0x80008000 tuya_process.bin;go 0x80008000运行，
启动后使用process <token>值运行demo。

*需要开发者更换设备 uuid、authkey、pid：tuya_ipc_mgr_handler.c 中，实际产品需要工厂烧录
*该sample中tuya的参数配置存放 /sd0/目录下，需要开发者运行 process 命令前先保证该目录可写，比如mount sd卡到该目录。
*该sample不涉及wifi配网的部分，需要开发者自行实现 tuya_linux_wifi.c 中的逻辑。
process运行前需保证网络已经联通公网。
*该sample匹配的SDK版本为 3.1.2_tuya_p2p_fs_storage_version
*测试APP可以使用以下地址，或从涂鸦项目经理获得最新的稳定版本
https://testapp.tuya.com/tuyaipcrd?version=1.1.12&os=android
https://testapp.tuya.com/tuyaipcrd?version=1.1.12&os=ios
