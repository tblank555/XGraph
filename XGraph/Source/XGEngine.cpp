// XGEngine.cpp
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#include "XGEngine.h"

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
    const float InverseTanHalfFovRadians = 1.0f / tanf(FieldOfViewDegrees / 2.0f * 3.14159f / 180.0f);

    ProjectionMatrix.Values[0][0] = AspectRatio / InverseTanHalfFovRadians;
    ProjectionMatrix.Values[1][1] = InverseTanHalfFovRadians;
    ProjectionMatrix.Values[2][2] = FarClipPlane / (FarClipPlane - NearClipPlane);
    ProjectionMatrix.Values[3][2] = (-FarClipPlane * NearClipPlane) / (FarClipPlane - NearClipPlane);
    ProjectionMatrix.Values[2][3] = 1.0f;
    ProjectionMatrix.Values[3][3] = 0.0f;

    // Set and normalize the global directional light
    LightDirection = XGVector3D(0.0f, 0.0f, -1.0f);
    LightDirection.Normalize();
    
    return true;
}

bool XGEngine::OnUserUpdate(float fElapsedTime)
{
    // Clear screen to black
    FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

    // Rotate cube
    XGMatrix4x4 RotateAroundXAxis;
    XGMatrix4x4 RotateAroundZAxis;
    CubeRotationAngle += 1.0f * fElapsedTime;

    // Define Z-axis rotation
    RotateAroundZAxis.Values[0][0] = cosf(CubeRotationAngle);
    RotateAroundZAxis.Values[0][1] = sinf(CubeRotationAngle);
    RotateAroundZAxis.Values[1][0] = -sinf(CubeRotationAngle);
    RotateAroundZAxis.Values[1][1] = cosf(CubeRotationAngle);
    RotateAroundZAxis.Values[2][2] = 1.0f;
    RotateAroundZAxis.Values[3][3] = 1.0f;

    // Define X-axis rotation (rotate at half the speed of the Z axis)
    const float XRotation = CubeRotationAngle * 0.5f;
    RotateAroundXAxis.Values[0][0] = 1.0f;
    RotateAroundXAxis.Values[1][1] = cosf(XRotation);
    RotateAroundXAxis.Values[1][2] = sinf(XRotation);
    RotateAroundXAxis.Values[2][1] = -sinf(XRotation);
    RotateAroundXAxis.Values[2][2] = cosf(XRotation);
    RotateAroundXAxis.Values[3][3] = 1.0f;

    // Draw triangles
    for (const XGTriangle& Triangle : MeshToRender.Triangles)
    {
        // Rotate the triangle around the Z axis
        XGTriangle TriangleRotatedAroundZ;
        Triangle.Points[0].MultiplyByMatrix(RotateAroundZAxis, TriangleRotatedAroundZ.Points[0]);
        Triangle.Points[1].MultiplyByMatrix(RotateAroundZAxis, TriangleRotatedAroundZ.Points[1]);
        Triangle.Points[2].MultiplyByMatrix(RotateAroundZAxis, TriangleRotatedAroundZ.Points[2]);

        // Rotate the triangle around the X axis
        XGTriangle TriangleRotatedAroundX;
        TriangleRotatedAroundZ.Points[0].MultiplyByMatrix(RotateAroundXAxis, TriangleRotatedAroundX.Points[0]);
        TriangleRotatedAroundZ.Points[1].MultiplyByMatrix(RotateAroundXAxis, TriangleRotatedAroundX.Points[1]);
        TriangleRotatedAroundZ.Points[2].MultiplyByMatrix(RotateAroundXAxis, TriangleRotatedAroundX.Points[2]);
        
        XGTriangle TranslatedTriangle = TriangleRotatedAroundX;

        // Translate the triangle along the Z axis so we can see it
        TranslatedTriangle.Points[0].Z = TriangleRotatedAroundX.Points[0].Z + 8.0f;
        TranslatedTriangle.Points[1].Z = TriangleRotatedAroundX.Points[1].Z + 8.0f;
        TranslatedTriangle.Points[2].Z = TriangleRotatedAroundX.Points[2].Z + 8.0f;

        // Calculate the triangle's normal
        
        const XGVector3D Side1 = XGVector3D(
            TranslatedTriangle.Points[1].X - TranslatedTriangle.Points[0].X,
            TranslatedTriangle.Points[1].Y - TranslatedTriangle.Points[0].Y,
            TranslatedTriangle.Points[1].Z - TranslatedTriangle.Points[0].Z
        );

        const XGVector3D Side2 = XGVector3D(
            TranslatedTriangle.Points[2].X - TranslatedTriangle.Points[0].X,
            TranslatedTriangle.Points[2].Y - TranslatedTriangle.Points[0].Y,
            TranslatedTriangle.Points[2].Z - TranslatedTriangle.Points[0].Z
        );

        XGVector3D Normal = XGVector3D(
            Side1.Y * Side2.Z - Side1.Z * Side2.Y,
            Side1.Z * Side2.X - Side1.X * Side2.Z,
            Side1.X * Side2.Y - Side1.Y * Side2.X
        );

        Normal.Normalize();

        // Find the vector from the camera to the triangle
        // We can use any point on the triangle, because we're using this for the dot product with the normal,
        // and the normal will be the same a any point on the triangle
        const XGVector3D CameraToTriangle = XGVector3D(
            TranslatedTriangle.Points[0].X - CameraPosition.X,
            TranslatedTriangle.Points[0].Y - CameraPosition.Y,
            TranslatedTriangle.Points[0].Z - CameraPosition.Z
        );

        const float DotProduct = CameraToTriangle.DotProduct(Normal);

        // If the dot product of the vector from the camera to the triangle and the normal is greater than 0,
        // this triangle is facing away from the camera, and therefore shouldn't be rendered
        if (DotProduct >= 0.0f)
        {
            continue;
        }

        const float Luminance = LightDirection.DotProduct(Normal);
        TranslatedTriangle.Color = CreateGrayscaleColor(Luminance);
        
        // Project the triangle from world space to an intermediate screen space
        XGTriangle ProjectedTriangle;
        TranslatedTriangle.Points[0].MultiplyByMatrix(ProjectionMatrix, ProjectedTriangle.Points[0]);
        TranslatedTriangle.Points[1].MultiplyByMatrix(ProjectionMatrix, ProjectedTriangle.Points[1]);
        TranslatedTriangle.Points[2].MultiplyByMatrix(ProjectionMatrix, ProjectedTriangle.Points[2]);
        ProjectedTriangle.Color = TranslatedTriangle.Color;

        // Scale triangle into view
        // Shift unit cube from -1 to 1 coordinates to 0 to 2
        ProjectedTriangle.Points[0].X += 1.0f;
        ProjectedTriangle.Points[0].Y += 1.0f;
        ProjectedTriangle.Points[1].X += 1.0f;
        ProjectedTriangle.Points[1].Y += 1.0f;
        ProjectedTriangle.Points[2].X += 1.0f;
        ProjectedTriangle.Points[2].Y += 1.0f;

        // Divide each point by half to get to 0 to 1 range, and multiply by screen height or width to scale it to our screen
        ProjectedTriangle.Points[0].X *= 0.5f * static_cast<float>(ScreenWidth());
        ProjectedTriangle.Points[0].Y *= 0.5f * static_cast<float>(ScreenHeight());
        ProjectedTriangle.Points[1].X *= 0.5f * static_cast<float>(ScreenWidth());
        ProjectedTriangle.Points[1].Y *= 0.5f * static_cast<float>(ScreenHeight());
        ProjectedTriangle.Points[2].X *= 0.5f * static_cast<float>(ScreenWidth());
        ProjectedTriangle.Points[2].Y *= 0.5f * static_cast<float>(ScreenHeight());

        FillTriangle(
            static_cast<int32_t>(ProjectedTriangle.Points[0].X),
            static_cast<int32_t>(ProjectedTriangle.Points[0].Y),
            static_cast<int32_t>(ProjectedTriangle.Points[1].X),
            static_cast<int32_t>(ProjectedTriangle.Points[1].Y),
            static_cast<int32_t>(ProjectedTriangle.Points[2].X),
            static_cast<int32_t>(ProjectedTriangle.Points[2].Y),
            ProjectedTriangle.Color
        );
    }
    
    return true;
}