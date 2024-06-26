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
