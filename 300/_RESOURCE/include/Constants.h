#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

enum MaterialType : unsigned {

    DIFFUSE,
    NORMAL,
    SPECULAR,
    SHININESS,
    EMISSION,
    AO
};


enum BloomType : unsigned {
    GAUSSIANBLUR = 0,
    GAUSSIANBLUR_VER2,
    PHYS_BASED_BLOOM,
    last_element
};

#endif