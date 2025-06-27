#pragma once
#include <map>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <functional>

class KeyboardInputThread {
public:
    using KeyMap = std::map<std::string, int>; // "A" -> VK_K
    using Callback = std::function<void(int controller_idx, std::map<std::string, bool> btns)>;
    KeyboardInputThread(const std::vector<KeyMap>& mappings, Callback cb);
    ~KeyboardInputThread();
    void stop();
private:
    std::vector<KeyMap> mappings_;
    std::thread th_;
    std::atomic<bool> running_;
    Callback cb_;
    void run();
};