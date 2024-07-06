//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// magic
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
#   define localtime_r(timer,buf) (localtime_s(buf,timer) ? NULL : buf)
#   define gmtime_r(timer,buf) (gmtime_s(buf,timer) ? NULL : buf)
#endif

// project
#include <utils/log.hpp>
#include <utils/thread_pool.hpp>
#include <view/components/IconsFinder.hpp>
#include <view/components/ImSpinnerDemo.hpp>
#include <view/components/InterfaceStyleEditor.hpp>
#include <view/components/LogViewer.hpp>
#include <view/components/TextEditorDemo.hpp>
#include <view/components/TextEditorStyleEditor.hpp>
#include <view/font.hpp>
#include <view/setup/gl3w.hpp>
#include <view/setup/glfw.hpp>
#include <view/setup/imgui.hpp>
#include <view/setup/implot.hpp>
#include <view/setup/window.hpp>
#include <view/style/colors.hpp>
#include <view/style/utils.hpp>
#include <view/utils/Window.hpp>

// external
#include <IconsFontAwesome6.h>
#include <ImGuiNotify.hpp>
#include <TextEditor.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <impop_datepicker.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
//#include <impop_footer.h>
#include <scope_guard.hpp>
#include <tl/expected.hpp>

// C++ standard
#include <chrono>
#include <iostream>
#include <optional>

namespace
{
    std::shared_ptr<spdlog::logger> logger = nullptr;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    setlocale(LC_ALL, "C");

    if(!logging::init_logger())
    {
        return EXIT_FAILURE;
    }
    logger = logging::get_logger();

    // Setup
    glfw_handle_t glfw_handle = setup::glfw();
    if(glfw_handle == nullptr)
    {
        logger->error("glfw setup failed");
        return EXIT_FAILURE;
    }
    main_window_handle_t main_window_handle = setup::main_window(glfw_handle, "testgui", 1280, 720);
    if(main_window_handle == nullptr)
    {
        logger->error("main window setup failed");
        return EXIT_FAILURE;
    }
    gl3w_handle_t gl3w_handle = setup::gl3w();
    if(gl3w_handle == nullptr)
    {
        logger->error("gl3w setup failed");
        return EXIT_FAILURE;
    }
    imgui_handle_t imgui_handle = setup::imgui(main_window_handle, gl3w_handle);
    if(imgui_handle == nullptr)
    {
        logger->error("imgui setup failed");
        return EXIT_FAILURE;
    }
    implot_handle_t implot_handle = setup::implot();
    if(implot_handle == nullptr)
    {
        logger->error("implot setup failed");
        return EXIT_FAILURE;
    }

    // Preload fonts (first will become the default)
    font::preload(font::embedded::DROID_SANS_MONO, font::DEFAULT_FONT_SIZE);
    font::preload(font::embedded::DROID_SANS_MONO, font::LARGE_FONT_SIZE);

    // font::preload(font::embedded::INTEL_ONE_MONO, font::DEFAULT_FONT_SIZE);
    // font::preload(font::embedded::INTEL_ONE_MONO, font::LARGE_FONT_SIZE);

    // font::preload(font::embedded::NOTO_SANS_MONO, font::DEFAULT_FONT_SIZE);
    // font::preload(font::embedded::NOTO_SANS_MONO, font::LARGE_FONT_SIZE);

    // font::preload(font::embedded::ROBOTO_MONO, font::DEFAULT_FONT_SIZE);
    // font::preload(font::embedded::ROBOTO_MONO, font::LARGE_FONT_SIZE);

    // font::preload(font::embedded::COUSINE, font::DEFAULT_FONT_SIZE);
    // font::preload(font::embedded::COUSINE, font::LARGE_FONT_SIZE);

    // font::preload(font::embedded::SOURCE_CODE_PRO, font::DEFAULT_FONT_SIZE);
    // font::preload(font::embedded::SOURCE_CODE_PRO, font::LARGE_FONT_SIZE);

    // State variables
    bool show_imgui_demo_window = true;
    bool show_implot_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImPlotTime t;
    ImPlotTime default_time = ImPlot::MakeTime(2024, 1, 1);
    ImPlotTime min_time = ImPlot::MakeTime(2023, 1, 1);
    ImPlotTime max_time = ImPop::LocTimeNow();

