#pragma once

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <dlfcn.h>
#include <vector>

#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include "main.h"
#include "Struct/tools.hpp"

#include "Icon/OPPOSans-H.h"
#include "Icon/Icon.h"
#include "Icon/Iconcpp.h"
#include "Icon/Font.h"

#include <Struct/ESP.h>

#include "imgui/stb_image.h"
#include "mkop.h"


// @ItSMKOP 
inline static bool g_IsSetup = false;
inline int prevWidth, prevHeight;


inline void (*origInput)(void*, void*, void*);
inline void myInput(void *thiz, void *ex_ab, void *ex_ac) {
    origInput(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
    return;
}


float density = -1;

ImFont *font;
/*
inline void SetupImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

   
    io.DisplaySize = ImVec2((float)g_GlWidth, (float)g_GlHeight);
   
     
    ImGui::StyleColorsDark();
    
    
    ImGui_ImplOpenGL3_Init("#version 300 es");
    
    
    static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
            ImFontConfig icons_config;
   
           ImFontConfig CustomFont;
            CustomFont.FontDataOwnedByAtlas = false;

            icons_config.MergeMode = true;
            icons_config.PixelSnapH = true;
            icons_config.OversampleH = 2.5;
            icons_config.OversampleV = 2.5;
            
            
            io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 20.f, &CustomFont);
            io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 20.0f, &icons_config, icons_ranges);

            ImFontConfig cfg;
            cfg.SizePixels = ((float) density / 20.0f);
            io.Fonts->AddFontDefault(&cfg);

    // Arbitrary scale-up
    ImGui::GetStyle().ScaleAllSizes(3.0f);
}
*/

struct TextureInfo { ImTextureID textureId; int x; int y; int w; int h; };

void DrawImage(int x, int y, int w, int h, ImTextureID Texture) {
ImGui::GetForegroundDrawList()->AddImage(Texture, ImVec2(x, y), ImVec2(x + w, y + h));}


static TextureInfo textureInfo;
TextureInfo createTexture(char *ImagePath) {
int w, h, n;
stbi_uc *data = stbi_load(ImagePath, &w, &h, &n, 0);
GLuint texture;
glGenTextures(1, &texture);
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
if (n == 3) {
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
} else {
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}stbi_image_free(data);
textureInfo.textureId = reinterpret_cast<ImTextureID>((GLuint *) texture);
textureInfo.w = w;
textureInfo.h = h;
return textureInfo;
}TextureInfo CreateTexture(const unsigned char* buf, int len) {
TextureInfo image;
unsigned char* image_data = stbi_load_from_memory(buf, len, &image.w, &image.h, NULL, 0);
if (image_data == NULL) perror("文件不存在"); GLuint image_texture;
glGenTextures(1, &image_texture);
glBindTexture(GL_TEXTURE_2D, image_texture);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //对于非二次方纹理，这在 WebGL 上是必需的
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //相同的
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.w, image.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
stbi_image_free(image_data);
image.textureId = (ImTextureID)image_texture;
return image;
}

//LOGO
TextureInfo mkop;
extern TextureInfo mkop;



void InitTexture() {
mkop = CreateTexture(mkop_data, sizeof(mkop_data));
}

void SetupCustomStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    
    // --- Define Main Colors ---

    // Dark panel/window background (a very dark gray/black)
    // --- Custom Color Scheme ---

// Main background (slightly transparent dark)
//ImGuiStyle& style = ImGui::GetStyle();

const ImVec4 main_bg_dark          = ImVec4(0.08f, 0.08f, 0.10f, 0.70f); // ~R20 G20 B25
const ImVec4 item_bg_dark          = ImVec4(0.12f, 0.12f, 0.15f, 1.00f); // ~R30 G30 B38
const ImVec4 accent_green          = ImVec4(0.00f, 1.00f, 0.00f, 0.90f); // ~R0 G255 B0
const ImVec4 accent_green_active   = ImVec4(0.00f, 0.75f, 0.00f, 1.00f);
const ImVec4 white_text            = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

// --- Text ---
style.Colors[ImGuiCol_Text]                 = white_text;
style.Colors[ImGuiCol_TextDisabled]         = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

// --- Backgrounds ---
style.Colors[ImGuiCol_WindowBg]             = main_bg_dark;
style.Colors[ImGuiCol_ChildBg]              = main_bg_dark;
style.Colors[ImGuiCol_PopupBg]              = ImVec4(0.10f, 0.10f, 0.12f, 0.94f);

// --- Frames / Widgets ---
style.Colors[ImGuiCol_FrameBg]              = item_bg_dark;
style.Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);
style.Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);

// --- Title Bar ---
style.Colors[ImGuiCol_TitleBg]              = ImVec4(0.05f, 0.05f, 0.07f, 1.00f);
style.Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
style.Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.05f, 0.05f, 0.07f, 0.51f);

// --- Buttons ---
style.Colors[ImGuiCol_Button]               = accent_green;
style.Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.20f, 1.00f, 0.20f, 1.00f);
style.Colors[ImGuiCol_ButtonActive]         = accent_green_active;

