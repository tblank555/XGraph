//
// __   _______                 _
// \ \ / /  __ \               | |
//  \ V /| |  \/_ __ __ _ _ __ | |__
//  /   \| | __| '__/ _` | '_ \| '_ \
// / / \ \ |_\ \ | | (_| | |_) | | | |
// \/   \/\____/_|  \__,_| .__/|_| |_|
//                       | |
//                       |_|
//
// A 3D Graphics Engine by T Blank
// Created following tutorial by One Lone Coder, aka javidx9
//
// Copyright (C) 2024 Travis Blankenship. All rights reserved.
//

// This engine uses a left hand coordinate system with Y being the up vector
// Y   Z
// | /
// |/__ X

#include "XGEngine.h"

int main(int ArgC, char* ArgV[])
{
    XGEngine Demo(
        "Resources/ValleyTerrain.obj",
        "Resources/Grass.bmp",
        true
    );

    Demo.RenderMode = Textured;
    Demo.ShouldDrawWireframe = false;
    
    if (!Demo.Construct(1920, 1080, 1, 1))
    {
        std::cout << "ERROR: Failed to construct engine" << std::endl;
        return 1;
    }

    Demo.Start();
    
    return 0;
}
