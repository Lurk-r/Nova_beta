#pragma once
#include "WSHelpers.hpp"
#include <json.hpp>

using json = nlohmann::ordered_json;

namespace Commands
{
    // MISC
    json Reload();
}