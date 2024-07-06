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
     * \param OutIntersectionScale A value from 0 to 1 that represents where along the given line the intersection
     * occurs. 0 means the plane intersects the line right at the start position, 1 means the end position, and 0.5
     * means the plane intersects the line exactly halfway between the start and end positions.
     */
    static XGVector3D GetLineToIntersectionWithPlane(
        const XGVector3D& PointOnPlane,
        const XGVector3D& PlaneNormal,
        const XGVector3D& LineStartPosition,
        const XGVector3D& LineEndPosition,
        float& OutIntersectionScale
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

    /**
     * \brief Returns the signed distance of this vector from the given plane
     * \details Sign will be positive if the vector describes a position that lies on the side of the plane that the
     * normal is pointing to
     * \param PointOnPlane Any point on the plane
     * \param PlaneNormal The normal of the plane
     */
    float GetSignedDistanceToPlane(const XGVector3D& PointOnPlane, const XGVector3D& PlaneNormal) const;
};
