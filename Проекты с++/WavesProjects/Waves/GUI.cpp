#pragma once
#include "GUI.h"
#include "Logs.h"

#include <thread>
#include <Windows.h>
#include <filesystem>
#include <sstream>
#include "imgui/imgui_internal.h"

enum class FileTypes
{
    Unknown,
    Map
};

struct listed_file
{
    string nameFull;
    string nameShort;
    string fileSize;
    string fullCaption;

    bool isSelected = false;

    FileTypes type = FileTypes::Unknown;
};

float GetMemoryStatus()
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    return statex.ullTotalPhys / 1024 / 1024 / 1024.f;
}

void LoadFileNames(std::vector<listed_file>& allFilenames)
{
    std::filesystem::path DirectoryName = "Saves";

    //Check if folder exists
    if (!std::filesystem::exists(DirectoryName))
    {
        //If not create a folder
        std::filesystem::create_directory(DirectoryName);
    }

    //Load list of filenames
    allFilenames.clear();

    for (const auto& entry : std::filesystem::directory_iterator(DirectoryName))
    {
        //Skip folders
        if (entry.is_directory())
            continue;

        listed_file f;

        //Full paths to files
        f.nameFull = entry.path().string();

        //Only file name
        f.nameShort = entry.path().filename().string();

        //File size
        uint size = entry.file_size();
        string unit;

        //Units
        if (size > 1000000)
        {
            size /= 1000000;
            unit = "MB";
        }
        else if (size > 1000)
        {
            size /= 1000;
            unit = "KB";
        }
        else
        {
            unit = "B";
        }

        f.fileSize += std::to_string(size);
        f.fileSize += unit;

        //Is world
        unit = f.nameShort;
        size_t pos = unit.rfind('.') + 1;
        unit = unit.substr(pos);
        if (unit == "map") f.type = FileTypes::Map;

        //Full file description
        f.fullCaption = f.nameShort;
        f.fullCaption.resize(25, ' ');
        f.fullCaption += f.fileSize;
        f.fullCaption.resize(40, ' ');
        //f.fullCaption += f.type;

        allFilenames.push_back(f);
    }
}

void DrawMenuInfo(float &fps, DynamicParams& params, const Simulation& sim)
{
    static bool menu_enabled[2] = { 1, 0 };
    static int menu_selected = 0;

    ImGui::Begin("Info", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
    {
        ImGui::BeginMenuBar();
        if (ImGui::MenuItem("Simulate", nullptr, &menu_enabled[0])) { menu_enabled[0] = 1; menu_enabled[1] = 0; menu_selected = 0; }
        if (ImGui::MenuItem("System", nullptr, &menu_enabled[1])) { menu_enabled[0] = 0; menu_enabled[1] = 1; menu_selected = 1; }
        ImGui::EndMenuBar();

        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);

        switch (menu_selected)
        {
        case 0:
            ImGui::Text("FPS: %.2f", fps);
            ImGui::Text("Seed: %d", params.seed);
            ImGui::Text("Object total: %d", sim.getCountBlocks());
            break;
        case 1:
            ImGui::Text("CPU cores: %d", std::thread::hardware_concurrency());
            ImGui::Text("RAM: %.2f Gb", GetMemoryStatus());
            ImGui::Text("Threads used: %d", 1);
            break;
        }
    }
    ImGui::End();
}

void DrawMenuControls(DynamicParams& params, Simulation& sim)
{
    ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
    {
        ImGui::Checkbox("Pause", &params.is_pause);
        ImGui::BeginTable("Restart/Reset", 2);
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if(ImGui::Button("Restart", ImVec2(130, 30)))
            sim.restart();
        ImGui::TableNextColumn();
        if (ImGui::Button("Reset to Default", ImVec2(130, 30)))
            sim.reset2Default();
        ImGui::EndTable();

        ImGui::SliderInt("Brash size", &params.brash_size, 50, 300);
        ImGui::SliderInt("Mass", &params.block_mass, 1, 500);
    }
    ImGui::End();
}

