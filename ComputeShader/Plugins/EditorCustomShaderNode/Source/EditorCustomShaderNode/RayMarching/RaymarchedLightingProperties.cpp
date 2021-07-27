#include "RaymarchedLightingProperties.h"

FRaymarchedLightingProperties::FRaymarchedLightingProperties()
    : LightOrigin{}
    , LightColor{ FColor(1.0f,1.0f,1.0f) }
    , AmbientColor{ FColor(1.0f,1.0f,1.0f) }
    , FogMultiplier{ 1.0f }
    , LightStrength{ 1.0f }
{
}
