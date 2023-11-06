#pragma once
#include "PhysX.h"
#include "PhysicsTypes.h"

static PxFilterFlags FilterShader(PxFilterObjectAttributes attributes0, PxFilterData,
    PxFilterObjectAttributes attributes1, PxFilterData,
    PxPairFlags& pairFlags, const void*, PxU32)
{
    // let triggers through
    if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
    {
        pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
        return PxFilterFlag::eDEFAULT;
    }

    // generate contacts between dynamic/static and dynamic/dynamic that were not filtered above
    pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
    return PxFilterFlag::eDEFAULT;
}
