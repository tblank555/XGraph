// XGEngine.cpp
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#include "XGEngine.h"

#include <algorithm>
#include "XGTriangle.h"

XGEngine::XGEngine(const std::string& MeshFilePath)
{
    if (!MeshFilePath.empty())
    {
        MeshToRender.LoadFromObjectFile(MeshFilePath);
    }
    else
    {
        // Create a unit cube to render
        MeshToRender.Triangles = {
            // South face
            XGTriangle(
                XGVector3D(0.0f, 0.0f, 0.0f),
                XGVector3D(0.0f, 1.0f, 0.0f),
                XGVector3D(1.0f, 1.0f, 0.0f)
            ),
            XGTriangle(
                XGVector3D(0.0f, 0.0f, 0.0f),
                XGVector3D(1.0f, 1.0f, 0.0f),
                XGVector3D(1.0f, 0.0f, 0.0f)
            ),
            // East face
            XGTriangle(
                XGVector3D(1.0f, 0.0f, 0.0f),
                XGVector3D(1.0f, 1.0f, 0.0f),
                XGVector3D(1.0f, 1.0f, 1.0f)
            ),
            XGTriangle(
                XGVector3D(1.0f, 0.0f, 0.0f),
                XGVector3D(1.0f, 1.0f, 1.0f),
                XGVector3D(1.0f, 0.0f, 1.0f)
            ),
            // North face
            XGTriangle(
                XGVector3D(1.0f, 0.0f, 1.0f),
                XGVector3D(1.0f, 1.0f, 1.0f),
                XGVector3D(0.0f, 1.0f, 1.0f)
            ),
            XGTriangle(
                XGVector3D(1.0f, 0.0f, 1.0f),
                XGVector3D(0.0f, 1.0f, 1.0f),
                XGVector3D(0.0f, 0.0f, 1.0f)
            ),
            // West face
            XGTriangle(
                XGVector3D(0.0f, 0.0f, 1.0f),
                XGVector3D(0.0f, 1.0f, 1.0f),
                XGVector3D(0.0f, 1.0f, 0.0f)
            ),
            XGTriangle(
                XGVector3D(0.0f, 0.0f, 1.0f),
                XGVector3D(0.0f, 1.0f, 0.0f),
                XGVector3D(0.0f, 0.0f, 0.0f)
            ),
            // Top face
            XGTriangle(
                XGVector3D(0.0f, 1.0f, 0.0f),
                XGVector3D(0.0f, 1.0f, 1.0f),
                XGVector3D(1.0f, 1.0f, 1.0f)
            ),
            XGTriangle(
                XGVector3D(0.0f, 1.0f, 0.0f),
                XGVector3D(1.0f, 1.0f, 1.0f),
                XGVector3D(1.0f, 1.0f, 0.0f)
            ),
            // Bottom face
            XGTriangle(
                XGVector3D(1.0f, 0.0f, 1.0f),
                XGVector3D(0.0f, 0.0f, 1.0f),
                XGVector3D(0.0f, 0.0f, 0.0f)
            ),
            XGTriangle(
                XGVector3D(1.0f, 0.0f, 1.0f),
                XGVector3D(0.0f, 0.0f, 0.0f),
                XGVector3D(1.0f, 0.0f, 0.0f)
            )
        };
    }
}

bool XGEngine::OnUserCreate()
{
    // Create perspective projection matrix
    constexpr float NearClipPlane = 0.1f;
    constexpr float FarClipPlane = 1000.0f;
    constexpr float FieldOfViewDegrees = 90.0f;
    const float AspectRatio = static_cast<float>(ScreenHeight()) / static_cast<float>(ScreenWidth());

    ProjectionMatrix = XGMatrix4x4::PerspectiveProjection(
        FieldOfViewDegrees,
        AspectRatio,
        NearClipPlane,
        FarClipPlane
    );

    // Set and normalize the global directional light
    LightDirection = XGVector3D(0.0f, 0.0f, -1.0f);
    LightDirection.Normalize();
    
    return true;
}

