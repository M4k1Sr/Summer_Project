#pragma once

#include <map>

#include "../../Common/ActorBase/ActorBase.h"

class StageBlockBase;

class FirstStage
	: public ActorBase
{
public:

	// 1タイルのサイズ
	static constexpr float TILE_SIZE = 150.0f;

	// チップサイズ
	static constexpr float CHIP_SIZE = 64.0f;

	// マップの大きさX,Y
	static constexpr int MAP_SIZE_X = 30;
	static constexpr int MAP_SIZE_Y = 17;

	FirstStage() {}

	~FirstStage()override = default;

	void Load(void)override;

	// コライダーすべてを取得
	std::vector<ColliderBase*> GetCollider(void)const;

private:

	// マップデータ
	using MapData = std::vector<std::vector<int>>;
	MapData mapData;
	
	// 初期化
	void SubInit(void)override;
	// 描画
	void SubDraw(void)override;
	// 解放
	void SubRelease(void)override;

	// ブロック配列
	std::map<int, std::map<int, StageBlockBase*>> stageBlocks;
};

