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
#include <ImGuiFileDialog.h>
#include <imgui.h>

// standard
#include <cstdio>
#include <exception>
#include <fstream>

#if __APPLE__
#    define SHORTCUT "Cmd-"
#else
#    define SHORTCUT "Ctrl-"
#endif

namespace
{
    constexpr std::string_view demo_text = R"(
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
)";
} // namespace

TextEditorDemo::TextEditorDemo() noexcept
{
    original_text = demo_text;
    editor.SetText(demo_text);
    editor.SetLanguage(TextEditor::Language::Cpp());
    editor.SetPalette(style::color::text_editor::palette);
    version = editor.GetUndoIndex();
    filename = "untitled";
}

void TextEditorDemo::new_file()
{
    if(is_dirty())
    {
        show_confirm_close([this]()
        {
            original_text.clear();
            editor.SetText("");
            version = editor.GetUndoIndex();
            filename = "untitled";
        });
    }
    else
    {
        original_text.clear();
        editor.SetText("");
        version = editor.GetUndoIndex();
        filename = "untitled";
    }
}

void TextEditorDemo::open_file()
{
    if(is_dirty())
    {
        show_confirm_close([this]() { show_file_open(); });
    }
    else
    {
        show_file_open();
    }
}

void TextEditorDemo::open_file(const std::string& path)
{
    try
    {
        std::ifstream stream(path.c_str());
        std::string text;

        stream.seekg(0, std::ios::end);
        text.reserve(stream.tellg());
        stream.seekg(0, std::ios::beg);

        text.assign((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        stream.close();

        original_text = text;
        editor.SetText(text);
        version = editor.GetUndoIndex();
        filename = path;
    }
    catch(std::exception& e)
    {
        show_error(e.what());
    }
}

void TextEditorDemo::save_file()
{
    try
    {
        editor.StripTrailingWhitespaces();
        std::ofstream stream(filename.c_str());
        stream << editor.GetText();
        stream.close();
        version = editor.GetUndoIndex();
    }
    catch(std::exception& e)
    {
        show_error(e.what());
    }
}

void TextEditorDemo::set_palette(const TextEditor::Palette& palette) noexcept
{
    editor.SetPalette(palette);
}

void TextEditorDemo::print() noexcept
{
    // add a menubar
    print_menu_bar();

    // render the text editor widget
    auto area = ImGui::GetContentRegionAvail();
    auto& style = ImGui::GetStyle();
    auto statusBarHeight = ImGui::GetFrameHeight() + 2.0f * style.WindowPadding.y;
    editor.Render("TextEditor", ImVec2(0.0f, area.y - style.ItemSpacing.y - statusBarHeight));

    // render a statusbar
    ImGui::Spacing();
    print_status_bar();

    if(state == State::diff)
    {
        render_diff();
    }
    else if(state == State::openFile)
    {
        render_file_open();
    }
    else if(state == State::saveFileAs)
    {
        render_save_as();
    }
    else if(state == State::confirmClose)
    {
        render_confirm_close();
    }
    else if(state == State::confirmQuit)
    {
        render_confirm_quit();
    }
    else if(state == State::confirmError)
    {
        render_confirm_error();
    }
}

void TextEditorDemo::try_to_quit()
{
    if(is_dirty())
    {
        show_confirm_quit();
    }
    else
    {
        done = true;
    }
}

void TextEditorDemo::print_menu_bar()
{
    // create menubar
    if(ImGui::BeginMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("New", SHORTCUT "N"))
            {
                new_file();
            }
            if(ImGui::MenuItem("Open...", SHORTCUT "O"))
            {
                open_file();
            }
            ImGui::Separator();

            if(ImGui::MenuItem("Save", SHORTCUT "S", nullptr, is_savable()))
            {
                save_file();
            }
            if(ImGui::MenuItem("Save As..."))
            {
                show_save_file_as();
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Edit"))
        {
            if(ImGui::MenuItem("Undo", " " SHORTCUT "Z", nullptr, editor.CanUndo()))
            {
                editor.Undo();
            }
#if __APPLE__
            if(ImGui::MenuItem("Redo", "^" SHORTCUT "Z", nullptr, editor.CanRedo()))
            {
                editor.Redo();
            }
#else
            if(ImGui::MenuItem("Redo", " " SHORTCUT "Y", nullptr, editor.CanRedo()))
            {
                editor.Redo();
            }
#endif

            ImGui::Separator();
            if(ImGui::MenuItem("Cut", " " SHORTCUT "X", nullptr, editor.AnyCursorHasSelection()))
            {
                editor.Cut();
            }
            if(ImGui::MenuItem("Copy", " " SHORTCUT "C", nullptr, editor.AnyCursorHasSelection()))
            {
                editor.Copy();
            }
            if(ImGui::MenuItem("Paste", " " SHORTCUT "V", nullptr, ImGui::GetClipboardText() != nullptr))
            {
                editor.Paste();
            }

            ImGui::Separator();
            if(ImGui::MenuItem("Tabs To Spaces"))
            {
                editor.TabsToSpaces();
            }
            if(ImGui::MenuItem("Spaces To Tabs"))
            {
                editor.SpacesToTabs();
            }
            if(ImGui::MenuItem("Strip Trailing Whitespaces"))
            {
                editor.StripTrailingWhitespaces();
            }

            ImGui::Separator();
            if(ImGui::MenuItem("Show Diff", " " SHORTCUT "I"))
            {
                show_diff();
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Selection"))
        {
            if(ImGui::MenuItem("Select All", " " SHORTCUT "A", nullptr, !editor.IsEmpty()))
            {
                editor.SelectAll();
            }
            ImGui::Separator();

            if(ImGui::MenuItem("Indent Line(s)", " " SHORTCUT "]", nullptr, !editor.IsEmpty()))
            {
                editor.IndentLines();
            }
            if(ImGui::MenuItem("Deindent Line(s)", " " SHORTCUT "[", nullptr, !editor.IsEmpty()))
            {
                editor.DeindentLines();
            }
            if(ImGui::MenuItem("Move Line(s) Up", nullptr, nullptr, !editor.IsEmpty()))
            {
                editor.MoveUpLines();
            }
            if(ImGui::MenuItem("Move Line(s) Down", nullptr, nullptr, !editor.IsEmpty()))
            {
                editor.MoveDownLines();
            }
            if(ImGui::MenuItem("Toggle Comments", " " SHORTCUT "/", nullptr, editor.HasLanguage()))
            {
                editor.ToggleComments();
            }
            ImGui::Separator();

            if(ImGui::MenuItem("To Uppercase", nullptr, nullptr, editor.AnyCursorHasSelection()))
            {
                editor.SelectionToUpperCase();
            }
            if(ImGui::MenuItem("To Lowercase", nullptr, nullptr, editor.AnyCursorHasSelection()))
            {
                editor.SelectionToLowerCase();
            }
            ImGui::Separator();

            if(ImGui::MenuItem("Add Next Occurrence", " " SHORTCUT "D", nullptr, editor.CurrentCursorHasSelection()))
            {
                editor.AddNextOccurrence();
            }
            if(ImGui::MenuItem("Select All Occurrences", "^" SHORTCUT "D", nullptr, editor.CurrentCursorHasSelection()))
            {
                editor.SelectAllOccurrences();
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("View"))
        {
            bool flag;
            flag = editor.IsShowWhitespacesEnabled();
            if(ImGui::MenuItem("Show Whitespaces", nullptr, &flag))
            {
                editor.SetShowWhitespacesEnabled(flag);
            };
            flag = editor.IsShowLineNumbersEnabled();
            if(ImGui::MenuItem("Show Line Numbers", nullptr, &flag))
            {
                editor.SetShowLineNumbersEnabled(flag);
            };
            flag = editor.IsShowingMatchingBrackets();
            if(ImGui::MenuItem("Show Matching Brackets", nullptr, &flag))
            {
                editor.SetShowMatchingBrackets(flag);
            };
            flag = editor.IsCompletingPairedGlyphs();
            if(ImGui::MenuItem("Complete Matching Glyphs", nullptr, &flag))
            {
                editor.SetCompletePairedGlyphs(flag);
            };
            flag = editor.IsShowPanScrollIndicatorEnabled();
            if(ImGui::MenuItem("Show Pan/Scroll Indicator", nullptr, &flag))
            {
                editor.SetShowPanScrollIndicatorEnabled(flag);
            };
            flag = editor.IsMiddleMousePanMode();
            if(ImGui::MenuItem("Middle Mouse Pan Mode", nullptr, &flag))
            {
                if(flag)
                    editor.SetMiddleMousePanMode();
                else
                    editor.SetMiddleMouseScrollMode();
            };
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Find"))
        {
            if(ImGui::MenuItem("Find", " " SHORTCUT "F"))
            {
                editor.OpenFindReplaceWindow();
            }
            if(ImGui::MenuItem("Find Next", " " SHORTCUT "G", nullptr, editor.HasFindString()))
            {
                editor.FindNext();
            }
            if(ImGui::MenuItem("Find All", "^" SHORTCUT "G", nullptr, editor.HasFindString()))
            {
                editor.FindAll();
            }
            ImGui::Separator();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // handle keyboard shortcuts
    if(ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::GetIO().WantCaptureKeyboard)
    {
        if(ImGui::IsKeyDown(ImGuiMod_Ctrl))
        {
            if(ImGui::IsKeyPressed(ImGuiKey_N))
            {
                new_file();
            }
            else if(ImGui::IsKeyPressed(ImGuiKey_O))
            {
                open_file();
            }
            else if(ImGui::IsKeyPressed(ImGuiKey_S))
            {
                if(filename == "untitled")
                {
                    show_save_file_as();
                }
                else
                {
                    save_file();
                }
            }
            else if(ImGui::IsKeyPressed(ImGuiKey_I))
            {
                show_diff();
            }
        }
    }
}

void TextEditorDemo::print_status_bar()
{
    // language support
    static const char* languages[] = {
      "C", "C++", "Cs", "AngelScript", "Lua", "Python", "GLSL", "HLSL", "JSON", "Markdown", "SQL"};

    static const TextEditor::Language* definitions[] = {TextEditor::Language::C(),
                                                        TextEditor::Language::Cpp(),
                                                        TextEditor::Language::Cs(),
                                                        TextEditor::Language::AngelScript(),
                                                        TextEditor::Language::Lua(),
                                                        TextEditor::Language::Python(),
                                                        TextEditor::Language::Glsl(),
                                                        TextEditor::Language::Hlsl(),
                                                        TextEditor::Language::Json(),
                                                        TextEditor::Language::Markdown(),
                                                        TextEditor::Language::Sql()};

    std::string language = editor.GetLanguageName();

    // create a statusbar window
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    ImGui::BeginChild("StatusBar", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Borders);
    ImGui::SetNextItemWidth(120.0f);

    // allow user to select language for colorizing
    if(ImGui::BeginCombo("##LanguageSelector", language.c_str()))
    {
        for(int n = 0; n < IM_ARRAYSIZE(languages); n++)
        {
            bool selected = (language == languages[n]);

            if(ImGui::Selectable(languages[n], selected))
            {
                editor.SetLanguage(definitions[n]);
            }

            if(selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    // determine horizontal gap so the rest is right aligned
    ImGui::SameLine(0.0f, 0.0f);
    ImGui::AlignTextToFramePadding();

    int line;
    int column;
    int tabSize = editor.GetTabSize();
    float glyphWidth = editor.GetGlyphWidth();
    editor.GetCurrentCursor(line, column);

    // determine status message
    char status[256];

    auto statusSize = std::snprintf(
      status, sizeof(status), "Ln %d, Col %d  Tab Size: %d  File: %s", line + 1, column + 1, tabSize, filename.c_str());

    auto size = glyphWidth * (statusSize + 3);
    auto width = ImGui::GetContentRegionAvail().x;

    ImGui::SameLine(0.0f, width - size);
    ImGui::TextUnformatted(status);

    // render "text dirty" indicator
    ImGui::SameLine(0.0f, glyphWidth * 1.0f);
    auto drawlist = ImGui::GetWindowDrawList();
    auto pos = ImGui::GetCursorScreenPos();
    auto offset = ImGui::GetFrameHeight() * 0.5f;
    auto radius = offset * 0.6f;
    auto color = is_dirty() ? IM_COL32(164, 0, 0, 255) : IM_COL32(164, 164, 164, 255);
    drawlist->AddCircleFilled(ImVec2(pos.x + offset, pos.y + offset), radius, color);

    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void TextEditorDemo::show_diff()
{
    diff.SetLanguage(editor.GetLanguage());
    diff.SetText(original_text, editor.GetText());
    state = State::diff;
}

void TextEditorDemo::show_file_open()
{
    // open a file selector dialog
    IGFD::FileDialogConfig config;
    config.countSelectionMax = 1;

    config.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_DontShowHiddenFiles
                   | ImGuiFileDialogFlags_ReadOnlyFileNameField;

    ImGuiFileDialog::Instance()->OpenDialog("file-open", "Select File to Open...", ".*", config);
    state = State::openFile;
}

void TextEditorDemo::show_save_file_as()
{
    IGFD::FileDialogConfig config;
    config.countSelectionMax = 1;

    config.flags =
      ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_DontShowHiddenFiles | ImGuiFileDialogFlags_ConfirmOverwrite;

    ImGuiFileDialog::Instance()->OpenDialog("file-saveas", "Save File as...", "*", config);
    state = State::saveFileAs;
}

void TextEditorDemo::show_confirm_close(std::function<void()> callback)
{
    on_confirm_close = callback;
    state = State::confirmClose;
}

void TextEditorDemo::show_confirm_quit()
{
    state = State::confirmQuit;
}

void TextEditorDemo::show_error(const std::string& message)
{
    error_message = message;
    state = State::confirmError;
}

void TextEditorDemo::render_diff()
{
    ImGui::OpenPopup("Changes since Opening File##diff");
    auto viewport = ImGui::GetMainViewport();
    ImVec2 center = viewport->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if(ImGui::BeginPopupModal("Changes since Opening File##diff", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        diff.Render("diff", viewport->Size * 0.8f, true);

        ImGui::Separator();
        static constexpr float buttonWidth = 80.0f;
        auto buttonOffset = ImGui::GetContentRegionAvail().x - buttonWidth;
        bool sideBySide = diff.GetSideBySideMode();

        if(ImGui::Checkbox("Show side-by-side", &sideBySide))
        {
            diff.SetSideBySideMode(sideBySide);
        }

        ImGui::SameLine();
        ImGui::Indent(buttonOffset);

        if(ImGui::Button("OK", ImVec2(buttonWidth, 0.0f)) || ImGui::IsKeyPressed(ImGuiKey_Escape, false))
        {
            state = State::edit;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void TextEditorDemo::render_file_open()
{
    // handle file open dialog
    ImVec2 maxSize = ImGui::GetMainViewport()->Size;
    ImVec2 minSize = maxSize * 0.5f;
    auto dialog = ImGuiFileDialog::Instance();

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    if(dialog->Display("file-open", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
    {
        // open selected file (if required)
        if(dialog->IsOk())
        {
            open_file(dialog->GetFilePathName());
            state = State::edit;
        }

        // close dialog
        dialog->Close();
    }
}

void TextEditorDemo::render_save_as()
{
    // handle saveas dialog
    ImVec2 maxSize = ImGui::GetMainViewport()->Size;
    ImVec2 minSize = maxSize * 0.5f;
    auto dialog = ImGuiFileDialog::Instance();

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    if(dialog->Display("file-saveas", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
    {
        // open selected file if required
        if(dialog->IsOk())
        {
            filename = dialog->GetFilePathName();
            save_file();
            state = State::edit;
        }
        else
        {
            state = State::edit;
        }

        // close dialog
        dialog->Close();
    }
}

void TextEditorDemo::render_confirm_close()
{
    ImGui::OpenPopup("Confirm Close");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if(ImGui::BeginPopupModal("Confirm Close", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("This file has changed!\nDo you really want to delete it?\n\n");
        ImGui::Separator();

        static constexpr float buttonWidth = 80.0f;
        ImGui::Indent(ImGui::GetContentRegionAvail().x - buttonWidth * 2.0f - ImGui::GetStyle().ItemSpacing.x);

        if(ImGui::Button("OK", ImVec2(buttonWidth, 0.0f)))
        {
            state = State::edit;
            on_confirm_close();
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if(ImGui::Button("Cancel", ImVec2(buttonWidth, 0.0f)) || ImGui::IsKeyPressed(ImGuiKey_Escape, false))
        {
            state = State::edit;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void TextEditorDemo::render_confirm_quit()
{
    ImGui::OpenPopup("Quit Editor?");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if(ImGui::BeginPopupModal("Quit Editor?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Your text has changed and is not saved!\nDo you really want to quit?\n\n");
        ImGui::Separator();

        static constexpr float buttonWidth = 80.0f;
        ImGui::Indent(ImGui::GetContentRegionAvail().x - buttonWidth * 2.0f - ImGui::GetStyle().ItemSpacing.x);

        if(ImGui::Button("OK", ImVec2(buttonWidth, 0.0f)))
        {
            done = true;
            state = State::edit;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if(ImGui::Button("Cancel", ImVec2(buttonWidth, 0.0f)) || ImGui::IsKeyPressed(ImGuiKey_Escape, false))
        {
            state = State::edit;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void TextEditorDemo::render_confirm_error()
{
    ImGui::OpenPopup("Error");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if(ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("%s\n", error_message.c_str());
        ImGui::Separator();

        static constexpr float buttonWidth = 80.0f;
        ImGui::Indent(ImGui::GetContentRegionAvail().x - buttonWidth);

        if(ImGui::Button("OK", ImVec2(buttonWidth, 0.0f)) || ImGui::IsKeyPressed(ImGuiKey_Escape, false))
        {
            error_message.clear();
            state = State::edit;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
