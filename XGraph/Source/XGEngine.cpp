// XGEngine.cpp
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#include "XGEngine.h"

#include <algorithm>
#include <string>
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
                { 0.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f, 0.0f },
                { 1.0f, 1.0f, 0.0f },
                { 0.0f, 1.0f },
                { 0.0f, 0.0f },
                { 1.0f, 0.0f }
            ),
            XGTriangle(
                { 0.0f, 0.0f, 0.0f },
                { 1.0f, 1.0f, 0.0f },
                { 1.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f },
                { 1.0f, 0.0f },
                { 1.0f, 1.0f }
            ),
            // East face
            XGTriangle(
                { 1.0f, 0.0f, 0.0f },
                { 1.0f, 1.0f, 0.0f },
                { 1.0f, 1.0f, 1.0f },
                { 0.0f, 1.0f },
                { 0.0f, 0.0f },
                { 1.0f, 0.0f }
            ),
            XGTriangle(
                { 1.0f, 0.0f, 0.0f },
                { 1.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f },
                { 1.0f, 0.0f },
                { 1.0f, 1.0f }
            ),
            // North face
            XGTriangle(
                { 1.0f, 0.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f },
                { 0.0f, 1.0f, 1.0f },
                { 0.0f, 1.0f },
                { 0.0f, 0.0f },
                { 1.0f, 0.0f }
            ),
            XGTriangle(
                { 1.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f, 1.0f },
                { 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f },
                { 1.0f, 0.0f },
                { 1.0f, 1.0f }
            ),
            // West face
            XGTriangle(
                { 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f, 1.0f },
                { 0.0f, 1.0f, 0.0f },
                { 0.0f, 1.0f },
                { 0.0f, 0.0f },
                { 1.0f, 0.0f }
            ),
            XGTriangle(
                { 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f },
                { 1.0f, 0.0f },
                { 1.0f, 1.0f }
            ),
            // Top face
            XGTriangle(
                { 0.0f, 1.0f, 0.0f },
                { 0.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f },
                { 0.0f, 1.0f },
                { 0.0f, 0.0f },
                { 1.0f, 0.0f }
            ),
            XGTriangle(
                { 0.0f, 1.0f, 0.0f },
                { 1.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 0.0f },
                { 0.0f, 1.0f },
                { 1.0f, 0.0f },
                { 1.0f, 1.0f }
            ),
            // Bottom face
            XGTriangle(
                { 1.0f, 0.0f, 1.0f },
                { 0.0f, 0.0f, 1.0f },
                { 0.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f },
                { 0.0f, 0.0f },
                { 1.0f, 0.0f }
            ),
            XGTriangle(
                { 1.0f, 0.0f, 1.0f },
                { 0.0f, 0.0f, 0.0f },
                { 1.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f },
                { 1.0f, 0.0f },
                { 1.0f, 1.0f }
            )
        };
    }
}

bool XGEngine::OnUserCreate()
{
    // Create perspective projection matrix
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
    LightDirection = { 0.0f, 1.0f, -1.0f };
    LightDirection.Normalize();

    // Load the global texture
    TextureToRender = new olc::Sprite();
    if (TextureToRender->LoadFromFile("Resources/BlueCircle.bmp") != olc::OK)
    {
        std::cout << "ERROR: Failed to load texture" << std::endl;
    }
    
    return true;
}

