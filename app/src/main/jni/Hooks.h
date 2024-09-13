#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "StarDust/ImGui/imgui.h"
#include "StarDust/ImGui/imgui_internal.h"
#include "StarDust/ImGui/backends/imgui_impl_opengl3.h"
#include "StarDust/ImGui/backends/imgui_impl_android.h"
#include "StarDust/ImGui/fonts/GoogleSans.h"

#include "StarDust/Unity/Quaternion.hpp"
#include "StarDust/Unity/Vector2.hpp"
#include "StarDust/Unity/Vector3.hpp"
#include "StarDust/Unity/Rect.h"
#include "StarDust/Unity/Color.h"
#include "StarDust/Unity/Unity.h"


#include "StarDust/ByNameModding/Tools.h"
#include "StarDust/ByNameModding/fake_dlfcn.h"
#include "StarDust/ByNameModding/Il2Cpp.h"

#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>

#include "StarDust/dobby/dobby.h"
#include "Global_Star.h"

#define libName OBFUSCATE("libil2cpp.so")

static vector<void*> players;
#include <chrono>

static std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
static int frameCount = 0;
static float fps = 0.0f;

/*
      
        ****** IMGUI ESP HOOKS BY STARDUST XDD ****** 
		
		• Auto Update Esp     • LGL 3.2   • 32bit ( Tested )
		• ByNameModding Library For Auto Update
		• V1.0 ( Beta )
        • t.me/xtreammodz | @StardustXdd
		
		• Base Calculation Refrence 
		   https://youtu.be/kGDKQXgxIrY
		   https://youtu.be/Xh4hl0ScLCo
		   
        ****** XDD ****** XDD ****** XDD ****** XDD ****
		
		• If you are using AIDE use Refresh Build Insted Direct Build

*/




void (*SetResolution)(int width, int height, bool fullscreen);
void *(*get_transform)(void *instance);
Vector3 (*get_position)(void *instance);
Vector3 (*WorldToScreen)(void *camera, Vector3 position);
void *(*get_main)();

Vector3 getPosition(void *transform) {
     return get_position(get_transform(transform)); 
} 

bool playerFind(void *player) {
    if (player != NULL) {
        for (int i = 0; i < players.size(); i++) {
            if (player == players[i]) return true;
        }
    }
    return false;
}

void (*old_Player_Update)(...);
void Player_Update(void *player) {
  if (player != NULL) { 
    if(!playerFind(player)) players.push_back(player);                
  }
  return old_Player_Update(player);
}	

void (*old_Player_OnDestroy)(...);
void Player_OnDestroy(void *player) {
    if (player != NULL) {
        old_Player_OnDestroy(player);
        players.clear();   
    }
}

bool (*old_Player_isAlive)(...);
bool Player_isAlive(void *player) {
    return old_Player_isAlive(player);
}

