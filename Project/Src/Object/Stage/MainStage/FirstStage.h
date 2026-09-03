#pragma once
#include "../../Common/ActorBase/ActorBase.h"

#include "../../Common/Collider/BoxCollider.h"

class FirstStage
	: public ActorBase
{
public:

	// 1タイルのサイズ
	static constexpr float TILE_SIZE = 50.0f;

	// チップサイズ
	static constexpr float CHIP_SIZE = 64.0f;

	// マップの大きさX,Y
	static constexpr int MAP_SIZE_X = 30;
	static constexpr int MAP_SIZE_Y = 17;

	FirstStage(
		const Vector3& tilePos = Vector3(),

		bool dynamicFlg = true,
		bool isGravity = false,
		bool pushFlg = true,
		unsigned char pushWeight = 50,
		bool isOperator = false
	)
	{
		SetDynamicFlg(dynamicFlg);
		SetGravityFlg(isGravity);
		SetPushFlg(pushFlg);
		SetPushWeight(pushWeight);
	}
		
	~FirstStage()override = default;

	void Load(void)override;

private:

	// タイルの座標
	Vector3 tilePos;
	
	// マップデータ
	using MapData = std::vector<std::vector<int>>;
	MapData mapData;
	
	// 初期化
	void SubInit(void)override;
	// 描画
	void SubDraw(void)override;
	// 解放
	void SubRelease(void)override;

	
};

