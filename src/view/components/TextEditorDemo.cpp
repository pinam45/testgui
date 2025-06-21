//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// header
#include "TextEditorDemo.hpp"

// project
#include <view/style/colors.hpp>

// external
#include <imgui.h>

TextEditorDemo::TextEditorDemo() noexcept
{
    _editor.SetLanguage(TextEditor::Language::Cpp());
    _editor.SetPalette(style::color::text_editor::palette);
    // TextEditor::ErrorMarkers markers;
    // markers.insert(
    //   std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
    // markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
    _editor.SetText(R"(
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
    _editor.AddMarker(6, style::ImU32_from_bytes(249, 38, 114, 255), style::ImU32_from_bytes(249, 38, 114, 255), "magic 1", "magic 2");
    assert(_editor.HasMarkers());
}

void TextEditorDemo::print() noexcept
{
    //auto cpos = _editor.GetCursorPosition();
    if(ImGui::BeginMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Save"))
            {
                auto textToSave = _editor.GetText();
                /// save text....
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Edit"))
        {
            bool ro = _editor.IsReadOnlyEnabled();
            if(ImGui::MenuItem("Read-only mode", nullptr, &ro))
                _editor.SetReadOnlyEnabled(ro);
            ImGui::Separator();

            if(ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && _editor.CanUndo()))
                _editor.Undo();
            if(ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && _editor.CanRedo()))
                _editor.Redo();

            ImGui::Separator();

            // if(ImGui::MenuItem("Copy", "Ctrl-C", nullptr, _editor.HasSelection()))
            //     _editor.Copy();
            // if(ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && _editor.HasSelection()))
            //     _editor.Cut();
            // if(ImGui::MenuItem("Delete", "Del", nullptr, !ro && _editor.HasSelection()))
            //     _editor.Delete();
            // if(ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
            //     _editor.Paste();

            ImGui::Separator();

            if(ImGui::MenuItem("Select all", nullptr, nullptr))
                _editor.SelectAll();

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("View"))
        {
            // if(ImGui::MenuItem("My palette"))
            //     _editor.SetPalette(style::color::text_editor::palette);
            if(ImGui::MenuItem("Dark palette"))
                _editor.SetPalette(TextEditor::GetDarkPalette());
            if(ImGui::MenuItem("Light palette"))
                _editor.SetPalette(TextEditor::GetLightPalette());
            // if(ImGui::MenuItem("Retro blue palette"))
            //     _editor.SetPalette(TextEditor::GetRetroBluePalette());
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s",
    //             cpos.mLine + 1,
    //             cpos.mColumn + 1,
    //             _editor.GetTotalLines(),
    //             _editor.IsOverwrite() ? "Ovr" : "Ins",
    //             _editor.CanUndo() ? "*" : " ",
    //             _editor.GetLanguageDefinition().mName.c_str(),
    //             "totallyafile.magic");

    ImFont* font = ImGui::GetFont();
    ImGui::PushFont(font);
    _editor.Render("TextEditor");
    ImGui::PopFont();
}

void TextEditorDemo::set_palette(const TextEditor::Palette& palette) noexcept
{
    _editor.SetPalette(palette);
}
