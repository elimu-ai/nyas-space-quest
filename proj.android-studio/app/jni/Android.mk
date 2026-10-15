LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../../Classes/AppDelegate.cpp \
                   ../../../Classes/BaseObject.cpp \
                   ../../../Classes/Bot.cpp \
                   ../../../Classes/Coin.cpp \
                   ../../../Classes/End.cpp \
                   ../../../Classes/GameMapScene.cpp \
                   ../../../Classes/LanguageManager.cpp \
                   ../../../Classes/LoadingScene.cpp \
                   ../../../Classes/MarsScene.cpp \
                   ../../../Classes/MenugScene.cpp \
                   ../../../Classes/NumberDisplay.cpp \
                   ../../../Classes/NumberTest.cpp \
                   ../../../Classes/Planet.cpp \
                   ../../../Classes/Player.cpp \
                   ../../../Classes/Spike.cpp \
                   ../../../Classes/Tutorial.cpp \
                   ../../../Classes/UranusScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
