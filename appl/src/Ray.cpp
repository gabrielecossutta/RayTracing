#include "Ray.h"

Ray::Ray()
{ }

Ray::Ray(const Vector3& InOrigin, const Vector3& InDirection)
    : Origin(InOrigin), Direction(InDirection)
{ }