// --- Sliders ---
style.Colors[ImGuiCol_SliderGrab]           = accent_green;
style.Colors[ImGuiCol_SliderGrabActive]     = accent_green_active;

// --- Checkmarks / Headers / Tabs ---
style.Colors[ImGuiCol_CheckMark]            = accent_green;
style.Colors[ImGuiCol_Header]               = accent_green;
style.Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.30f, 1.00f, 0.30f, 0.80f);
style.Colors[ImGuiCol_HeaderActive]         = accent_green_active;

// --- Separators / Borders ---
style.Colors[ImGuiCol_Separator]            = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);

// --- Scrollbars / Resize ---
style.Colors[ImGuiCol_ResizeGrip]           = ImVec4(0.00f, 1.00f, 0.00f, 0.25f);
style.Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.00f, 1.00f, 0.00f, 0.67f);
style.Colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.00f, 1.00f, 0.00f, 0.95f);
style.Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.10f, 0.10f, 0.12f, 0.53f);
style.Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.25f, 0.25f, 0.30f, 0.80f);
style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.35f, 0.40f, 1.00f);
style.Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.45f, 0.45f, 0.50f, 1.00f);
}


void SetupImgui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    InitTexture();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
   // ImGuiStyle* style = &ImGui::GetStyle();
    io.DisplaySize = ImVec2((float)g_GlWidth, (float)g_GlHeight);

    ImGui::StyleColorsDark();
    
    ImGuiStyle *style = &ImGui::GetStyle();
// --- General Styling and Spacing ---
style->WindowPadding = ImVec2(10, 4);
style->WindowRounding = 12.0f;
style->FramePadding = ImVec2(10, 2);
style->ItemSpacing = ImVec2(8, 8);
style->ItemInnerSpacing = ImVec2(8, 2);
style->FrameRounding = 12.0f;
style->ChildRounding = 8.0f;
style->WindowBorderSize = 0.0f;
style->FrameBorderSize = 0.0f;
style->PopupRounding = 18.0f;
style->ScrollbarSize = 0.0f;
style->ScrollbarRounding = 16.0f;
style->GrabRounding = 16.0f;
style->GrabMinSize = 15.0f;
style->TabRounding = 12.0f;
style->TabBorderSize = 0.0f;
style->ButtonTextAlign = ImVec2(0.5f, 0.5f);
style->WindowTitleAlign = ImVec2(0.5f, 0.5f);



    
    ImGui_ImplOpenGL3_Init("#version 300 es");
    
    // We load the default font with increased size to improve readability on many devices with "high" DPI.
    static const ImWchar icons_ranges[] = {0xf000, 0xf3ff, 0};
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.OversampleH = 2.5;
		icons_config.OversampleV = 2.5;
		ImFontConfig CustomFont;
CustomFont.FontDataOwnedByAtlas = false;

io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 30.f, &CustomFont);


}



    
    



JavaVM *jvm;
JNIEnv *genv;



