/**-------------------------------------------------------------------------------------/
 * @file
 *  Random.hpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class definition of Camera.
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#ifndef RANDOM_HPP
#define RANDOM_HPP

namespace GFX {
    namespace Utils {
        unsigned rand();                        // returns a random 32-bit integer
        float    frand();                       // return a random 32-bit floating point number
        void     srand(unsigned, unsigned);     // seed the generator
        int      Random(int low, int high);     // range
        float    Random(float low, float high); // range
    }
}

#endif
