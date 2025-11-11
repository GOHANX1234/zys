#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <Il2Cpp.h>
#include <Tools.h>
#include <Vector3.hpp>
#include <Color.h>
#include "../../../Struct/obfuscate.h"
#include "Unity/unity.h"
#include "Class.h"
#include "Rect.h"
#include "Bools.h"
#include <math.h>

inline static int g_GlHeight, g_GlWidth;

// Master ESP colors
ImVec4 normalESPColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // White
ImVec4 knockedESPColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red

// Unified color selector
ImColor GetESPColor(bool isKnocked) {
    ImVec4 c = isKnocked ? knockedESPColor : normalESPColor;
    return ImColor(c.x * 255, c.y * 255, c.z * 255, c.w * 255);
}

struct cfg {
    bool aimbot;
    int aim_type = 0;
    struct _esp {
        bool Box = false;
        bool Line = false;
        bool Info = false;
        bool Health = false;
        bool Distance = false;
    }ESP;
}inline Config;

bool ARModsAimbot = false;
bool AimWhen2 = false;
bool Enable = true;

static float SetFieldOfView = 0, GetFieldOfView = 0;
float smoothSpeedAIM = 5; 
void *MyPlayer = NULL;

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

Quaternion GetRotationToTheLocation(Vector3 Target, float Height, Vector3 MyEnemy) {
    return Quaternion::LookRotation((Target + Vector3(0, Height, 0)) - MyEnemy, Vector3(0, 1, 0));
}

bool isVisible(void *enemy)  {
    if(enemy != NULL)  {
         void *hitObj = NULL;
         auto Camera = Transform_GetPosition(Component_GetTransform(Camera_main()));
         auto Target = Transform_GetPosition(Component_GetTransform(Player_GetHeadCollider(enemy)));
         return !Physics_Raycast(Camera, Target, 12, &hitObj);
    }
    return false;
}

void *GetClosestEnemy() {
    float shortestDistance = 9999;
    void* closestEnemy = NULL;   
     
    void* get_MatchGame = Curent_Match();
    void* LocalPlayer = GetLocalPlayer(get_MatchGame);

    if (LocalPlayer != NULL && get_MatchGame != NULL && Enable && get_MatchGame) {
        monoDictionary<uint8_t *, void **> *players = *(monoDictionary<uint8_t*, void **> **)((long)get_MatchGame + ListPlayer);
        
        for (int u = 0; u < players->getNumValues(); u++) {
            void* Player = players->getValues()[u]; 
            if (Player != NULL && !get_isLocalTeam(Player) && !get_IsDieing(Player) && get_isVisible(Player) && get_MaxHP(Player)) {    
                Vector3 PlayerPos = getPosition(Player);
                Vector3 LocalPlayerPos = getPosition(LocalPlayer);
                Vector3 pos2 = WorldToScreenPoint(Camera_main(), PlayerPos);
                bool isFov1 = isFov(Vector3(pos2.x, pos2.y), Vector3(g_GlWidth / 2, g_GlHeight / 2), Fov_Aim);
                
                float distance = Vector3::Distance(LocalPlayerPos, PlayerPos);
                if (distance < 1000) {
                    Vector3 targetDir = Vector3::Normalized(PlayerPos - LocalPlayerPos);
                    float angle = Vector3::Angle(targetDir, GetForward(Component_GetTransform(Camera_main()))) * 100.0;             
            
                    if (angle <= Fov_Aim && isFov1 && angle < shortestDistance) {
                     if(isVisible(Player)) {
                        shortestDistance = angle;
                        closestEnemy = Player;
                    }
                   }
                }
            }
        }
    }
    return closestEnemy;
}

// Headshot Aimbot
void* GetBestVisibleEnemy(void* local, void* match) {
    void* bestEnemy = nullptr;
    float minAngle = 9999.0f;

    if (!local || !match) return nullptr;

    auto* players = *(monoDictionary<uint8_t*, void**>**)((uintptr_t)match + ListPlayer);
    if (!players) return nullptr;

    Vector3 localPos = getPosition(local);
    Vector3 camForward = GetForward(Component_GetTransform(Camera_main()));

    for (int i = 0; i < players->getNumValues(); i++) {
        void* player = players->getValues()[i];
        if (!player || get_isLocalTeam(player) || get_IsDieing(player) || get_MaxHP(player) <= 0) continue;
        if (!isVisible(player)) continue;

        Vector3 enemyPos = getPosition(player);
        float dist = Vector3::Distance(localPos, enemyPos);
        if (dist > 99999.0f) continue;

        // For very close enemies (< 5 units), prioritize by distance
        if (dist < 5.0f) {
            if (dist < minAngle) {
                minAngle = dist;
                bestEnemy = player;
            }
        } else {
            // Normal range: use FOV and angle-based targeting
            Vector3 screen = WorldToScreenPoint(Camera_main(), enemyPos);
            bool insideFov = isFov(Vector3(screen.x, screen.y), Vector3(g_GlWidth / 2, g_GlHeight / 2), Fov_Aim);
            if (!insideFov) continue;

            Vector3 dir = Vector3::Normalized(enemyPos - localPos);
            float angle = Vector3::Angle(dir, camForward) * 100.0f;

            if (angle < minAngle) {
                minAngle = angle;
                bestEnemy = player;
            }
        }
    }
    return bestEnemy;
}

