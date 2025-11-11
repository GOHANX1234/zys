#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <cfloat> // Added for FLT_MAX
#include <imgui.h>
#include <imgui_internal.h>
#include <Il2Cpp.h>
#include <Tools.h>
#include <Vector3.hpp>
#include <Color.h>
#include <Struct/main.h>
#include "obfuscate.h"
#include "Unity/unity.h"
#include "../CheatCode/Hacks/Class.h"
#include "../CheatCode/Hacks/Rect.h"
inline static int g_GlHeight, g_GlWidth;
#include "../CheatCode/Hacks/Bools.h"
#include <math.h>
#include <stdio.h>
typedef char PACKAGENAME;
//#include "ItsRohitOpKill.h"
bool AimSilent = false;
bool Reset = false;
bool Headshot = false;
bool AimbotLegit = false;
bool HidePanel = false;
struct MassKill {
float Sucks1 = 9.0f;
bool Sucks = false;
float FrontOffset = 0.5f;
}Mass;
bool UpPlayer = false;
bool ActiveFeature = true;
int SetDamage = 1;
float AimFov2 = 360.0f;
bool SpeedHack = false;
bool SilentAim = false;
bool AimVisible2 = true;
bool AimbotTrigger = true;
const float MAX_DISTANCE = 99999.0f;
const float AIM_ANGLE_MULTIPLIER = 70.0f;
static int aimPosition = 0;
const float NECK_OFFSET = -0.15f;
const float CHEST_OFFSET = -0.35f;
static int aimSmoothing = 0;
static bool ignoreKnockedEnemies = false;

bool AimVisible = false;
bool AimbotAim = false; //ff
bool AimbotAuto = false; 
float savedScreenWidth = 0;
float savedScreenHeight = 0;
float active = 0.0f;
float desactive = 0.0f;

bool saved = false;
int Fov = 0; //ff
//int Aimdis = 130; //ff
//bool Fov = false;
bool Startgs = true;

float DistanceTele = 0;
    ImColor ESPColor = ImColor(255, 255, 255, 255); // <— Add this line
bool isOutsideScreen(ImVec2 pos, ImVec2 screen) {
    if (pos.y < 0) {
        return true;
    }
    if (pos.x > screen.x) {
        return true;
    }
    if (pos.y > screen.y) {
        return true;
    }
    return pos.x < 0;
}

ImVec2 pushToScreenBorder(ImVec2 Pos, ImVec2 screen, int offset) {
    int x = (int) Pos.x;
    int y = (int) Pos.y;
    
    if (Pos.y < 0) {
        y = -offset;
    }
    
    if (Pos.x > screen.x) {
        x = (int) screen.x + offset;
    }
    
    if (Pos.y > screen.y) {
        y = (int) screen.y + offset;
    }
    
    if (Pos.x < 0) {
        x = -offset;
    }
    return ImVec2(x, y);
}

static float tm_alpha = 127 / 255.f;

void DrawCircleHealth(ImVec2 position, int health, int max_health, float radius) {
    float a_max = ((3.14159265359f * 2.0f));
    ImU32 healthColor = IM_COL32(45, 180, 45, 255);
    if (health <= (max_health * 0.6)) {
        healthColor = IM_COL32(180, 180, 45, 255);
    }
    if (health < (max_health * 0.3)) {
        healthColor = IM_COL32(180, 45, 45, 255);
    }
    ImGui::GetForegroundDrawList()->PathArcTo(position, radius, (-(a_max / 4.0f)) + (a_max / max_health) * (max_health - health), a_max - (a_max / 4.0f));
    ImGui::GetForegroundDrawList()->PathStroke(healthColor, ImDrawFlags_None, 4);
}



void drawText2(int x, int y, long color, const char *str, float size) {
        ImGui::GetBackgroundDrawList()->AddText(NULL, size, ImVec2(x, y), color, str);
    }


void DrawRectFilled(int x, int y, int w, int h, ImColor color) {
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h),
                                               color, 0, 0);
}


void DrawRectFilledHealth(int x, int y, int w, int h, ImVec4 color) {
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h),
                                               ImGui::ColorConvertFloat4ToU32(color), 0, 0);
}


