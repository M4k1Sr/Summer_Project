#include "UiManager.h"

UiManager::UiManager()
{
}

UiManager::~UiManager()
{
    Release();
}

void UiManager::Update()
{
    for (auto* Ui : UiList)
    {
        Ui->Update();
    }
}

void UiManager::Draw()
{
    for (auto* Ui : UiList)
    {
        Ui->Draw();
    }
}

void UiManager::Release()
{
    for (auto* Ui : UiList)
    {
        delete Ui;
    }

    UiList.clear();
}