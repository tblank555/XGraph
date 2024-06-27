// XGMatrix4x4.h
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#pragma once
#include "XGVector3D.h"

struct XGVector3D;

struct XGMatrix4x4
{
    float Values[4][4] = { { 0.0f } };

    static XGMatrix4x4 Identity();
    static XGMatrix4x4 RotationX(const float& AngleRadians);
    static XGMatrix4x4 RotationY(const float& AngleRadians);
    static XGMatrix4x4 RotationZ(const float& AngleRadians);
    static XGMatrix4x4 Translation(const XGVector3D& TranslationVector);
    static XGMatrix4x4 PerspectiveProjection(
        const float& FieldOfViewDegrees,
        const float& AspectRatio,
        const float& NearClipPlaneDistance,
        const float& FarClipPlaneDistance
    );

    XGMatrix4x4 operator*(const XGMatrix4x4& OtherMatrix) const;
    XGVector3D operator*(const XGVector3D& Vector) const;
};
