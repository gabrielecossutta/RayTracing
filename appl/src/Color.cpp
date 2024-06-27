#include "Color.h"

XColor::XColor()
{
    R = 0;
    G = 0;
    B = 0;
}

XColor::XColor(float InR, float InG, float InB)
{
    R = InR;
    G = InG;
    B = InB;
}

XColor XColor::operator*(float InScalar) const
{
    return {R * InScalar, G * InScalar, B * InScalar};
}

XColor XColor::operator+(const XColor& InOther) const
{
    return {R + InOther.R, G + InOther.G, B + InOther.B};
}

XColor XColor::Clamp() const
{
    XColor Result;
    Result.R = R > 1.f ? 1.f : R;
    Result.G = G > 1.f ? 1.f : G;
    Result.B = B > 1.f ? 1.f : B;
    return Result;
}