#pragma once
#include <vector>
#include <cstdint>

namespace Cheat
{
    void Init();

    namespace Assets {
        inline std::vector<uint8_t> BackgroundImageBytes;
        inline bool HasBytes = false;
    }
}