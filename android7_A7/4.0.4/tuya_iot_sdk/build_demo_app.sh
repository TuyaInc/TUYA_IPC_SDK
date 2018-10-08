#!/bin/sh

APP_BIN_NAME=
if [ -z "$1" ];then
	echo "please input the target app name "
	exit 1
else
	APP_BIN_NAME=$1
fi

#开始编译APP
make  APP_BIN_NAME=$APP_BIN_NAME clean
make  APP_BIN_NAME=$APP_BIN_NAME build_app