#pragma once
#include "Color.h"
#include "Ray.h"
#include "Scene.h"

class Raytracer 
{
public:
    XColor RayTrace(const Ray& InRay, Scene& InScene, int InCurrentDepth);
};