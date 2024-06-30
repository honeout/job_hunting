#include "UiManager.h"

void UiManager::Register(std::shared_ptr<Actor> ui)
{
    uies.emplace_back(ui);
}

void UiManager::Clear()
{
    uies.clear();
}

void UiManager::Remove(std::shared_ptr<Actor> ui)
{
    removes.insert(ui);
}
