#pragma once
#include "Vector3.h"

struct Ray 
{
    Vector3 Origin;
    Vector3 Direction;

    Ray();

    Ray(const Vector3& InOrigin, const Vector3& InDirection);
};