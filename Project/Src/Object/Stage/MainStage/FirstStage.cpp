#include <fstream>
#include <sstream>
#include <string>

#include "FirstStage.h"

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

	//// オブジェクト配置
	//for (int y = 0; y < MAP_SIZE_Y; ++y) {
	//	for (int x = 0; x < MAP_SIZE_X; ++x) {

	//		// オブジェクトの種類
	//		int tileType = mapData[y][x];

	//		// 何もない場所(-1)
	//		if (tileType == -1) continue;

	//		// マップ上の座標を計算
	//		tilePos = Vector3(
	//			static_cast<float>(x) * TILE_SIZE,
	//			-static_cast<float>(y) * TILE_SIZE,
	//			0.0f
	//		);

	//		// CSV数値に応じてオブジェクトセットアップ
	//		switch (tileType) {
	//			case 109:	// 通常の道ステージ(109)
	//			{
					// コライダーのサイズ指定
					Vector3 colliderSize(TILE_SIZE, TILE_SIZE, TILE_SIZE);

					// ★ 対策1: コライダーを中心に配置するために半サイズ分ずらす
					Vector3 colliderPos = tilePos + Vector3(TILE_SIZE * 0.5f, -TILE_SIZE * 0.5f, 0.0f);

					// コライダー設置
					ColliderCreate(new BoxCollider(COLLIDER_TAG::Stage, colliderSize, colliderPos));
					//break;
				//}
		//	}
		//}
	//}
}

void FirstStage::SubInit(void)
{
	trans.LoadModel("Stage/StageMapChip/Grass");

}

void FirstStage::SubDraw(void)
{
	//// 対応する箇所を描画
	//for (size_t y = 0; y < MAP_SIZE_Y; ++y) {
	//	for (size_t x = 0; x < MAP_SIZE_X; ++x) {
	//		int tileType = mapData[y][x];

	//		// 何もない場所はスキップ
	//		if (tileType == -1) continue;
	//		
	//		// CSV数値に応じてオブジェクト配置
	//		switch (tileType) {
	//		case 109:	// 通常の道ステージ(109)
	//		{
	//			// マップ上の座標を計算
	//			tilePos = Vector3(
	//				static_cast<float>(x) * TILE_SIZE,
	//				-static_cast<float>(y) * TILE_SIZE,
	//				0.0f
	//			);

				// 座標更新
				trans.pos = tilePos;

				// 描画
				trans.Draw();

				// デバッグ処理
				DrawBox(trans.pos.x, trans.pos.y, trans.pos.x + TILE_SIZE, trans.pos.y + TILE_SIZE, GetColor(255, 0, 0), FALSE);
				//break;
	//		}

	//		}
	//	}
	//}
}

void FirstStage::SubRelease(void)
{
}
