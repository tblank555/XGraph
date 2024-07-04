﻿// XGEngine.h
// XGraph
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.

#pragma once

#include "../ThirdParty/olcPixelGameEngine.h"
#include "XGMatrix4x4.h"
#include "XGMesh.h"
#include "XGVector3D.h"

class XGEngine : public olc::PixelGameEngine
{
public:
    XGEngine() : XGEngine("") {}
    XGEngine(const std::string& MeshFilePath);

    /**
     * \brief The distance the near clip plane should be from the camera
     */
    float NearClipPlane = 0.1f;

    /**
     * \brief The direction of the global directional light
     */
    XGVector3D LightDirection;

    bool ShouldDrawWireframe = false;

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

private:
    /**
     * \brief The mesh that will be rendered
     */
    XGMesh MeshToRender;

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
     * \brief Create a grayscale color
     * \param Brightness A value from 0 to 1 that indicates how bright the color should be. 0 = black, 1 = white.
     * \return The color
     */
    static olc::Pixel CreateGrayscaleColor(const float& Brightness);
};
