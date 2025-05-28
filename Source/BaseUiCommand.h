#pragma once
#include <unordered_map>
#include "Component.h"
#include "SpriteControll.h"

class BaseUiCommand : public Component
{
public:
    BaseUiCommand() {};
    ~BaseUiCommand() override;

public:

    enum class CommandMain
    {
        Battole,
        Magic,
        Special
    };

    enum class CommandMagicSeconde
    {
        Fire,
        Thander,
        Ice,
        Heale
    };

    enum class CommandSpecialSeconde
    {
        SlashSpecial,
        FireSpecial
    };

private:
    std::unordered_map<std::string, bool> commandPages;
};
