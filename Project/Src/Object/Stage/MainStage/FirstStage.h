#pragma once
#include "../../Common/ActorBase/ActorBase.h"

#include "../../Common/Collider/BoxCollider.h"

class FirstStage
	: public ActorBase
{
public:

	// 1タイルのサイズ
	static constexpr float TILE_SIZE = 100.0f;

	FirstStage(
		const Vector3& size,

		const Vector3& pos = Vector3(),

		bool dynamicFlg = true,
		bool isGravity = true,
		bool pushFlg = true,
		unsigned char pushWeight = 50,
		bool isOperator = false
	)
	{
	}
		
	~FirstStage()override = default;

	void Load(void)override {
		ColliderCreate(new BoxCollider(COLLIDER_TAG::Stage, size));
	}

private:

	VECTOR size;

	// 描画
	void SubDraw(void)override;
	// 解放
	void SubRelease(void)override;

	
};

