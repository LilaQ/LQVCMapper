#include <windows.h>
#include "controller_manager.h"
#include "named_pipe_ipc.h"
#include "keyboard_input.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <map>
#include <thread>
#include "keynames.h"
#include "tray_icon.h"

using json = nlohmann::json;

std::vector<KeyboardInputThread::KeyMap> load_mappings(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Could not open mappings.json, using empty mappings!\n";
        return std::vector<KeyboardInputThread::KeyMap>(4);
    }
    json j; f >> j;
    std::vector<KeyboardInputThread::KeyMap> out;
    for (const auto& jc : j["controllers"]) {
        KeyboardInputThread::KeyMap km;
        for (auto it = jc.begin(); it != jc.end(); ++it) {
            std::string btn = it.key();
            std::string key = it.value();
            km[btn] = keyname_to_vk(key); // SUPPORTS ALL KEYS
        }
        out.push_back(km);
    }
    while (out.size() < 4) out.emplace_back();
    return out;
}

// Forward declaration
LRESULT CALLBACK TrayWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int) {
    // 1. Register a hidden window for tray messages
    WNDCLASSEX wc = { sizeof(wc) };
    wc.lpfnWndProc = TrayWndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ViGEmTrayClass";
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindowEx(0, wc.lpszClassName, L"", 0, 0, 0, 0, 0,
        HWND_MESSAGE, NULL, hInstance, NULL);

    // 2. Load an icon (see below for custom icon instructions)
    HICON hIcon = LoadIcon(NULL, IDI_APPLICATION);

    // 3. Create tray icon
    TrayIcon tray(hWnd, 1, L"LQ - Virtual Controller Service", hIcon);

    // 4. Start your service logic in a thread
    std::thread service_thread([&] {
        ControllerManager controllers;
        controllers.activate_n(1);

        auto mappings = load_mappings("mappings.json");

        KeyboardInputThread kb_thread(
            mappings,
            [&](int idx, std::map<std::string, bool> btns) { controllers.update_buttons(idx, btns); }
        );

        auto handle_command = [&](const std::string& cmd) -> std::string {
            std::istringstream iss(cmd);
            std::string token;
            iss >> token;
            if (token == "status") {
                std::ostringstream oss;
                auto s = controllers.status();
                for (size_t i = 0; i < s.size(); ++i)
                    oss << "Controller " << (i + 1) << ": " << (s[i] ? "Active" : "Inactive") << "\n";
                return oss.str();
            }
            else if (token == "stop") {
                controllers.stop_all();
                return "All controllers stopped\n";
            }
            else if (token == "start") {
                int n = 1; iss >> n;
                controllers.activate_n(n);
                return "Started " + std::to_string(controllers.active_count()) + " controller(s)\n";
            }
            else if (token == "add") {
                controllers.add_one();
                return "Now " + std::to_string(controllers.active_count()) + " controller(s) active\n";
            }
            return "Unknown command\n";
            };

        std::cout << "LQ - Virtual Controller Service running.\n";
        pipe_server(handle_command);
        });

    // 5. Run message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 6. Clean up
    service_thread.detach();
    return 0;
}

// Tray icon message handler
LRESULT CALLBACK TrayWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_TRAYICON) {
        if (lParam == WM_RBUTTONUP) {
            POINT pt;
            GetCursorPos(&pt);
            HMENU hMenu = CreatePopupMenu();
            AppendMenuW(hMenu, MF_STRING, 1, L"Exit");
            SetForegroundWindow(hWnd);
            int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, hWnd, NULL);
            DestroyMenu(hMenu);
            if (cmd == 1) PostQuitMessage(0);
        }
    }
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}