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

    /**
     * \brief Creates a matrix that can be used to transform vectors in one coordinate space to point at a target vector
     * \param Position The amount to offset the position
     * \param Target The target vector to point things at
     * \param Up The new up vector for the new orientation
     * \return The transformation matrix
     */
    static XGMatrix4x4 PointAt(const XGVector3D& Position, const XGVector3D& Target, const XGVector3D& Up);

    XGMatrix4x4 operator*(const XGMatrix4x4& OtherMatrix) const;
    XGVector3D operator*(const XGVector3D& Vector) const;

    /**
     * \brief Inverts the matrix using a quicker algorithm
     * \details This only works on rotation and/or translation matrices
     */
    void InvertQuick();
};
