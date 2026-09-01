#pragma once
#include "../../Common/ActorBase/ActorBase.h"

class FirstStage
	: public ActorBase
{
public:

	FirstStage();
	FirstStage(const std::string& parameterPath);

	~FirstStage()override = default;

private:

	// ‰Šú‰»
	void SubInit(void)override;
	// XV
	void SubUpdate(void)override;
	// •`‰æ
	void SubDraw(void)override;
	// ”¼“§–¾•`‰æ
	void SubAlphaDraw(void)override;
	// ‰ğ•ú
	void SubRelease(void)override;

protected:

	
};