void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color, int T) {
    ImGui::GetBackgroundDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3),
                                                   color);
}
/*
bool IsVisible(void * player){
    if(player != NULL) {
        void *hitObj = NULL;
        Vector3 cameraLocation = Transform_GetPosition(Component_GetTransform(Camera_main()));
        Vector3 headLocation = Transform_GetPosition(Component_GetTransform(Player_GetHeadCollider(player)));
            return !Physics_Raycast(cameraLocation, headLocation, 12, &hitObj);
    }
    return false;
}
bool Visible_Check(void *closestEnemy) {
    void *Object = static_cast<void *>(nullptr);
    Vector3 Camera = Transform_INTERNAL_GetPosition(Component_GetTransform(Camera_main()));
    Vector3 Collider = Transform_INTERNAL_GetPosition(Component_GetTransform(GetHeadCollider(closestEnemy)));
    if (!Cristiano_RayVery(Camera, Collider, 12, &Object)) {
        return true;
    } else {
        return false;
    }
}*/

void DrawHealths(Rect box, float entityHealth, float maxHealth, int TeamID, const char* name, long color) {
    float x = box.x - (140 - box.w) / 2;
    float y = box.y;
    char dis[20];
    // sprintf(dis, "%0.fM", d);

    char TeamText[20];
    sprintf(TeamText, "%d", (int)TeamID);
    ImVec4 HPColor = ImVec4{1, 1, 0, 127 / 255.f}; // Màu mặc định cho máu

    if (entityHealth < maxHealth) {
        float healthPercentage = entityHealth / maxHealth;

        if (healthPercentage < 0.3f) {
            HPColor = ImVec4{0.5f, 0.0f, 0.0f, 127 / 255.f}; // Màu đỏ cho máu thấp
        } else if (healthPercentage < 0.6f) {
            HPColor = ImVec4{1, 0, 0, 127 / 255.f}; // Màu cam cho máu trung bình
        }
    }

    tm_alpha = 100.f / 255.f;
    DrawRectFilled(x - strlen(name), y - 41, 120 + strlen(name) * 2, 20, color); // Đám mây nền cho tên người chơi
    tm_alpha = 255.f / 255.f;
    DrawRectFilled(x - strlen(name), box.y - 41, 25, 20, color);
    if (strlen(TeamText) < 2) {
        drawText2(x + 6 - strlen(name), box.y - 42, ImColor(255,255,255),
                       TeamText, 18.943);//队伍ID
    } else {
        drawText2(x - strlen(name), box.y - 42, ImColor(255,255,255),
                       TeamText, 18.943);//队伍ID
    }
    drawText2(x + 28 - strlen(name), y - 43,
                   ImColor(255,255,255), name, 18.943);//玩家名称
    float maxWidth = 120; // Độ dài tối đa của thanh máu
    float healthBarWidth = entityHealth * maxWidth / maxHealth; // Độ dài thanh máu dựa trên tỷ lệ máu hiện tại và máu tối đa
    DrawRectFilledHealth(x - maxWidth / 2, y - 18, healthBarWidth, 8, HPColor);// cập nhật vị trí và kích thước của thanh máu
    drawText2(x + 125 + strlen(name), y - 43, ImColor(255,255,255), dis,18.943);//距离显示
    DrawTriangle(box.x + box.w / 2 - 10, y - 8, box.x + box.w / 2 + 15 - 10, y - 8,
                 box.x + box.w / 2 - 2, y, ImColor(255,255,255), 1);//第一个左边 第二个右边 第三个底边
}


static float SetFieldOfView = 0, GetFieldOfView = 0;

void DroneView() {
    if (GetFieldOfView == 0) {
        GetFieldOfView = get_fieldOfView();
    }
    if (SetFieldOfView > 0 && GetFieldOfView != 0) {
        set_fieldOfView((float)GetFieldOfView + SetFieldOfView);
    }
    if (SetFieldOfView == 0 && GetFieldOfView != 0) {
        set_fieldOfView((float)GetFieldOfView);
    }
}
bool IsVisible(void* closestEnemy) {
    void* Object = static_cast<void*>(nullptr);
    Vector3 Camera = Transform_INTERNAL_GetPosition(Component_GetTransform(Camera_main()));
    Vector3 Collider = Transform_INTERNAL_GetPosition(Component_GetTransform(Player_GetHeadCollider(closestEnemy)));
    if (!Physics_Raycast(Camera, Collider, 12, &Object)) {
        return true;
    }
    else {
        return false;
    }
}


