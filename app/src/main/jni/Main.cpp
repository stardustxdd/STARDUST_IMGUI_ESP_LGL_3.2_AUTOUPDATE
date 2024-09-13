#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>

#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"

#include "KittyMemory/MemoryPatch.h"
#include "Menu/Setup.h"
#include "Hooks.h"

jobjectArray GetFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;

    const char *features[] = {    
	        OBFUSCATE("Category_Esp Menu"),
            OBFUSCATE("1_Toggle_Enable ESP"),
			OBFUSCATE("2_Toggle_ESP Line"),   
			OBFUSCATE("3_Toggle_ESP Box"),	
			OBFUSCATE("4_Toggle_ESP Circle"),	
			OBFUSCATE("5_Toggle_Player Count"),
			OBFUSCATE("6_Toggle_Player Distance"),
			OBFUSCATE("7_Toggle_Fps Counter"),
			OBFUSCATE("8_Toggle_Esp CrossHair"),
			OBFUSCATE("9_Toggle_Esp Health"),
			OBFUSCATE("Category_Esp Controls"),
			OBFUSCATE("21_SeekBar_Circle Radius_1_400"),
			OBFUSCATE("22_SeekBar_Circle Thikness_1_5"),
			OBFUSCATE("23_SeekBar_Esp Thickness_1_5"),
			OBFUSCATE("24_SeekBar_Box Thickness_1_5"),
			OBFUSCATE("25_SeekBar_CrossHair Size_10_80"),
			
    };

    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}

void Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint featNum, jstring featName, jint value,
                                        jboolean boolean, jstring str) {
   
 switch (featNum) {
        case 1:
		    isESP = boolean;
          break;
		case 2:
			isESPLine = boolean;
			break;
	    case 3:
			isESPBox = boolean;
			break;
			
		case 4:
			isESPCircle = boolean;
			break;
			
		case 5:
			isShowPlayerCount = boolean;
			break;
		
		case 6:
			isESPDistance = boolean;
			break;
			
		case 7:
			isFPSCounterEnabled = boolean;
			break;
			
		case 8:
			isCrosshairEnabled = boolean;
			break;
			
		case 9:
			isESPHealth = boolean;
			break;
			
		case 21:
			radius = value;
			break;
			
		case 22:
			circleThickness = value;
			break;
			
		case 23:
			esplineThickness = value;
			break;
			
		case 24:
			boxlineThickness = value;
			break;
			
		case 25:
			crosshairSize = value;
			break;
    }
}


int RegisterMenu(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {OBFUSCATE("Icon"), OBFUSCATE("()Ljava/lang/String;"), reinterpret_cast<void *>(Icon)},
            {OBFUSCATE("IconWebViewData"),  OBFUSCATE("()Ljava/lang/String;"), reinterpret_cast<void *>(IconWebViewData)},
            {OBFUSCATE("IsGameLibLoaded"),  OBFUSCATE("()Z"), reinterpret_cast<void *>(isGameLibLoaded)},
            {OBFUSCATE("Init"),  OBFUSCATE("(Landroid/content/Context;Landroid/widget/TextView;Landroid/widget/TextView;)V"), reinterpret_cast<void *>(Init)},
            {OBFUSCATE("SettingsList"),  OBFUSCATE("()[Ljava/lang/String;"), reinterpret_cast<void *>(SettingsList)},
            {OBFUSCATE("GetFeatureList"),  OBFUSCATE("()[Ljava/lang/String;"), reinterpret_cast<void *>(GetFeatureList)},
    };

    jclass clazz = env->FindClass(OBFUSCATE("com/android/support/Menu"));
    if (!clazz)
        return JNI_ERR;
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return JNI_ERR;
    return JNI_OK;
}

int RegisterPreferences(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {OBFUSCATE("Changes"), OBFUSCATE("(Landroid/content/Context;ILjava/lang/String;IZLjava/lang/String;)V"), reinterpret_cast<void *>(Changes)},
    };
    jclass clazz = env->FindClass(OBFUSCATE("com/android/support/Preferences"));
    if (!clazz)
        return JNI_ERR;
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return JNI_ERR;
    return JNI_OK;
}

int RegisterMain(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {OBFUSCATE("CheckOverlayPermission"), OBFUSCATE("(Landroid/content/Context;)V"), reinterpret_cast<void *>(CheckOverlayPermission)},
    };
    jclass clazz = env->FindClass(OBFUSCATE("com/android/support/Main"));
    if (!clazz)
        return JNI_ERR;
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return JNI_ERR;

    return JNI_OK;
}

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (RegisterMenu(env) != 0)
        return JNI_ERR;
    if (RegisterPreferences(env) != 0)
        return JNI_ERR;
    if (RegisterMain(env) != 0)
        return JNI_ERR;
    return JNI_VERSION_1_6;
}
