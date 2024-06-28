// XGTriangle.cpp
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#include "XGTriangle.h"

XGVector3D XGTriangle::GetNormal() const
{
    const XGVector3D Side1 = Points[1] - Points[0];
    const XGVector3D Side2 = Points[2] - Points[0];
    
    XGVector3D Normal = Side1.CrossProduct(Side2);
    Normal.Normalize();
    
    return Normal;
}

int XGTriangle::ClipAgainstPlane(
    const XGVector3D& PointOnPlane,
    const XGVector3D& PlaneNormal,
    XGTriangle& OutTriangle1,
    XGTriangle& OutTriangle2) const
{
    const XGVector3D NormalizedPlaneNormal = PlaneNormal.GetNormalizedCopy();

    auto GetSignedDistanceToPlane = [&](const XGVector3D& Position)
    {
        return (NormalizedPlaneNormal.X * Position.X + NormalizedPlaneNormal.Y + Position.Y + NormalizedPlaneNormal.Z + Position.Z - NormalizedPlaneNormal.DotProduct(Position));
    };

    const XGVector3D* InsidePoints[3];
    int InsidePointCount = 0;
    const XGVector3D* OutsidePoints[3];
    int OutsidePointCount = 0;

    const float Distance0 = GetSignedDistanceToPlane(Points[0]);
    const float Distance1 = GetSignedDistanceToPlane(Points[1]);
    const float Distance2 = GetSignedDistanceToPlane(Points[2]);

    // If the signed distance to the plane is positive, then the point lies inside the clipping plane
    
    if (Distance0 >= 0.0f)
    {
        InsidePoints[InsidePointCount++] = &Points[0];
    }
    else
    {
        OutsidePoints[OutsidePointCount++] = &Points[0];
    }

    if (Distance1 >= 0.0f)
    {
        InsidePoints[InsidePointCount++] = &Points[1];
    }
    else
    {
        OutsidePoints[OutsidePointCount++] = &Points[1];
    }

    if (Distance2 >= 0.0f)
    {
        InsidePoints[InsidePointCount++] = &Points[2];
    }
    else
    {
        OutsidePoints[OutsidePointCount++] = &Points[2];
    }

    if (InsidePointCount == 0)
    {
        // All points of the triangle are outside the clipping plane
        return 0;
    }
    else if (InsidePointCount == 1)
    {
        // This triangle needs to be clipped, yielding one new triangle
        
        if (OutsidePointCount != 2)
        {
            std::cout << "ERROR: Unexpected triangle point count while clipping. InsidePointCount = " << std::to_string(InsidePointCount) << ", OutsidePointCount = " << std::to_string(OutsidePointCount) << std::endl;
            return -1;
        }

        OutTriangle1.Color = Color;

        // The inside point is valid, so keep that one
        OutTriangle1.Points[0] = *InsidePoints[0];

        // The two new points are where the edges of the triangle intersect with the clipping plane
        
        OutTriangle1.Points[1] = XGVector3D::GetLineToIntersectionWithPlane(
            PointOnPlane,
            PlaneNormal,
            *InsidePoints[0],
            *OutsidePoints[0]
        );

        OutTriangle1.Points[2] = XGVector3D::GetLineToIntersectionWithPlane(
            PointOnPlane,
            PlaneNormal,
            *InsidePoints[0],
            *OutsidePoints[1]
        );

        return 1;
    }
    else if (InsidePointCount == 2)
    {
        // This triangle needs to be clipped, forming two new triangles that form a quadrangle
        
        if (OutsidePointCount != 1)
        {
            std::cout << "ERROR: Unexpected triangle point count while clipping. InsidePointCount = " << std::to_string(InsidePointCount) << ", OutsidePointCount = " << std::to_string(OutsidePointCount) << std::endl;
            return -1;
        }

        OutTriangle1.Color = Color;
        OutTriangle2.Color = Color;

        // The first new triangle uses the two inside points and a new point where the first side of the triangle
        // intersects with the clipping plane
        OutTriangle1.Points[0] = *InsidePoints[0];
        OutTriangle1.Points[1] = *InsidePoints[1];
        OutTriangle1.Points[2] = XGVector3D::GetLineToIntersectionWithPlane(
            PointOnPlane,
            PlaneNormal,
            *InsidePoints[0],
            *OutsidePoints[0]
        );

        // The second new triangle uses the second inside point, the new point we just calculated above, and a new point
        // where the second side of the triangle intersects the clipping plane
        OutTriangle2.Points[0] = *InsidePoints[1];
        OutTriangle2.Points[1] = OutTriangle1.Points[2];
        OutTriangle2.Points[2] = XGVector3D::GetLineToIntersectionWithPlane(
            PointOnPlane,
            PlaneNormal,
            *InsidePoints[1],
            *OutsidePoints[0]
        );

        return 2;
    }
    else if (InsidePointCount == 3)
    {
        // All points of the triangle are inside the clipping plane, so no need to calculate new triangles
        OutTriangle1 = *this;
        return 1;
    }

    std::cout << "ERROR: Unexpected triangle point count while clipping. InsidePointCount = " << std::to_string(InsidePointCount) << ", OutsidePointCount = " << std::to_string(OutsidePointCount) << std::endl;
    return -1;
}
