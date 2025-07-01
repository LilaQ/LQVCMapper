#pragma once
#include <Windows.h>
#include <ViGEm/Client.h>
#include <vector>
#include <string>
#include <map>

struct ControllerInstance {
    PVIGEM_TARGET target = nullptr;
    XUSB_REPORT report = {};
};

class ControllerManager {
public:
    ControllerManager();
    ~ControllerManager();
    void activate_n(int n);
    void add_one();
    void stop_all();
    void update_buttons(int idx, const std::map<std::string, bool>& btns);
    std::vector<bool> status() const;
    int active_count() const;
    int desired_virtual_count();
private:
    PVIGEM_CLIENT client_;
    std::vector<ControllerInstance> ctrls_;
    int desired_virtual_count_ = 0;
};

extern ControllerManager controller_manager;