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
	// ÅI”h¶ŒÅ—L
	SubInit();

	// ’†ŠÔŠî’êŒÅ—L
	BaseInit();

	// qƒAƒNƒ^[‚Ì‰Šú‰»
	for (ActorBase* child : childActors) { child->Init(); }

	trans.Attach();
	
	// ƒpƒ‰ƒ[ƒ^‚Ì‰ğ•ú
	if (parameter != nullptr) {
		parameter->Release();
		delete parameter;
		parameter = nullptr;
	}
}

void ActorBase::Update(void)
{
	// 1ƒtƒŒ[ƒ€‘O‚Ì‰Á‘¬“x‚ğ•Û
	Vector3 prevVelocity = velocity;
	// “®“IƒIƒuƒWƒFƒNƒg‚Ìê‡1ƒtƒŒ[ƒ€‘O‚ÌÀ•W‚ğ•Û
	if (dynamicFlg) { trans.prevPos = trans.pos; }

	// ÅI”h¶ŒÅ—L
	SubUpdate();

	// ’†ŠÔŠî’êŒÅ—L
	BaseUpdate();

	if (dynamicFlg) {
		// ‰Á‘¬“xXV
		VelocityUpdate(velocity == prevVelocity);

		// Ú’n”»’è‚ÌƒŠƒZƒbƒg
		isGroundMaster = false;
	}

	// qƒAƒNƒ^[‚ÌXV
	for (ActorBase* child : childActors) { child->Update(); }
}

void ActorBase::Draw(void)
{
	// •`‰æ”»’è
	if (!isDraw) { return; }

	// ÅI”h¶ŒÅ—L
	SubDraw();

	// ’†ŠÔŠî’êŒÅ—L
	BaseDraw();

	// ƒ‚ƒfƒ‹‚Ì•`‰æ
	trans.Draw();
}

void ActorBase::Release(void)
{
	// ÅI”h¶ŒÅ—L
	SubRelease();

	// ’†ŠÔŠî’êŒÅ—L
	BaseRelease();

	// qƒAƒNƒ^[‚Ì‰ğ•ú
	for (ActorBase*& child : childActors) {
		if (!child) { continue; }
		child->Release();
		delete child;
		child = nullptr;
	}

	// ƒpƒ‰ƒ[ƒ^‚Ì‰ğ•ú
	if (parameter != nullptr) {
		parameter->Release();
		delete parameter;
		parameter = nullptr;
	}

	// “–‚½‚è”»’èî•ñ‚ğ‰ğ•ú
	for (ColliderBase*& c : colliders) {
		if (!c) { continue; }
		delete c;
		c = nullptr;
	}
	colliders.clear();

	// ƒ‚ƒfƒ‹§Œäî•ñ‚Ì‰ğ•ú
	trans.Release();
}

std::vector<ColliderBase*> ActorBase::GetColliders(void)const
{
	std::vector<ColliderBase*> ret = {};

	for (ColliderBase* collider : colliders) { ret.emplace_back(collider); }

	for(ActorBase* child : childActors) {
		for (ColliderBase* collider : child->GetColliders()) { ret.emplace_back(collider); }
	}

	return ret;
}

