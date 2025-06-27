#include "named_pipe_ipc.h"
#include <windows.h>
#include <string>
#include <iostream>

void pipe_server(std::function<std::string(const std::string&)> on_command) {
    while (true) {
        HANDLE hPipe = CreateNamedPipeA(
            PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            1, 4096, 4096, 0, NULL);
        if (hPipe == INVALID_HANDLE_VALUE) continue;
        if (!ConnectNamedPipe(hPipe, NULL)) { CloseHandle(hPipe); continue; }
        char buffer[4096] = {};
        DWORD read;
        if (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &read, NULL)) {
            std::string reply = on_command(std::string(buffer, read));
            DWORD written = 0;
            WriteFile(hPipe, reply.c_str(), (DWORD)reply.size(), &written, NULL);
        }
        DisconnectNamedPipe(hPipe);
        CloseHandle(hPipe);
    }
}

std::string pipe_send_command(const std::string& cmd) {
    HANDLE hPipe = CreateFileA(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hPipe == INVALID_HANDLE_VALUE) return "Service not running or pipe unavailable\n";
    DWORD written;
    WriteFile(hPipe, cmd.c_str(), (DWORD)cmd.size(), &written, NULL);
    char buffer[4096] = {};
    DWORD read = 0;
    ReadFile(hPipe, buffer, sizeof(buffer) - 1, &read, NULL);
    buffer[read] = 0;
    CloseHandle(hPipe);
    return std::string(buffer);
}