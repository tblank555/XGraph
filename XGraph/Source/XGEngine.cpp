// XGEngine.cpp
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#include "XGEngine.h"

#include <algorithm>
#include <string>
#include "XGTriangle.h"

XGEngine::XGEngine(const std::string& MeshFilePath, const std::string& TextureFilePath, bool InvertUVMapping)
{
    if (!MeshFilePath.empty())
    {
        const bool HasTexture = !TextureFilePath.empty();
        MeshToRender.LoadFromObjectFile(MeshFilePath, HasTexture, InvertUVMapping);
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

    if (!TextureFilePath.empty())
    {
        TextureToRender = new olc::Sprite();
        if (TextureToRender->LoadFromFile(TextureFilePath) != olc::OK)
        {
            std::cout << "ERROR: Failed to load texture at path: " << TextureFilePath << std::endl;
        }
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

    // Initialize the depth buffer
    const unsigned long long BufferSize = static_cast<unsigned long long>(ScreenWidth()) * static_cast<unsigned long long>(ScreenHeight());
    DepthBuffer = new float[BufferSize];
    
    return true;
}

bool XGEngine::OnUserUpdate(float fElapsedTime)
{
    ProcessKeyboardInput(fElapsedTime);

    // Move the mesh out in front of the camera
    const XGMatrix4x4 WorldMatrix = XGMatrix4x4::Translation({ 0.0f, 0.0f, 5.0f });
    
    // Calculate the camera's look direction based on the current yaw value
    const XGVector3D DefaultCameraLookDirection = { 0.0f, 0.0f, 1.0f };
    CameraLookDirection = XGMatrix4x4::RotationY(CameraYaw) * DefaultCameraLookDirection;

    // Calculate the view matrix based on the camera's position and orientation
    const XGVector3D CameraUp = { 0.0f, 1.0f, 0.0f };
    const XGVector3D CameraTarget = CameraPosition + CameraLookDirection;
    XGMatrix4x4 ViewMatrix = XGMatrix4x4::PointAt(CameraPosition, CameraTarget, CameraUp);
    ViewMatrix = ViewMatrix.QuickInverse();

    std::vector<XGTriangle> TrianglesToDraw;
    TransformAndProjectTriangles(
        MeshToRender,
        WorldMatrix,
        ViewMatrix,
        TrianglesToDraw
    );

    // Sort the triangles from farthest away from the camera to closest if we're in FlatShaded mode.
    // The depth buffer handles draw order issues in textured mode, and it doesn't matter in wireframe mode.
    if (RenderMode == FlatShaded)
    {
        std::sort(TrianglesToDraw.begin(), TrianglesToDraw.end(), [](const XGTriangle& Triangle1, const XGTriangle& Triangle2)
        {
            // Get the midpoint in the Z axis of the triangles
            const float Depth1 = (Triangle1.Points[0].Z + Triangle1.Points[1].Z + Triangle1.Points[2].Z) / 3.0f;
            const float Depth2 = (Triangle2.Points[0].Z + Triangle2.Points[1].Z + Triangle2.Points[2].Z) / 3.0f;
    
            return Depth1 < Depth2;
        });
    }

    // Clear screen to black
    FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

    // Clear the depth buffer
    for (int i = 0; i < ScreenWidth() * ScreenHeight(); ++i)
    {
        DepthBuffer[i] = 0.0f;
    }

    // Clip and rasterize the triangles
    ClipAndRasterizeTriangles(TrianglesToDraw);
    
    return true;
}

olc::Pixel XGEngine::CreateGrayscaleColor(const float& Brightness)
{
    return olc::PixelF(Brightness, Brightness, Brightness, 1.0f);
}

void XGEngine::ProcessKeyboardInput(const float& SecondsElapsedThisFrame)
{
    if (GetKey(olc::UP).bHeld)
    {
        CameraPosition.Y += 8.0f * SecondsElapsedThisFrame;
    }

    if (GetKey(olc::DOWN).bHeld)
    {
        CameraPosition.Y -= 8.0f * SecondsElapsedThisFrame;
    }

    if (GetKey(olc::LEFT).bHeld)
    {
        CameraPosition.X -= 8.0f * SecondsElapsedThisFrame;
    }

    if (GetKey(olc::RIGHT).bHeld)
    {
        CameraPosition.X += 8.0f * SecondsElapsedThisFrame;
    }

    const XGVector3D CameraForwardMovement = CameraLookDirection * 8.0f * SecondsElapsedThisFrame;

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
        CameraYaw -= 2.0f * SecondsElapsedThisFrame;
    }

    if (GetKey(olc::D).bHeld)
    {
        CameraYaw += 2.0f * SecondsElapsedThisFrame;
    }
}

void XGEngine::TransformAndProjectTriangles(
        const XGMesh& Mesh,
        const XGMatrix4x4& WorldMatrix,
        const XGMatrix4x4& ViewMatrix,
        std::vector<XGTriangle>& OutProjectedTriangles)
{
    OutProjectedTriangles.clear();
    
    for (const XGTriangle& Triangle : Mesh.Triangles)
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

            // Update the texture coordinates to respect projected depth
            
            ProjectedTriangle.TextureCoordinates[0].U /= ProjectedTriangle.Points[0].W;
            ProjectedTriangle.TextureCoordinates[1].U /= ProjectedTriangle.Points[1].W;
            ProjectedTriangle.TextureCoordinates[2].U /= ProjectedTriangle.Points[2].W;
            
            ProjectedTriangle.TextureCoordinates[0].V /= ProjectedTriangle.Points[0].W;
            ProjectedTriangle.TextureCoordinates[1].V /= ProjectedTriangle.Points[1].W;
            ProjectedTriangle.TextureCoordinates[2].V /= ProjectedTriangle.Points[2].W;

            // Set the W component of the texture coordinates to the inverse of the W component of the vertices
            ProjectedTriangle.TextureCoordinates[0].W = 1.0f / ProjectedTriangle.Points[0].W;
            ProjectedTriangle.TextureCoordinates[1].W = 1.0f / ProjectedTriangle.Points[1].W;
            ProjectedTriangle.TextureCoordinates[2].W = 1.0f / ProjectedTriangle.Points[2].W;

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

            OutProjectedTriangles.push_back(ProjectedTriangle);
        }
    }
}

