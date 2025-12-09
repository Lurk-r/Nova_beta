#pragma once
#include "WSHelpers.hpp"
#include "../Data/Lists.hpp"
#include "../../Variables/Variables.hpp"

#include <json.hpp>

using json = nlohmann::ordered_json;

namespace Commands
{
    // MISC
    json Reload();

    // ACC STUFF
    json Currency(bool spend = false);
    json AddConsumable(CommandsID CommandID, int consumable, int type);
}