//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// header
#include "TextEditorStyleEditor.hpp"

// external
#include <IconsFontAwesome6.h>
#include <scope_guard.hpp>
#include <utils/log.hpp>
#include <view/style/colors.hpp>
#include <view/style/utils.hpp>

// C++ standard
#include <cassert>
#include <utility>

TextEditorStyleEditor::TextEditorStyleEditor(const TextEditor::Palette& text_editor_palette) noexcept
    : style_changed()
    , _base_text_editor_palette(text_editor_palette)
    , _text_editor_palette(text_editor_palette)
    , _example_editor()
    , _logger(logging::get_logger("StyleEditor"))
{
    _example_editor.SetReadOnlyEnabled(true);
    _example_editor.SetLanguage(TextEditor::Language::Cpp());
    _example_editor.SetPalette(_text_editor_palette);
    // TextEditor::ErrorMarkers markers;
    // markers.insert(
    //   std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
    // markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
    // _example_editor.SetErrorMarkers(markers);
    _example_editor.AddMarker(6, style::ImU32_from_bytes(249, 38, 114, 255), style::ImU32_from_bytes(249, 38, 114, 255), "magic 1", "magic 2");
    _example_editor.SetText(R"(
/*
 * Block comment
 */
#include <cstdio>
#include <vector>

#define STRINGIFY(x) #x

using namespace std;  // line comment
namespace foo {

  typedef struct Struct {
    int field;
  } Typedef;
  enum Enum {Foo = 1, Bar = 2};

  Typedef *globalVar;
  extern Typedef *externVar;

  template<typename T, int N>
  class Class {
    T n;
  public:
    /**
     * Semantic highlighting:
     * Generated spectrum to pick colors for local variables and parameters:
     *  Color#1 SC1.1 SC1.2 SC1.3 SC1.4 Color#2 SC2.1 SC2.2 SC2.3 SC2.4 Color#3
     *  Color#3 SC3.1 SC3.2 SC3.3 SC3.4 Color#4 SC4.1 SC4.2 SC4.3 SC4.4 Color#5
     */
    void function(int param1, int param2, int param3) {
      int localVar1, localVar2, localVar3;
      int *localVar = new int[1];
      std::vector<int> vec = { 1, 2, 3 };
      this->n = N;
      localVar1 = param1 + param2 + localVar3;

    label:
      printf("Formatted string %d\n\g", localVar[0]);
      printf(R"**(Formatted raw-string %d\n)**", 1);
      std::cout << (1 << 2) << std::endl;

    /**
     * Macro documentation comment
     * @param A description
     */
    #define FOO(A) A
    #ifdef DEBUG
      printf("debug");
    #endif
    }
  };

  template <typename T>
  concept Concept = requires (T t) {
    t.field;
  };

  template<typename T>
  struct Widget {
      Widget(T t);
  };

  template<typename T>
  Widget(T) -> Widget<typename T::value_type>;
}
)");
}

void TextEditorStyleEditor::print() noexcept
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, 0),
                                        ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, FLT_MAX));
    ImGui::BeginChild(
      "ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0.), ImGuiChildFlags_ResizeX, ImGuiWindowFlags_None);
    {
        SCOPE_EXIT
        {
            ImGui::EndChild();
        };
        ImFont* font = ImGui::GetFont();
        ImGui::PushFont(font);
        _example_editor.Render("Example editor");
        ImGui::PopFont();
    }

    ImGui::SameLine();
    ImGui::BeginChild("ChildR", ImVec2(0., 0.), ImGuiChildFlags_None, ImGuiWindowFlags_None);
    {
        SCOPE_EXIT
        {
            ImGui::EndChild();
        };
        if(ImGui::Button("Export"))
        {
            const std::string palette_txt = [&]()
            {
                if(_export_style == 0)
                {
                    return style::palette_cpp256(_text_editor_palette);
                }
                return style::palette_cpp1(_text_editor_palette);
            }();
            if(_export_dest == 0)
            {
                ImGui::SetClipboardText(palette_txt.c_str());
            }
            else
            {
                fmt::print("{}\n", palette_txt);
            }
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::CalcTextSize("To program output").x + ImGui::GetFrameHeight()
                                + ImGui::GetStyle().FramePadding.x * 2.0f);
        ImGui::Combo("##export_dest", &_export_dest, "To Clipboard\0To program output\0");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::CalcTextSize("[0.0, 1.0]").x + ImGui::GetFrameHeight()
                                + ImGui::GetStyle().FramePadding.x * 2.0f);
        ImGui::Combo("##_export_style", &_export_style, "[0, 255]\0[0.0, 1.0]\0");

        ImGui::BeginChild("##colors",
                          ImVec2(0, -(ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y)),
                          ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened,
                          ImGuiWindowFlags_AlwaysVerticalScrollbar);
        {
            SCOPE_EXIT
            {
                ImGui::EndChild();
            };
            ImGui::PushItemWidth(ImGui::GetFontSize() * -18);
            {
                SCOPE_EXIT
                {
                    ImGui::PopItemWidth();
                };
                size_t changed = 0;
                for(size_t i = 0; i < _text_editor_palette.size(); ++i)
                {
                    ImVec4 tmp = style::ImVec4_from_ImU32(_text_editor_palette[i]);
                    changed += ImGui::ColorEdit4(style::color::text_editor::palette_color_name[i].data(),
                                                 reinterpret_cast<float*>(&tmp));
                    _text_editor_palette[i] = ImGui::GetColorU32(tmp);
                }
                if(changed > 0)
                {
                    _example_editor.SetPalette(_text_editor_palette);
                }
            }
        }

        // ICON_FA_CIRCLE_CHECK
        if(ImGui::Button(ICON_FA_CHECK_TO_SLOT " Apply"))
        {
            // TODO
            _base_text_editor_palette = _text_editor_palette;
            style_changed(style_info{_base_text_editor_palette});
        }
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_ARROW_ROTATE_LEFT " Revert"))
        {
            // TODO
            _text_editor_palette = _base_text_editor_palette;
            _example_editor.SetPalette(_text_editor_palette);
        }
    }
}
