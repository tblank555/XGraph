// XGMesh.cpp
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#pragma once

#include "XGMesh.h"

#include <fstream>
#include <strstream>

bool XGMesh::LoadFromObjectFile(std::string FilePath, bool HasTexture, bool InvertUVMapping)
{
    std::ifstream FileStream(FilePath);
    if (!FileStream.is_open())
    {
        // TODO: Log an error here
        return false;
    }

    std::vector<XGVector3D> Vertices;
    std::vector<XGVector2D> TextureCoordinates;

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
            if (Line[1] == 't')
            {
                // This line defines some texture coordinates
                XGVector2D VertexTextureCoordinates;

                // Throw out both the 'v' and 't' characters, then store the rest of the info in our 2D vector
                // Example .obj file line:
                // vt 0.45000 0.75000
                LineStream >> Unused >> Unused >> VertexTextureCoordinates.U >> VertexTextureCoordinates.V;

                if (InvertUVMapping)
                {
                    VertexTextureCoordinates.U = 1.0f - VertexTextureCoordinates.U;
                    VertexTextureCoordinates.V = 1.0f - VertexTextureCoordinates.V;
                }

                TextureCoordinates.push_back(VertexTextureCoordinates);
            }
            else
            {
                // This line defines a vertex, so we'll create an instance of XGVertex3D for it
                XGVector3D Vertex;

                // Read the contents of LineStream into a vector, throwing out the initial 'v' character
                // Example .obj file line:
                // v 0.00045 -3.00465 1.11046
                LineStream >> Unused >> Vertex.X >> Vertex.Y >> Vertex.Z;

                Vertices.push_back(Vertex);
            }
        }
        else if (Line[0] == 'f')
        {
            // .obj files have a different format for the lines that define faces if the file contains texture data as well
            if (HasTexture)
            {
                // Parse lines that define a face for vertex indices and texture coordinate indices
                // Example line:
                // f 2/1 4/2 1/3

                // Throw out the 'f' character
                LineStream >> Unused;

                std::string Tokens[8];
                int TokenCount = -1;
                
                while (!LineStream.eof())
                {
                    char Character = LineStream.get();
                    if (Character == ' ' || Character == '/')
                    {
                        TokenCount++;
                    }
                    else
                    {
                        Tokens[TokenCount].append(1, Character);
                    }
                }

                Tokens[TokenCount].pop_back();
                
                Triangles.push_back({
                    Vertices[stoi(Tokens[0]) - 1],
                    Vertices[stoi(Tokens[2]) - 1],
                    Vertices[stoi(Tokens[4]) - 1],
                    TextureCoordinates[stoi(Tokens[1]) - 1],
                    TextureCoordinates[stoi(Tokens[3]) - 1],
                    TextureCoordinates[stoi(Tokens[5]) - 1]
                });

                // This line defined a quad, so add the second triangle
                if (TokenCount == 7)
                {
                    Triangles.push_back({
                        Vertices[stoi(Tokens[0]) - 1],
                        Vertices[stoi(Tokens[4]) - 1],
                        Vertices[stoi(Tokens[6]) - 1],
                        TextureCoordinates[stoi(Tokens[1]) - 1],
                        TextureCoordinates[stoi(Tokens[5]) - 1],
                        TextureCoordinates[stoi(Tokens[7]) - 1]
                    });
                }
            }
            else
            {
                // Parse lines that define a face for vertex indices
                // Example line:
                // f 2 4 1
                
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
    }
    
    return true;
}
