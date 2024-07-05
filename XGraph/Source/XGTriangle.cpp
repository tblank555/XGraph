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
        return (NormalizedPlaneNormal.X * Position.X + NormalizedPlaneNormal.Y * Position.Y + NormalizedPlaneNormal.Z * Position.Z - NormalizedPlaneNormal.DotProduct(PointOnPlane));
    };

    const XGVector3D* InsidePoints[3];
    int InsidePointCount = 0;
    const XGVector3D* OutsidePoints[3];
    int OutsidePointCount = 0;

    const XGVector2D* InsideTextureCoordinates[3];
    int InsideTextureCoordinateCount = 0;
    const XGVector2D* OutsideTextureCoordinates[3];
    int OutsideTextureCoordinateCount = 0;

    const float Distance0 = GetSignedDistanceToPlane(Points[0]);
    const float Distance1 = GetSignedDistanceToPlane(Points[1]);
    const float Distance2 = GetSignedDistanceToPlane(Points[2]);

    // If the signed distance to the plane is positive, then the point lies inside the clipping plane
    
    if (Distance0 >= 0.0f)
    {
        InsidePoints[InsidePointCount++] = &Points[0];
        InsideTextureCoordinates[InsideTextureCoordinateCount++] = &TextureCoordinates[0];
    }
    else
    {
        OutsidePoints[OutsidePointCount++] = &Points[0];
        OutsideTextureCoordinates[OutsideTextureCoordinateCount++] = &TextureCoordinates[0];
    }

    if (Distance1 >= 0.0f)
    {
        InsidePoints[InsidePointCount++] = &Points[1];
        InsideTextureCoordinates[InsideTextureCoordinateCount++] = &TextureCoordinates[1];
    }
    else
    {
        OutsidePoints[OutsidePointCount++] = &Points[1];
        OutsideTextureCoordinates[OutsideTextureCoordinateCount++] = &TextureCoordinates[1];
    }

    if (Distance2 >= 0.0f)
    {
        InsidePoints[InsidePointCount++] = &Points[2];
        InsideTextureCoordinates[InsideTextureCoordinateCount++] = &TextureCoordinates[2];
    }
    else
    {
        OutsidePoints[OutsidePointCount++] = &Points[2];
        OutsideTextureCoordinates[OutsideTextureCoordinateCount++] = &TextureCoordinates[2];
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
        OutTriangle1.TextureCoordinates[0] = *InsideTextureCoordinates[0];

        // The two new points are where the edges of the triangle intersect with the clipping plane

        float IntersectionScale1;
        OutTriangle1.Points[1] = XGVector3D::GetLineToIntersectionWithPlane(
            PointOnPlane,
            NormalizedPlaneNormal,
            *InsidePoints[0],
            *OutsidePoints[0],
            IntersectionScale1
        );

        // Calculate the new texture coordinates for this new point
        OutTriangle1.TextureCoordinates[1].U = IntersectionScale1 * (OutsideTextureCoordinates[0]->U - InsideTextureCoordinates[0]->U) + InsideTextureCoordinates[0]->U;
        OutTriangle1.TextureCoordinates[1].V = IntersectionScale1 * (OutsideTextureCoordinates[0]->V - InsideTextureCoordinates[0]->V) + InsideTextureCoordinates[0]->V;
        OutTriangle1.TextureCoordinates[1].W = IntersectionScale1 * (OutsideTextureCoordinates[0]->W - InsideTextureCoordinates[0]->W) + InsideTextureCoordinates[0]->W;

        float IntersectionScale2;
        OutTriangle1.Points[2] = XGVector3D::GetLineToIntersectionWithPlane(
            PointOnPlane,
            NormalizedPlaneNormal,
            *InsidePoints[0],
            *OutsidePoints[1],
            IntersectionScale2
        );

        // Calculate the new texture coordinates for this new point
        OutTriangle1.TextureCoordinates[2].U = IntersectionScale2 * (OutsideTextureCoordinates[1]->U - InsideTextureCoordinates[0]->U) + InsideTextureCoordinates[0]->U;
        OutTriangle1.TextureCoordinates[2].V = IntersectionScale2 * (OutsideTextureCoordinates[1]->V - InsideTextureCoordinates[0]->V) + InsideTextureCoordinates[0]->V;
        OutTriangle1.TextureCoordinates[2].W = IntersectionScale2 * (OutsideTextureCoordinates[1]->W - InsideTextureCoordinates[0]->W) + InsideTextureCoordinates[0]->W;

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
        OutTriangle1.TextureCoordinates[0] = *InsideTextureCoordinates[0];
        
        OutTriangle1.Points[1] = *InsidePoints[1];
        OutTriangle1.TextureCoordinates[1] = *InsideTextureCoordinates[1];

        float IntersectionScale1;
        OutTriangle1.Points[2] = XGVector3D::GetLineToIntersectionWithPlane(
            PointOnPlane,
            NormalizedPlaneNormal,
            *InsidePoints[0],
            *OutsidePoints[0],
            IntersectionScale1
        );

        // Calculate the new texture coordinates for this new point
        OutTriangle1.TextureCoordinates[2].U = IntersectionScale1 * (OutsideTextureCoordinates[0]->U - InsideTextureCoordinates[0]->U) + InsideTextureCoordinates[0]->U;
        OutTriangle1.TextureCoordinates[2].V = IntersectionScale1 * (OutsideTextureCoordinates[0]->V - InsideTextureCoordinates[0]->V) + InsideTextureCoordinates[0]->V;
        OutTriangle1.TextureCoordinates[2].W = IntersectionScale1 * (OutsideTextureCoordinates[0]->W - InsideTextureCoordinates[0]->W) + InsideTextureCoordinates[0]->W;

        // The second new triangle uses the second inside point, the new point we just calculated above, and a new point
        // where the second side of the triangle intersects the clipping plane

        OutTriangle2.Points[0] = *InsidePoints[1];
        OutTriangle2.TextureCoordinates[0] = *InsideTextureCoordinates[1];
        
        OutTriangle2.Points[1] = OutTriangle1.Points[2];
        OutTriangle2.TextureCoordinates[1] = OutTriangle1.TextureCoordinates[2];

        float IntersectionScale2;
        OutTriangle2.Points[2] = XGVector3D::GetLineToIntersectionWithPlane(
            PointOnPlane,
            NormalizedPlaneNormal,
            *InsidePoints[1],
            *OutsidePoints[0],
            IntersectionScale2
        );

        // Calculate the new texture coordinates for this new point
        OutTriangle2.TextureCoordinates[2].U = IntersectionScale2 * (OutsideTextureCoordinates[0]->U - InsideTextureCoordinates[1]->U) + InsideTextureCoordinates[1]->U;
        OutTriangle2.TextureCoordinates[2].V = IntersectionScale2 * (OutsideTextureCoordinates[0]->V - InsideTextureCoordinates[1]->V) + InsideTextureCoordinates[1]->V;
        OutTriangle2.TextureCoordinates[2].W = IntersectionScale2 * (OutsideTextureCoordinates[0]->W - InsideTextureCoordinates[1]->W) + InsideTextureCoordinates[1]->W;

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
