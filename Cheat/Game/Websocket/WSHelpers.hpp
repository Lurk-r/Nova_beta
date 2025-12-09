#pragma once

#include <string>
#include <json.hpp>
#include "../Data/Enums.hpp"

using json = nlohmann::ordered_json;

namespace WSFunctions
{
    std::string RandHex(int hexLength = 4);
    std::string GetLocalDate();

    int Random(int min, int max);

    json CreateCommand(CommandsID id, const json& params, const std::vector<int>& u = { 0 });
    json CreateSnapshot(const json& commands);
}

namespace Constants
{
    extern const json EmptyCMD;

    extern const std::unordered_map<int, std::pair<int, int>> ConsumableMapping;
    extern const std::map<int, std::pair<int, int>> FortDataMap;
    extern const std::map<int, std::pair<int, int>> TankDataMap;
}