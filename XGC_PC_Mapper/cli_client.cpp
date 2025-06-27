#include <windows.h>
#include "named_pipe_ipc.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: cli_client.exe <command>\n";
        return 1;
    }
    std::string cmd;
    for (int i = 1; i < argc; ++i) {
        if (i > 1) cmd += " ";
        cmd += argv[i];
    }
    std::cout << pipe_send_command(cmd);
}