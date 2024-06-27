#pragma once

struct  XColor 
{
    float R;
    float G;
    float B;

    XColor();

    XColor(float InR, float InG, float InB);

    XColor operator*(float InScalar) const;

    XColor operator+(const XColor& InOther) const;

    XColor Clamp() const;

};