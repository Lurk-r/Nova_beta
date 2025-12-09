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

#pragma region ACC_STUFF
    json Currency(bool spend)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(spend ? CommandsID::SpendCurrency : CommandsID::AddCurrency, {
                {OBF("c"), Lists::Websocket::m_ccCurrencies[Variables::Websocket::m_iCurrencyType]},
                {OBF("v"), Variables::Websocket::m_iCurrencyAmount},
                {OBF("ca"), AddCurrencyCause::GameTask}
            }
        ));

        return WSFunctions::CreateSnapshot(commands);
    }

    json AddConsumable(CommandsID CommandID, int consumable, int type)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandID, { 
                {OBF("c"), Variables::Websocket::m_iConsumableAmount},
                {OBF("i"), consumable},
                {OBF("t"), type}
            }
        ));

        return WSFunctions::CreateSnapshot(commands);
    }
#pragma endregion
}