void UpdateSpeedhack() {
    if (Enable) {
        void* Simulation = GetSimulationTimer();
     if (Simulation != nullptr) {
   float FixedDeltaTime = GetTimer(Simulation);
            if(!saved) {
                active = FixedDeltaTime * 1.82f;
                desactive = FixedDeltaTime;
                saved = true;
            }
   if (SpeedHack) {
       if (FixedDeltaTime != active) {
                    SetTimer(Simulation, active);
    }
   }
   else {
       if (FixedDeltaTime != desactive) {
                    SetTimer(Simulation, desactive);
    }
   }
  }
 }
}

void* GetClosestEnemy() {
    float shortestDistance = MAX_DISTANCE;
    void* closestEnemy = NULL;
    void* get_MatchGame = Curent_Match();
    void* LocalPlayer = GetLocalPlayer(get_MatchGame);
    if (LocalPlayer != NULL && get_MatchGame != NULL && Startgs) {
        monoDictionary<uint8_t*, void**>* players = *(monoDictionary<uint8_t*, void**>**)((long)get_MatchGame + ListPlayer);
        for (int u = 0; u < players->getNumValues(); u++) {
            void* Player = players->getValues()[u];
            if (Player != NULL && !get_isLocalTeam(Player)
                && (ignoreKnockedEnemies || !get_IsDieing(Player))
                && get_isVisible(Player) && get_MaxHP(Player)) {
                Vector3 PlayerPos = getPosition(Player);
                Vector3 LocalPlayerPos = getPosition(LocalPlayer);
                float distance = Vector3::Distance(LocalPlayerPos, PlayerPos);
                Vector3 targetDir = Vector3::Normalized(PlayerPos - LocalPlayerPos);
                float angle = Vector3::Angle(targetDir, GetForward(Component_GetTransform(Camera_main()))) * AIM_ANGLE_MULTIPLIER;
                if (distance < Fov) {
                    if (angle < shortestDistance) {
                        shortestDistance = angle;
                        closestEnemy = Player;
                    }
                }
            }
        }
    }
    return closestEnemy;
}

// ================== PONTO DE MIRA AJUSTADO ==================
// 0=CABEÇA, 1=PESCOÇO, 2=PEITO, 3=QUADRIL
static inline Vector3 GetAdjustedPosition(void* enemy) {
    Vector3 head = GetHeadPosition(enemy);
    Vector3 hip  = GetHipPosition(enemy);

    switch (aimPosition) {
        case 2: // PESCOÇO: ~20% do caminho cabeça→quadril
            return Vector3::Lerp(head, hip, 0.20f);

        case 3: // PEITO: ~55% do caminho quadril→cabeça
            return Vector3::Lerp(hip, head, 0.55f);

        
        case 0:
        default: // CABEÇA
            return head;
    }
}


Quaternion GetCameraRotation(void* localPlayer) {
    return GetRotation(Component_GetTransform(Camera_main()));
}

bool IsInFov(Vector3 enemyPos, Vector3 playerPos, float fovLimit) {
    float dx = enemyPos.x - playerPos.x;
    float dy = enemyPos.y - playerPos.y;
    float dz = enemyPos.z - playerPos.z;

    float distance = sqrtf(dx * dx + dy * dy + dz * dz);
    if (distance == 0) return false;

    float angle = acosf(dx / distance) * (360.0f / 3.14159f);
    return angle <= fovLimit;
}
float (*get_deltaTime)();
float smoothSpeedAIM = 5; 
void *MyPlayer = NULL;
bool Drone;

const float RADAR_SIZE = 200.0f; // kích thước của radar
const ImVec2 RADAR_CENTER(100.0f, 100.0f); // tọa độ trung tâm của radar


   
float x,y;

   float enemy1 = 2.0f;


float aimSmoothness = 0.1f;

inline void DrawAIMBOT(float screenWidth, float screenHeight) {
     
}