std::string getClipboard() {
    std::string result;
    
    jvm->AttachCurrentThread(&genv, NULL);
    
    auto looperClass = genv->FindClass(OBFUSCATE("android/os/Looper"));
    auto prepareMethod = genv->GetStaticMethodID(looperClass, OBFUSCATE("prepare"), OBFUSCATE("()V"));
    genv->CallStaticVoidMethod(looperClass, prepareMethod);
    
    jclass activityThreadClass = genv->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jfieldID sCurrentActivityThreadField = genv->GetStaticFieldID(activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
    jobject sCurrentActivityThread = genv->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
    
    jfieldID mInitialApplicationField = genv->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"), OBFUSCATE("Landroid/app/Application;"));
    jobject mInitialApplication = genv->GetObjectField(sCurrentActivityThread, mInitialApplicationField);
    
    auto contextClass = genv->FindClass(OBFUSCATE("android/content/Context"));
    auto getSystemServiceMethod = genv->GetMethodID(contextClass, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
    
    auto str = genv->NewStringUTF(OBFUSCATE("clipboard"));
    auto clipboardManager = genv->CallObjectMethod(mInitialApplication, getSystemServiceMethod, str);
    genv->DeleteLocalRef(str);
    
    jclass ClipboardManagerClass = genv->FindClass(OBFUSCATE("android/content/ClipboardManager"));
    auto getText = genv->GetMethodID(ClipboardManagerClass, OBFUSCATE("getText"), OBFUSCATE("()Ljava/lang/CharSequence;"));

    jclass CharSequenceClass = genv->FindClass(OBFUSCATE("java/lang/CharSequence"));
    auto toStringMethod = genv->GetMethodID(CharSequenceClass, OBFUSCATE("toString"), OBFUSCATE("()Ljava/lang/String;"));

    auto text = genv->CallObjectMethod(clipboardManager, getText);
    if (text) {
        str = (jstring) genv->CallObjectMethod(text, toStringMethod);
        result = genv->GetStringUTFChars(str, 0);
        genv->DeleteLocalRef(str);
        genv->DeleteLocalRef(text);
    }
    genv->DeleteLocalRef(CharSequenceClass);
    genv->DeleteLocalRef(ClipboardManagerClass);
    genv->DeleteLocalRef(clipboardManager);
    genv->DeleteLocalRef(contextClass);
    genv->DeleteLocalRef(mInitialApplication);
    genv->DeleteLocalRef(activityThreadClass);   
    jvm->DetachCurrentThread();
    return result.c_str();
}

jclass UnityPlayer_cls;
jfieldID UnityPlayer_CurrentActivity_fid;



jobject getGlobalContext()
{
    jclass activityThread = genv->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jmethodID currentActivityThread = genv->GetStaticMethodID(activityThread, OBFUSCATE("currentActivityThread"), OBFUSCATE("()Landroid/app/ActivityThread;"));
    jobject at = genv->CallStaticObjectMethod(activityThread, currentActivityThread);
    jmethodID getApplication = genv->GetMethodID(activityThread, OBFUSCATE("getApplication"), OBFUSCATE("()Landroid/app/Application;"));
    jobject context = genv->CallObjectMethod(at, getApplication);
    return context;
}

void displayKeyboard(bool pShow) {
    jclass ctx = genv->FindClass(OBFUSCATE("android/content/Context"));
    jfieldID fid = genv->GetStaticFieldID(ctx, OBFUSCATE("INPUT_METHOD_SERVICE"), OBFUSCATE("Ljava/lang/String;"));
    jmethodID mid = genv->GetMethodID(ctx, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
    jobject context = genv->GetStaticObjectField(UnityPlayer_cls, UnityPlayer_CurrentActivity_fid);
    jobject InputManObj = genv->CallObjectMethod(context, mid, (jstring) genv->GetStaticObjectField(ctx, fid));
    jclass ClassInputMethodManager = genv->FindClass(OBFUSCATE("android/view/inputmethod/InputMethodManager"));
    jmethodID toggleSoftInputId = genv->GetMethodID(ClassInputMethodManager, OBFUSCATE("toggleSoftInput"), OBFUSCATE("(II)V"));
    if (pShow) {
        genv->CallVoidMethod(InputManObj, toggleSoftInputId, 2, 0);
    } else {
        genv->CallVoidMethod(InputManObj, toggleSoftInputId, 0, 0);
    }
}

int ShowSoftKeyboardInput() {
    jint result;
    jint flags = 0;
    
    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);
    
    jclass looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
    auto prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"), OBFUSCATE("()V"));
    env->CallStaticVoidMethod(looperClass, prepareMethod);
    
    jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
    jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
    
    jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"), OBFUSCATE("Landroid/app/Application;"));
    jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);
    
    jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
    jfieldID fieldINPUT_METHOD_SERVICE = env->GetStaticFieldID(contextClass, OBFUSCATE("INPUT_METHOD_SERVICE"), OBFUSCATE("Ljava/lang/String;"));
    jobject INPUT_METHOD_SERVICE = env->GetStaticObjectField(contextClass, fieldINPUT_METHOD_SERVICE);
    jmethodID getSystemServiceMethod = env->GetMethodID(contextClass, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
    jobject callObjectMethod = env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, INPUT_METHOD_SERVICE);
    
    jclass classInputMethodManager = env->FindClass(OBFUSCATE("android/view/inputmethod/InputMethodManager"));
    jmethodID toggleSoftInputId = env->GetMethodID(classInputMethodManager, OBFUSCATE("toggleSoftInput"), OBFUSCATE("(II)V"));
    
    if (result) {
        env->CallVoidMethod(callObjectMethod, toggleSoftInputId, 2, flags);
    } else {
        env->CallVoidMethod(callObjectMethod, toggleSoftInputId, flags, flags);
    }
    
    env->DeleteLocalRef(classInputMethodManager);
    env->DeleteLocalRef(callObjectMethod);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(mInitialApplication);
    env->DeleteLocalRef(activityThreadClass);
    jvm->DetachCurrentThread();
    
    return result;
}



struct UnityEngine_Vector2_Fields {
    float x;
    float y;
};

struct UnityEngine_Vector2_o {
    UnityEngine_Vector2_Fields fields;
};

enum TouchPhase {
    Began = 0,
    Moved = 1,
    Stationary = 2,
    Ended = 3,
    Canceled = 4
};

struct UnityEngine_Touch_Fields {
    int32_t m_FingerId;
    struct UnityEngine_Vector2_o m_Position;
    struct UnityEngine_Vector2_o m_RawPosition;
    struct UnityEngine_Vector2_o m_PositionDelta;
    float m_TimeDelta;
    int32_t m_TapCount;
    int32_t m_Phase;
    int32_t m_Type;
    float m_Pressure;
    float m_maximumPossiblePressure;
    float m_Radius;
    float m_RadiusVariance;
    float m_AltitudeAngle;
    float m_AzimuthAngle;
};


