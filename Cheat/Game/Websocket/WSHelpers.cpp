#include "WSHelpers.hpp"

#include <random>
#include <Obfusheader.hpp>

namespace WSFunctions
{
    std::string RandHex(int hexLength)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 255);

        std::stringstream ss;
        for (int i = 0; i < hexLength; i++)
        {
            ss << std::hex << std::setw(2) << std::setfill('0') << distrib(gen);
        }

        return ss.str();
    }

    int Random(int min, int max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(min, max);
        return distrib(gen);
    }

    std::string GetLocalDate()
    {
        std::time_t ActualTime = std::time(nullptr);
        std::tm* LocalDate = std::localtime(&ActualTime);

        std::ostringstream oss;
        oss << (LocalDate->tm_mon + 1) << '-'  << LocalDate->tm_mday << '-'  << (LocalDate->tm_year + 1900);

        return oss.str();
    }

    json CreateCommand(CommandsID id, const json& params, const std::vector<int>& u)
    {
        return {
            { OBF("id"), id },
            { OBF("ci"), RandHex() },
            { OBF("p"), params },
            { OBF("h"), json::object() },
            { OBF("u"), u }
        };
    }

    json CreateSnapshot(const json& commands)
    {
        return json::array({
            OBF("update_progress"),
            {
                { OBF("id"), CommandsID::Snapshot },
                { OBF("i"), RandHex() },
                { OBF("p"), { { OBF("c"), commands } } }
            }
        });
    }
}

namespace Constants
{
    const json EmptyCMD = WSFunctions::CreateCommand(CommandsID::EmptyCommand, json::object());

    const std::unordered_map<int, std::pair<int, int>> ConsumableMapping = {
        {0, {1, 10}}, {1, {2, 10}}, {2, {3, 10}}, // Chests
        {3, {1030, 1520}}, {4, {2030, 1520}}, {5, {3030, 1520}}, {6, {4030, 1520}}, {7, {6030, 1520}}, // Boosters
        {8, {24015, 1450}}, // VIP
        {9, {17018, 1230}}, // War Hero Chest
        {10, {1024, 1380}}, {11, {2024, 1380}}, {12, {3024, 1380}}, {13, {4024, 1380}}, {14, {5024, 1380}}, {15, {6024, 1380}} // Free Upgrades
    };

    const std::map<int, std::pair<int, int>> FortDataMap = {
        {1, {0, 14}}, {2, {1, 124}}, {3, {2, 4159}}, {4, {3, 7869}},
        {5, {4, 20239}}, {6, {5, 40469}}, {7, {6, 80939}},
        {8, {7, 170999}}, {9, {8, 341999}}, {10, {9, 342000}}
    };

    const std::map<int, std::pair<int, int>> TankDataMap = {
        {1, {0, 29}}, {2, {1, 499}}, {3, {2, 1349}}, {4, {3, 5649}},
        {5, {4, 10199}}, {6, {5, 18749}}, {7, {6, 38999}},
        {8, {7, 141999}}, {9, {8, 219999}}, {10, {9, 220000}}
    };
}