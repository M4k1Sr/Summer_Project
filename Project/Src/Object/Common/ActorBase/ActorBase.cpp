#include "ActorBase.h"

#include <stdexcept>

#include "../../../Common/Vector2.h"
#include "../../../Common/Vector2I.h"

#include "../../../Application/Application.h"

#include "../../../Manager/TimeScale/TimeScale.h"

#include "../Collider/ColliderBase.h"

#include "../DataLoad/ParameterLoad.h"
#include "../../../Scene/Common/GameSpace/GameSpaceController.h"


ActorBase::ActorBase() :
	trans(),
	colliders(),

	dynamicFlg(true),
	gravityFlg(false),

	pushFlg(true),
	pushWeight(0),

	velocity(),

	isGroundMaster(false),

	isDraw(true),
	drawType(ACTOR_DRAW_TYPE::Normal),

	parameter(nullptr),

	gameSpace(nullptr),
	spaceConstraint(SPACE_CONSTRAINT::StageDefault),

	childActors()
{
}

ActorBase::ActorBase(const std::string& parameterPath) :
	trans(),
	colliders(),

	dynamicFlg(true),
	gravityFlg(false),

	pushFlg(true),
	pushWeight(0),

	velocity(),

	isGroundMaster(false),

	isDraw(true),
	drawType(ACTOR_DRAW_TYPE::Normal),

	parameter(new ParameterLoad(parameterPath)),

	gameSpace(nullptr),
	spaceConstraint(SPACE_CONSTRAINT::StageDefault),

	childActors()
{
}

void ActorBase::Init(void)
{
	// 最終派生固有
	SubInit();

	// 中間基底固有
	BaseInit();

	// 子アクターの初期化
	for (ActorBase* child : childActors) { child->Init(); }

	trans.Attach();
	
	// パラメータの解放
	if (parameter != nullptr) {
		parameter->Release();
		delete parameter;
		parameter = nullptr;
	}
}

void ActorBase::Update(void)
{
	// 1フレーム前の加速度を保持
	Vector3 prevVelocity = velocity;
	// 動的オブジェクトの場合1フレーム前の座標を保持
	if (dynamicFlg) { trans.prevPos = trans.pos; }

	// 最終派生固有
	SubUpdate();

	// 中間基底固有
	BaseUpdate();

	if (dynamicFlg) {
		// 加速度更新
		VelocityUpdate(velocity == prevVelocity);

		// 接地判定のリセット
		isGroundMaster = false;
	}

	// 子アクターの更新
	for (ActorBase* child : childActors) { child->Update(); }
}

void ActorBase::Draw(void)
{
	// 描画判定
	if (!isDraw) { return; }

	// 最終派生固有
	SubDraw();

	// 中間基底固有
	BaseDraw();

	// モデルの描画
	trans.Draw();
}

void ActorBase::Release(void)
{
	// 最終派生固有
	SubRelease();

	// 中間基底固有
	BaseRelease();

	// 子アクターの解放
	for (ActorBase*& child : childActors) {
		if (!child) { continue; }
		child->Release();
		delete child;
		child = nullptr;
	}

	// パラメータの解放
	if (parameter != nullptr) {
		parameter->Release();
		delete parameter;
		parameter = nullptr;
	}

	// 当たり判定情報を解放
	for (ColliderBase*& c : colliders) {
		if (!c) { continue; }
		delete c;
		c = nullptr;
	}
	colliders.clear();

	// モデル制御情報の解放
	trans.Release();
}

void ActorBase::DrawColliderDebug(void) const
{
	for (ColliderBase* collider : colliders) {
		if (collider->GetJudgeFlg()) collider->DrawDebug();
	}
}

bool ActorBase::GetJudgeFlg(void)
{
	for (ColliderBase*& c : colliders) {
		if (!c) { continue; }
		if (c->GetJudgeFlg()) { return true; }
	}
	return false;
}

void ActorBase::Gravity(void)
{
	if (!gravityFlg) { return; }

	velocity.y += GRAVITY;
	if (velocity.y < GRAVITY_MAX) { velocity.y = GRAVITY_MAX; }
}

void ActorBase::VelocityUpdate(bool	deceleration)
{
	// 重力処理
	Gravity();

#pragma region 横軸（X,Z）
	// 横軸のみの加速度の大きさを求める
	float widthAccelLen = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

	// 横軸の加速度がある場合
	if (widthAccelLen > 0.0f) {

		// 補正倍率
		float scale = 1.0f;

		// 減速処理
		if (deceleration) {
			// 横軸の加速度が減速の半分以下の場合は、横軸の加速度を0にする
			if (widthAccelLen <= DECEL_RATE * 0.5f) { velocity.x = velocity.z = 0.0f; }

			// 減速
			scale = (widthAccelLen - DECEL_RATE) / widthAccelLen;
		}

		// 最大加速制限～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

		// 最大加速を超えていれば、超過分を打ち消す補正倍率を組み込む
		if (widthAccelLen > ACCEL_MAX) { scale = ACCEL_MAX / widthAccelLen; }

		// 実際に補正
		velocity.x *= scale;
		velocity.z *= scale;
		// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～最大加速制限
	}
#pragma endregion

	// 加速度を座標に反映
	if (velocity != 0.0f) { trans.pos += velocity; }
}

