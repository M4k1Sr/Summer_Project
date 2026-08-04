#pragma once

#include <map>

#include"Common/Collider/ColliderBase.h"
#include"Common/DataLoad/ParameterLoad.h"

#include "../Scene/Common/GameSpace/SpaceDefine.h"

class GameSpaceController;

class ActorBase
{
public:
	// デフォルトコンストラクタ
	ActorBase();
	// パラメーターを外部から読み込む場合に使うコンストラクタ
	ActorBase(const std::string& parameterPath);
	virtual ~ActorBase() = default;

	virtual void Load(void) = 0;
	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void AlphaDraw(void);
	virtual void UiDraw(void) {}
	virtual void Release(void);

	// モデルを複製する
	void ModelDuplicate(int model) { trans.model = MV1DuplicateModel(model); }

	// モデル制御情報構造体のゲット関数
	const Transform& GetTrans(void)const { return trans; }

#pragma region ゲーム空間制御

	// WorldSceneBaseが所有するゲーム空間管理クラスを登録する
	void SetGameSpaceController(const GameSpaceController* controller) {
		gameSpace = controller;
		for (ColliderBase*& coll : collider) {
			if (coll == nullptr) { continue; }
			coll->SetGameSpaceControllerPtr(gameSpace);
		}
	}

	// ゲーム空間管理クラスを取得する
	const GameSpaceController& GetGameSpaceController(void)const { return *gameSpace; }

	// オブジェクトごとの空間制約を設定する
	void SetSpaceConstraint(SPACE_CONSTRAINT constraint) { spaceConstraint = constraint; }

	// オブジェクトごとの空間制約を取得する
	SPACE_CONSTRAINT GetSpaceConstraint(void)const { return spaceConstraint; }

	// 座標を設定する
	void SetPos(const Vector3& pos) { trans.pos = pos; }

	// 指定量だけ座標を移動する
	void AddMove(const Vector3& move) { trans.pos += move; }

	// 現在のゲーム空間に合わせて方向を補正する
	Vector3 RestrictSpaceDirection(const Vector3& direction)const;

	// 現在のゲーム空間に合わせて加速度を補正する
	void RestrictAccel(const GameSpaceController& controller);

#pragma endregion

	// 当たり判定の通知
	virtual void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) {}

	// 接地判定の通知
	virtual void OnGrounded() {
		if (dynamicFlg) {
			accelSum.y = (accelSum.y < 0.0f) ? 0.0f : accelSum.y;
		}
		isGroundMaster = true;
	}

	/// <summary>
	/// コライダーすべてを取得
	/// </summary>
	virtual std::vector<ColliderBase*> GetCollider(void)const { return collider; }

	/// <summary>
	/// 当たり判定フラグの取得
	/// </summary>
	/// <returns>どれか一つでも「判定する」状態ならtrue</returns>
	bool GetJudgeFlg(void) {
		for (ColliderBase*& c : collider) {
			if (!c) { continue; }
			if (c->GetJudge()) { return true; }
		}
		return false;
	}

	// 描画判定の取得
	bool GetIsDraw(void)const { return isDraw; }

private:
	// 当たり判定情報
	std::vector<ColliderBase*> collider;

	// 動的オブジェクトか否か（true = 動的、false = 静的）
	bool dynamicFlg;

	// 重力を適用するかどうか(true = する、false = しない)
	bool isGravity;

	// 衝突時押し出しを行うか否か（true = 押し出す、false = 押し出さず通り抜ける）
	bool pushFlg;

	// 押し出しを行う際の重さ（0 ～ 100 で設定）
	unsigned char pushWeight;

	// 加速度の更新
	void AccelUpdate(void);

	// 重力更新
	void Gravity(void);

	// 重力
	static constexpr float GRAVITY = -0.6f;
	static constexpr float GRAVITY_MAX = -30.0f;

	// 接地判定 管理用(派生先で変更不可で参照渡し)
	bool isGroundMaster;

	// 描画判定 （true = 「描画する」、false = 「描画しない」）
	bool isDraw;

	// アルファ判定（true = 「メインの描画にアルファをかける」、false = 「メインの描画を普通に描画する」）（メインの描画 = 基底クラスで自動で描画されるモデルなど）
	bool isAlphaDraw;

	// パラメーター外部ファイル管理クラス
	ParameterLoad* parameter;

	// WorldSceneBaseが所有するゲーム空間管理クラスへの参照
	// オブジェクト側は所有しないためdeleteしない
	const GameSpaceController* gameSpace;

	// このオブジェクトへ適用する空間制約
	SPACE_CONSTRAINT spaceConstraint;

protected:
	// モデル制御情報構造体
	Transform trans;

	// 加速度
	Vector3 accelSum;

	// 横軸加速度の１フレームごとの減衰量
	float ATTENUATION = 3.0f;
	// 横軸加速度の最大値
	float ACCEL_MAX = 30.0f;

	// 接地判定(派生先で参照用)
	const bool& isGround = isGroundMaster;

	/// <summary>
	/// 特定のコライダーを探す
	/// </summary>
	/// <typeparam name="T">探したいクラス</typeparam>
	/// <param name="tag">タグ種類</param>
	/// <returns></returns>
	template<typename T = ColliderBase>
	std::vector<T*> ColliderSerch(COLLIDER_TAG tag = COLLIDER_TAG::NON) {
		std::vector<T*> out;
		out.reserve(collider.size());

		for (auto c : collider) {
			if (!c) continue;
			if (auto* ptr = dynamic_cast<T*>(c)) {
				if (c->GetTag() == tag || tag == COLLIDER_TAG::NON) { out.push_back(ptr); }
			}
		}
		return out;
	}
