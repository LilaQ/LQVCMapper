#pragma once
#include <string>
#include <unordered_map>
#include <windows.h>

inline int keyname_to_vk(const std::string& name) {
    static const std::unordered_map<std::string, int> map = {
        // Letters
        {"A", 'A'}, {"B", 'B'}, {"C", 'C'}, {"D", 'D'}, {"E", 'E'}, {"F", 'F'},
        {"G", 'G'}, {"H", 'H'}, {"I", 'I'}, {"J", 'J'}, {"K", 'K'}, {"L", 'L'},
        {"M", 'M'}, {"N", 'N'}, {"O", 'O'}, {"P", 'P'}, {"Q", 'Q'}, {"R", 'R'},
        {"S", 'S'}, {"T", 'T'}, {"U", 'U'}, {"V", 'V'}, {"W", 'W'}, {"X", 'X'},
        {"Y", 'Y'}, {"Z", 'Z'},
        // Numbers (top row)
        {"0", '0'}, {"1", '1'}, {"2", '2'}, {"3", '3'}, {"4", '4'}, {"5", '5'},
        {"6", '6'}, {"7", '7'}, {"8", '8'}, {"9", '9'},
        // Numpad
        {"Num0", VK_NUMPAD0}, {"Num1", VK_NUMPAD1}, {"Num2", VK_NUMPAD2}, {"Num3", VK_NUMPAD3},
        {"Num4", VK_NUMPAD4}, {"Num5", VK_NUMPAD5}, {"Num6", VK_NUMPAD6}, {"Num7", VK_NUMPAD7},
        {"Num8", VK_NUMPAD8}, {"Num9", VK_NUMPAD9}, {"Numpad0", VK_NUMPAD0}, {"Numpad1", VK_NUMPAD1},
        {"Numpad2", VK_NUMPAD2}, {"Numpad3", VK_NUMPAD3}, {"Numpad4", VK_NUMPAD4},
        {"Numpad5", VK_NUMPAD5}, {"Numpad6", VK_NUMPAD6}, {"Numpad7", VK_NUMPAD7},
        {"Numpad8", VK_NUMPAD8}, {"Numpad9", VK_NUMPAD9},
        {"NumLock", VK_NUMLOCK}, {"NumpadEnter", VK_RETURN}, {"NumpadPlus", VK_ADD},
        {"NumpadMinus", VK_SUBTRACT}, {"NumpadMultiply", VK_MULTIPLY}, {"NumpadDivide", VK_DIVIDE},
        {"NumpadDot", VK_DECIMAL},
        // Arrows
        {"Left", VK_LEFT}, {"Right", VK_RIGHT}, {"Up", VK_UP}, {"Down", VK_DOWN},
        // Special keys
        {"Enter", VK_RETURN}, {"Tab", VK_TAB}, {"Space", VK_SPACE}, {"LShift", VK_LSHIFT},
        {"RShift", VK_RSHIFT}, {"LControl", VK_LCONTROL}, {"RControl", VK_RCONTROL},
        {"LAlt", VK_LMENU}, {"RAlt", VK_RMENU}, {"Backspace", VK_BACK}, {"CapsLock", VK_CAPITAL},
        {"Esc", VK_ESCAPE}, {"Escape", VK_ESCAPE}, {"PageUp", VK_PRIOR}, {"PageDown", VK_NEXT},
        {"Home", VK_HOME}, {"End", VK_END}, {"Insert", VK_INSERT}, {"Delete", VK_DELETE},
        {"ScrollLock", VK_SCROLL},
        // Function keys
        {"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3}, {"F4", VK_F4}, {"F5", VK_F5}, {"F6", VK_F6},
        {"F7", VK_F7}, {"F8", VK_F8}, {"F9", VK_F9}, {"F10", VK_F10}, {"F11", VK_F11}, {"F12", VK_F12},
        // Punctuation
        {",", VK_OEM_COMMA}, {".", VK_OEM_PERIOD}, {";", VK_OEM_1}, {"'", VK_OEM_7}, {"-", VK_OEM_MINUS},
        {"=", VK_OEM_PLUS}, {"/", VK_OEM_2}, {"\\", VK_OEM_5}, {"[", VK_OEM_4}, {"]", VK_OEM_6},
        {"`", VK_OEM_3}
    };
    auto it = map.find(name);
    if (it != map.end()) return it->second;
    if (name.size() == 1) return static_cast<int>(name[0]);
    return 0;
}