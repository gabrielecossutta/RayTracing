#include "Raytracer.h"
#include "Sphere.h"
#include <cmath>
#include <limits>

struct RayHit 
{
    Vector3 Point;
    Vector3 Normal;
    const Sphere* Object;
    float Distance;
};

enum class ERayCastStrategy 
{
    RAYCAST_BEST,
    RAYCAST_FIRST
};


static bool RaySphereIntersection(const Ray& InRay, const Sphere& InSphere, RayHit& Hit) 
{
    Vector3 L = InSphere.Center - InRay.Origin;
    float Tca = L.Dot(InRay.Direction);

    float MagnL2 = L.MagnetudeSquared();

    float D2 = MagnL2 - Tca * Tca;
    float Radius2 = InSphere.Radius * InSphere.Radius;
    if (D2 > Radius2) return false;

    float Thc = sqrtf(Radius2 - D2);

    float T0 = Tca - Thc; //
    float T1 = Tca + Thc;

    if (T1 < T0)  //In T0 we will keep the smallest value
    {
        float Temp = T0;
        T0 = T1;
        T1 = Temp;
    }

    if (T0 < 0) 
    {
        T0 = T1; //Case: where T1 > T0
        if (T0 < 0) return false;
    }

    Hit.Point = InRay.Origin + InRay.Direction * T0;
    Hit.Normal = (Hit.Point - InSphere.Center).Normalized();
    Hit.Object = &InSphere;
    Hit.Distance = T0;
    return true;
}

static bool RayCast(const Ray& InRay, const Scene& InScene, RayHit& HitOut, ERayCastStrategy CastStategy) 
{
    RayHit BestHit;
    BestHit.Distance = std::numeric_limits<float>::max();
    
    bool HasBestHit = false;

    for(auto& EachSphere : InScene.Spheres) 
    {   
        RayHit Hit;
        bool HasHit = RaySphereIntersection(InRay, EachSphere, Hit);

        if (HasHit && Hit.Distance < BestHit.Distance) 
        {
            BestHit = Hit;
            HasBestHit = true;
            if (CastStategy == ERayCastStrategy::RAYCAST_FIRST) break;
        }
    }

    if (HasBestHit) {
        HitOut = BestHit;
    }
    return HasBestHit;
}


XColor Raytracer::RayTrace(const Ray& InRay, Scene& InScene, int InCurrentDepth)
{
    if (InCurrentDepth > 3) return InScene.BackgroundColor;

    //Primary Result
    RayHit Hit;
    bool HasHit = RayCast(InRay, InScene, Hit, ERayCastStrategy::RAYCAST_BEST);
    if (!HasHit) return InScene.BackgroundColor;

    //Shadow ray
    Vector3 InvertedLightDirection = InScene.Light.Direction * -1.f;

    static float bias = 1e-4;
    Ray ShadowRay;
    ShadowRay.Origin = Hit.Point + (Hit.Normal * bias);
    ShadowRay.Direction = InvertedLightDirection;

    RayHit ShadowHit;
    bool ShadowHasHit = RayCast(ShadowRay, InScene, ShadowHit, ERayCastStrategy::RAYCAST_FIRST);
    //if (ShadowHasHit) return {0, 0, 0};

    XColor SphereColor = Hit.Object->Material.Albedo;

    //BLINN-PHONG
    //1. Ambient
    float AmbientFactor = 0.1f;
    XColor Ambient = SphereColor * AmbientFactor;

    //2. Diffuse
    float Lambert = fmaxf(0, InvertedLightDirection.Dot(Hit.Normal));

    XColor Diffuse = (SphereColor + InScene.Light.Color)  * Lambert;

    //3. Specular
    Vector3 L = InvertedLightDirection;
    Vector3 V = (Hit.Point * -1.f).Normalized();
    Vector3 H = (L + V).Normalized();

    float SpecularFactor = fmaxf(0, Hit.Normal.Dot(H));
    float SpecularIntensity = powf(SpecularFactor, Hit.Object->Material.SpecularShiningFactor);
    XColor Specular = InScene.Light.Color * SpecularIntensity;

    XColor Phong = InScene.BackgroundColor;
    Phong = Phong + Ambient;
    Phong = Phong + Diffuse;
    Phong = Phong + Specular;
    //Phong = Phong.Clamp();
    //return Phong;

    XColor FinalColor = Phong;

    if (Hit.Object->Material.ReflectionFactor > 0.f) 
    {
        Ray SecondaryRay;
        SecondaryRay.Origin = Hit.Point + (Hit.Normal * bias);
        SecondaryRay.Direction = InRay.Direction.Reflect(Hit.Normal);

        XColor ReflectionColor = RayTrace(SecondaryRay, InScene, InCurrentDepth + 1);
        
        FinalColor = FinalColor + ReflectionColor * Hit.Object->Material.ReflectionFactor;
    }
    FinalColor = FinalColor.Clamp();
    return FinalColor;
}