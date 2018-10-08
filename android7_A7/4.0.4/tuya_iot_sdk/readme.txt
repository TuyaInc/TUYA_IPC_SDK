文件说明：
include：应用相关头文件
sample_base_gw：demo代码目录
	build：编译参数目录
	include：用户应用代码头文件，makefile会自动关联
	libs：用户代码关联库路径
	shell：相关脚本目录
	user：用户代码
build_app.sh：用户应用代码编译脚本
libtuya_iot.a：tuya iot sdk静态库
	
编译说明：
1 打开Makefile文件，设置变量COMPILE_PREX ?= 具体交叉编译工具链的绝对路径
2 执行：sh build_app.sh sample_base_gw，完成编译