void DrawMenuMouseMode(DynamicParams& params)
{
    static bool mouse_mode[] = { 1, 0, 0, 0 };
    ImGui::Begin("Mouse mode", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
    {
        ImGui::BeginTable("mouse_modes", 2, ImGuiTableFlags_PadOuterX);
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::RadioButton("None", mouse_mode[0])) { mouse_mode[0] = 1; mouse_mode[1] = 0; mouse_mode[2] = 0; mouse_mode[3] = 0; params.mouse_mode = 0; params.block_select = false; }
        ImGui::TableNextColumn();
        if (ImGui::RadioButton("Brash", mouse_mode[3])) { mouse_mode[0] = 0; mouse_mode[1] = 0; mouse_mode[2] = 0; mouse_mode[3] = 1; params.mouse_mode = 3; params.block_select = false; }
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::RadioButton("Select", mouse_mode[1])) { mouse_mode[0] = 0; mouse_mode[1] = 1; mouse_mode[2] = 0; mouse_mode[3] = 0; params.mouse_mode = 1; params.block_select = false; }
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::RadioButton("Move", mouse_mode[2])) { mouse_mode[0] = 0; mouse_mode[1] = 0; mouse_mode[2] = 1; mouse_mode[3] = 0; params.mouse_mode = 2; params.block_select = false; }
        ImGui::EndTable();
    }
    ImGui::End();
}

void DrawMenuDisplayMode(DynamicParams& params)
{
    static bool display_mode[] = { 1, 0, 0 };
    ImGui::Begin("Display mode", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
    {
        if (ImGui::RadioButton("Default", display_mode[0])) { display_mode[0] = 1; display_mode[1] = 0; display_mode[2] = 0; params.display_mode = 0; }
        if (ImGui::RadioButton("No blocks", display_mode[1])) { display_mode[0] = 0; display_mode[1] = 1; display_mode[2] = 0; params.display_mode = 1; }
        if (ImGui::RadioButton("Smoothed", display_mode[2])) { display_mode[0] = 0; display_mode[1] = 0; display_mode[2] = 1; params.display_mode = 2; }
    }
    ImGui::End();
}

void DrawMenuAdditionOptions(DynamicParams& params, bool& show_save_load, bool& show_zooblast_info)
{
    ImGui::Begin("Addition options", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
    {
        //if (ImGui::Button("Save/Load", ImVec2(80, 20)))
        ImGui::Checkbox("Show Save/Load menu", &show_save_load);
        ImGui::Checkbox("Show logs", &params.show_console);
        ImGui::Checkbox("Show block info", &show_zooblast_info);
    }
    ImGui::End();
}

void DrawMenuLogs(bool show_console)
{
    if (show_console)
    {
        ImGui::SetNextWindowSize(ImVec2(500, 250));
        ImGui::Begin("Logs", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);
        {

            if (ImGui::Button("Clear logs"))
                logClear();
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.24, 0.24, 0.27, 1.00f));

            ImGui::BeginChild("scrolling", ImVec2(480, 190), true);
            {
                ImGui::TextUnformatted(getLogText().Buf.Data);

                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
            }
            ImGui::EndChild();

            ImGui::PopStyleColor();
        }
        ImGui::End();
    }
}