bool XGEngine::OnUserUpdate(float fElapsedTime)
{
    // Clear screen to black
    FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

    RotationAngle += 1.0f * fElapsedTime;
    const XGMatrix4x4 RotateAroundZAxis = XGMatrix4x4::RotationZ(RotationAngle);
    const XGMatrix4x4 RotateAroundXAxis = XGMatrix4x4::RotationX(RotationAngle * 0.5f);

    const XGMatrix4x4 TranslateAlongZAxis = XGMatrix4x4::Translation({ 0.0f, 0.0f, 8.0f });

    const XGMatrix4x4 WorldMatrix = RotateAroundZAxis * RotateAroundXAxis * TranslateAlongZAxis;

    std::vector<XGTriangle> TrianglesToDraw;

    // Transform and project triangles
    for (const XGTriangle& Triangle : MeshToRender.Triangles)
    {
        XGTriangle TransformedTriangle = {
            WorldMatrix * Triangle.Points[0],
            WorldMatrix * Triangle.Points[1],
            WorldMatrix * Triangle.Points[2],
        };
        
        XGVector3D Normal = TransformedTriangle.GetNormal();

        // Find the vector from the camera to the triangle
        // We can use any point on the triangle, because we're using this for the dot product with the normal,
        // and the normal will be the same a any point on the triangle
        const XGVector3D CameraToTriangle = TransformedTriangle.Points[0] - CameraPosition;

        const float DotProduct = CameraToTriangle.DotProduct(Normal);

        // If the dot product of the vector from the camera to the triangle and the normal is greater than 0,
        // this triangle is facing away from the camera, and therefore shouldn't be rendered
        if (DotProduct >= 0.0f)
        {
            continue;
        }

        const float Luminance = LightDirection.DotProduct(Normal);
        TransformedTriangle.Color = CreateGrayscaleColor(Luminance);
        
        // Project the triangle from world space to an intermediate screen space
        XGTriangle ProjectedTriangle;
        TransformedTriangle.Points[0].MultiplyByMatrix(ProjectionMatrix, ProjectedTriangle.Points[0]);
        TransformedTriangle.Points[1].MultiplyByMatrix(ProjectionMatrix, ProjectedTriangle.Points[1]);
        TransformedTriangle.Points[2].MultiplyByMatrix(ProjectionMatrix, ProjectedTriangle.Points[2]);
        ProjectedTriangle.Color = TransformedTriangle.Color;

        // Scale triangle into view
        // Shift unit cube from -1 to 1 coordinates to 0 to 2
        const XGVector3D Offset = { 1.0f, 1.0f, 0.0f };
        ProjectedTriangle.Points[0] += Offset;
        ProjectedTriangle.Points[1] += Offset;
        ProjectedTriangle.Points[2] += Offset;

        // Divide each point by half to get to 0 to 1 range, and multiply by screen height or width to scale it to our screen
        ProjectedTriangle.Points[0].X *= 0.5f * static_cast<float>(ScreenWidth());
        ProjectedTriangle.Points[0].Y *= 0.5f * static_cast<float>(ScreenHeight());
        ProjectedTriangle.Points[1].X *= 0.5f * static_cast<float>(ScreenWidth());
        ProjectedTriangle.Points[1].Y *= 0.5f * static_cast<float>(ScreenHeight());
        ProjectedTriangle.Points[2].X *= 0.5f * static_cast<float>(ScreenWidth());
        ProjectedTriangle.Points[2].Y *= 0.5f * static_cast<float>(ScreenHeight());

        TrianglesToDraw.push_back(ProjectedTriangle);
    }

    // Sort triangles from farthest away from the camera to closest
    std::sort(TrianglesToDraw.begin(), TrianglesToDraw.end(), [](const XGTriangle& Triangle1, const XGTriangle& Triangle2)
    {
        // Get the midpoint in the Z axis of the triangles
        const float Depth1 = (Triangle1.Points[0].Z + Triangle1.Points[1].Z + Triangle1.Points[2].Z) / 3.0f;
        const float Depth2 = (Triangle2.Points[0].Z + Triangle2.Points[1].Z + Triangle2.Points[2].Z) / 3.0f;

        return Depth1 > Depth2;
    });

    // Rasterize the triangles
    for (const XGTriangle& Triangle : TrianglesToDraw)
    {
        FillTriangle(
            static_cast<int32_t>(Triangle.Points[0].X),
            static_cast<int32_t>(Triangle.Points[0].Y),
            static_cast<int32_t>(Triangle.Points[1].X),
            static_cast<int32_t>(Triangle.Points[1].Y),
            static_cast<int32_t>(Triangle.Points[2].X),
            static_cast<int32_t>(Triangle.Points[2].Y),
            Triangle.Color
        );
    }
    
    return true;
}

olc::Pixel XGEngine::CreateGrayscaleColor(const float& Brightness)
{
    return olc::PixelF(Brightness, Brightness, Brightness, 1.0f);
}
