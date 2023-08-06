#ifndef RANDOM_HPP
#define RANDOM_HPP

namespace CS380 {
    namespace Utils {
        unsigned rand();                        // returns a random 32-bit integer
        float    frand();                       // return a random 32-bit floating point number
        void     srand(unsigned, unsigned);     // seed the generator
        int      Random(int low, int high);     // range
        float    Random(float low, float high); // range
    }
}

#endif
