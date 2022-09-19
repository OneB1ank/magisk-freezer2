LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
APP_PLATFORM :=android-26
APP_ABI := arm64-v8a
APP_STL := c++_static
NDK_TOOLCHAIN_VERSION := clang
APP_BUILD_SCRIPT := Android.mk
include $(BUILD_EXECUTABLE)