#pragma once
#include "ColliderBase.h"

struct AttackData
{
    float range;
    float duration;
    int damage;
};

class AttackCollider :
    public ColliderBase
{

public:

    AttackCollider(COLLIDER_TAG type, const Vector3& size, const Vector3& pos = Vector3(), const Vector3& angle = Vector3()) :
        ColliderBase(type, pos, angle)
    {
        SetShape(COLLIDER_SHAPE::AttackSphere);
    }
    ~AttackCollider()override {}

    float GetRadius(void)const { return radius; }
    void SetRadius(float radius) { this->radius = radius; }

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

