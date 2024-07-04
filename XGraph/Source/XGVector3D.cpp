// XGVector3D.cpp
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#include "XGVector3D.h"

#include <cmath>

XGVector3D XGVector3D::GetLineToIntersectionWithPlane(const XGVector3D& PointOnPlane, const XGVector3D& PlaneNormal,
    const XGVector3D& LineStartPosition, const XGVector3D& LineEndPosition)
{
    const XGVector3D NormalizedPlaneNormal = PlaneNormal.GetNormalizedCopy();
    const float PlaneDotProduct = -NormalizedPlaneNormal.DotProduct(PointOnPlane);
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
