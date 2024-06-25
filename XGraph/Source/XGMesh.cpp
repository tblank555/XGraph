// XGMesh.cpp
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#pragma once

#include "XGMesh.h"

#include <fstream>
#include <strstream>

bool XGMesh::LoadFromObjectFile(std::string FilePath)
{
    std::ifstream FileStream(FilePath);
    if (!FileStream.is_open())
    {
        // TODO: Log an error here
        return false;
    }

    std::vector<XGVector3D> Vertices;

    while (!FileStream.eof())
    {
        // WARNING: Assumption that each line in the file is no longer than 128 characters
        char Line[128];
        FileStream.getline(Line, 128);

        std::strstream LineStream;
        LineStream << Line;

        char Unused;

        if (Line[0] == 'v')
        {
            // This line defines a vertex, so we'll create an instance of XGVertex3D for it
            XGVector3D Vertex;

            // Read the contents of LineStream into a vector, throwing out the initial 'v' character
            // Example .obj file line:
            // v 0.00045 -3.00465 1.11046
            LineStream >> Unused >> Vertex.X >> Vertex.Y >> Vertex.Z;

            Vertices.push_back(Vertex);
        }
        else if (Line[0] == 'f')
        {
            // This line defines a face, so we'll create an instance of XGTriangle and add it to this mesh
            int VertexIndices[3];

            // Read the contents of LineStream into VertexIndices, throwing out the initial 'f' character
            // Example .obj file line:
            // f 5 9 14
            LineStream >> Unused >> VertexIndices[0] >> VertexIndices[1] >> VertexIndices[2];

            // Create a triangle using the vertex indices we read in. Subtract one from each index before we look it
            // up in Vertices, because .obj files use base-1 vertex indices instead of base-0.
            XGTriangle Face = XGTriangle(
                XGVector3D(Vertices[VertexIndices[0] - 1]),
                XGVector3D(Vertices[VertexIndices[1] - 1]),
                XGVector3D(Vertices[VertexIndices[2] - 1])
            );

            Triangles.push_back(Face);
        }
    }
    
    return true;
}
