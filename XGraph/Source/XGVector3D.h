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

    /**
     * \brief Returns a vector from the LineStartPosition to the place on the given plane when the line intersects it
     * \param PointOnPlane Any point on the plane
     * \param PlaneNormal The normal of the plane
     * \param LineStartPosition The start position of the line
     * \param LineEndPosition The end position of the line
     */
    static XGVector3D GetLineToIntersectionWithPlane(
        const XGVector3D& PointOnPlane,
        const XGVector3D& PlaneNormal,
        const XGVector3D& LineStartPosition,
        const XGVector3D& LineEndPosition
    );

    XGVector3D operator+(const XGVector3D& OtherVector) const;
    XGVector3D& operator+=(const XGVector3D& OtherVector);
    XGVector3D operator-(const XGVector3D& OtherVector) const;
    XGVector3D& operator-=(const XGVector3D& OtherVector);
    XGVector3D operator*(const float& Scale) const;
    XGVector3D& operator*=(const float& Scale);
    XGVector3D operator/(const float& Scale) const;
    XGVector3D& operator/=(const float& Scale);

    void Normalize();
    XGVector3D GetNormalizedCopy() const;

    float GetLength() const;

    float DotProduct(const XGVector3D& OtherVector) const;
    XGVector3D CrossProduct(const XGVector3D& OtherVector) const;
};
