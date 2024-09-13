LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# Build static library
LOCAL_MODULE := libdobby
LOCAL_SRC_FILES := StarDust/Dobby/libraries/$(TARGET_ARCH_ABI)/libdobby.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/StarDust/dobby/
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

# Build shared library
LOCAL_MODULE := StarDust

LOCAL_CFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions
LOCAL_CPPFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -Werror -std=c++17 -Wno-error=c++11-narrowing -Wall -fexceptions
LOCAL_LDFLAGS := -Wl,--gc-sections,--strip-all
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2
LOCAL_ARM_MODE := arm

LOCAL_STATIC_LIBRARIES := libdobby

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
    $(LOCAL_PATH)/StarDust/ImGui \
    $(LOCAL_PATH)/StarDust/ImGui/backends

LOCAL_SRC_FILES := \
    main.cpp \
    Substrate/hde64.c \
    Substrate/SubstrateDebug.cpp \
    Substrate/SubstrateHook.cpp \
    Substrate/SubstratePosixMemory.cpp \
    Substrate/SymbolFinder.cpp \
    StarDust/ByNameModding/Tools.cpp \
    StarDust/ByNameModding/fake_dlfcn.cpp \
    StarDust/ByNameModding/Il2Cpp.cpp \
    KittyMemory/KittyMemory.cpp \
    KittyMemory/MemoryPatch.cpp \
    KittyMemory/MemoryBackup.cpp \
    KittyMemory/KittyUtils.cpp \
    StarDust/Unity/Vector2.hpp \
    StarDust/Unity/Vector3.hpp \
    StarDust/Unity/Quaternion.hpp \
    StarDust/ImGui/imgui.cpp \
    StarDust/ImGui/imgui_draw.cpp \
    StarDust/ImGui/imgui_widgets.cpp \
    StarDust/ImGui/imgui_tables.cpp \
    StarDust/ImGui/backends/imgui_impl_opengl3.cpp \
    StarDust/ImGui/backends/imgui_impl_android.cpp

include $(BUILD_SHARED_LIBRARY)