void DrawSaveLoadMenu(DynamicParams& params, bool show_save_load, Simulation& sim)
{
    static char buffer[25];
    if (!show_save_load)
    {
        memset(buffer, '\0', 25);
        return;
    }

    static std::vector<listed_file> allFilenames;
    static uint selectedFile = -1;

    ImGui::SetNextWindowSize(ImVec2(300, 300));
    ImGui::Begin("Save\\Load Menu", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
    {
        //List of files
        ImGui::Text("File list:");

        LoadFileNames(allFilenames);
        char* text_hint = (char*)"";
        if (selectedFile != -1)
        {
            for (int b = 0; b < allFilenames.size(); b++)
                allFilenames[b].isSelected = false;

            if (selectedFile < allFilenames.size())
            {
                allFilenames[selectedFile].isSelected = true;
                text_hint = (char*)allFilenames[selectedFile].nameShort.c_str();
            }
        }

        ImGui::ListBoxHeader("##", ImVec2(273, 180));

        for (int i = 0; i < allFilenames.size(); ++i)
        {
            if (ImGui::Selectable(allFilenames[i].fullCaption.c_str(), &allFilenames[i].isSelected))
                selectedFile = i;
        }

        ImGui::ListBoxFooter();

        ImGui::Text("Select file");
        ImGui::InputTextWithHint(" ", text_hint, buffer, sizeof(buffer) - 1);
        ImGui::BeginTable("File selected", 3);
        {
            char* filename;
            FileTypes file_type;
            string path = "Saves\\";

            if (!buffer[0])
                filename = text_hint;
            else
                filename = buffer;

            string unit = filename;
            size_t pos = unit.rfind('.') + 1;
            unit = unit.substr(pos);
            if (unit == "map") file_type = FileTypes::Map;
            else file_type = FileTypes::Unknown;


            ImGui::TableNextColumn();
            if (ImGui::Button("Save", ImVec2(80, 20)))
            {
                switch (file_type)
                {
                case FileTypes::Unknown:
                    logPrint("File: \"%s\" is of unknown type\n", filename);
                    logPrint("To save the map, use '.map'\n");
                    params.show_console = true;
                    break;
                case FileTypes::Map:
                    sim.saveMap(filename);
                    break;
                }
            }

            ImGui::TableNextColumn();
            if (ImGui::Button("Load", ImVec2(80, 20)) && selectedFile != -1)
            {
                switch (file_type)
                {
                case FileTypes::Unknown:
                    logPrint("File: \"%s\" is of unknown type\n", filename);
                    logPrint("To save the map, use '.map'\n");
                    params.show_console = true;
                    break;
                case FileTypes::Map:
                    sim.loadMap(filename);
                    break;
                }
            }

            ImGui::TableNextColumn();
            if (ImGui::Button("Delete", ImVec2(80, 20)) && selectedFile != -1)
            {
                string filepath = path + filename;
                if (remove(filepath.c_str()))
                {
                    logPrint("File \"%s\" not found\n", filename);
                    params.show_console = true;
                }
                else
                    logPrint("File \"%s\" deleted\n", filename);
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
}

float clamp(float value, float min, float max)
{
    return std::fmin(std::fmax(value, min), max);
}

void drawArrow(float power)
{
    if (power < 0)
    {
        float p = 1 - power;
        float pos_y = 85 - p * 10;
        pos_y = clamp(pos_y, 20, 140);
        ImGui::DrawLine(sf::Vector2f(85, 85), sf::Vector2f(85, pos_y), sf::Color::Red, 2);

        ImGui::DrawLine(sf::Vector2f(85, pos_y), sf::Vector2f(75, pos_y + 10), sf::Color::Red, 2);
        ImGui::DrawLine(sf::Vector2f(85, pos_y), sf::Vector2f(95, pos_y + 10), sf::Color::Red, 2);
    }
    
    if (power > 0)
    {
        float p = power;
        float pos_y = 85 + p * 10;
        pos_y = clamp(pos_y, 85, 150);
        ImGui::DrawLine(sf::Vector2f(85, 85), sf::Vector2f(85, pos_y), sf::Color::Red, 2);

        ImGui::DrawLine(sf::Vector2f(85, pos_y), sf::Vector2f(75, pos_y - 10), sf::Color::Red, 2);
        ImGui::DrawLine(sf::Vector2f(85, pos_y), sf::Vector2f(95, pos_y - 10), sf::Color::Red, 2);
    }
}

void DrawBlockMenu(DynamicParams& params, bool show_bot_info, Simulation& sim)
{
    if (show_bot_info)
    {
        ImGui::SetNextWindowSize(ImVec2(600, 300));
        ImGui::Begin("Block info", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
        {
            if (!params.block_select && params.mouse_mode != 1)
                ImGui::Text("Please select block to view info");
            else
            {
                sf::Vector2f pos = sim.getSelectPosition();
                ImGui::Text("Position: x: %.2f, y: %.2f", pos.x, pos.y);
                sf::Vector2f vec = sim.getSelectVelosity();
                ImGui::Text("Velosity: x: %.2f, y: %.2f", vec.x, vec.y);
                float mass = sim.getSelectMass();
                ImGui::Text("Mass: %.2f", mass);
                bool is_fixed = sim.getSelectFixed();
                if(is_fixed)
                    ImGui::Text("Is fixed: true");
                else
                    ImGui::Text("Is fixed: false");

                sf::FloatRect rect;
                rect.left = 10;
                rect.top = 10;
                rect.width = 150;
                rect.height = 150;

                ImGui::DrawRectFilled(rect, sim.getSelectColor());
                ImGui::DrawRect(rect, sf::Color::White);

                drawArrow(vec.y);
            }
        }
        ImGui::End();
    }
}

void ImGuiMenu(sf::RenderWindow& window, sf::Clock& delta_clock, DynamicParams& params, Simulation& sim)
{
    int windowX = window.getSize().x;
    int windowY = window.getSize().y;
    ImVec2 window_size = { 300, 120 };

    static bool show_console = false;
    static bool show_save_load = false;
    static bool show_bot_info = false;

    static int current_file = 0;

    static float fps = 0;
    float curent_time = delta_clock.getElapsedTime().asSeconds();

    static float time;
    time += curent_time;
    if (time > 0.05f)
    {
        time = 0;
        fps = 1 / curent_time;
    }

    ImGui::SFML::Update(window, delta_clock.restart());

    // Info //
    float window_offset = 0;
    ImGui::SetNextWindowSize(ImVec2(300, 120));
    ImGui::SetNextWindowPos(ImVec2(windowX - window_size.x, window_offset));
    DrawMenuInfo(fps, params, sim);
    // Info END //


    // Controls //
    window_offset += window_size.y;
    ImGui::SetNextWindowSize(ImVec2(window_size.x, 260));
    ImGui::SetNextWindowPos(ImVec2(windowX - window_size.x, window_offset));
    DrawMenuControls(params, sim);
    // Controls END //

    // Mouse mode //
    window_offset += 260;
    ImGui::SetNextWindowSize(ImVec2(window_size.x, 105));
    ImGui::SetNextWindowPos(ImVec2(windowX - window_size.x, window_offset));
    DrawMenuMouseMode(params);
    // Mouse mode END //

    // Display mode //
    window_offset += 105;
    ImGui::SetNextWindowSize(ImVec2(window_size.x, 123));
    ImGui::SetNextWindowPos(ImVec2(windowX - window_size.x, window_offset));
    DrawMenuDisplayMode(params);
    // Display mode END //

    // Addition options //
    window_offset += 123;
    ImGui::SetNextWindowSize(ImVec2(window_size.x, windowY - window_offset));
    ImGui::SetNextWindowPos(ImVec2(windowX - window_size.x, window_offset));
    DrawMenuAdditionOptions(params, show_save_load, show_bot_info);
    // Addition options END //

    // Logs
    DrawMenuLogs(params.show_console);

    // Save/Load
    DrawSaveLoadMenu(params, show_save_load, sim);

    // Block Info
    DrawBlockMenu(params, show_bot_info, sim);

    params.focus_on_field = true;
    if (params.mouse_pos.x > windowX - window_size.x || ImGui::IsWindowFocused(ImGuiFocusedFlags_::ImGuiFocusedFlags_AnyWindow) || ImGui::IsWindowHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_AnyWindow))
    {
        params.focus_on_field = false;

        if (params.mouse_mode == 2)
        {
            params.block_drag = false;
            params.block_drag_id = -1;
        }
    }
}