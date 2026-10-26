#pragma once
#include "../../../Common/ActorBase/ActorBase.h"
class HP :
    public ActorBase
{
public:

    HP(void);
    ~HP(void)override = default;

    void Load(void)override;
    void SubDraw(void)override;

private:

    //‰æ‘œƒnƒ“ƒhƒ‹
    int image;

};

