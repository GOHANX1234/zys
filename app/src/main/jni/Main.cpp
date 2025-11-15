#include <android/log.h>
#include <unistd.h>
#include <thread>
#include <limits>

#include <xdl.h>
#include <KittyUtils.h>
#include <KittyMemory.h>
#include <Il2Cpp.h>
#include <SubstrateHook.h>
#include <CydiaSubstrate.h>
#include "Struct/Gui.hpp"
#include <Struct/main.h>
#include "fonts/FontAwesome6_solid.h"
#include "ImGui/Toggle.h"
#include "zygisk.hpp"
#include "ff.h"
#include "ItSRohitOp.h"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

void hack();

class MyModule : public zygisk::ModuleBase {
 public:
  void onLoad(Api *api, JNIEnv *env) override {
    this->api_ = api;
    this->env_ = env;
    genv = env;
  }
  void preAppSpecialize(AppSpecializeArgs *args) override {
    static constexpr const char *packages[] = {
        "com.dts.freefiremax", "com.dts.freefireth"
    }; // Game Package Names
    const char *process = env_->GetStringUTFChars(args->nice_name, nullptr);
    for (const auto *package: packages) {
      is_game_ = (strcmp(process, package) == 0);
      if (is_game_) {
        break;
      }
    }
    env_->ReleaseStringUTFChars(args->nice_name, process);
  }
  void postAppSpecialize(const AppSpecializeArgs *args) override {
    if (is_game_) {
       genv->GetJavaVM(&jvm);
      std::thread{hack}.detach();
    }
  }
 private:
  Api *api_ = nullptr;
  JNIEnv *env_ = nullptr;
  bool is_game_ = false;
};


void DrawText(std::string text) {
ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
ImGui::Text(text.c_str());
}
//========BYPASS========\\

void SetDarkGrayTheme()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
    colors[ImGuiCol_Border]                 = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
    colors[ImGuiCol_CheckMark]              = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
    colors[ImGuiCol_Button]                 = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
    colors[ImGuiCol_Header]                 = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
    colors[ImGuiCol_Separator]              = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

    style->ChildRounding = 4.0f;
    style->FrameBorderSize = 1.0f;
    style->FrameRounding = 2.0f;
    style->GrabMinSize = 7.0f;
    style->PopupRounding = 2.0f;
    style->ScrollbarRounding = 12.0f;
    style->ScrollbarSize = 13.0f;
    style->TabBorderSize = 1.0f;
    style->TabRounding = 0.0f;
    style->WindowRounding = 4.0f;
}


void(*Bypass1)(void* _this);
void _Bypass1(void* _this) {
    return;
}

void(*Bypass2)(void* _this);
void _Bypass2(void* _this) {
    return;
}


// ========================= \\

uintptr_t il2cpp_base = 0;
void *getRealAddr(ulong offset) {
    return reinterpret_cast<void*>(il2cpp_base + offset);
};

namespace Settings
{
    static int Tab = 1;
}


bool clearMousePos = true;
bool initImGui = false;

inline EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
inline EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &g_GlWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_GlHeight);

    if (!g_IsSetup) {
        prevWidth = g_GlWidth;
        prevHeight = g_GlHeight;
        SetupImgui();
        SetupCustomStyle();
        g_IsSetup = true;
    }

    ImGuiIO &io = ImGui::GetIO();
    //SetDarkGrayTheme();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(g_GlWidth, g_GlHeight);
    ImGui::NewFrame();
    int touchCount = (((int (*)())(Class_Input__get_touchCount))());
    if (touchCount > 0) {
        UnityEngine_Touch_Fields touch = ((UnityEngine_Touch_Fields(*)(int))(Class_Input__GetTouch))(0);
        float reverseY = io.DisplaySize.y - touch.m_Position.fields.y;
        switch (touch.m_Phase) {
            case TouchPhase::Began:
            case TouchPhase::Stationary:
                io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
                UpdateSpeedhack();
                io.MouseDown[0] = true;
                break;
            case TouchPhase::Ended:
            case TouchPhase::Canceled:
                io.MouseDown[0] = false;
                clearMousePos = true;
                break;
            case TouchPhase::Moved:
                io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
                break;
            default:
                break;
        }
    }
	
DrawESP(g_GlWidth, g_GlHeight);

