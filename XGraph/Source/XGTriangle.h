// XGTriangle.h
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#pragma once

#include "../ThirdParty/olcPixelGameEngine.h"
#include "XGVector2D.h"
#include "XGVector3D.h"

struct XGTriangle
{
    XGVector3D Points[3];

    XGVector2D TextureCoordinates[3];

    olc::Pixel Color;

    XGTriangle() : Points{ XGVector3D(), XGVector3D(), XGVector3D() }, Color(olc::WHITE) {}
    
    XGTriangle(const XGVector3D& Point1, const XGVector3D& Point2, const XGVector3D& Point3)
        : Points{ Point1, Point2, Point3 }, Color(olc::WHITE) {}

    XGTriangle(
        const XGVector3D& Point1,
        const XGVector3D& Point2,
        const XGVector3D& Point3,
        const XGVector2D& TextureCoordinate1,
        const XGVector2D& TextureCoordinate2,
        const XGVector2D& TextureCoordinate3
    ) : Points{ Point1, Point2, Point3 },
        TextureCoordinates{ TextureCoordinate1, TextureCoordinate2, TextureCoordinate3 },
        Color(olc::WHITE) {}

    XGVector3D GetNormal() const;

    /**
     * \brief Clips this triangle against the given plane
     * \param PointOnPlane Any point on the plane
     * \param PlaneNormal The normal of the plane
     * \param OutTriangle1 If the return value is 1 or greater, this is the first new triangle created by clipping
     * \param OutTriangle2 If the return value is 2, this is the second new triangle created by clipping
     * \return The number of new triangles created by clipping. May be 0, 1, or 2.
     */
    int ClipAgainstPlane(
        const XGVector3D& PointOnPlane,
        const XGVector3D& PlaneNormal,
        XGTriangle& OutTriangle1,
        XGTriangle& OutTriangle2
    ) const;
};