/*
// TO show player Health Bar

//  This is player health factor depends pn your game
// you can define it manually or load it from methods eg = get_maxHealth()
float HealthFactor = 100.0f;

float(*old_player_health)(...);
float GetPlayerHealth(void *player) {
	return old_player_health(player);
}
*/
void DrawESP(ImDrawList *draw, int screenWidth, int screenHeight) {
    if (!isLibLoaded) {
        SetResolution(screenWidth, screenHeight, true); 
    }

    if (isESP) {
        for (int i = 0; i < players.size(); i++) {
            auto Player = players[i];
            if (Player != NULL) {
                if (!Player_isAlive(Player)) {
                    continue; 
                }

                auto camera = get_main();
                Vector3 playerPosition = getPosition(Player);
                Vector3 playerFeetPos = WorldToScreen(camera, playerPosition); 
                Vector3 playerHeadPos = WorldToScreen(camera, Vector3(playerPosition.X, playerPosition.Y + 2.0f, playerPosition.Z)); // Head (2 units above)

                if (playerFeetPos.Z < 0.1f || playerHeadPos.Z < 0.1f) continue; 

                playerFeetPos.Y = screenHeight - playerFeetPos.Y;
                playerHeadPos.Y = screenHeight - playerHeadPos.Y;

                float boxHeight = playerFeetPos.Y - playerHeadPos.Y;
                float boxWidth = boxHeight / 2; 

                ImVec2 topLeft(playerHeadPos.X - boxWidth / 2, playerHeadPos.Y);
                ImVec2 topRight(playerHeadPos.X + boxWidth / 2, playerHeadPos.Y);
                ImVec2 bottomLeft(playerFeetPos.X - boxWidth / 2, playerFeetPos.Y);
                ImVec2 bottomRight(playerFeetPos.X + boxWidth / 2, playerFeetPos.Y);

                ImVec2 topCenter((topLeft.x + topRight.x) / 2, topLeft.y);

                if (isESPLine) {
                    draw->AddLine(ImVec2(screenWidth / 2, 0), topCenter, lineColor, esplineThickness);
                }

                if (isESPBox) {
                    draw->AddLine(topLeft, topRight, boxColor, boxlineThickness);
                    draw->AddLine(topRight, bottomRight, boxColor, boxlineThickness);
                    draw->AddLine(bottomRight, bottomLeft, boxColor, boxlineThickness);
                    draw->AddLine(bottomLeft, topLeft, boxColor, boxlineThickness);
                }
	  
	          if (isESPHealth) {
				  
				  /*
                   float playerHealth = GetPlayerHealth(Player); // Assume this function retrieves player's health as a float between 1 and 100
                   float normalizedHealth = playerHealth / HealthFactor; // Normalize to a value between 0 and 1
                   float healthBarHeight = boxHeight * normalizedHealth; 
                   ImVec2 healthBarTop(playerHeadPos.X - boxWidth / 2 - healthBarWidth - 2, playerHeadPos.Y);
                   ImVec2 healthBarBottom(playerHeadPos.X - boxWidth / 2 - healthBarWidth - 2, playerFeetPos.Y); 
                   ImVec2 healthFillTop(healthBarTop.x, playerFeetPos.Y - healthBarHeight); 
                  if (normalizedHealth > 0) { 
                       draw->AddRectFilled(healthFillTop, healthBarBottom, IM_COL32(0, 255, 0, 255));
                   }
                   draw->AddRect(healthBarTop, healthBarBottom, IM_COL32(128, 128, 128, 255)); 
				   
				   */
              }

                
                if (isESPDistance) {
                    float distance = sqrtf(powf(playerPosition.X, 2) + powf(playerPosition.Y, 2) + powf(playerPosition.Z, 2));
                    std::string distanceText = "Distance: " + std::to_string(static_cast<int>(distance));
                    ImVec2 textPos(playerHeadPos.X, playerHeadPos.Y - boxHeight - 5); 
                    draw->AddText(textPos, distanceTextColor, distanceText.c_str());
                }
            }
        }
    }

    if (isESPCircle) {
        ImVec2 center(screenWidth / 2, screenHeight / 2); 
        draw->AddCircle(center, radius, IM_COL32(0, 255, 0, 255), 50, circleThickness);
    }

 
    if (isCrosshairEnabled) {
        ImVec2 center(screenWidth / 2, screenHeight / 2);
        draw->AddLine(ImVec2(center.x - crosshairSize, center.y), ImVec2(center.x + crosshairSize, center.y), crosshairColor, crosshairThickness);
        draw->AddLine(ImVec2(center.x, center.y - crosshairSize), ImVec2(center.x, center.y + crosshairSize), crosshairColor, crosshairThickness);
    }
	
    float verticalOffset = 20; // Space between texts

    ImVec2 fpsTextSize = ImVec2(0, 0), playerCountSize = ImVec2(0, 0), watermarkSize = ImVec2(0, 0);
    std::string fpsText = "", playerCountText = "", watermarkText = "Stardust";

    if (isFPSCounterEnabled) {
        fpsText = "FPS: " + std::to_string(static_cast<int>(fps));
        fpsTextSize = ImGui::CalcTextSize(fpsText.c_str());
    }

    if (isShowPlayerCount) {
        int alivePlayerCount = 0;
        for (int i = 0; i < players.size(); i++) {
            auto Player = players[i];
            if (Player != NULL && Player_isAlive(Player)) {
                alivePlayerCount++;  
            }
        }
        playerCountText = "Player Count: " + std::to_string(alivePlayerCount);
        playerCountSize = ImGui::CalcTextSize(playerCountText.c_str());
    }

    watermarkSize = ImGui::CalcTextSize(watermarkText.c_str());

    float totalWidth = 0;
    if (isFPSCounterEnabled) totalWidth += fpsTextSize.x;
    if (isShowPlayerCount) totalWidth += playerCountSize.x;
    totalWidth += watermarkSize.x;

    float totalSpacing = verticalOffset * 2;
    if (isFPSCounterEnabled && isShowPlayerCount) totalSpacing += verticalOffset;
    totalWidth += totalSpacing;

    float startX = (screenWidth - totalWidth) / 2;
    float startY = screenHeight - watermarkSize.y - 10;
    float initialX = startX;

    if (isFPSCounterEnabled) {
        ImVec2 fpsTextPos(startX, startY);
        draw->AddText(fpsTextPos, textColor, fpsText.c_str());
        startX += fpsTextSize.x + verticalOffset;
    }

    if (isShowPlayerCount) {
        ImVec2 playerCountPos(startX, startY);
        draw->AddText(playerCountPos, textColor, playerCountText.c_str());
        startX += playerCountSize.x + verticalOffset;
    }

    ImVec2 watermarkPos(initialX + totalWidth - watermarkSize.x, startY);
    draw->AddText(watermarkPos, textColor, watermarkText.c_str());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}




EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);
    
    if (glWidth <= 0 || glHeight <= 0) {
        return eglSwapBuffers(dpy, surface);
    }
    
    if (!g_Initialized) {
        ImGui::CreateContext();
        ImGuiStyle* style = &ImGui::GetStyle();
        ImGui::StyleColorsDark();
        
        style->WindowPadding = ImVec2(5, 5);
        style->WindowRounding = 2.0f;
        style->WindowBorderSize = 0.0f;
        style->FramePadding = ImVec2(2, 2);
        style->FrameRounding = 2.0f;
        style->ItemInnerSpacing = ImVec2(8, 6);
        style->IndentSpacing = 10.0f;
        style->ScrollbarSize = 5.0f;
        style->ScrollbarRounding = 5.0f;
        style->GrabMinSize = 5.0f;
        style->GrabRounding = 3.0f;
        style->ScaleAllSizes(1.0f);
        style->ScrollbarSize /= 1;
        
        ImGuiIO* io = &ImGui::GetIO();
        io->IniFilename = nullptr;

        ImGui_ImplOpenGL3_Init("#version 300 es");
        
        ImFontConfig font_cfg;
        io->Fonts->AddFontFromMemoryCompressedTTF(GoogleSans_compressed_data, GoogleSans_compressed_size, 20.0f, &font_cfg);            
       
        g_Initialized = true;
    }
    
    // Calculate FPS
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = currentTime - lastTime;
    frameCount++;
    if (elapsed.count() >= 1.0f) { // Update FPS every second
        fps = frameCount / elapsed.count();
        frameCount = 0;
        lastTime = currentTime;
    }
    
    ImGuiIO* io = &ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    ImGui::NewFrame();
    
    DrawESP(ImGui::GetBackgroundDrawList(), glWidth, glHeight);     
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    return old_eglSwapBuffers(dpy, surface);
}






void *hack_thread(void *) {  
    ProcMap il2cppMap;
    do {
       il2cppMap = KittyMemory::getLibraryMap(libName);
       sleep(1);
    } while (!il2cppMap.isValid());
      
	while (!m_EGL) {
        m_EGL = dlopen("libEGL.so", RTLD_NOW);
        sleep(1);
    }
	
	Il2CppAttach();
	sleep(5);  
    
    DobbyHook((void *) dlsym(m_EGL, OBFUSCATE("eglSwapBuffers")), (void *) eglSwapBuffers, (void **) &old_eglSwapBuffers);
	  
	// Camera.get_main();
    get_main = (void *(*)()) (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "get_main",0);
	// Camera.WorldToScreenPoint();
    WorldToScreen = (Vector3 (*)(void *, Vector3)) (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "WorldToScreenPoint", 1);
	// Component.get_transform()
    get_transform = (void *(*)(void *)) (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Component", "get_transform", 0);
	// Transform.get_position();
    get_position = (Vector3 (*)(void *)) (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Transform", "get_position", 0);
    // Screen.SetResolution();
	SetResolution = (void (*)(int, int, bool)) (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Screen", "SetResolution", 0);
    
	// Player.Update();
	Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "AgentHuman", "LateUpdate", 0), 
    (void *) Player_Update, 
    (void **) &old_Player_Update);  
    
	// Player.OnDestroy();
	Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "AgentHuman", "OnDestroy", 0), 
    (void *) Player_OnDestroy, 
    (void **) &old_Player_OnDestroy);    
    
 
	Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "AgentHuman", "get_IsAlive", 0), 
    (void *) Player_isAlive,
    (void **) &old_Player_isAlive);    
	
	// For Health Bar
	/*
	Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "ClassName", "get_Health", 0), 
    (void *) GetPlayerHealth,
    (void **) &old_player_health);    
	*/
	
	isLibLoaded = true;
	
    return NULL;
}

__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}
