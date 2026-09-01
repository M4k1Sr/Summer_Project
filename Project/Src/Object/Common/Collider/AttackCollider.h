#pragma once
#include "ColliderBase.h"

struct AttackData
{
    int count;  //攻撃判定の個数
	float duration; //攻撃判定の持続時間
    int damage; //ダメージ量(基本は１固定)
};

class AttackCollider :
    public ColliderBase
{

public:


    AttackCollider(
        COLLIDER_TAG type,
        const Vector3& size,
        int count,
        float duration,
        int damage,
        const Vector3& pos,
        const Vector3& angle)
        : ColliderBase(type, pos, angle),
        attackData{ count, duration, damage }
    {
        SetShape(COLLIDER_SHAPE::AttackSphere);
    }
    ~AttackCollider()override {}

    float GetRadius(void)const { return radius; }
    void SetRadius(float radius) { this->radius = radius; }

	//攻撃データを取得
	AttackData GetAttackData(void)const { return attackData; }

    AABB GetAABB(void) const override {
        Vector3 p = GetPos();
        Vector3 r = radius;

        return AABB(p - r, p + r);
    }

    void DrawDebug(unsigned int color = 0xffffff)override {
        DrawSphere3D(GetPos().ToVECTOR(), radius, 12, color, color, true);
    }

private:

    float radius;

	AttackData attackData;
    
};

