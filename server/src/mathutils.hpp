/**
 * @file math.hpp
 * @brief Header only file that includes several math utility functions
 * @version 0.1
 * @date 2024-03-29
 *
 */

#include <math.h>

namespace math {
    float randf() {
        return static_cast<float>(rand()) / RAND_MAX;
    }
}