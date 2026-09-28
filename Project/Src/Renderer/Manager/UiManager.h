#pragma once
#include <vector>
#include <algorithm>

#include "../Base/UiBase.h"

class UiManager
{
public:

    UiManager();
    ~UiManager();
    void Update();
    void Draw();
    void Release();

private:

	std::vector<UiBase*> UiList;
};