bool XGEngine::OnUserUpdate(float fElapsedTime)
{
    // Clear screen to black
    FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

    // Process keyboard input
    
    if (GetKey(olc::UP).bHeld)
    {
        CameraPosition.Y += 8.0f * fElapsedTime;
    }

    if (GetKey(olc::DOWN).bHeld)
    {
        CameraPosition.Y -= 8.0f * fElapsedTime;
    }

    if (GetKey(olc::LEFT).bHeld)
    {
        CameraPosition.X -= 8.0f * fElapsedTime;
    }

    if (GetKey(olc::RIGHT).bHeld)
    {
        CameraPosition.X += 8.0f * fElapsedTime;
    }

    const XGVector3D CameraForwardMovement = CameraLookDirection * 8.0f * fElapsedTime;

    if (GetKey(olc::W).bHeld)
    {
        CameraPosition += CameraForwardMovement;
    }

    if (GetKey(olc::S).bHeld)
    {
        CameraPosition -= CameraForwardMovement;
    }

    if (GetKey(olc::A).bHeld)
    {
        CameraYaw -= 2.0f * fElapsedTime;
    }

    if (GetKey(olc::D).bHeld)
    {
        CameraYaw += 2.0f * fElapsedTime;
    }

    std::cout << "CameraPosition = (" + std::to_string(CameraPosition.X) + ", " + std::to_string(CameraPosition.Y) + ", " + std::to_string(CameraPosition.Z) + ")" << std::endl;

    // Uncomment to enable rotation
    //RotationAngle += 1.0f * fElapsedTime;
    const XGMatrix4x4 RotateAroundZAxis = XGMatrix4x4::RotationZ(RotationAngle);
    const XGMatrix4x4 RotateAroundXAxis = XGMatrix4x4::RotationX(RotationAngle * 0.5f);

    const XGMatrix4x4 TranslateAlongZAxis = XGMatrix4x4::Translation({ 0.0f, 0.0f, 5.0f });

    const XGMatrix4x4 WorldMatrix = RotateAroundZAxis * RotateAroundXAxis * TranslateAlongZAxis;
    
    const XGVector3D CameraUp = { 0.0f, 1.0f, 0.0f };

    const XGVector3D DefaultCameraLookDirection = { 0.0f, 0.0f, 1.0f };
    CameraLookDirection = XGMatrix4x4::RotationY(CameraYaw) * DefaultCameraLookDirection;
    const XGVector3D CameraTarget = CameraPosition + CameraLookDirection;

    XGMatrix4x4 ViewMatrix = XGMatrix4x4::PointAt(CameraPosition, CameraTarget, CameraUp);
    ViewMatrix = ViewMatrix.QuickInverse();

    std::vector<XGTriangle> TrianglesToDraw;

    // Transform and project triangles
    for (const XGTriangle& Triangle : MeshToRender.Triangles)
    {
        XGTriangle TransformedTriangle = {
            WorldMatrix * Triangle.Points[0],
            WorldMatrix * Triangle.Points[1],
            WorldMatrix * Triangle.Points[2],
            Triangle.TextureCoordinates[0],
            Triangle.TextureCoordinates[1],
            Triangle.TextureCoordinates[2],
        };
        
        XGVector3D Normal = TransformedTriangle.GetNormal();

        // Find the vector from the camera to the triangle
        // We can use any point on the triangle, because we're using this for the dot product with the normal,
        // and the normal will be the same a any point on the triangle
        const XGVector3D TriangleToCamera = TransformedTriangle.Points[0] - CameraPosition;

        const float DotProduct = TriangleToCamera.DotProduct(Normal);

        // If the dot product of the vector from the camera to the triangle and the normal is greater than 0,
        // this triangle is facing away from the camera, and therefore shouldn't be rendered
        if (DotProduct >= 0.0f)
        {
            continue;
        }

        // Project the triangle from world space to view space
        const XGTriangle ViewedTriangle = {
            ViewMatrix * TransformedTriangle.Points[0],
            ViewMatrix * TransformedTriangle.Points[1],
            ViewMatrix * TransformedTriangle.Points[2],
            TransformedTriangle.TextureCoordinates[0],
            TransformedTriangle.TextureCoordinates[1],
            TransformedTriangle.TextureCoordinates[2],
        };

        // Clip the triangle in view space against the near clip plane
        // In world space, the near clip plane will always have a normal pointing along the Z axis
        int ClippedTriangleCount = -1;
        XGTriangle ClippedTriangles[2];
        ClippedTriangleCount = ViewedTriangle.ClipAgainstPlane(
            { 0.0f, 0.0f, NearClipPlane },
            { 0.0f, 0.0f, 1.0f },
            ClippedTriangles[0],
            ClippedTriangles[1]
        );

        for (int i = 0; i < ClippedTriangleCount; ++i)
        {
            // Project the triangle from view space to an intermediate screen space
            XGTriangle ProjectedTriangle = {
                ProjectionMatrix * ClippedTriangles[i].Points[0],
                ProjectionMatrix * ClippedTriangles[i].Points[1],
                ProjectionMatrix * ClippedTriangles[i].Points[2],
                ClippedTriangles[i].TextureCoordinates[0],
                ClippedTriangles[i].TextureCoordinates[1],
                ClippedTriangles[i].TextureCoordinates[2],
            };

            // Normalize into Cartesian space
            ProjectedTriangle.Points[0] /= ProjectedTriangle.Points[0].W;
            ProjectedTriangle.Points[1] /= ProjectedTriangle.Points[1].W;
            ProjectedTriangle.Points[2] /= ProjectedTriangle.Points[2].W;

            // Calculate the color of the triangle based on its normal (in world space)
            const float Luminance = std::max(0.1f, LightDirection.DotProduct(Normal));
            ProjectedTriangle.Color = CreateGrayscaleColor(Luminance);

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
    }

    // Sort triangles from farthest away from the camera to closest
    std::sort(TrianglesToDraw.begin(), TrianglesToDraw.end(), [](const XGTriangle& Triangle1, const XGTriangle& Triangle2)
    {
        // Get the midpoint in the Z axis of the triangles
        const float Depth1 = (Triangle1.Points[0].Z + Triangle1.Points[1].Z + Triangle1.Points[2].Z) / 3.0f;
        const float Depth2 = (Triangle2.Points[0].Z + Triangle2.Points[1].Z + Triangle2.Points[2].Z) / 3.0f;

        return Depth1 < Depth2;
    });

    // Clip and rasterize the triangles
    for (const XGTriangle& Triangle : TrianglesToDraw)
    {
        std::list<XGTriangle> TriangleList;
        
        TriangleList.push_back(Triangle);
        unsigned long long NumNewTriangles = 1;

        for (int PlaneIndex = 0; PlaneIndex < 4; ++PlaneIndex)
        {
            int NumTrianglesToAdd = 0;
            while (NumNewTriangles > 0)
            {
                const XGTriangle TriangleToClip = TriangleList.front();
                TriangleList.pop_front();
                NumNewTriangles--;

                // Clip triangles against all four screen edges
                XGTriangle NewTrianglesFromClipping[2];
                switch (PlaneIndex)
                {
                case 0:
                    NumTrianglesToAdd = TriangleToClip.ClipAgainstPlane(
                        { 0.0f, 0.0f, 0.0f },
                        { 0.0f, 1.0f, 0.0f },
                        NewTrianglesFromClipping[0],
                        NewTrianglesFromClipping[1]
                    );
                    break;
                case 1:
                    NumTrianglesToAdd = TriangleToClip.ClipAgainstPlane(
                        { 0.0f, static_cast<float>(ScreenHeight()) - 1.0f, 0.0f },
                        { 0.0f, -1.0f, 0.0f },
                        NewTrianglesFromClipping[0],
                        NewTrianglesFromClipping[1]
                    );
                    break;
                case 2:
                    NumTrianglesToAdd = TriangleToClip.ClipAgainstPlane(
                        { 0.0f, 0.0f, 0.0f },
                        { 1.0f, 0.0f, 0.0f },
                        NewTrianglesFromClipping[0],
                        NewTrianglesFromClipping[1]
                    );
                    break;
                case 3:
                    NumTrianglesToAdd = TriangleToClip.ClipAgainstPlane(
                        { static_cast<float>(ScreenWidth()) - 1.0f, 0.0f, 0.0f },
                        { -1.0f, 0.0f, 0.0f },
                        NewTrianglesFromClipping[0],
                        NewTrianglesFromClipping[1]
                    );
                    break;
                }

                for (int i = 0; i < NumTrianglesToAdd; ++i)
                {
                    TriangleList.push_back(NewTrianglesFromClipping[i]);
                }
            }

            NumNewTriangles = TriangleList.size();
        }

        // Rasterize the final list of triangles
        for (const XGTriangle& TriangleToRasterize : TriangleList)
        {
            if (ShouldDrawWireframe)
            {
                DrawTriangle(
                    static_cast<int32_t>(TriangleToRasterize.Points[0].X),
                    static_cast<int32_t>(TriangleToRasterize.Points[0].Y),
                    static_cast<int32_t>(TriangleToRasterize.Points[1].X),
                    static_cast<int32_t>(TriangleToRasterize.Points[1].Y),
                    static_cast<int32_t>(TriangleToRasterize.Points[2].X),
                    static_cast<int32_t>(TriangleToRasterize.Points[2].Y),
                    olc::WHITE
                );
            }
            else
            {
                // FillTriangle(
                //     static_cast<int32_t>(TriangleToRasterize.Points[0].X),
                //     static_cast<int32_t>(TriangleToRasterize.Points[0].Y),
                //     static_cast<int32_t>(TriangleToRasterize.Points[1].X),
                //     static_cast<int32_t>(TriangleToRasterize.Points[1].Y),
                //     static_cast<int32_t>(TriangleToRasterize.Points[2].X),
                //     static_cast<int32_t>(TriangleToRasterize.Points[2].Y),
                //     Triangle.Color
                // );

                DrawTexturedTriangle(TriangleToRasterize, *TextureToRender);
            }
        }
    }
    
    return true;
}

olc::Pixel XGEngine::CreateGrayscaleColor(const float& Brightness)
{
    return olc::PixelF(Brightness, Brightness, Brightness, 1.0f);
}

void XGEngine::DrawTexturedTriangle(const XGTriangle& Triangle, const olc::Sprite& TextureSprite)
{
    // Create a bunch of local variable's so Javidx9's code is easier to compare to
    
    float u1 = Triangle.TextureCoordinates[0].U;
    float u2 = Triangle.TextureCoordinates[1].U;
    float u3 = Triangle.TextureCoordinates[2].U;
    float v1 = Triangle.TextureCoordinates[0].V;
    float v2 = Triangle.TextureCoordinates[1].V;
    float v3 = Triangle.TextureCoordinates[2].V;

    // Since we're drawing pixels, we're going to use integer X and Y coordinates
    int x1 = static_cast<int>(Triangle.Points[0].X);
    int x2 = static_cast<int>(Triangle.Points[1].X);
    int x3 = static_cast<int>(Triangle.Points[2].X);
    int y1 = static_cast<int>(Triangle.Points[0].Y);
    int y2 = static_cast<int>(Triangle.Points[1].Y);
    int y3 = static_cast<int>(Triangle.Points[2].Y);

    // Sort the variables based on the Y position of each point of the triangle
    
    if (y2 < y1)
    {
        std::swap(y1, y2);
        std::swap(x1, x2);
        std::swap(u1, u2);
        std::swap(v1, v2);
    }

    if (y3 < y1)
    {
        std::swap(y1, y3);
        std::swap(x1, x3);
        std::swap(u1, u3);
        std::swap(v1, v3);
    }

    if (y3 < y2)
    {
        std::swap(y2, y3);
        std::swap(x2, x3);
        std::swap(u2, u3);
        std::swap(v2, v3);
    }

    // Calculate the slope of Line A
    int dy1 = y2 - y1;
    int dx1 = x2 - x1;
    float dv1 = v2 - v1;
    float du1 = u2 - u1;

    // Calculate the slope of Line B
    int dy2 = y3 - y1;
    int dx2 = x3 - x1;
    float dv2 = v3 - v1;
    float du2 = u3 - u1;

    float dax_step = 0.0f;
    float dbx_step = 0.0f;
    float du1_step = 0.0f;
    float dv1_step = 0.0f;
    float du2_step = 0.0f;
    float dv2_step = 0.0f;

    if (dy1 > 0)
    {
        dax_step = dx1 / static_cast<float>(std::abs(dy1));
    }

    if (dy2 > 0)
    {
        dbx_step = dx2 / static_cast<float>(std::abs(dy2));
    }

    if (dy1 > 0)
    {
        du1_step = du1 / static_cast<float>(std::abs(dy1));
        dv1_step = dv1 / static_cast<float>(std::abs(dy1));
    }

    if (dy2 > 0)
    {
        du2_step = du2 / static_cast<float>(std::abs(dy2));
        dv2_step = dv2 / static_cast<float>(std::abs(dy2));
    }

    float TexU;
    float TexV;

    // Draw the top half of the triangle as long as Line A isn't flat
    if (dy1 > 0)
    {
        // For each row in the top half of the triangle,
        for (int i = y1; i <= y2; i++)
        {
            int ax = x1 + static_cast<float>(i - y1) * dax_step;
            int bx = x1 + static_cast<float>(i - y1) * dbx_step;

            float UStart = u1 + static_cast<float>(i - y1) * du1_step;
            float VStart = u1 + static_cast<float>(i - y1) * dv1_step;
            
            float UEnd = u1 + static_cast<float>(i - y1) * du2_step;
            float VEnd = v1 + static_cast<float>(i - y1) * dv2_step;

            // Sort ax and bx to make sure we're always going from a smaller X value to a larger one
            if (ax > bx)
            {
                std::swap(ax, bx);
                std::swap(UStart, UEnd);
                std::swap(VStart, VEnd);
            }

            TexU = UStart;
            TexV = VStart;

            float TStep = 1.0f / static_cast<float>(bx - ax);
            float T = 0.0f;

            for (int j = ax; j < bx; j++)
            {
                TexU = (1.0f - T) * UStart + T * UEnd;
                TexV = (1.0f - T) * VStart + T * VEnd;

                const olc::Pixel SampledColor = TextureSprite.Sample(TexU, TexV);
                Draw(j, i, SampledColor);
                
                T += TStep;
            }
        }
    }

    // Update our "d" values for the bottom half of the triangle
    dy1 = y3 - y2;
    dx1 = x3 - x2;
    dv1 = v3 - v2;
    du1 = u3 - u2;

    // Update the step values only for Line A. Since it's a triangle, Line B will not have changed for the bottom half.
    du1_step = 0.0f;
    dv1_step = 0.0f;

    if (dy1 > 0)
    {
        dax_step = dx1 / static_cast<float>(std::abs(dy1));
    }

    if (dy2 > 0)
    {
        dbx_step = dx2 / static_cast<float>(std::abs(dy2));
    }

    if (dy1 > 0)
    {
        du1_step = du1 / static_cast<float>(std::abs(dy1));
        dv1_step = dv1 / static_cast<float>(std::abs(dy1));
    }

    // Draw the bottom half of the triangle as long as Line A isn't flat
    if (dy1 > 0)
    {
        // For each row in the bottom half of the triangle,
        for (int i = y2; i <= y3; i++)
        {
            int ax = x2 + static_cast<float>(i - y2) * dax_step;
            int bx = x1 + static_cast<float>(i - y1) * dbx_step;

            float UStart = u2 + static_cast<float>(i - y2) * du1_step;
            float VStart = u2 + static_cast<float>(i - y2) * dv1_step;
        
            float UEnd = u1 + static_cast<float>(i - y1) * du2_step;
            float VEnd = v1 + static_cast<float>(i - y1) * dv2_step;

            // Sort ax and bx to make sure we're always going from a smaller X value to a larger one
            if (ax > bx)
            {
                std::swap(ax, bx);
                std::swap(UStart, UEnd);
                std::swap(VStart, VEnd);
            }

            TexU = UStart;
            TexV = VStart;

            float TStep = 1.0f / static_cast<float>(bx - ax);
            float T = 0.0f;

            for (int j = ax; j < bx; j++)
            {
                TexU = (1.0f - T) * UStart + T * UEnd;
                TexV = (1.0f - T) * VStart + T * VEnd;

                const olc::Pixel SampledColor = TextureSprite.Sample(TexU, TexV);
                Draw(j, i, SampledColor);
            
                T += TStep;
            }
        }
    }
}
