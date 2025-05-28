#include "UiManager.h"

void UiManager::Update(float elapsedTime)
{
    //for (auto ui : uies)
    //{
    //    if (ui->GetComponent<Ui>())
    //    {

    //    }
    //}
}

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

void UiLoadingManager::Register(std::shared_ptr<Actor> ui)
{
    uies.emplace_back(ui);
}

void UiLoadingManager::Clear()
{
    uies.clear();
}

void UiLoadingManager::Remove(std::shared_ptr<Actor> ui)
{
    removes.insert(ui);
}
