#pragma once
#include "PhysX.h"
#include "PhysicsTypes.h"

static PxFilterFlags FilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
    PxFilterObjectAttributes attributes1, PxFilterData filterData1,
    PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    // let triggers through
    if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
    {
        pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
        return PxFilterFlag::eDEFAULT;
    }
    pairFlags = PxPairFlag::eCONTACT_DEFAULT;

    // ignore static/static collision
    if (filterData0.word0 == static_cast<unsigned char>(MOTION::STATIC) 
     && filterData1.word0 == static_cast<unsigned char>(MOTION::STATIC))
        return PxFilterFlag::eKILL;

    // generate contacts between dynamic/static and dynamic/dynamic that were not filtered above
    pairFlags |= PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
    return PxFilterFlag::eDEFAULT;
}