/*
static bool itsmk = true;
static bool clickInProgress = false;
static float clickStartTime = 0;
if (itsmk == false) {
if (ImGui::Begin((" MKOP" ), 0,ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground)) {
 
const auto& pWindowDrawList = ImGui::GetWindowDrawList();
const auto& p = ImGui::GetWindowPos();
ImGui::SetCursorPos(ImVec2(5, 5));
ImVec2 imageSize(100,100);
ImGui::Image(mkop.textureId, imageSize);
if (ImGui::IsItemHovered()) {
if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {clickInProgress = true;
clickStartTime = ImGui::GetTime();}}
if (clickInProgress && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {clickInProgress = false;
float currentTime = ImGui::GetTime();
float clickDuration = currentTime - clickStartTime;
if (clickDuration < 0.2f) {itsmk = true;
}}}}

ImGui::End();

                    
if (itsmk){

// Automatically Size Calculate 
	ImGuiIO& io = ImGui::GetIO();

// Get screen center position
ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

// Set next window position to center (only first time)
ImGui::SetNextWindowPos(center, ImGuiCond_Once, ImVec2(0.5f, 0.5f));

    if  (ImGui::Begin(("Mkop Testing new Layout "), 0 ,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus)){


// Centered top text that acts as a clickable "close" trigger


ImGui::PopStyleColor(3);


    DrawText("POWER CHEAT SAFE");

static bool isLogin = ;
    ImGui::Separator();
    if (!isLogin) {
ImGui::Spacing();
        ImGui::Text("Power Cheat Access");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Use USER.");
        ImGui::Text("Copy to clipboard then press the button below.");
        ImGui::Spacing();

        static char s[128] = "";
        ImGui::PushItemWidth(-1);
        ImGui::InputText("##key", s, sizeof(s));
        ImGui::PopItemWidth();

        ImGui::Spacing();
        if (ImGui::Button("PASTE & SIGN IN", ImVec2(ImGui::GetContentRegionAvailWidth(), 45)))
        {
            std::string key = getClipboard();
            strncpy(s, key.c_str(), sizeof(s));
        }

        static std::string err;
        if (ImGui::Button("LOGIN", ImVec2(ImGui::GetContentRegionAvailWidth(), 45)))
        {
            err = Login(s);
            if (err == "OK")
                isLogin = bValid && g_Auth == g_Token;
        }

        ImGui::Spacing();
        ImGui::Text("COPY YOUR CREDENTIALS AND PRESS THE BUTTON ABOVE");

        if (!err.empty() && err != "OK")
        {
            ImGui::Spacing();
}} else {
       //   static bool isLogin = false;

	
            
            */
            
            
            
//    ImGui::Text("STATE: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(0,1,0,1), "ACTIVE");
   // ImGui::Text("ADMIN: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1,0,0,1), "SARIFGAMING");
   // ImGui::Text("EXPIRY: 2025-10-17 11:37");
    //ImGui::Text("USER: Divyansh");

    // =============================================================
//  XReg-Style ImGui Full UI with Close & Minimize (final)
//  Keeps your same logic & itsmk system
// =============================================================

