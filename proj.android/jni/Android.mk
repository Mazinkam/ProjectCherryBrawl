LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libhellocpp

LOCAL_SRC_FILES := game/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/GameObjects/ActionSprite.cpp \
                   ../../Classes/Layers/GameLayer.cpp \
                   ../../Classes/Scenes/GameScene.cpp \
                   ../../Classes/Scenes/SplashScene.cpp \
                   ../../Classes/Scenes/MenuScene.cpp \
                   ../../Classes/GameObjects/EnemyFemale.cpp \
                   ../../Classes/GameObjects/EnemyBoss.cpp \
                   ../../Classes/GameObjects/EnemyBossWings.cpp \
                   ../../Classes/Layers/HudLayer.cpp \
                   ../../Classes/GameObjects/Cherry.cpp \
                   ../../Classes/GameObjects/SimpleDPad.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)