    // Log viewer
    Window<LogViewer> log_viewer("Logs");

    // Text editor style editor
    Window<TextEditorStyleEditor> text_editor_style_editor("Text editor style", style::color::text_editor::palette);

    // Interface style editor
    Window<InterfaceStyleEditor> interface_style_editor("Interface style");

    // Text Editor Demo
    Window<TextEditorDemo, ImGuiWindowFlags_MenuBar> text_editor_demo("Text Editor Demo");
    sigslot::connection con = text_editor_style_editor.content.style_changed.connect([&text_editor_demo](const TextEditorStyleEditor::style_info& style_info) {
        text_editor_demo.content.set_palette(style_info.palette);
    });
    SCOPE_EXIT { con.disconnect(); };

    // ImSpinner Demo
    Window<ImSpinnerDemo> imspinner_demo("ImSpinner Demo");

    // Icons finder
    Window<IconsFinder> icons_finder("Icons finder");

    // background tasks
    thread_pool tp(8);
    std::optional<std::string> test;
    std::future<std::string> test_future;

    // Main loop
    while(!glfwWindowShouldClose(main_window_handle->glf_window))
    {
        // Process events
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main window (background)
        {
            // Open "full-windowed" imgui window
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("main window",
                         nullptr,
                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                           ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar |
                           ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus |
                           ImGuiWindowFlags_NoNavFocus);
            ImGui::PopStyleVar(2);

            // Show menu bar
            if(ImGui::BeginMenuBar())
            {
                if(ImGui::BeginMenu("Edit"))
                {
                    if(ImGui::MenuItem(ICON_FA_PAINT_ROLLER " Interface style", nullptr, &interface_style_editor.open))
                    {
                        if(interface_style_editor.open)
                        {
                            SPDLOG_LOGGER_DEBUG(logger, "Show interface style editor");
                        }
                        else
                        {
                            SPDLOG_LOGGER_DEBUG(logger, "Hide interface style editor");
                        }
                    }
                    if(ImGui::MenuItem(ICON_FA_PAINTBRUSH " Text editor style", nullptr, &text_editor_style_editor.open))
                    {
                        if(text_editor_style_editor.open)
                        {
                            SPDLOG_LOGGER_DEBUG(logger, "Show text editor style editor");
                        }
                        else
                        {
                            SPDLOG_LOGGER_DEBUG(logger, "Hide text editor style editor");
                        }
                    }
                    ImGui::EndMenu();
                }
                if(ImGui::BeginMenu("View"))
                {
                    if(ImGui::MenuItem(ICON_FA_WRENCH " ImGui demo", nullptr, &show_imgui_demo_window))
                    {
                        if(show_imgui_demo_window)
                        {
                            SPDLOG_LOGGER_DEBUG(logger, "Show ImGui demo window");
                        }
                        else
                        {
                            SPDLOG_LOGGER_DEBUG(logger, "Hide ImGui demo window");
                        }
                    }
                    if(ImGui::MenuItem(ICON_FA_WRENCH " ImPlot demo", nullptr, &show_implot_demo_window))
                    {
                        if(show_implot_demo_window)
                        {
                            SPDLOG_LOGGER_DEBUG(logger, "Show ImPlot demo window");
                        }
                        else
                        {
                            SPDLOG_LOGGER_DEBUG(logger, "Hide ImPlot demo window");
                        }
                    }
                    if(ImGui::MenuItem("Other window", nullptr, &show_another_window))
                    {
                        if(show_another_window)
                        {
                            SPDLOG_LOGGER_DEBUG(logger, "Show other window");
                        }
                        else
                        {
                            SPDLOG_LOGGER_DEBUG(logger, "Hide other window");
                        }
                    }
                    if(ImGui::MenuItem(ICON_FA_CLIPBOARD_LIST " Logs", nullptr, &log_viewer.open))
                    {
                        if(log_viewer.open)
                        {
                            SPDLOG_LOGGER_DEBUG(logger, "Show logs console");
                        }
                        else
                        {
                            SPDLOG_LOGGER_DEBUG(logger, "Hide logs console");
                        }
                    }
                    ImGui::EndMenu();
                }
                if(ImGui::BeginMenu("About"))
                {
                    if(ImGui::MenuItem(ICON_FA_INFO " Version", nullptr))
                    {
                        SPDLOG_LOGGER_ERROR(logger, "TODO: version");
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            // Setup dockspace
            ImGuiID dockspace_id = ImGui::GetID("main dockspace");
            if(ImGui::DockBuilderGetNode(dockspace_id) == nullptr)
            {
                // Main dockspace initial setup
                ImGui::DockBuilderRemoveNode(dockspace_id);// Clear out existing layout
                ImGui::DockBuilderAddNode(dockspace_id);   // Add empty node
                ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

                ImGuiID dock_main_id = dockspace_id;
                ImGuiID dock_id_bottom =
                  ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.22f, nullptr, &dock_main_id);
                ImGuiID dock_id_right =
                  ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.4f, nullptr, &dock_main_id);

                ImGui::DockBuilderDockWindow("Dear ImGui Demo", dock_main_id);
                ImGui::DockBuilderDockWindow("ImPlot Demo", dock_main_id);
                ImGui::DockBuilderDockWindow("Text Editor Demo", dock_main_id);
                ImGui::DockBuilderDockWindow("ImSpinner Demo", dock_main_id);
                ImGui::DockBuilderDockWindow("Text editor style", dock_id_right);
                ImGui::DockBuilderDockWindow("Interface style", dock_id_right);
                ImGui::DockBuilderDockWindow("Icons finder", dock_id_right);
                ImGui::DockBuilderDockWindow("Test", dock_id_right);
                ImGui::DockBuilderDockWindow("Logs", dock_id_bottom);
                ImGui::DockBuilderFinish(dockspace_id);
                SPDLOG_LOGGER_DEBUG(logger, "Set initial position of windows in the main dockspace");
            }
            ImGui::DockSpace(dockspace_id);

            ImGui::End();
        }

        // Imgui demo window
        if(show_imgui_demo_window)
        {
            ImGui::ShowDemoWindow(&show_imgui_demo_window);
        }

        // ImPlot demo window
        if(show_implot_demo_window)
        {
            ImPlot::ShowDemoWindow(&show_implot_demo_window);
        }

        // Text editor demo
        if(text_editor_demo.open)
        {
            text_editor_demo.show();
        }

        // ImSpinner Demo
        if(imspinner_demo.open)
        {
            imspinner_demo.show();
        }

        // Log viewer
        if(log_viewer.open)
        {
            log_viewer.show();
        }

        // Text editor style editor
        if(text_editor_style_editor.open)
        {
            text_editor_style_editor.show();
        }

        // Interface style editor
        if(interface_style_editor.open)
        {
            interface_style_editor.show();
        }

        // Icons finder
        if(icons_finder.open)
        {
            icons_finder.show();
        }

        // Test window
        {
            font::push(font::LARGE_FONT_SIZE);
            ImGui::Begin("Test");

            if(ImPop::DatePicker("date", &t, &default_time, &min_time, &max_time))
            {
                // `t` has been updated
            }

            if(ImGui::Button(ICON_FA_WAND_MAGIC_SPARKLES " do something"))
            {
                test_future = tp.submit([]() -> std::string {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    return "test";
                });
            }
            if(test_future.valid() && test_future.wait_for(std::chrono::seconds(0)) ==
                                        std::future_status::ready)
            {
                test = test_future.get();
                test_future = {};
                SPDLOG_LOGGER_DEBUG(logger, "test updated");
            }
            if(test)
            {
                ImGui::Text("test:\n%s", test->data());
            }

            ImGui::SeparatorText("NOTIFICATIONS");
            if(ImGui::Button("Success"))
            {
                ImGui::InsertNotification({ImGuiToastType::Success, 3000, "That is a success! %s", "(Format here)"});
            }
            ImGui::SameLine();
            if(ImGui::Button("Warning"))
            {
                ImGui::InsertNotification({ImGuiToastType::Warning, 3000, "Hello World! This is a warning! %d", 0x1337});
            }
            ImGui::SameLine();
            if(ImGui::Button("Error"))
            {
                ImGui::InsertNotification({ImGuiToastType::Error, 3000, "Hello World! This is an error! 0x%X", 0xDEADBEEF});
            }
            ImGui::SameLine();
            if(ImGui::Button("Info"))
            {
                ImGui::InsertNotification({ImGuiToastType::Info, 3000, "Hello World! This is an info!"});
            }
            ImGui::SameLine();
            if(ImGui::Button("Long info"))
            {
                ImGui::InsertNotification({ImGuiToastType::Info, 3000, "Hi, I'm a long notification. I'm here to show you that you can write a lot of text in me. I'm also here to show you that I can wrap text, so you don't have to worry about that."});
            }
            ImGui::SameLine();
            if(ImGui::Button("Error with button"))
            {
                ImGui::InsertNotification({ImGuiToastType::Error, 3000, "Click me!", []() { ImGui::InsertNotification({ImGuiToastType::Success, 3000, "Thanks for clicking!"}); }, "Notification content"});
            }
            ImGui::SameLine();
            if(ImGui::Button("Custom title"))
            {
                ImGuiToast toast(ImGuiToastType::Success, 3000);// <-- content can also be passed here as above
                toast.setTitle("This is a %s title %d", "wonderful", 3);
                toast.setContent("Lorem ipsum dolor sit amet");
                ImGui::InsertNotification(toast);
            }

            font::push(font::embedded::DROID_SANS_MONO, font::LARGE_FONT_SIZE);
            ImGui::SeparatorText("DROID_SANS_MONO " ICON_FA_ADDRESS_CARD);
            ImGui::BulletText("The quick brown fox jumps over the lazy dog");
            font::pop();

            // font::push(font::embedded::INTEL_ONE_MONO, font::LARGE_FONT_SIZE);
            // ImGui::SeparatorText("INTEL_ONE_MONO " ICON_FA_ADDRESS_CARD);
            // ImGui::BulletText("The quick brown fox jumps over the lazy dog");
            // font::pop();

            // font::push(font::embedded::NOTO_SANS_MONO, font::LARGE_FONT_SIZE);
            // ImGui::SeparatorText("NOTO_SANS_MONO " ICON_FA_ADDRESS_CARD);
            // ImGui::BulletText("The quick brown fox jumps over the lazy dog");
            // font::pop();

            // font::push(font::embedded::ROBOTO_MONO, font::LARGE_FONT_SIZE);
            // ImGui::SeparatorText("ROBOTO_MONO " ICON_FA_ADDRESS_CARD);
            // ImGui::BulletText("The quick brown fox jumps over the lazy dog");
            // font::pop();

            // font::push(font::embedded::COUSINE, font::LARGE_FONT_SIZE);
            // ImGui::SeparatorText("COUSINE " ICON_FA_ADDRESS_CARD);
            // ImGui::BulletText("The quick brown fox jumps over the lazy dog");
            // font::pop();

            // font::push(font::embedded::SOURCE_CODE_PRO, font::LARGE_FONT_SIZE);
            // ImGui::SeparatorText("SOURCE_CODE_PRO " ICON_FA_ADDRESS_CARD);
            // ImGui::BulletText("The quick brown fox jumps over the lazy dog");
            // font::pop();

            ImGui::End();
            font::pop();
        }

        // "Hello, world!" window
        {
            ImGui::Begin("Hello, world!");// Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_imgui_demo_window);// Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            static float f = 0.0f;
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*) &clear_color);// Edit 3 floats representing a color

            static int counter = 0;
            if(ImGui::Button("Button"))
            {
                counter++;
            }
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // "Another Window" window
        if(show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if(ImGui::Button("Close Me"))
            {
                show_another_window = false;
            }
            ImGui::End();
        }

        // Perf footer
        //ImPop::PerfFooter();

        // Rendering
        ImGui::RenderNotifications();
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(main_window_handle->glf_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(main_window_handle->glf_window);
    }

    nlohmann::json a = {{"pi", 3.141},
                        {"happy", true},
                        {"name", "Niels"},
                        {"nothing", nullptr},
                        {"answer", {{"everything", 42}}},
                        {"list", {1, 0, 2}},
                        {"object", {{"currency", "USD"}, {"value", 42.99}}}};
    SPDLOG_INFO("test, {}", a.dump(4));

    tl::expected<void, std::string> b;

    return 0;
}
