#include "Commands.hpp"
#include <Obfusheader.hpp>

namespace Commands
{
#pragma region MISC
    json Reload()
    {
        return json::array({ OBF("update_progress"), json::object() });
    }
#pragma endregion
}