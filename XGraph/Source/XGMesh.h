// XGMesh.h
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include "XGTriangle.h"

struct XGMesh
{
    std::vector<XGTriangle> Triangles;

    bool LoadFromObjectFile(const std::string& FilePath, bool HasTexture = false, bool InvertUVMapping = false);
};
