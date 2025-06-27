#include <Windows.h>
#include "controller_manager.h"
#include <ViGEm/Client.h>

ControllerManager::ControllerManager() {
    client_ = vigem_alloc();
    vigem_connect(client_);
    ctrls_.resize(4);
}
ControllerManager::~ControllerManager() {
    stop_all();
    vigem_disconnect(client_);
    vigem_free(client_);
}
void ControllerManager::activate_n(int n) {
    if (n < 0) n = 0;
    if (n > 4) n = 4;
    for (int i = 0; i < 4; ++i) {
        if (i < n) {
            if (!ctrls_[i].target) {
                ctrls_[i].target = vigem_target_x360_alloc();
                vigem_target_add(client_, ctrls_[i].target);
            }
        }
        else {
            if (ctrls_[i].target) {
                vigem_target_remove(client_, ctrls_[i].target);
                vigem_target_free(ctrls_[i].target);
                ctrls_[i].target = nullptr;
            }
        }
    }
}
void ControllerManager::add_one() {
    for (int i = 0; i < 4; ++i) {
        if (!ctrls_[i].target) {
            ctrls_[i].target = vigem_target_x360_alloc();
            vigem_target_add(client_, ctrls_[i].target);
            break;
        }
    }
}
void ControllerManager::stop_all() {
    for (auto& c : ctrls_)
        if (c.target) {
            vigem_target_remove(client_, c.target);
            vigem_target_free(c.target);
            c.target = nullptr;
        }
}
void ControllerManager::update_buttons(int idx, const std::map<std::string, bool>& btns) {
    if (idx < 0 || idx >= (int)ctrls_.size()) return;
    auto& c = ctrls_[idx];
    if (!c.target) return;
    XUSB_REPORT rep = {};
    for (auto&& kv : btns) {
        if (!kv.second) continue;
        if (kv.first == "A") rep.wButtons |= XUSB_GAMEPAD_A;
        if (kv.first == "B") rep.wButtons |= XUSB_GAMEPAD_B;
        if (kv.first == "X") rep.wButtons |= XUSB_GAMEPAD_X;
        if (kv.first == "Y") rep.wButtons |= XUSB_GAMEPAD_Y;
        if (kv.first == "LB") rep.wButtons |= XUSB_GAMEPAD_LEFT_SHOULDER;
        if (kv.first == "RB") rep.wButtons |= XUSB_GAMEPAD_RIGHT_SHOULDER;
        if (kv.first == "START") rep.wButtons |= XUSB_GAMEPAD_START;
        if (kv.first == "BACK") rep.wButtons |= XUSB_GAMEPAD_BACK;
        if (kv.first == "UP") rep.wButtons |= XUSB_GAMEPAD_DPAD_UP;
        if (kv.first == "DOWN") rep.wButtons |= XUSB_GAMEPAD_DPAD_DOWN;
        if (kv.first == "LEFT") rep.wButtons |= XUSB_GAMEPAD_DPAD_LEFT;
        if (kv.first == "RIGHT") rep.wButtons |= XUSB_GAMEPAD_DPAD_RIGHT;
        if (kv.first == "LT") rep.bLeftTrigger = 255;
        if (kv.first == "RT") rep.bRightTrigger = 255;
    }
    c.report = rep;
    vigem_target_x360_update(client_, c.target, rep);
}
std::vector<bool> ControllerManager::status() const {
    std::vector<bool> s;
    for (const auto& c : ctrls_) s.push_back(c.target != nullptr);
    return s;
}
int ControllerManager::active_count() const {
    int n = 0;
    for (const auto& c : ctrls_) if (c.target) ++n;
    return n;
}