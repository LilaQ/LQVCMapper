#include <windows.h>
#include <Xinput.h>
#include <atomic>
#include <thread>
#include "controller_manager.h"

// Forward declaration of your controller manager instance and desired virtual count.
extern ControllerManager controller_manager;
extern int desired_virtual_count;

std::atomic<bool> monitor_thread_running{ false };

int get_total_xinput_controllers_connected() {
    int count = 0;
    for (DWORD i = 0; i < 4; ++i) {
        XINPUT_STATE state = {};
        if (XInputGetState(i, &state) == ERROR_SUCCESS)
            ++count;
    }
    return count;
}

void real_controller_monitor_thread() {
    bool virtuals_active = true;
    while (monitor_thread_running) {
        int desired = controller_manager.desired_virtual_count();
        int active = get_total_xinput_controllers_connected();

        if (virtuals_active && active > desired) {
            // A real controller has preempted a slot, stop all virtual ones
            controller_manager.stop_all();
            virtuals_active = false;
        }
        else if (!virtuals_active && active == 0 && desired > 0) {
            // No controllers are present; time to restore virtuals
            controller_manager.activate_n(desired);
            virtuals_active = true;
        }
        Sleep(1000); // Poll every second
    }
}

void start_real_controller_monitor() {
    if (monitor_thread_running) return;
    monitor_thread_running = true;
    std::thread(real_controller_monitor_thread).detach();
}

void stop_real_controller_monitor() {
    monitor_thread_running = false;
}