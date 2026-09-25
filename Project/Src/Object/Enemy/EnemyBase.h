#pragma once
#include "../Common/CharacterBase/CharacterBase.h"
class EnemyBase :
	public CharacterBase
{
public:

	// 種別
	enum class TYPE
	{
		RAT,
		RASE,
		LARGE
	};

	// エネミーデータ
	struct EnemyData
	{
		int id;
		//タイプ
		EnemyBase::TYPE type;
		int hp;
	};

	EnemyBase();
	~EnemyBase();


private:
};

