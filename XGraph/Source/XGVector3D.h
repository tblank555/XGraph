// XGVector3D.h
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#pragma once

struct XGMatrix4x4;

struct XGVector3D
{
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;

    float W = 1.0f; // Necessary for multiplication with 4x4 matrices

    XGVector3D() = default;
    XGVector3D(const float& X, const float& Y, const float& Z, const float&W = 1.0f) : X(X), Y(Y), Z(Z), W(W) {}

    XGVector3D operator+(const XGVector3D& OtherVector) const;
    XGVector3D& operator+=(const XGVector3D& OtherVector);
    XGVector3D operator-(const XGVector3D& OtherVector) const;
    XGVector3D& operator-=(const XGVector3D& OtherVector);
    XGVector3D operator*(const float& Scale) const;
    XGVector3D& operator*=(const float& Scale);
    XGVector3D operator/(const float& Scale) const;
    XGVector3D& operator/=(const float& Scale);

    void Normalize();

    float GetLength() const;

    float DotProduct(const XGVector3D& OtherVector) const;
    XGVector3D CrossProduct(const XGVector3D& OtherVector) const;

    void MultiplyByMatrix(const XGMatrix4x4& Matrix, XGVector3D& Result) const;
};