void XGEngine::ClipAndRasterizeTriangles(const std::vector<XGTriangle>& Triangles)
{
    for (const XGTriangle& Triangle : Triangles)
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
                default:
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
            if (RenderMode == FlatShaded)
            {
                FillTriangle(
                    static_cast<int32_t>(TriangleToRasterize.Points[0].X),
                    static_cast<int32_t>(TriangleToRasterize.Points[0].Y),
                    static_cast<int32_t>(TriangleToRasterize.Points[1].X),
                    static_cast<int32_t>(TriangleToRasterize.Points[1].Y),
                    static_cast<int32_t>(TriangleToRasterize.Points[2].X),
                    static_cast<int32_t>(TriangleToRasterize.Points[2].Y),
                    Triangle.Color
                );
            }
            else if (RenderMode == Textured)
            {
                DrawTexturedTriangle(TriangleToRasterize, *TextureToRender);
            }

            if (RenderMode == Wireframe || ShouldDrawWireframe)
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
        }
    }
}

void XGEngine::DrawTexturedTriangle(const XGTriangle& Triangle, const olc::Sprite& TextureSprite)
{
    // Create local copies of the data so we can swap them to make our calculations easier
    // Since we're drawing pixels, we use integer X and Y coordinates
    int X1 = static_cast<int>(Triangle.Points[0].X);
    int X2 = static_cast<int>(Triangle.Points[1].X);
    int X3 = static_cast<int>(Triangle.Points[2].X);
    int Y1 = static_cast<int>(Triangle.Points[0].Y);
    int Y2 = static_cast<int>(Triangle.Points[1].Y);
    int Y3 = static_cast<int>(Triangle.Points[2].Y);
    float U1 = Triangle.TextureCoordinates[0].U;
    float U2 = Triangle.TextureCoordinates[1].U;
    float U3 = Triangle.TextureCoordinates[2].U;
    float V1 = Triangle.TextureCoordinates[0].V;
    float V2 = Triangle.TextureCoordinates[1].V;
    float V3 = Triangle.TextureCoordinates[2].V;
    float W1 = Triangle.TextureCoordinates[0].W;
    float W2 = Triangle.TextureCoordinates[1].W;
    float W3 = Triangle.TextureCoordinates[2].W;

    // Sort the variables based on the Y position of each point of the triangle
    
    if (Y2 < Y1)
    {
        std::swap(X1, X2);
        std::swap(Y1, Y2);
        std::swap(U1, U2);
        std::swap(V1, V2);
        std::swap(W1, W2);
    }

    if (Y3 < Y1)
    {
        std::swap(X1, X3);
        std::swap(Y1, Y3);
        std::swap(U1, U3);
        std::swap(V1, V3);
        std::swap(W1, W3);
    }

    if (Y3 < Y2)
    {
        std::swap(X2, X3);
        std::swap(Y2, Y3);
        std::swap(U2, U3);
        std::swap(V2, V3);
        std::swap(W2, W3);
    }

    // Calculate the slope of Line A
    int LineADeltaX = X2 - X1;
    int LineADeltaY = Y2 - Y1;
    float LineADeltaU = U2 - U1;
    float LineADeltaV = V2 - V1;
    float LineADeltaW = W2 - W1;

    // Calculate the slope of Line B
    int LineBDeltaX = X3 - X1;
    int LineBDeltaY = Y3 - Y1;
    float LineBDeltaU = U3 - U1;
    float LineBDeltaV = V3 - V1;
    float LineBDeltaW = W3 - W1;

    float LineAStepX = 0.0f;
    float LineAStepU = 0.0f;
    float LineAStepV = 0.0f;
    float LineAStepW = 0.0f;
    
    float LineBStepX = 0.0f;
    float LineBStepU = 0.0f;
    float LineBStepV = 0.0f;
    float LineBStepW = 0.0f;

    if (LineADeltaY > 0)
    {
        LineAStepX = static_cast<float>(LineADeltaX) / static_cast<float>(std::abs(LineADeltaY));
        LineAStepU = LineADeltaU / static_cast<float>(std::abs(LineADeltaY));
        LineAStepV = LineADeltaV / static_cast<float>(std::abs(LineADeltaY));
        LineAStepW = LineADeltaW / static_cast<float>(std::abs(LineADeltaY));
    }

    if (LineBDeltaY > 0)
    {
        LineBStepX = static_cast<float>(LineBDeltaX) / static_cast<float>(std::abs(LineBDeltaY));
        LineBStepU = LineBDeltaU / static_cast<float>(std::abs(LineBDeltaY));
        LineBStepV = LineBDeltaV / static_cast<float>(std::abs(LineBDeltaY));
        LineBStepW = LineBDeltaW / static_cast<float>(std::abs(LineBDeltaY));
    }
    
    float TexU;
    float TexV;
    float TexW;

    // Draw the top half of the triangle as long as Line A isn't flat
    if (LineADeltaY > 0)
    {
        // For each row in the top half of the triangle,
        for (int Y = Y1; Y <= Y2; Y++)
        {
            const float CurrentDeltaY = static_cast<float>(Y - Y1);
            
            int LineAX = X1 + static_cast<int>(CurrentDeltaY * LineAStepX);
            int LineBX = X1 + static_cast<int>(CurrentDeltaY * LineBStepX);

            float UStart = U1 + CurrentDeltaY * LineAStepU;
            float VStart = V1 + CurrentDeltaY * LineAStepV;
            float WStart = W1 + CurrentDeltaY * LineAStepW;
            
            float UEnd = U1 + CurrentDeltaY * LineBStepU;
            float VEnd = V1 + CurrentDeltaY * LineBStepV;
            float WEnd = W1 + CurrentDeltaY * LineBStepW;

            // Sort our variables to make sure we're always going from a smaller X value to a larger one
            if (LineAX > LineBX)
            {
                std::swap(LineAX, LineBX);
                std::swap(UStart, UEnd);
                std::swap(VStart, VEnd);
                std::swap(WStart, WEnd);
            }

            float TStep = 1.0f / static_cast<float>(LineBX - LineAX);
            float T = 0.0f;

            for (int X = LineAX; X < LineBX; X++)
            {
                TexU = (1.0f - T) * UStart + T * UEnd;
                TexV = (1.0f - T) * VStart + T * VEnd;
                TexW = (1.0f - T) * WStart + T * WEnd;

                // If the depth buffer has pixels that are closer to the screen than this one, don't draw it
                if (TexW < DepthBuffer[Y * ScreenWidth() + X])
                {
                    const olc::Pixel SampledColor = TextureSprite.Sample(TexU / TexW, TexV / TexW);
                    Draw(X, Y, SampledColor);

                    DepthBuffer[Y * ScreenWidth() + X] = TexW;
                }
                
                T += TStep;
            }
        }
    }

    // Update our "d" values for the bottom half of the triangle
    LineADeltaY = Y3 - Y2;
    LineADeltaX = X3 - X2;
    LineADeltaV = V3 - V2;
    LineADeltaU = U3 - U2;
    LineADeltaW = W3 - W2;

    // Update the step values only for Line A. Since it's a triangle, Line B will not have changed for the bottom half.
    LineAStepU = 0.0f;
    LineAStepV = 0.0f;
    LineAStepW = 0.0f;

    if (LineADeltaY > 0)
    {
        LineAStepX = static_cast<float>(LineADeltaX) / static_cast<float>(std::abs(LineADeltaY));
        LineAStepU = LineADeltaU / static_cast<float>(std::abs(LineADeltaY));
        LineAStepV = LineADeltaV / static_cast<float>(std::abs(LineADeltaY));
        LineAStepW = LineADeltaW / static_cast<float>(std::abs(LineADeltaY));
    }

    // Draw the bottom half of the triangle as long as Line A isn't flat
    if (LineADeltaY > 0)
    {
        // For each row in the bottom half of the triangle,
        for (int Y = Y2; Y <= Y3; Y++)
        {
            const auto CurrentLineADeltaY = static_cast<float>(Y - Y2);
            const auto CurrentLineBDeltaY = static_cast<float>(Y - Y1);
            
            int LineAX = X2 + static_cast<int>(CurrentLineADeltaY * LineAStepX);
            int LineBX = X1 + static_cast<int>(CurrentLineBDeltaY * LineBStepX);

            float UStart = U2 + CurrentLineADeltaY * LineAStepU;
            float VStart = V2 + CurrentLineADeltaY * LineAStepV;
            float WStart = W2 + CurrentLineADeltaY * LineAStepW;
        
            float UEnd = U1 + CurrentLineBDeltaY * LineBStepU;
            float VEnd = V1 + CurrentLineBDeltaY * LineBStepV;
            float WEnd = W1 + CurrentLineBDeltaY * LineBStepW;

            // Sort our variables to make sure we're always going from a smaller X value to a larger one
            if (LineAX > LineBX)
            {
                std::swap(LineAX, LineBX);
                std::swap(UStart, UEnd);
                std::swap(VStart, VEnd);
                std::swap(WStart, WEnd);
            }

            float TStep = 1.0f / static_cast<float>(LineBX - LineAX);
            float T = 0.0f;

            for (int X = LineAX; X < LineBX; X++)
            {
                TexU = (1.0f - T) * UStart + T * UEnd;
                TexV = (1.0f - T) * VStart + T * VEnd;
                TexW = (1.0f - T) * WStart + T * WEnd;

                // If the depth buffer has pixels that are closer to the screen than this one, don't draw it
                if (TexW < DepthBuffer[Y * ScreenWidth() + X])
                {
                    const olc::Pixel SampledColor = TextureSprite.Sample(TexU / TexW, TexV / TexW);
                    Draw(X, Y, SampledColor);

                    DepthBuffer[Y * ScreenWidth() + X] = TexW;
                }
            
                T += TStep;
            }
        }
    }
}
