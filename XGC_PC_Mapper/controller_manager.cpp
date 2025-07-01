#include <Windows.h>
#include "controller_manager.h"
#include <ViGEm/Client.h>
#include "real_controller_monitor.h"

ControllerManager controller_manager;

ControllerManager::ControllerManager() {
    client_ = vigem_alloc();
    vigem_connect(client_);
    ctrls_.resize(4);
    start_real_controller_monitor();
}
ControllerManager::~ControllerManager() {
    stop_all();
    vigem_disconnect(client_);
    vigem_free(client_);
    stop_real_controller_monitor();
}
void ControllerManager::activate_n(int n) {

    desired_virtual_count_ = n;

	if (get_total_xinput_controllers_connected() > 0)         
        return; // Don't allow virtual controllers if any real ones are connected

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
    const SHORT stick_magnitude = 32767; // Use full analog stick range
    if (idx < 0 || idx >= (int)ctrls_.size()) return;
    auto& c = ctrls_[idx];
    if (!c.target) return;
    XUSB_REPORT rep = {};

    // Analog stick accumulators
    SHORT lx = 0, ly = 0;

    for (const auto& kv : btns) {
        if (!kv.second) continue;
        if (kv.first == "A") rep.wButtons |= XUSB_GAMEPAD_A;
        else if (kv.first == "B") rep.wButtons |= XUSB_GAMEPAD_B;
        else if (kv.first == "X") rep.wButtons |= XUSB_GAMEPAD_X;
        else if (kv.first == "Y") rep.wButtons |= XUSB_GAMEPAD_Y;
        else if (kv.first == "LB") rep.wButtons |= XUSB_GAMEPAD_LEFT_SHOULDER;
        else if (kv.first == "RB") rep.wButtons |= XUSB_GAMEPAD_RIGHT_SHOULDER;
        else if (kv.first == "START") rep.wButtons |= XUSB_GAMEPAD_START;
        else if (kv.first == "BACK") rep.wButtons |= XUSB_GAMEPAD_BACK;
        else if (kv.first == "UP") ly += stick_magnitude;
        else if (kv.first == "DOWN") ly -= stick_magnitude;
        else if (kv.first == "LEFT") lx -= stick_magnitude;
        else if (kv.first == "RIGHT") lx += stick_magnitude;
        else if (kv.first == "LT") rep.bLeftTrigger = 255;
        else if (kv.first == "RT") rep.bRightTrigger = 255;
    }

    rep.sThumbLX = lx;
    rep.sThumbLY = ly;

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

int ControllerManager::desired_virtual_count() {
    // Return the desired number of virtual controllers, i.e., those that should be active.
    return desired_virtual_count_;
}