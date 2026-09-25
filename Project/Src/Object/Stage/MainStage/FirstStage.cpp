#include <fstream>
#include <sstream>
#include <string>

#include "FirstStage.h"

#include "../StageBlock/GrassBlock.h"
#include "../StageBlock/DirtBlock.h"
#include "../StageBlock/WallBlock.h"

#include "../StageBlock/StageBlockBase.h"

#include "../../../Manager/Camera/CurrentCamera.h"

FirstStage::FirstStage()
	: 
	stageBlockType(STAGE_BLOCK_TYPE::NONE)
{
}

void FirstStage::Load()
{
	// CSV読み取り
	std::ifstream file("Data/Model/Stage/StageCSV/FirstStage.csv");
	if (!file.is_open()) {
		throw std::runtime_error("CSVファイルを開けませんでした");
		return;
	}

	std::string line;
	mapData.clear();

	// 1行ずつ
	while (std::getline(file, line)) {
		std::vector<int> row;
		std::stringstream ss(line);
		std::string value;

		// カンマ区切りで値を取得
		while (std::getline(ss, value, ',')) {
			row.push_back(std::stoi(value));	// 文字列をintに変換
		}
		mapData.push_back(row);	// 1行を追加
	}

	// オブジェクト配置
	for (int y = 0; y < MAP_SIZE_Y; ++y) {
		for (int x = 0; x < MAP_SIZE_X; ++x) {

			// オブジェクトの種類
			int tileType = mapData[y][x];

			// 何もない場所(-1)
			if (tileType == -1) continue;
			
			// マップ上の座標を計算
			Vector3 tilePos = Vector3(
				static_cast<float>(x) * TILE_SIZE_XZ,
				-static_cast<float>(y) * TILE_SIZE_Y,
				0.0f
			);

			// CSV数値に応じてオブジェクトセットアップ
			switch (tileType) {
			case 11:	// 通常の道ステージ(11)
				{
					stageBlocks[y][x] = new GrassBlock(tilePos,false,false,true);
					break;
				}
				case 0:		// 土ステージ(0)
				{
					stageBlocks[y][x] = new DirtBlock(tilePos,false,false,true);
					break;
				}
			}
		}
	}

	// 生成したブロック全ての読み込み処理
	for (auto& row : stageBlocks) {
		for (auto& block : row.second) {
			block.second->Load();
		}
	}
}

void FirstStage::SubInit(void)
{
	// 生成したブロック全ての初期化処理
	for (auto& row : stageBlocks) {
		for (auto& block : row.second) {
			block.second->Init();
		}
	}
}

void FirstStage::SubDraw(void)
{
	// カメラ座標を取得
	const Vector3& camPos = CurrentCamera::Get().GetPos();

	// カメラ座標によりタイルの配列番号の計算
	int camTileX = static_cast<int>(camPos.x / TILE_SIZE_XZ);
	int camTileY = static_cast<int>(-camPos.y / TILE_SIZE_Y);

	// 描画範囲の最大値・最小値の計算
	int minX = (std::max)(0, camTileX - VIEW_RANGE_X);
	int maxX = (std::min)(MAP_SIZE_X - 1, camTileX + VIEW_RANGE_X);
	int minY = (std::max)(0, camTileY - VIEW_RANGE_Y);
	int maxY = (std::min)(MAP_SIZE_Y - 1, camTileY + VIEW_RANGE_Y);

	// 描画範囲内のブロックを描画
	for (int y = minY; y <= maxY; ++y) {
		auto rowIt = stageBlocks.find(y);
		if (rowIt == stageBlocks.end()) { continue; }

		for (int x = minX; x <= maxX; ++x) {
			auto blockIt = rowIt->second.find(x);
			if (blockIt == rowIt->second.end()) { continue; }

			blockIt->second->Draw();
		}
	}
}

void FirstStage::SubRelease(void)
{
	// 生成したブロック全ての解放処理
	for(auto& row : stageBlocks) {
		for(auto& block : row.second) {
			block.second->Release();
			delete block.second;
			block.second = nullptr;
		}
		row.second.clear();
	}
	stageBlocks.clear();
}

std::vector<ColliderBase*> FirstStage::GetColliders(void)const
{
	std::vector<ColliderBase*> ret = {};

	// 生成したブロック全てのコライダーを取得する仕様に
	for(auto& row : stageBlocks) {
		for(auto& block : row.second) {
			for (auto& collider : block.second->GetColliders()) { ret.push_back(collider); }
		}
	}

	return ret;
}
