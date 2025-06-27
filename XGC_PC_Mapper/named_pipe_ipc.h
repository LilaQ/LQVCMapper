#pragma once
#include <string>
#include <functional>

constexpr const char* PIPE_NAME = R"(\\.\pipe\vigem_svc)";

void pipe_server(std::function<std::string(const std::string&)> on_command);
std::string pipe_send_command(const std::string& cmd);