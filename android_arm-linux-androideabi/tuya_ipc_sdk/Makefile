COMPILE_PREX =
AR = $(COMPILE_PREX)ar
CC = $(COMPILE_PREX)gcc
NM = $(COMPILE_PREX)nm
CPP = $(COMPILE_PREX)g++
OBJCOPY = $(COMPILE_PREX)objcopy
OBJDUMP = $(COMPILE_PREX)objdump

LINKFLAGS = \
	-L$(ROOT_DIR)/libs -ltuya_ipc -lpthread -lm -lrt -ldl -lstdc++ -lAVAPIs -lIOTCAPIs
	
CCFLAGS = \
	-g 

DEFINES = 

CFLAGS = $(CCFLAGS) $(DEFINES) $(SDK_INCS) $(USER_INCS)

#编译所用变量
ROOT_DIR = $(abspath .)
OUTPUT_DIR = $(ROOT_DIR)/output/$(APP_BIN_NAME)
OUTPUT_DIR_OBJS = $(OUTPUT_DIR)/objs

#sdk头文件路径
SDK_INC_BASE_DIR = $(ROOT_DIR)/include
SDK_INCS =  $(addprefix -I ,  $(shell find $(SDK_INC_BASE_DIR) -type d) )

#user app基准路径
USER_SRC_BASE_DIR =  $(ROOT_DIR)/$(APP_BIN_NAME)
USER_INC_BASE_DIR = $(ROOT_DIR)/$(APP_BIN_NAME)/include

#user app 编译文件
USER_SRC_DIRS = $(shell find $(USER_SRC_BASE_DIR) -type d)
USER_SRCS += $(foreach dir, $(USER_SRC_DIRS), $(wildcard $(dir)/*.c)) 
USER_SRCS += $(foreach dir, $(USER_SRC_DIRS), $(wildcard $(dir)/*.cpp)) 
USER_SRCS += $(foreach dir, $(USER_SRC_DIRS), $(wildcard $(dir)/*.s)) 
USER_SRCS += $(foreach dir, $(USER_SRC_DIRS), $(wildcard $(dir)/*.S)) 
#user头文件
USER_INCS = $(addprefix -I ,  $(shell find $(USER_INC_BASE_DIR) -type d) )
#user的obj命令
USER_OBJS = $(addsuffix .o, $(basename  $(USER_SRCS) ) )
#user的实际obj地址
USER_OBJS_OUT =  $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $(USER_OBJS))

build_app: $(USER_OBJS)
	@mkdir -p $(OUTPUT_DIR)
	$(CC)  $(USER_OBJS_OUT) $(CFLAGS)  $(LINKFLAGS)  -o $(OUTPUT_DIR)/linux_ipc_$(APP_BIN_NAME) 
	@echo "Build APP Finish"

%.o: %.c
	@mkdir -p $(dir $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)); 
	$(CC) $(CFLAGS) -o  $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)   -c $<
	
%.o: %.cpp
	@mkdir -p $(dir $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)); 
	$(CC) $(CFLAGS) -o $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)   -c $<

%.o: %.s
	@mkdir -p $(dir $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)); 
	$(CC) $(CFLAGS) -o $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)  -c $<

%.o: %.S
	@mkdir -p $(dir $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)); 
	$(CC) $(CFLAGS) -D__ASSEMBLER__ -o $(subst $(ROOT_DIR),$(OUTPUT_DIR_OBJS), $@)  -c $<
	
	
.PHONY:clean SHOWARGS
clean:
	rm -rf $(OUTPUT_DIR)
