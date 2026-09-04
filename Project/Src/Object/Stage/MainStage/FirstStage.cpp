#include <fstream>
#include <sstream>
#include <string>

#include "FirstStage.h"

#include "../StageBlock/GrassStageBlock.h"

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
				static_cast<float>(x) * TILE_SIZE,
				-static_cast<float>(y) * TILE_SIZE,
				0.0f
			);

			// CSV数値に応じてオブジェクトセットアップ
			switch (tileType) {
				case 109:	// 通常の道ステージ(109)
				{
					stageBlocks[y][x] = new GrassStageBlock(tilePos);
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
	// 現在は総呼び出ししているが、
	// ここで配列番号を使って描画範囲制限を行う
	// ブロック1つ1つをカメラに映っているか判定するのではなく、
	// そもそも映っているであろう範囲の配列番号のブロックの処理しか呼び出さないように

	// 生成したブロック全ての描画処理
	for (auto& row : stageBlocks) {
		for (auto& block : row.second) {
			block.second->Draw();
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

std::vector<ColliderBase*> FirstStage::GetCollider(void)const
{
	std::vector<ColliderBase*> ret = {};

	// 生成したブロック全てのコライダーを取得する仕様に
	for(auto& row : stageBlocks) {
		for(auto& block : row.second) {
			for (auto& collider : block.second->GetCollider()) { ret.push_back(collider); }
		}
	}

	return ret;
}
