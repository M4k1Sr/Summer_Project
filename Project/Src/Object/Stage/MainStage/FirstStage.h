#pragma once

#include <map>

#include "../../Common/ActorBase/ActorBase.h"

class StageBlockBase;

class FirstStage
	: public ActorBase
{
public:

	enum class STAGE_BLOCK_TYPE
	{
		NONE = -1,	// なし
		DIRT = 0,	// 土ブロック
		GRASS = 11,	// 草ブロック
	};

	// 1タイルのサイズ
	static constexpr float TILE_SIZE_XZ = 200.0f;
	static constexpr float TILE_SIZE_Y = 150.0f;

	// チップサイズ
	static constexpr float CHIP_SIZE = 64.0f;

	// マップの大きさX,Y
	static constexpr int MAP_SIZE_X = 30;
	static constexpr int MAP_SIZE_Y = 17;

	// 描画するタイルの範囲X,Y
	static constexpr int VIEW_RANGE_X = 10;
	static constexpr int VIEW_RANGE_Y = 6;

	// コンストラクタ
	FirstStage();

	// デストラクタ
	~FirstStage()override = default;

	// 読み込み処理
	void Load(void)override;

	// コライダーすべてを取得
	std::vector<ColliderBase*> GetColliders(void)const;

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

	// ステージブロックの種類
	STAGE_BLOCK_TYPE stageBlockType;

	// ブロック配列
	std::map<int, std::map<int, StageBlockBase*>> stageBlocks;

	// カメラ座標
	Vector3 camPos;
};

