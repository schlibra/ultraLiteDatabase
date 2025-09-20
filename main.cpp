#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>

#include "imgui.h"
#include "imgui_backends/imgui_impl_glfw.h"
#include "imgui_backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>


void load_member(int key, char *name, std::unordered_map<int, std::vector<std::string> > &data)
{
    std::stringstream ss(name);
    std::string token{};
    std::vector<std::string> result{};

    while (std::getline(ss, token, ';'))
    {
        result.emplace_back(token);
    }
    if (!data.empty())
    {
        if (result.size() == data.begin()->second.size())
        {
            data[key] = result;
        }
    } else
    {
        data[key] = result;
    }
}

void show_find(int key, std::unordered_map<int, std::vector<std::string> > &data)
{
    ImGui::Begin("Find");
    if (ImGui::BeginTable("Data", data[key].size() + 1))
    {
        // 设置表头
        // ImGui::TableSetupColumn("Key");
        // ImGui::TableSetupColumn("Name");
        // ImGui::TableSetupColumn("Details");
        // ImGui::TableHeadersRow();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        std::string temp = std::to_string(key);
        ImGui::Text(temp.c_str());
        for (int i = 0; i < data[key].size(); i++)
        {
            ImGui::TableSetColumnIndex(i + 1);
            ImGui::Text(data[key][i].c_str());
        }
        ImGui::EndTable();
    }
    ImGui::End();
}

void show_list(std::unordered_map<int, std::vector<std::string> > &data)
{
    ImGui::Begin("List");
    if (ImGui::TreeNode("Tree", "click to expand"))
    {
        if (!data.empty())
        {
            if (ImGui::BeginTable("Data", data.begin()->second.size() + 1))
            {
                // 设置表头
                // ImGui::TableSetupColumn("Key");
                // ImGui::TableSetupColumn("Name");
                // ImGui::TableSetupColumn("Details");
                // ImGui::TableHeadersRow();
                for (const auto &[key, value]: data)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    std::string temp = std::to_string(key);
                    ImGui::Text(temp.c_str());
                    for (int i = 0; i < value.size(); i++)
                    {
                        ImGui::TableSetColumnIndex(i + 1);
                        ImGui::Text(value[i].c_str());
                    }
                }
                ImGui::EndTable();
            }
        }
        ImGui::TreePop();
    }
    ImGui::End();
}

int main()
{
    // 初始化 GLFW =========================================================================================
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建窗口
    GLFWwindow *window = glfwCreateWindow(800, 600, "ImGui Test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    // 初始化 ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    // 初始化平台/渲染后端
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __APPLE__
    const char *glsl_version = "#version 150";
#else
    const char *glsl_version = "#version 330";
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    static std::unordered_map<int, std::vector<std::string> > data{};

    static bool list_statues = false, remove_statues = true, find_statues = false;
    static int key_pressed = 0;

    // 主循环 -----------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 输入窗口
        ImGui::Begin("Input");
        static int key = 0;
        ImGui::InputInt("Key", &key, 1);
        static char name[256] = "";
        ImGui::InputTextWithHint("Contents", "use english; to separate item", name, sizeof(name));
        ImGui::End();

        // 命令窗口
        ImGui::Begin("Command");
        if (ImGui::Button("Submit"))
        {
            load_member(key, name, data);
        }
        if (ImGui::Button("Remove"))
        {
            remove_statues = data.erase(key);
        }
        if (ImGui::Button("Find"))
        {
            if (data.find(key) != data.end())
            {
                find_statues = true;
                key_pressed = key;
            } else
            {
                find_statues = false;
            }
        }
        if (ImGui::Button("List"))
        {
            list_statues = !list_statues;
        }
        if (!remove_statues)
        {
            ImGui::Text("error! there is no member with key %d", key);
        }
        if (find_statues)
        {
            show_find(key_pressed, data);
        }
        ImGui::End();

        // 数据展示窗口
        if (list_statues)
        {
            show_list(data);
        }

        // 打开 Demo 窗口，确认 UI 工作正常
        // ImGui::ShowDemoWindow();

        // 渲染
        ImGui::Render();
        int display_w{}, display_h{};
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 清理
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
