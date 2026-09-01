#pragma once
#include "../Common/ActorBase/ActorBase.h"

class ColliderOperator :public ActorBase
{
public:

	ColliderOperator();
	~ColliderOperator()override = default;

	void Load(void) override;
	void Update(void)override;

private:



};


