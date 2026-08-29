#include "CharacterBase.h"

#include <stdexcept>

#include "../../../Application/Application.h"

#include "CharacterStateBase.h"

#include "../AnimationController/AnimationController.h"

CharacterBase::CharacterBase() :
	ActorBase(),

	state(-1),
	stateMap(),

	DEFAULT_COLOR(),

	anime(nullptr),

	inviCounter(0),
	isInviEffect(false),

	isDeath(false)
{
}

CharacterBase::CharacterBase(const std::string& parameterPath):
	ActorBase(parameterPath),

	state(-1),
	stateMap(),

	DEFAULT_COLOR(),

	anime(nullptr),

	inviCounter(0),
	isInviEffect(false),

	isDeath(false)
{
}

void CharacterBase::SubInit(void)
{
	// モデルのカラーの初期化
 	SetInviEffectFlg();

	// キャラクター固有の初期化
	CharacterInit();
}

void CharacterBase::SubUpdate(void)
{
	// 無敵カウンターの更新
	Invi();

	// キャラクター固有の更新
	CharacterUpdate();

	// ステート更新
	if (stateMap.contains(state)) {
		stateMap.at(state)->OtherStateConditionsUpdate();
		stateMap.at(state)->Update();
	}
	for (std::pair<const int, CharacterStateBase*>& statePair : stateMap) {
		statePair.second->AlwaysUpdate();
	}

	// アニメーション更新
	if (anime) { anime->Update(); }
}

void CharacterBase::SubDraw(void)
{
	// キャラクター固有の描画
	CharacterDraw();
}

void CharacterBase::SubAlphaDraw(void)
{
	// キャラクター固有の描画
	CharacterAlphaDraw();
}

void CharacterBase::SubRelease(void)
{
	// キャラクター固有の解放
	CharacterRelease();

	// ステート管理用マップの解放
	for (auto& s : stateMap) {
		if (s.second) { delete s.second; s.second = nullptr; }
	}
	stateMap.clear();

	// デフォルトカラー情報の解放
	if (!DEFAULT_COLOR.empty()) { DEFAULT_COLOR.clear(); }

	// アニメーションコントローラーの解放（使われていたら）
	if (anime) {
		anime->Release();
		delete anime;
		anime = nullptr;
	}
}

void CharacterBase::SetInviCounter(unsigned char counter)
{
	inviCounter = counter;

	// 0 以下の代入であればついでに（バグ防止のために）
	if (counter <= 0) {
		for (int i = 0; i < DEFAULT_COLOR.size(); i++) {
			MV1SetMaterialEmiColor(trans.model, i, DEFAULT_COLOR[i]);
		}
	}
}

void CharacterBase::Invi(void)
{
	if (inviCounter > 0) { inviCounter--; }
	else { inviCounter = 0; }

	// ダメージ演出
	if (!isInviEffect) { return; }

	if (inviCounter > 1) {
		if (inviCounter / 10 % 2 == 0) {
			for (int i = 0; i < DEFAULT_COLOR.size(); i++) {
				MV1SetMaterialEmiColor(trans.model, i, DEFAULT_COLOR[i]);
			}
		}
		else {
			for (int i = 0; i < DEFAULT_COLOR.size(); i++) {
				COLOR_F emi = DEFAULT_COLOR[i];
				emi.r = (std::min)(DEFAULT_COLOR[i].r + 0.6f, 1.0f);
				MV1SetMaterialEmiColor(trans.model, i, emi);
			}
		}

	}
	else if (inviCounter == 1) {
		for (int i = 0; i < DEFAULT_COLOR.size(); i++) {
			MV1SetMaterialEmiColor(trans.model, i, DEFAULT_COLOR[i]);
		}
	}
}

void CharacterBase::AddState(int stateNum, CharacterStateBase* stateIns)
{
	// 重複追加を防ぐ（安全処理）
	if (stateMap.contains(stateNum)) {
		delete stateIns;
		throw std::runtime_error("同じステート番号が既に登録されています");
	}

	// 自分の状態に遷移させる関数のポインタをセットする（共通初期設定）
	stateIns->SetOwnChangeStatePtr([this, stateNum]() { ChangeState(stateNum); });

	// 現在自分の状態かどうかを返す関数のポインタをセットする（共通初期設定）
	stateIns->SetIsOwnStatePtr([this, stateNum]() { return state == stateNum; });

	// 格納
	stateMap.emplace(stateNum, stateIns);
}

CharacterStateBase& CharacterBase::GetStateIns(int state)
{
	auto it = stateMap.find(state);
	if (it != stateMap.end()) { return *(it->second); }
	else { throw std::runtime_error("指定のステートインスタンスが見つかりません"); }
}

void CharacterBase::ChangeState(int nextState)
{
	// 遷移前のステートの終了処理を呼び出す
	if (stateMap.contains(state)) { stateMap.at(state)->Exit(); }

	// 遷移
	state = nextState;

	// 遷移後のステートの初期化処理を呼び出す
	if (stateMap.contains(state)) { stateMap.at(state)->Enter(); }
}

#pragma region アニメーションコントローラー

void CharacterBase::CreateAnimationController(void) { if (anime == nullptr) anime = new AnimationController(trans.model); }

void CharacterBase::AddInFbxAnimation(int inFbxMaxIndex, float speed)
{
	for (int index = 0; index < inFbxMaxIndex; index++) {
		anime->AddInFbx(index, speed, index);
	}
}

void CharacterBase::AddInFbxAnimation(int inFbxMaxIndex, const float* speed)
{
	for (int index = 0; index < inFbxMaxIndex; index++) {
		anime->AddInFbx(index, speed[index], index);
	}
}

void CharacterBase::AddAnimation(int index, float speed, const char* filePath) { anime->Add(index, speed, filePath); }

void CharacterBase::AnimePlay(int type, bool loop) { anime->Play(type, loop); }

bool CharacterBase::IsAnimeEnd(void) const { return anime->IsAnimEnd(); }

float CharacterBase::GetAnimeRatio(void) const { return anime->GetAnimeRatio(); }

float CharacterBase::GetAnimeTotalTime(void) const { return anime->GetAnimeTotalTime(); }

int CharacterBase::GetAnimePlayType(void)const { return anime->GetAnimePlayType(); }

float CharacterBase::GetAnimeStep(void)const { return anime->GetAnimeStep(); }

void CharacterBase::SetAnimeStep(float step) { anime->SetAnimeStep(step); }

#pragma endregion