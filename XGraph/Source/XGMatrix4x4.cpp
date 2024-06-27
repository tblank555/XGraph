// XGMatrix4x4.cpp
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#include "XGMatrix4x4.h"

#include <cmath>
#include "XGVector3D.h"

XGMatrix4x4 XGMatrix4x4::Identity()
{
    XGMatrix4x4 Identity;
    Identity.Values[0][0] = 1.0f;
    Identity.Values[1][1] = 1.0f;
    Identity.Values[2][2] = 1.0f;
    Identity.Values[3][3] = 1.0f;
    return Identity;
}

XGMatrix4x4 XGMatrix4x4::RotationX(const float& AngleRadians)
{
    XGMatrix4x4 Rotation;
    Rotation.Values[0][0] = 1.0f;
    Rotation.Values[1][1] = cosf(AngleRadians);
    Rotation.Values[1][2] = sinf(AngleRadians);
    Rotation.Values[2][1] = -sinf(AngleRadians);
    Rotation.Values[2][2] = cosf(AngleRadians);
    Rotation.Values[3][3] = 1.0f;
    return Rotation;
}

XGMatrix4x4 XGMatrix4x4::RotationY(const float& AngleRadians)
{
    XGMatrix4x4 Rotation;
    Rotation.Values[0][0] = cosf(AngleRadians);
    Rotation.Values[0][2] = sinf(AngleRadians);
    Rotation.Values[1][1] = 1.0f;
    Rotation.Values[2][0] = -sinf(AngleRadians);
    Rotation.Values[2][2] = cosf(AngleRadians);
    Rotation.Values[3][3] = 1.0f;
    return Rotation;
}

XGMatrix4x4 XGMatrix4x4::RotationZ(const float& AngleRadians)
{
    XGMatrix4x4 Rotation;
    Rotation.Values[0][0] = cosf(AngleRadians);
    Rotation.Values[0][1] = sinf(AngleRadians);
    Rotation.Values[1][0] = -sinf(AngleRadians);
    Rotation.Values[1][1] = cosf(AngleRadians);
    Rotation.Values[2][2] = 1.0f;
    Rotation.Values[3][3] = 1.0f;
    return Rotation;
}

XGMatrix4x4 XGMatrix4x4::Translation(const XGVector3D& TranslationVector)
{
    XGMatrix4x4 Translation = Identity();
    Translation.Values[3][0] = TranslationVector.X;
    Translation.Values[3][1] = TranslationVector.Y;
    Translation.Values[3][2] = TranslationVector.Z;
    return Translation;
}

XGMatrix4x4 XGMatrix4x4::PerspectiveProjection(
    const float& FieldOfViewDegrees,
    const float& AspectRatio,
    const float& NearClipPlaneDistance,
    const float& FarClipPlaneDistance)
{
    const float InverseTanHalfFovRadians = 1.0f / tanf(FieldOfViewDegrees / 2.0f * 3.14159f / 180.0f);
    
    XGMatrix4x4 Projection;
    Projection.Values[0][0] = AspectRatio * InverseTanHalfFovRadians;
    Projection.Values[1][1] = InverseTanHalfFovRadians;
    Projection.Values[2][2] = FarClipPlaneDistance / (FarClipPlaneDistance - NearClipPlaneDistance);
    Projection.Values[2][3] = -1.0f;
    Projection.Values[3][2] = (-FarClipPlaneDistance * NearClipPlaneDistance) / (FarClipPlaneDistance - NearClipPlaneDistance);
    
    return Projection;
}

XGMatrix4x4 XGMatrix4x4::PointAt(const XGVector3D& Position, const XGVector3D& Target, const XGVector3D& Up)
{
    XGVector3D NewForward = Target - Position;
    NewForward.Normalize();

    const float ForwardComponentOfUp = Up.DotProduct(NewForward);
    const XGVector3D UpOffset = NewForward * ForwardComponentOfUp;
    XGVector3D NewUp = Up - UpOffset;
    NewUp.Normalize();

    const XGVector3D NewRight = NewUp.CrossProduct(NewForward);

    // Create transformation matrix
    XGMatrix4x4 PointAt;
    PointAt.Values[0][0] = NewRight.X;
    PointAt.Values[0][1] = NewRight.Y;
    PointAt.Values[0][2] = NewRight.Z;
    PointAt.Values[1][0] = NewUp.X;
    PointAt.Values[1][1] = NewUp.Y;
    PointAt.Values[1][2] = NewUp.Z;
    PointAt.Values[2][0] = NewForward.X;
    PointAt.Values[2][1] = NewForward.Y;
    PointAt.Values[2][2] = NewForward.Z;
    PointAt.Values[3][0] = Position.X;
    PointAt.Values[3][1] = Position.Y;
    PointAt.Values[3][2] = Position.Z;
    PointAt.Values[3][3] = 1.0f;

    return PointAt;
}

XGMatrix4x4 XGMatrix4x4::operator*(const XGMatrix4x4& OtherMatrix) const
{
    XGMatrix4x4 Result;
    for (int Column = 0; Column < 4; ++Column)
    {
        for (int Row = 0; Row < 4; ++Row)
        {
            Result.Values[Row][Column] =
                Values[Row][0] * OtherMatrix.Values[0][Column] +
                Values[Row][1] * OtherMatrix.Values[1][Column] +
                Values[Row][2] * OtherMatrix.Values[2][Column] +
                Values[Row][3] * OtherMatrix.Values[3][Column];
        }
    }

    return Result;
}

XGVector3D XGMatrix4x4::operator*(const XGVector3D& Vector) const
{
    return {
        Vector.X * Values[0][0] + Vector.Y * Values[1][0] + Vector.Z * Values[2][0] + Vector.W * Values[3][0],
        Vector.X * Values[0][1] + Vector.Y * Values[1][1] + Vector.Z * Values[2][1] + Vector.W * Values[3][1],
        Vector.X * Values[0][2] + Vector.Y * Values[1][2] + Vector.Z * Values[2][2] + Vector.W * Values[3][2],
        Vector.X * Values[0][3] + Vector.Y * Values[1][3] + Vector.Z * Values[2][3] + Vector.W * Values[3][3]
    };
}

void XGMatrix4x4::InvertQuick()
{
    Values[0][0] = Values[0][0];
    Values[0][1] = Values[1][0];
    Values[0][2] = Values[2][0];
    Values[0][3] = 0.0f;
    Values[1][0] = Values[0][1];
    Values[1][1] = Values[1][1];
    Values[1][2] = Values[2][1];
    Values[1][3] = 0.0f;
    Values[2][0] = Values[0][2];
    Values[2][1] = Values[1][2];
    Values[2][2] = Values[2][2];
    Values[2][3] = 0.0f;
    Values[3][0] = -(Values[3][0] * Values[0][0] + Values[3][1] * Values[1][0] + Values[3][2] * Values[2][0]);
    Values[3][1] = -(Values[3][0] * Values[0][1] + Values[3][1] * Values[1][1] + Values[3][2] * Values[2][1]);
    Values[3][2] = -(Values[3][0] * Values[0][2] + Values[3][1] * Values[1][2] + Values[3][2] * Values[2][2]);
    Values[3][3] = 1.0f;
}