void ActorBase::ColliderCreate(ColliderBase* newClass)
{
	colliders.emplace_back(newClass);
	colliders.back()->SetTransformPtr(&trans);
	colliders.back()->SetDynamicFlgPtr(&dynamicFlg);
	colliders.back()->SetPushFlgPtr(&pushFlg);
	colliders.back()->SetPushWeightPtr(&pushWeight);
	colliders.back()->SetGameSpaceControllerPtr(gameSpace);
	colliders.back()->SetSpaceConstraintPtr(&spaceConstraint);
	colliders.back()->SetOnCollisionFunc([this](COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) { this->OnCollision(ownTag, other, result); });
	colliders.back()->SetOnGroundedFunc([this](void) { this->OnGrounded(); });
}

void ActorBase::SetDynamicFlg(bool flg)
{
	dynamicFlg = flg;
	trans.dynamicFlg = flg;
}

void ActorBase::SetPushFlg(bool flg)
{
	pushFlg = flg;
}

void ActorBase::SetPushWeight(unsigned char weight)
{
	pushWeight = weight;
}
#pragma region パラメーター外部ファイル管理に関する関数

bool ActorBase::IsParameterExist(const std::string& fileName, const std::string& parameterName)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
	return parameter->IsParameterExist(fileName, parameterName);
}

float ActorBase::GetParameter(const std::string& fileName, const std::string& parameterName, int index)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
	return parameter->GetParameter(fileName, parameterName, index);
}

const std::vector<float>& ActorBase::GetParameterArray(const std::string& fileName, const std::string& parameterName)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
	return parameter->GetParameterArray(fileName, parameterName);
}

int ActorBase::GetParameterToInt(const std::string& fileName, const std::string& parameterName, int index)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
	return parameter->GetParameterToInt(fileName, parameterName, index);
}

Vector3 ActorBase::GetParameterToVector3(const std::string& fileName, const std::string& parameterName)
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
	return parameter->GetParameterToVector3(fileName, parameterName);
}

Vector2 ActorBase::GetParameterToVector2(const std::string& fileName, const std::string& parameterName)
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
	return parameter->GetParameterToVector2(fileName, parameterName);
}

Vector2I ActorBase::GetParameterToVector2I(const std::string& fileName, const std::string& parameterName)
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadクラスが生成されていません"); }
	return parameter->GetParameterToVector2I(fileName, parameterName);
}

#pragma endregion

void ActorBase::SetJudge(bool flg)
{
	for (ColliderBase*& c : colliders) {
		if (!c) { continue; }
		c->SetJudgeFlg(flg);
	}
}

void ActorBase::SetGameSpaceController(const GameSpaceController* controller)
{
	gameSpace = controller;
	for (ColliderBase* coll : colliders) {
		if (coll == nullptr) { continue; }
		coll->SetGameSpaceControllerPtr(gameSpace);
		coll->SetSpaceConstraintPtr(&spaceConstraint);
	}
}

Vector3 ActorBase::RestrictSpaceDirection(const Vector3& direction)const
{
	if (gameSpace == nullptr) { return direction; }
	return gameSpace->RestrictDirection(direction, trans.pos, spaceConstraint);
}

void ActorBase::RestrictVelocity(void)
{
	if (gameSpace == nullptr) { return; }

	velocity = gameSpace->RestrictDirection(velocity, trans.pos, spaceConstraint);
}

void ActorBase::MoveAccel(const Vector3& vec)
{
	if (vec == 0.0f) { return; }

	// 加速
	velocity += (vec * ACCEL_RATE) * TimeScale::Get();

	// 目標角度
	float targetAngle = atan2f(vec.x, vec.z);

	// 現在角度から目標角度までの角度差
	float diffAngle = targetAngle - trans.angle.y;

	// -π ～ +π に正規化して最短方向を求める
	while (diffAngle > DX_PI_F) { diffAngle -= DX_TWO_PI_F; }
	while (diffAngle < -DX_PI_F) { diffAngle += DX_TWO_PI_F; }

	// 最短方向に補間
	trans.angle.y += (diffAngle * 0.25f) * TimeScale::Get();
}