#pragma region 初期設定
	// 当たり判定情報を生成
	void ColliderCreate(ColliderBase* newClass) {
		collider.emplace_back(newClass);
		collider.back()->SetTransformPtr(&trans);
		collider.back()->SetDynamicFlgPtr(&dynamicFlg);
		collider.back()->SetPushFlgPtr(&pushFlg);
		collider.back()->SetPushWeightPtr(&pushWeight);
		collider.back()->SetGameSpaceControllerPtr(gameSpace);
		collider.back()->SetSpaceConstraintPtr(&spaceConstraint);
		collider.back()->SetOnCollisionFunc([this](COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) { this->OnCollision(ownTag, other, result); });
		collider.back()->SetOnGroundedFunc([this](void) { this->OnGrounded(); });
	}

	/// <summary>
	/// 移動するかを切り替える
	/// </summary>
	/// <param name="flg">true = 「移動する」に切り替える、false = 「移動しない」に切り替える</param>
	void SetDynamicFlg(bool flg) {
		dynamicFlg = flg;
		trans.dynamicFlg = flg;
	}

	/// <summary>
	/// 重力を適用するかを切り替える
	/// </summary>
	/// <param name="flg">true = 「する」に切り替える、false = 「しない」に切り替える</param>
	void SetGravityFlg(bool flg) { isGravity = flg; }

	/// <summary>
	/// 衝突時押し出しを行うかを設定する
	/// </summary>
	/// <param name="flg">true = 押し出す、false = 押し出さず通り抜ける</param>
	void SetPushFlg(bool flg) { pushFlg = flg; }

	/// <summary>
	/// 押し出しを行う際の重さ
	/// </summary>
	/// <param name="weight">0 ～ 100 で設定（数値が大きいほど重い）</param>
	void SetPushWeight(unsigned char weight) { pushWeight = weight; }

#pragma region パラメーター外部ファイル管理に関する関数
	bool IsParameterExist(const std::string& fileName, const std::string& parameterName)const {
		if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
		return parameter->IsParameterExist(fileName, parameterName);
	}

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターの指定の配列番号の値だけを取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <param name="index">配列番号（指定なしで0）</param>
	/// <returns></returns>
	float GetParameter(const std::string& fileName, const std::string& parameterName, int index = 0)const {
		if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
		return parameter->GetParameter(fileName, parameterName, index);
	}

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターを配列ごと取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <returns></returns>
	const std::vector<float>& GetParameterArray(const std::string& fileName, const std::string& parameterName)const {
		if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
		return parameter->GetParameterArray(fileName, parameterName);
	}

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターの指定の配列番号の値だけをint型にキャストして取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <param name="index">配列番号（指定なしで0）</param>
	/// <returns></returns>
	int GetParameterToInt(const std::string& fileName, const std::string& parameterName, int index = 0)const {
		if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
		return parameter->GetParameterToInt(fileName, parameterName, index);
	}

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターをVector3構造体にして取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <returns></returns>
	Vector3 GetParameterToVector3(const std::string& fileName, const std::string& parameterName) {
		if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
		return parameter->GetParameterToVector3(fileName, parameterName);
	}
#pragma endregion

#pragma endregion

	// 当たり判定の設定（true = 「判定する」、false = 「判定しない」）
	void SetJudge(bool flg) {
		for (ColliderBase*& c : collider) {
			if (!c) { continue; }
			c->SetJudgeFlg(flg);
		}
	}

	bool GetDynamicFlg(void)const { return dynamicFlg; }
	bool GetGravityFlg(void)const { return isGravity; }

	// 描画判定の設定（true = 「描画する」、false = 「描画しない」）
	void SetIsDraw(bool flg) { isDraw = flg; }
	// 描画判定の設定（引数省略で現在の逆にスイッチ）
	void SetIsDraw(void) { isDraw = !isDraw; }

	// アルファ判定（true = 「メインの描画にアルファをかける」、false = 「メインの描画を普通に描画する」）（メインの描画 = 基底クラスで自動で描画されるモデルなど）
	bool GetIsAlphaDraw(void)const { return isAlphaDraw; }

	// アルファ判定の設定（true = 「メインの描画にアルファをかける」、false = 「メインの描画を普通に描画する」）（メインの描画 = 基底クラスで自動で描画されるモデルなど）
	void SetIsAlphaDraw(bool flg) { isAlphaDraw = flg; }
	// アルファ判定の設定（引数省略で現在の逆にスイッチ）
	void SetIsAlphaDraw(void) { isAlphaDraw = !isAlphaDraw; }

	// 派生先追加初期化
	virtual void SubInit(void) {}
	// 派生先追加更新
	virtual void SubUpdate(void) {}
	// 派生先追加描画
	virtual void SubDraw(void) {}
	// 派生先追加アルファ描画
	virtual void SubAlphaDraw(void) {}
	// 派生先追加解放
	virtual void SubRelease(void) {}
};