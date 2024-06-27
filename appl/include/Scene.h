#pragma once
#include "SDL.h"
#include <vector>
#include "Sphere.h"
#include "Color.h"


struct Light 
{
    Vector3 Direction;
    XColor Color;
};

class Scene 
{
public:
    Scene(int w, int h, SDL_Renderer*);
    ~Scene();
    void Update(float delta_time);
    
    std::vector<Sphere> Spheres;
    Light Light;
    XColor BackgroundColor;

private:
    int Width;
    int Height;
    SDL_Renderer* Renderer;
    
};