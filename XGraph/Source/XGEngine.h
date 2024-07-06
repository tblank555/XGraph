// XGEngine.h
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#pragma once

#include "../ThirdParty/olcPixelGameEngine.h"
#include "XGMatrix4x4.h"
#include "XGMesh.h"
#include "XGVector3D.h"

/**
 * \brief The different types of rendering the engine is capable of performing
 */
enum XGRenderMode
{
    Wireframe,
    FlatShaded,
    Textured
};

class XGEngine : public olc::PixelGameEngine
{
public:
    XGEngine() : XGEngine("") {}
    XGEngine(const std::string& MeshFilePath) : XGEngine(MeshFilePath, "") {}
    XGEngine(const std::string& MeshFilePath, const std::string& TextureFilePath, bool InvertUVMapping = false);

    /**
     * \brief The type of rendering the engine should perform
     */
    XGRenderMode RenderMode = Textured;

    /**
     * \brief Whether wireframes should be drawn, even when in a RenderMode other than Wireframe
     */
    bool ShouldDrawWireframe = false;
    
    /**
     * \brief The distance the near clip plane should be from the camera
     */
    float NearClipPlane = 0.1f;

    /**
     * \brief The direction of the global directional light
     */
    XGVector3D LightDirection;

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

private:
    /**
     * \brief The mesh that will be rendered
     */
    XGMesh MeshToRender;

    /**
     * \brief The texture to apply to all triangles of the mesh
     */
    olc::Sprite* TextureToRender;

    /**
     * \brief Perspective projection matrix
     */
    XGMatrix4x4 ProjectionMatrix;

    /**
     * \brief The position of the camera in world space
     * \details The camera is assumed to always be aligned with the Z axis for now
     */
    XGVector3D CameraPosition;

    /**
     * \brief The yaw angle of the camera
     */
    float CameraYaw = 0.0f;

    /**
     * \brief A unit vector that describes the direction the camera is looking
     * \details Z is the forward vector
     */
    XGVector3D CameraLookDirection = { 0.0f, 0.0f, 1.0f };

    /**
     * \brief The amount of rotation to apply to the mesh this frame
     */
    float RotationAngle = 0.0f;

    /**
     * \brief The depth value (W) of the texture being drawn at each pixel on the screen
     */
    float* DepthBuffer = nullptr;

    /**
     * \brief Create a grayscale color
     * \param Brightness A value from 0 to 1 that indicates how bright the color should be. 0 = black, 1 = white.
     * \return The color
     */
    static olc::Pixel CreateGrayscaleColor(const float& Brightness);

    /**
     * \brief Draws the given triangle on the screen with the given texture
     * \param Triangle The triangle to draw (in screen space)
     * \param TextureSprite The texture to apply to the triangle
     */
    void DrawTexturedTriangle(const XGTriangle& Triangle, const olc::Sprite& TextureSprite);
};
