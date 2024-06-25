////////////
// XGraph //
////////////
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
    XGEngine Demo;
    if (!Demo.Construct(1920, 1080, 1, 1))
    {
        // TODO: Log error here
        return 1;
    }

    Demo.Start();
    
    return 0;
}
