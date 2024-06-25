// XGVector3D.h
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#pragma once

struct XGMatrix4x4;

struct XGVector3D
{
    float X, Y, Z;

    XGVector3D() : X(0.0f), Y(0.0f), Z(0.0f) {}
    XGVector3D(const float& X, const float& Y, const float& Z) : X(X), Y(Y), Z(Z) {}

    void Normalize();

    float DotProduct(const XGVector3D& OtherVector) const;

    void MultiplyByMatrix(const XGMatrix4x4& Matrix, XGVector3D& Result) const;
};
