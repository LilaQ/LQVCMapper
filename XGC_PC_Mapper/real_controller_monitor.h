#pragma once

// Starts the background monitor thread that manages virtual controllers
// based on the presence of real controllers.
void start_real_controller_monitor();

// Stops the background monitor thread.
void stop_real_controller_monitor();

int get_total_xinput_controllers_connected();