static bool itsmk = true;
static bool clickInProgress = false;
static float clickStartTime = 0;
if (itsmk == false) {
if (ImGui::Begin((" MKOP" ), 0,ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground)) {
 
const auto& pWindowDrawList = ImGui::GetWindowDrawList();
const auto& p = ImGui::GetWindowPos();
ImGui::SetCursorPos(ImVec2(5, 5));
ImVec2 imageSize(100,100);
ImGui::Image(mkop.textureId, imageSize);
if (ImGui::IsItemHovered()) {
if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {clickInProgress = true;
clickStartTime = ImGui::GetTime();}}
if (clickInProgress && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {clickInProgress = false;
float currentTime = ImGui::GetTime();
float clickDuration = currentTime - clickStartTime;
if (clickDuration < 0.2f) {itsmk = true;
}}}}

ImGui::End();
// ---------------------- MAIN PANEL ----------------------
if (itsmk)
{
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 center(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Once, ImVec2(0.5f, 0.5f));

    // XReg-style color palette
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.12f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.13f, 0.13f, 0.16f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.25f, 0.25f, 0.30f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.20f, 0.30f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.20f, 0.25f, 0.40f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.10f, 0.30f, 0.50f, 1.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 6));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));

    if (ImGui::Begin("STREAMERX", 0,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
    {
        // ======================
        //  TOP TITLE BAR
        // ======================
      
        static bool isLogin = false;

    if (!isLogin) {
ImGui::Spacing();
        

      //  ImGui::Text("Use USER.");
        ImGui::Text("Copy to paste then press the button below.");
        ImGui::Spacing();

       ImGui::PushItemWidth(-1);
static char s[64];
ImGui::InputText("##key", s, sizeof s);
ImGui::PopItemWidth();
if (ImGui::Button( " PASTE KEY", ImVec2(ImGui::GetContentRegionAvailWidth(), 55))) {
auto key = getClipboard();
strncpy(s, key.c_str(), sizeof s); }
static std::string err;
if (ImGui::Button( " LOGIN TO CONTINUE " , ImVec2(ImGui::GetContentRegionAvailWidth(), 55))) {
 err = Login(s);
if (err == "OK") {
isLogin = bValid && g_Auth == g_Token;
} }
if (!err.empty() && err != "OK") {
ImGui::Text( "SERVER: %s", err.c_str());
}} else {
        // ======================
        //  FEATURE PANELS
        // ======================
        // ======== INFINITY TEAM PRO V3 UI ========





    // ===== Header =====
    ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "STREAMERX BRUTAL");
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "STATUS: INJECTED");
    //ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "GOD: AYUSH");
    ImGui::Spacing();

    // ===== Tabs =====
    if (ImGui::BeginTabBar("Tabs"))
    {
        // ==== Overview ====
        if (ImGui::BeginTabItem("Overview"))
        {
            ImGui::Text("Welcome to STREAMERX BRUTAL");
            ImGui::Text("Choose your desired feature tab above.");
            ImGui::EndTabItem();
        }

        // ==== ESP ====
        if (ImGui::BeginTabItem("ESP"))
        {
            ImGui::Spacing();
            ImGui::Text("ESP SETTINGS");
            ImGui::Separator();
            ImGui::Spacing();

            Toggle("ENABLE FEATURES", &Enable);
            Toggle("ESP LINE", &Config.ESP.Line);
            Toggle("ESP BOX", &Config.ESP.Box);
            Toggle("ESP NAME", &Config.ESP.Info);
            
			
            ImGui::EndTabItem();
        }

        // ==== Aimbot ====
        if (ImGui::BeginTabItem("Aimbot"))
        {
            ImGui::Spacing();
            ImGui::Text("AIMBOT SETTINGS");
            ImGui::Separator();
            ImGui::Spacing();

           Toggle("ENABLE AIMBOT", &Aimbot);
           Toggle("ENABLE SPEED", &SpeedHack);
            ImGui::SliderInt("AIM FOV", &Fov, 0, 360);

            const char* items[] = { "CABEÇA", "PESCOÇO", "PEITO" };
            const char* current_item = items[aimPosition];
            if (ImGui::BeginCombo("LOCAL AIM", current_item))
            {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                {
                    bool is_selected = (aimPosition == n);
                    if (ImGui::Selectable(items[n], is_selected))
                        aimPosition = n;
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::Text("TYPE OF AIMBOT:");
            if (ImGui::RadioButton("AO SHOOTING", !AimbotAuto))
            {
                AimbotAuto = false;
                HidePanel = true;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("AO LOOK", AimbotAuto))
            {
                AimbotAuto = true;
                HidePanel = false;
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    // ===== Bottom Section =====
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();

    float windowWidth = ImGui::GetWindowSize().x;
    ImVec2 buttonSize(windowWidth - 40, 35);
    float buttonX = (windowWidth - buttonSize.x) * 0.5f;
    ImGui::SetCursorPosX(buttonX);
    ImGui::Spacing();

    // Green rounded MINIMIZE button
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.80f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.90f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.70f, 0.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

    if (ImGui::Button("MINIMIZE", buttonSize))
    {
        itsmk = false;
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);

    ImGui::End();
}
}
    ImGui::PopStyleColor(6);
    ImGui::PopStyleVar(4);
}
ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        return old_eglSwapBuffers(dpy, surface);
}




typedef unsigned long DWORD;
static uintptr_t libBase;


inline void StartGUI() {
    void *ptr_eglSwapBuffer = DobbySymbolResolver("/system/lib/libEGL.so", "eglSwapBuffers");
    if (ptr_eglSwapBuffer != nullptr) {
        DobbyHook((void *)ptr_eglSwapBuffer, (void *)hook_eglSwapBuffers, (void **)&old_eglSwapBuffers);
        LOGD("GUI started successfully");
    }
}

void hack_thread(pid_t pid) {
    LOGD("Hack Thread initiated for PID: %i", pid);

    for (int i = 0; i < 10; i++) {
        il2cpp_base = get_module_base(pid, "libil2cpp.so");
        if (il2cpp_base != 0) break;
        sleep(10);
    }

    if (il2cpp_base == 0) {
        LOGE("libil2cpp.so not found in thread %d", pid);
        std::terminate();
    }

    LOGD("IL2CPP base address: 0x%" PRIxPTR, il2cpp_base);
    sleep(10);

    Il2CppAttach();
	// If you have Your own bypass then past here 🙂 
	// =======
	
    StartGUI();
}

    
    
void hack() {
     LOGD("Inject Ok");
    std::thread thread_hack(hack_thread, getpid());
    thread_hack.detach();
    
}

REGISTER_ZYGISK_MODULE(MyModule)