void ActorBase::DrawColliderDebug(void) const
{
	for (ColliderBase* collider : GetColliders()) {
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
	// d—Íˆ—
	Gravity();

#pragma region ‰¡²iX,Zj
	// ‰¡²‚Ì‚İ‚Ì‰Á‘¬“x‚Ì‘å‚«‚³‚ğ‹‚ß‚é
	float widthAccelLen = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

	// ‰¡²‚Ì‰Á‘¬“x‚ª‚ ‚éê‡
	if (widthAccelLen > 0.0f) {

		// •â³”{—¦
		float scale = 1.0f;

		// Œ¸‘¬ˆ—
		if (deceleration) {
			// ‰¡²‚Ì‰Á‘¬“x‚ªŒ¸‘¬‚Ì”¼•ªˆÈ‰º‚Ìê‡‚ÍA‰¡²‚Ì‰Á‘¬“x‚ğ0‚É‚·‚é
			if (widthAccelLen <= DECEL_RATE * 0.5f) { velocity.x = velocity.z = 0.0f; }

			// Œ¸‘¬
			scale = (widthAccelLen - DECEL_RATE) / widthAccelLen;
		}

		// Å‘å‰Á‘¬§ŒÀ`````````````````````````````

		// Å‘å‰Á‘¬‚ğ’´‚¦‚Ä‚¢‚ê‚ÎA’´‰ß•ª‚ğ‘Å‚¿Á‚·•â³”{—¦‚ğ‘g‚İ‚Ş
		if (widthAccelLen > ACCEL_MAX) { scale = ACCEL_MAX / widthAccelLen; }

		// ÀÛ‚É•â³
		velocity.x *= scale;
		velocity.z *= scale;
		// `````````````````````````````Å‘å‰Á‘¬§ŒÀ
	}
#pragma endregion

	// ‰Á‘¬“x‚ğÀ•W‚É”½‰f
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
#pragma region ƒpƒ‰ƒ[ƒ^[ŠO•”ƒtƒ@ƒCƒ‹ŠÇ—‚ÉŠÖ‚·‚éŠÖ”

bool ActorBase::IsParameterExist(const std::string& fileName, const std::string& parameterName)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadƒNƒ‰ƒX‚ª¶¬‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"); }
	return parameter->IsParameterExist(fileName, parameterName);
}

float ActorBase::GetParameter(const std::string& fileName, const std::string& parameterName, int index)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadƒNƒ‰ƒX‚ª¶¬‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"); }
	return parameter->GetParameter(fileName, parameterName, index);
}

const std::vector<float>& ActorBase::GetParameterArray(const std::string& fileName, const std::string& parameterName)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadƒNƒ‰ƒX‚ª¶¬‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"); }
	return parameter->GetParameterArray(fileName, parameterName);
}

int ActorBase::GetParameterToInt(const std::string& fileName, const std::string& parameterName, int index)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadƒNƒ‰ƒX‚ª¶¬‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"); }
	return parameter->GetParameterToInt(fileName, parameterName, index);
}

Vector3 ActorBase::GetParameterToVector3(const std::string& fileName, const std::string& parameterName)
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadƒNƒ‰ƒX‚ª¶¬‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"); }
	return parameter->GetParameterToVector3(fileName, parameterName);
}

Vector2 ActorBase::GetParameterToVector2(const std::string& fileName, const std::string& parameterName)
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadƒNƒ‰ƒX‚ª¶¬‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"); }
	return parameter->GetParameterToVector2(fileName, parameterName);
}

Vector2I ActorBase::GetParameterToVector2I(const std::string& fileName, const std::string& parameterName)
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadƒNƒ‰ƒX‚ª¶¬‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"); }
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

	// ‰Á‘¬
	velocity += (vec * ACCEL_RATE) * TimeScale::Get();

	// –Ú•WŠp“x
	float targetAngle = atan2f(vec.x, vec.z);

	// Œ»İŠp“x‚©‚ç–Ú•WŠp“x‚Ü‚Å‚ÌŠp“x·
	float diffAngle = targetAngle - trans.angle.y;

	// -ƒÎ ` +ƒÎ ‚É³‹K‰»‚µ‚ÄÅ’Z•ûŒü‚ğ‹‚ß‚é
	while (diffAngle > DX_PI_F) { diffAngle -= DX_TWO_PI_F; }
	while (diffAngle < -DX_PI_F) { diffAngle += DX_TWO_PI_F; }

	// Å’Z•ûŒü‚É•âŠÔ
	trans.angle.y += (diffAngle * 0.5f) * TimeScale::Get();
}
