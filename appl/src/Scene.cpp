#include "Scene.h"

//#define _USE_MATH_DEFINES
//#include <cmath>
//M_PI

#include <numbers>
#include <cmath>
#include "Vector3.h"
#include "Ray.h"
#include "Color.h"
#include "Raytracer.h"

Scene::Scene(int InWidth, int InHeight, SDL_Renderer* InRender) 
{
   Width = InWidth;
   Height = InHeight;
   Renderer = InRender;

   Sphere Sphere01{Vector3{-1.5, 0, -5}, 1.f};
   Sphere01.Material.Albedo = {1, 0, 0};
   Sphere01.Material.SpecularShiningFactor = 40;
   Sphere01.Material.ReflectionFactor = 0.f;

   Sphere Sphere02{Vector3{1.5, 0, -5}, 1.f};
   Sphere02.Material.Albedo = {0, 1, 0};
   Sphere02.Material.SpecularShiningFactor = 80;
   Sphere02.Material.ReflectionFactor = 0.8f;

   Sphere Sphere03{Vector3{0, -1.5, -5}, 1.f};
   Sphere03.Material.Albedo = {0, 0, 1};
   Sphere03.Material.SpecularShiningFactor = 1000;
   Sphere03.Material.ReflectionFactor = 0.f;


   Spheres.push_back(Sphere01);
   Spheres.push_back(Sphere02);
   Spheres.push_back(Sphere03);

   Light.Direction = {0, -1, 0};
   Light.Color = {1, 0, 0};

   BackgroundColor = {0, 0, 0};
}

Scene::~Scene() 
{

}

void Scene::Update(float InDeltaTime) 
{   
    static float AspectRatio = (float)Width / (float)Height;

    static float VerticalFovDegrees = 60.f;
    static float HalfFovRadiants = (VerticalFovDegrees * 0.5f) * std::numbers::pi / 180.f;
    static float Fov = tanf(HalfFovRadiants);

    static Vector3 CameraPosition{0, 0, 0};
    static Raytracer Raytracer;

    for(int H = 0; H < Height; ++H) 
    {
        for(int W = 0; W < Width; ++W) 
        {
            float NDC_X = ((float)W + 0.5f) / (float)Width;  // [0, 1]
            float NDC_Y = ((float)H + 0.5f) / (float)Height; // [0, 1]

            float Plane_X = (2.f * NDC_X) - 1.f; // [-1, 1]
            float Plane_Y = 1.f - 2.f * NDC_Y;  // [-1, 1]
            
            Plane_X *= Fov * AspectRatio;
            Plane_Y *= Fov;

            Vector3 PlanePoint{Plane_X, Plane_Y, -1.f};

            Vector3 RayDirection = PlanePoint - CameraPosition;
            RayDirection = RayDirection.Normalized();

            Ray PixelRay{CameraPosition, RayDirection};

            XColor PixelColor = Raytracer.RayTrace(PixelRay, *this, 1);

            SDL_SetRenderDrawColor(this->Renderer, PixelColor.R * 255.f, PixelColor.G * 255.f, PixelColor.B * 255.f, 255);
            SDL_RenderDrawPoint(this->Renderer, W, H);
        }   
    }
}