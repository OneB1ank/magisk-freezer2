LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cpp .cc .c

LOCAL_MODULE    := HC_freezer

LOCAL_CPPFLAGS += -std=c++14 

LOCAL_SRC_FILES :=  main.cpp 

LOCAL_CFLAGS += -O3 -fPIE -pie -fstack-protector-all 

LOCAL_ARM_MODE := arm

include $(BUILD_EXECUTABLE)