// XGEngine.h
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
    XGEngine() {}

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

private:
    /**
     * \brief Unit cube at origin
     */
    XGMesh MeshCube;

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
     * \brief The direction of the global directional light
     */
    XGVector3D LightDirection;

    /**
     * \brief The amount of rotation we want to apply to the cube this frame
     */
    float CubeRotationAngle;

    /**
     * \brief Create a grayscale color
     * \param Brightness A value from 0 to 1 that indicates how bright the color should be. 0 = black, 1 = white
     * \return The color
     */
    static olc::Pixel CreateGrayscaleColor(const float& Brightness)
    {
        return olc::PixelF(Brightness, Brightness, Brightness, 1.0f);
    }
};