void (*LateUpdate)(void* Player);
void _LateUpdate(void* Player) {
    if (!Player || !ARModsAimbot) {
        LateUpdate(Player);
        return;
    }
    void* match = Curent_Match();
    void* local = GetLocalPlayer(match);
    if (!match || !local) {
        LateUpdate(Player);
        return;
    }
    void* enemy = GetBestVisibleEnemy(local, match);
    if (!enemy) {
        LateUpdate(Player);
        return;
    }
    Vector3 enemyHead = GetHeadPosition(enemy);
    Vector3 localCam = CameraMain(local);
    Quaternion aimRot = GetRotationToTheLocation(enemyHead, 0.045f, localCam);

    bool isFiring = get_IsFiring(local);

    if (AimWhen2 && isFiring) {
        set_aim(local, aimRot);
    } else if (!AimWhen2) {
        set_aim(local, aimRot);
    }
    LateUpdate(Player);
}

inline void DrawESP(float screenWidth, float screenHeight) {
    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    if (!draw) return;

    if (Enable) {
        void* current_Match = Curent_Match();
        void* local_player = GetLocalPlayer(current_Match);

        if (local_player && current_Match) {
            auto* players = *(monoDictionary<uint8_t*, void**>**)((long)current_Match + ListPlayer);
            void* camera = Camera_main();

            if (players && camera) {
                for (int u = 0; u < players->getNumValues(); u++) {
                    void* closestEnemy = players->getValues()[u];
                    if (closestEnemy != local_player && closestEnemy && get_isVisible(closestEnemy) && !get_isLocalTeam(closestEnemy)) {
                        Vector3 Toepos = getPosition(closestEnemy);
                        Vector3 Toeposi = WorldToScreenPoint(camera, Toepos);
                        if (Toeposi.z < 1) continue;

                        Vector3 HeadPos = getPosition(closestEnemy) + Vector3(0, 1.9f, 0);
                        Vector3 HeadPosition = WorldToScreenPoint(camera, HeadPos);
                        if (HeadPosition.z < 1) continue;

                        // FOV Circle
                        draw->AddCircle(ImVec2(screenWidth / 2, screenHeight / 2), Fov_Aim, ImColor(255, 255, 255), 0, 1.5f);

                        float distance = Vector3::Distance(getPosition(local_player), Toepos);
                        float Height = abs(HeadPosition.y - Toeposi.y) * (1.2 / 1.1);
                        float Width = Height * 0.50f;
                        Rect rect = Rect(HeadPosition.x - Width / 2.f, screenHeight - HeadPosition.y, Width, Height);

                        // ESP Line
                        if (Config.ESP.Line) {
                            if(get_IsDieing(closestEnemy)) {
                                draw->AddLine(ImVec2(screenWidth / 2, 80), ImVec2(rect.x + rect.w / 2, rect.y + rect.h / 35), ImColor(255, 0, 0), 1.7);
                            } else {
                                draw->AddLine(ImVec2(screenWidth / 2, 80), ImVec2(rect.x + rect.w / 2, rect.y + rect.h / 35), ImColor(255, 255, 255), 1.7);
                            }
                        }

                        // ESP Box
                        if (Config.ESP.Box) {
                            int x = rect.x;
                            int y = rect.y;
                            int _width = rect.w;
                            int _height = rect.h;

                            if (get_IsDieing(closestEnemy)) {
                                draw->AddRect(ImVec2(x, y), ImVec2(x + _width, y + _height), ImColor(255, 0, 0), 0, 0, 2.0f);
                            } else {
                                // Corner-style box for alive enemies
                                ImColor color = ImColor(255, 255, 255);
                                float thickness = 2.0f;

                                // Top corners
                                draw->AddLine(ImVec2(x, y), ImVec2(x + 10, y), color, thickness);
                                draw->AddLine(ImVec2(x, y), ImVec2(x, y + 10), color, thickness);
                                draw->AddLine(ImVec2(x + _width, y), ImVec2(x + _width - 10, y), color, thickness);
                                draw->AddLine(ImVec2(x + _width, y), ImVec2(x + _width, y + 10), color, thickness);

                                // Bottom corners
                                draw->AddLine(ImVec2(x, y + _height), ImVec2(x + 10, y + _height), color, thickness);
                                draw->AddLine(ImVec2(x, y + _height), ImVec2(x, y + _height - 10), color, thickness);
                                draw->AddLine(ImVec2(x + _width, y + _height), ImVec2(x + _width - 10, y + _height), color, thickness);
                                draw->AddLine(ImVec2(x + _width, y + _height), ImVec2(x + _width, y + _height - 10), color, thickness);
                            }
                        }

                        // Health Bar
                        if (Config.ESP.Health) {
                            int xx = rect.x + rect.w + 2;
                            int yy = rect.y;

                            if (!get_IsDieing(closestEnemy)) {
                                float maxHealth = get_MaxHP(closestEnemy);
                                float currentHealth = GetHp(closestEnemy);
                                float healthPercentage = currentHealth / maxHealth;

                                ImVec4 healthColor = ImLerp(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), 1.0f - healthPercentage);

                                float barWidth = 5.0f;
                                float barHeight = rect.h * healthPercentage;

                                ImVec2 barPos(xx + 1, yy + rect.h - barHeight);
                                ImVec2 barEnd(xx + 4, yy + rect.h);

                                draw->AddRectFilled(ImVec2(xx, yy), ImVec2(xx + barWidth, yy + rect.h), ImColor(0, 0, 0, 255));
                                draw->AddRectFilled(barPos, barEnd, ImColor(healthColor));
                                draw->AddRect(ImVec2(xx, yy), ImVec2(xx + barWidth, yy + rect.h), IM_COL32(255, 255, 255, 255));
                            }
                        }

                        // Player Info
                        if (Config.ESP.Info) {
                            monoString* Nick = get_NickName(closestEnemy);
                            std::string names;

                            if (Nick != nullptr) {
                                int len = Nick->getLength();
                                for (int i = 0; i < len; i++) {
                                    char c = get_Chars(Nick, i);
                                    names += (isascii(c) && isprint(c)) ? c : '?';
                                }

                                float smallSize = 12.0f;
                                ImVec2 nameSize = ImGui::CalcTextSize(names.c_str());
                                nameSize.x *= smallSize / ImGui::GetFontSize();
                                nameSize.y *= smallSize / ImGui::GetFontSize();

                                float nameX = rect.x + (rect.w / 2.0f) - (nameSize.x / 2.0f);
                                float nameY = rect.y - nameSize.y - 5.0f;

                                ImU32 textColor = GetESPColor(get_IsDieing(closestEnemy));
                                ImU32 shadowColor = IM_COL32(0, 0, 0, 200);

                                // Text shadow
                                for (int dx = -1; dx <= 1; dx++) {
                                    for (int dy = -1; dy <= 1; dy++) {
                                        if (dx != 0 || dy != 0)
                                            draw->AddText(ImGui::GetFont(), smallSize, ImVec2(nameX + dx, nameY + dy), shadowColor, names.c_str());
                                    }
                                }
                                draw->AddText(ImGui::GetFont(), smallSize, ImVec2(nameX, nameY), textColor, names.c_str());
                            }
                        }

                        // Distance
                        if (Config.ESP.Distance) {
                            char distText[32];
                            snprintf(distText, sizeof(distText), "%.2fm", distance);

                            float smallSize = 12.0f;
                            ImVec2 distSize = ImGui::CalcTextSize(distText);
                            distSize.x *= smallSize / ImGui::GetFontSize();
                            distSize.y *= smallSize / ImGui::GetFontSize();

                            float distX = rect.x + (rect.w / 2.0f) - (distSize.x / 2.0f);
                            float distY = rect.y + rect.h + 5.0f;

                            ImU32 textColor = GetESPColor(get_IsDieing(closestEnemy));
                            ImU32 shadowColor = IM_COL32(0, 0, 0, 200);

                            // Text shadow
                            for (int dx = -1; dx <= 1; dx++) {
                                for (int dy = -1; dy <= 1; dy++) {
                                    if (dx != 0 || dy != 0)
                                        draw->AddText(ImGui::GetFont(), smallSize, ImVec2(distX + dx, distY + dy), shadowColor, distText);
                                }
                            }
                            draw->AddText(ImGui::GetFont(), smallSize, ImVec2(distX, distY), textColor, distText);
                        }
                    }
                }
            }
        }
    }
}