inline void DrawESP(float screenWidth, float screenHeight) {
 ImDrawList*draw = ImGui::GetBackgroundDrawList();
    if (!draw) return;   
    
         
if (Aimbot) {
      
    
			void* current_Match = Curent_Match();
		if (current_Match == NULL) return; // Adicionado: Verifica se a partida é válida

		void* local_player = GetLocalPlayer(current_Match);
		if (local_player == NULL) return; // Adicionado: Verifica se o jogador local é válido

		// Adicionado: Verifica se o jogador local está vivo antes de tentar acessar seus atributos
		if (get_IsDieing(local_player)) return; 

		void* closestEnemy = GetClosestEnemy();
		
		// Move esta linha para dentro da verificação de local_player válido
		//*(bool*)((uintptr_t)local_player + 0x688) = GhostHack;

		if (closestEnemy != NULL) { // current_Match e local_player já foram verificados

			Vector3 enemyLocation = GetAdjustedPosition(closestEnemy);
			Vector3 playerLocation = CameraMain(local_player);}
	


    if (closestEnemy != NULL && local_player != NULL && current_Match != NULL) {
        Vector3 enemyLocation   = GetAdjustedPosition(closestEnemy);
        Vector3 playerLocation  = CameraMain(local_player);

		if (AimVisible && !IsVisible(closestEnemy)) return;
			
        Quaternion currentRotation = GetCameraRotation(local_player);
        Quaternion targetRotation  = GetRotationToLocation(enemyLocation, 0.1f, playerLocation);

        Quaternion smoothRotation;
        if (aimSmoothing == 0) {
            smoothRotation = targetRotation;
        } else {
            float smoothFactor = aimSmoothing / 10.0f;
            if (smoothFactor > 1.0f) smoothFactor = 1.0f;
            smoothRotation = Quaternion::Slerp(currentRotation, targetRotation, smoothFactor);
        }

        bool isScopeOn = get_IsSighting(local_player);
        bool isFiring  = get_IsFiring(local_player);

        if (AimbotAuto) {
                set_aim(local_player, smoothRotation);
            } else if (AimbotTrigger && isFiring) {
                set_aim(local_player, smoothRotation);
            } else if (AimbotAim && isScopeOn) {
                set_aim(local_player, smoothRotation);
            } else if (AimbotTrigger && AimbotAim && (isFiring || isScopeOn)) {
                set_aim(local_player, smoothRotation);
        }
    }
}




           

    if (Enable) {   
        int totalEnemies = 0, totalBots = 0;
        void *current_Match = Curent_Match();      
        void* local_player = GetLocalPlayer(current_Match);    
      
            
        if (local_player != nullptr && current_Match != nullptr) {              
            monoDictionary<uint8_t *, void **> *players = *(monoDictionary<uint8_t*, void **> **)((long)current_Match +ListPlayer);
            void *camera = Camera_main();
            if (players != nullptr && camera != nullptr) {
                for(int u = 0; u < players->getNumValues(); u++) {                  
                    void* closestEnemy = players->getValues()[u];
                    if (closestEnemy != local_player && closestEnemy != nullptr && get_isVisible(closestEnemy) && !get_isLocalTeam(closestEnemy)) {
                        
                        
                    Vector3 Toepos = getPosition(closestEnemy);
                    Vector3 Toeposi = WorldToScreenPoint(camera, Vector3(Toepos.x, Toepos.y, Toepos.z));
                    if (Toeposi.z < 1) continue;
        
                    
                    
                    Vector3 HeadPos = getPosition(closestEnemy) + Vector3(0, 1.9f, 0);
                    Vector3 HeadPosition = WorldToScreenPoint(camera, Vector3(HeadPos.x, HeadPos.y, HeadPos.z));
                    if (HeadPosition.z < 1) continue;
        
                  
                    
                    float distance = Vector3::Distance(getPosition(local_player), Toepos);
        float Hight = abs(HeadPosition.y - Toeposi.y) * (1.2 / 1.1);
        float Width = Hight * 0.50f;
        Rect rect = Rect(HeadPosition.x - Width / 2.f, screenHeight - HeadPosition.y, Width, Hight);
    
// LINE
// ESP LINE
if (Config.ESP.Line) {
    // Draw a snapline from top-center of the screen to top of the enemy box
    draw->AddLine(
        ImVec2(screenWidth / 2, 0),                                  // Start at top-center
        ImVec2(rect.x + rect.w / 2, rect.y),                         // End at top of box
        ImColor(255, 255, 255, 255),                                      // White color
        1.7f                                                         // Thickness
    );
}
// --- BOX ESP (with RGB animation) ---
if (Config.ESP.Box) {
    int x = rect.x;
    int y = rect.y;
    int w = rect.w;
    int h = rect.h;

    // -------------------------
    // ✅ RGB Animated Box Color
    // -------------------------
    static float hueBox = 0.0f;
    hueBox += 0.002f; // Speed of color cycle (higher = faster)
    if (hueBox > 1.0f)
        hueBox = 0.0f;

    float rB, gB, bB;
    ImGui::ColorConvertHSVtoRGB(hueBox, 1.0f, 1.0f, rB, gB, bB);
    ImColor rgbBoxColor = ImColor(rB, gB, bB);

    // -------------------------
    // Draw RGB ESP Box
    // -------------------------
    draw->AddRect(
        ImVec2(x, y),
        ImVec2(x + w, y + h),
        rgbBoxColor,
        0.0f,      // No corner rounding
        0,         // Flags (not used)
        2.0f       // Line thickness
    );

    // Optional: Add double-outline for style (inner thin layer)
    draw->AddRect(
        ImVec2(x + 1, y + 1),
        ImVec2(x + w - 1, y + h - 1),
        ImColor(rB, gB, bB, 0.6f), // semi-transparent inner color
        0.0f,
        0,
        1.0f
    );
}


    // Optional: Draw name above box
    
if (Config.ESP.Info) {
    // 1. Get nickname
    monoString* Nick = get_NickName(closestEnemy);
    std::string nameStr;

    if (Nick != NULL) {
        int nick_Len = Nick->getLength();
        for (int i = 0; i < nick_Len; i++) {
            char data = get_Chars(Nick, i);
            nameStr += isascii(data) ? data : '?';
        }
    }

    // 2. Get distance
    std::string distStr = "[" + int_to_string((int)distance) + "M]";

    // -------------------------
    // ✅ Dynamic RGB / Distance Color System
    // -------------------------
    static float hue = 0.0f;
    hue += 0.002f; // speed of color cycle
    if (hue > 1.0f) hue = 0.0f;

    float r, g, b;
    ImGui::ColorConvertHSVtoRGB(hue, 1.0f, 1.0f, r, g, b);

    // RGB cycling for name
    ImColor nameColor = ImColor(r, g, b);

    // Distance-based color (green = near, red = far)
    float normalizedDist = std::min(distance / 300.0f, 1.0f);
    ImColor distColor = ImColor(normalizedDist, 1.0f - normalizedDist, 0.0f);

    // Optional fixed font size
    float fontSize = 13.0f;

    // -------------------------
    // Draw name (top of box)
    // -------------------------
    ImVec2 nameSize = ImGui::CalcTextSize(nameStr.c_str());
    float nameX = rect.x + (rect.w / 2.0f) - (nameSize.x / 2.0f);
    float nameY = rect.y - nameSize.y - 2.0f;

    draw->AddText(NULL, fontSize, ImVec2(nameX, nameY), nameColor, nameStr.c_str());

    // -------------------------
    // Draw distance (bottom of box)
    // -------------------------
    ImVec2 distSize = ImGui::CalcTextSize(distStr.c_str());
    float distX = rect.x + (rect.w / 2.0f) - (distSize.x / 2.0f);
    float distY = rect.y + rect.h + 2.0f;

    draw->AddText(NULL, fontSize, ImVec2(distX, distY), distColor, distStr.c_str());
}

// -------------------------
// ✅ Player Count Display (center top)
// -------------------------
if (totalEnemies > 0) {
    std::string topText = " PLAYERS: ";
    std::string enemiesCount = std::to_string(totalEnemies);
    std::string fullText = topText + enemiesCount;

    ImVec2 textSize = ImGui::CalcTextSize(fullText.c_str());
    ImVec2 textPos(screenWidth / 2.0f - textSize.x / 2.0f, 65);

    // Animated RGB color for player count
    static float hueP = 0.0f;
    hueP += 0.001f;
    if (hueP > 1.0f) hueP = 0.0f;
    float rP, gP, bP;
    ImGui::ColorConvertHSVtoRGB(hueP, 1.0f, 1.0f, rP, gP, bP);

    draw->AddText(NULL, 30.0f, textPos, ImColor(rP, gP, bP), fullText.c_str());
}

        

       } 
      }
     }
   }                  
  }

    
    
    
    
    }
    
    
    
 

   bool(*_ResetGuest)(void* _this);
bool ResetGuest(void* _this){
    if (_this != NULL) {
        if (Reset) {
            return true; 
            remove(OBFUSCATE("/storage/emulated/0/com.garena.msdk/guest100067.dat"));
        }
    }
} 
    



     
     
 
bool(*Bypass)(void* bypass); 
bool _Bypass(void* bypass) { 
   return false;
}

