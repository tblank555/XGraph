// XGTriangle.h
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#pragma once

#include "../ThirdParty/olcPixelGameEngine.h"
#include "XGVector3D.h"

struct XGTriangle
{
    XGVector3D Points[3];

    olc::Pixel Color;

    XGTriangle() : Points{ XGVector3D(), XGVector3D(), XGVector3D() }, Color(olc::WHITE) {}
    
    XGTriangle(const XGVector3D& Point1, const XGVector3D& Point2, const XGVector3D& Point3)
        : Points{ Point1, Point2, Point3 }, Color(olc::WHITE) {}
};
