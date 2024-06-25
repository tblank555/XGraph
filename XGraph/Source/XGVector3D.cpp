// XGVector3D.cpp
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#include "XGVector3D.h"

#include <cmath>
#include "XGMatrix4x4.h"

void XGVector3D::Normalize()
{
    const float Length = sqrtf(X * X + Y * Y + Z * Z);
    X /= Length;
    Y /= Length;
    Z /= Length;
}

float XGVector3D::DotProduct(const XGVector3D& OtherVector) const
{
    return X * OtherVector.X + Y * OtherVector.Y + Z * OtherVector.Z;
}

void XGVector3D::MultiplyByMatrix(const XGMatrix4x4& Matrix, XGVector3D& Result) const
{
    Result.X = X * Matrix.Values[0][0] +  Y * Matrix.Values[1][0] + Z * Matrix.Values[2][0] + Matrix.Values[3][0];
    Result.Y = X * Matrix.Values[0][1] +  Y * Matrix.Values[1][1] + Z * Matrix.Values[2][1] + Matrix.Values[3][1];
    Result.Z = X * Matrix.Values[0][2] +  Y * Matrix.Values[1][2] + Z * Matrix.Values[2][2] + Matrix.Values[3][2];

    const float W = X * Matrix.Values[0][3] +  Y * Matrix.Values[1][3] + Z * Matrix.Values[2][3] + Matrix.Values[3][3];

    if (W != 0.0f)
    {
        Result.X /= W;
        Result.Y /= W;
        Result.Z /= W;
    }
}

