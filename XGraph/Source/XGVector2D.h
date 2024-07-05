#pragma once

struct XGVector2D
{
    float U = 0.0f;
    float V = 0.0f;
    
    float W = 1.0f; // Necessary for perspective projection of textures
};
