// XGVector3D.cpp
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#include "XGVector3D.h"

#include <cmath>
#include "XGMatrix4x4.h"

XGVector3D XGVector3D::GetLineToIntersectionWithPlane(const XGVector3D& PointOnPlane, const XGVector3D& PlaneNormal,
    const XGVector3D& LineStartPosition, const XGVector3D& LineEndPosition)
{
    const XGVector3D NormalizedPlaneNormal = PlaneNormal.GetNormalizedCopy();
    const float PlaneDotProduct = NormalizedPlaneNormal.DotProduct(PointOnPlane);
    const float LineStartDotProduct = LineStartPosition.DotProduct(NormalizedPlaneNormal);
    const float LineEndDotProduct = LineEndPosition.DotProduct(NormalizedPlaneNormal);
    const float IntersectionScale = (-PlaneDotProduct - LineStartDotProduct) / (LineEndDotProduct - LineStartDotProduct);
    const XGVector3D LineStartToEnd = LineEndPosition - LineStartPosition;
    const XGVector3D LineStartToIntersection = LineStartToEnd * IntersectionScale;
    return LineStartPosition + LineStartToIntersection;
}

XGVector3D XGVector3D::operator+(const XGVector3D& OtherVector) const
{
    return { X + OtherVector.X, Y + OtherVector.Y, Z + OtherVector.Z };
}

XGVector3D& XGVector3D::operator+=(const XGVector3D& OtherVector)
{
    X += OtherVector.X;
    Y += OtherVector.Y;
    Z += OtherVector.Z;
    return *this;
}

XGVector3D XGVector3D::operator-(const XGVector3D& OtherVector) const
{
    return { X - OtherVector.X, Y - OtherVector.Y, Z - OtherVector.Z };
}

XGVector3D& XGVector3D::operator-=(const XGVector3D& OtherVector)
{
    X -= OtherVector.X;
    Y -= OtherVector.Y;
    Z -= OtherVector.Z;
    return *this;
}

XGVector3D XGVector3D::operator*(const float& Scale) const
{
    return { X * Scale, Y * Scale, Z * Scale };
}

XGVector3D& XGVector3D::operator*=(const float& Scale)
{
    X *= Scale;
    Y *= Scale;
    Z *= Scale;
    return *this;
}

XGVector3D XGVector3D::operator/(const float& Scale) const
{
    return { X / Scale, Y / Scale, Z / Scale };
}

XGVector3D& XGVector3D::operator/=(const float& Scale)
{
    X /= Scale;
    Y /= Scale;
    Z /= Scale;
    return *this;
}

void XGVector3D::Normalize()
{
    const float Length = GetLength();
    X /= Length;
    Y /= Length;
    Z /= Length;
}

XGVector3D XGVector3D::GetNormalizedCopy() const
{
    XGVector3D Normalized;
    const float Length = GetLength();
    Normalized.X = X/Length;
    Normalized.Y = Y/Length;
    Normalized.Z = Z/Length;

    return Normalized;
}

float XGVector3D::GetLength() const
{
    return sqrtf(X * X + Y * Y + Z * Z);
}

float XGVector3D::DotProduct(const XGVector3D& OtherVector) const
{
    return X * OtherVector.X + Y * OtherVector.Y + Z * OtherVector.Z;
}

XGVector3D XGVector3D::CrossProduct(const XGVector3D& OtherVector) const
{
    return {
        Y * OtherVector.Z - Z * OtherVector.Y,
        Z * OtherVector.X - X * OtherVector.Z,
        X * OtherVector.Y - Y * OtherVector.X
    };
}

void XGVector3D::MultiplyByMatrix(const XGMatrix4x4& Matrix, XGVector3D& Result) const
{
    Result.X = X * Matrix.Values[0][0] + Y * Matrix.Values[1][0] + Z * Matrix.Values[2][0] + Matrix.Values[3][0];
    Result.Y = X * Matrix.Values[0][1] + Y * Matrix.Values[1][1] + Z * Matrix.Values[2][1] + Matrix.Values[3][1];
    Result.Z = X * Matrix.Values[0][2] + Y * Matrix.Values[1][2] + Z * Matrix.Values[2][2] + Matrix.Values[3][2];
    
    const float Whoa = X * Matrix.Values[0][3] +  Y * Matrix.Values[1][3] + Z * Matrix.Values[2][3] + Matrix.Values[3][3];
    
    if (W != 0.0f)
    {
        Result.X /= Whoa;
        Result.Y /= Whoa;
        Result.Z /= Whoa;
    }
}

