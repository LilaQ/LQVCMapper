#include "keyboard_input.h"
#include <windows.h>
#include <chrono>

KeyboardInputThread::KeyboardInputThread(const std::vector<KeyMap>& mappings, Callback cb)
    : mappings_(mappings), cb_(cb), running_(true) {
    th_ = std::thread([this]() { run(); });
}
KeyboardInputThread::~KeyboardInputThread() { stop(); }
void KeyboardInputThread::stop() { running_ = false; if (th_.joinable()) th_.join(); }
void KeyboardInputThread::run() {
    std::vector<std::map<std::string, bool>> btn_states(mappings_.size());
    while (running_) {
        for (size_t i = 0; i < mappings_.size(); ++i) {
            auto& km = mappings_[i];
            std::map<std::string, bool> btns;
            for (const auto& kv : km) {
                SHORT key_state = GetAsyncKeyState(kv.second);
                btns[kv.first] = (key_state & 0x8000) != 0;
            }
            btn_states[i] = btns;
            cb_((int)i, btns);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}