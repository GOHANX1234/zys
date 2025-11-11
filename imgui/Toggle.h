#pragma once
#include "imgui_internal.h"
// Add this line at the top of your file if not already


// ✅ Animation storage
// ============================================================
// INFINITY TEAM PRO V3 Toggle Switch (Red Accent Animated)
// ============================================================

static std::map<ImGuiID, float> s_toggle_anim_state;

bool Toggle(const char* label, bool* state) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const float toggle_height = label_size.y * 1.1f;
    const float toggle_width = toggle_height * 2.5f;
    const float radius = toggle_height * 0.5f;
    const float vertical_margin = 0;
    const float total_widget_height = toggle_height + vertical_margin * 2.0f;
    const float total_width = ImGui::GetContentRegionAvail().x;
    const ImRect bb(window->DC.CursorPos, ImVec2(window->DC.CursorPos.x + total_width, window->DC.CursorPos.y + total_widget_height));

    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    if (pressed) {
        *state = !*state;
        ImGui::MarkItemEdited(id);
    }

    float target_state = *state ? 1.0f : 0.0f;
    if (s_toggle_anim_state.find(id) == s_toggle_anim_state.end()) {
        s_toggle_anim_state[id] = target_state;
    }
    s_toggle_anim_state[id] = ImLerp(s_toggle_anim_state[id], target_state, g.IO.DeltaTime * 10.0f);
    float current_anim_state = s_toggle_anim_state[id];

    // Colors
    const ImU32 bg_color_off = IM_COL32(50, 50, 50, 255);   // Gray (OFF)
    const ImU32 bg_color_on = IM_COL32(0, 255, 0, 255);     // ✅ Green (ON)
    const ImU32 knob_color = IM_COL32(255, 255, 255, 255);  // White knob

    float centered_y = bb.Min.y + vertical_margin + (toggle_height - label_size.y) * 0.5f;
    float text_padding = 20.0f;
    ImVec2 text_pos(bb.Min.x + toggle_width + text_padding, centered_y);
    window->DrawList->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), label);

    ImVec2 toggle_pos(bb.Min.x, bb.Min.y + vertical_margin);

    ImU32 bg_color = ImGui::ColorConvertFloat4ToU32(
        ImLerp(ImColor(bg_color_off).Value, ImColor(bg_color_on).Value, current_anim_state)
    );

    // Draw toggle background
    window->DrawList->AddRectFilled(toggle_pos, ImVec2(toggle_pos.x + toggle_width, toggle_pos.y + toggle_height), bg_color, radius);

    // Draw knob
    float knob_radius = radius - 2.0f;
    ImVec2 knob_center(toggle_pos.x + radius + current_anim_state * (toggle_width - 2.0f * radius), toggle_pos.y + radius);
    window->DrawList->AddCircleFilled(knob_center, knob_radius + 1.5f, IM_COL32(0, 0, 0, 50));
    window->DrawList->AddCircleFilled(knob_center, knob_radius, knob_color);

    return pressed;
}
