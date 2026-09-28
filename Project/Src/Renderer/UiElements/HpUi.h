#pragma once
#include "../Base/UIBase.h"

class HpUi : public UiBase
{
public:
    HpUi(float* hp, Vector2 pos);
    ~HpUi();

    void Update() override;
    void Draw() override;

private:

    // スタミナ計算
    float* hp;

    // Hp値保持変数
    float oldHp;

};