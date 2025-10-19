#include "Common.h"
#include <fstream>
#include <sstream>

std::optional<std::string> LoadTextFileOrEmpty(const std::string& filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open())
    {
        SPDLOG_ERROR("Failded To Open File: {}", filename);
        return {};
    }

    std::stringstream text;
    text << fin.rdbuf();
    return text.str();
}
