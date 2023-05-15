/*!*****************************************************************************
\file vec3.h
\author Jazz Teoh Yu Jue, Chen Jia Wen
\par DP email: j.teoh\@digipen.edu, c.jiawen\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
This file contains the implementation of 3D vector and it's operations.
*******************************************************************************/
#pragma once
#include <iostream>
#include <glm/glm.hpp>

namespace Math {
  struct Vec2;

  struct Vec3 {
    union {
      float x;
      float r;
    };
    union {
      float y;
      float g;
    };
    union {
      float z;
      float b;
    };
    /*!*****************************************************************************
    Parametrized constructor

    \param float
    - x value
    
    \param float
    - y value

    \param float
    - z value
    *******************************************************************************/
    Vec3(float, float, float);

    // Default destructor
    ~Vec3() {}

    /*!*****************************************************************************
    Default constructor, initialises all values to be indicated value, or 0 by 
    default

    \param float
    - Value to initialise all numbers of vector to. 0 by default
    *******************************************************************************/
    Vec3(float = 0);

    /*!*****************************************************************************
    Copy Constructor

    \param const Vec3&
    - Another vec3 to copy to current vec3
    *******************************************************************************/
    Vec3(const Vec3&);

    /*!*****************************************************************************
    Copy assignment operator

    \param const glm::vec3&
    - Vector3 to copy to current vec3
    *******************************************************************************/
    Vec3& operator=(const Vec3&);

    /*!*****************************************************************************
    Explicit conversion constructor

    \param const glm::vec3&
    - Vector3 from glm
    *******************************************************************************/
    explicit Vec3(const glm::vec3&);

    /*!*****************************************************************************
    Explicit conversion constructor. 
    First 2 values are replaced by vec2. z value is replaced with 0.

    \param const Vec2&
    - Vec2 to insert into vec3
    *******************************************************************************/
    explicit Vec3(const Vec2&);

    /*!*****************************************************************************
    Operator overload for +=

    \param const Vec3&
    - Vector 3 to add to current one

    \return Vec3&
    - Reference to current vec3
    *******************************************************************************/
    Vec3& operator+=(const Vec3&);

    /*!*****************************************************************************
    Operator overload for -=

    \param const Vec3&
    - Vector 3 to subtract from current one

    \return Vec3&
    - Reference to current vec3
    *******************************************************************************/
    Vec3& operator-=(const Vec3&);

    /*!*****************************************************************************
    Operator overload for *=

    \param float
    - Value to multiply to current vec3

    \return Vec3&
    - Reference to current vec3
    *******************************************************************************/
    Vec3& operator*=(float);

    /*!*****************************************************************************
    Operator overload for /=

    \param float
    - Value to divide current vec3 by

    \return Vec3&
    - Reference to current vec3
    *******************************************************************************/
    Vec3& operator/=(float);

    /*!*****************************************************************************
    Operator overload for -. Negate all values of current vec3

    \return Vec3
    Negated version of current vec3
    *******************************************************************************/
    Vec3 operator-() const;

    /*!*****************************************************************************
    Normalise current vec3

    \return Vec3&
    - Reference to current vec3
    *******************************************************************************/
    Vec3& Normalize();
    
    /*!*****************************************************************************
    Return value of normalised vec3

    \return Vec3
    - Normalised version of current vec3
    *******************************************************************************/
    Vec3 Normalized() const;
    
    /*!*****************************************************************************
    Get magnitude of vec3

    \return
    - Magnitude of current vec3
    *******************************************************************************/
    float Magnitude() const;
    
    /*!*****************************************************************************
    Get squared magnitude of current vec3

    \return
    - Squared magnitude of current vec3
    *******************************************************************************/
    float SqMagnitude() const;
    
    /*!*****************************************************************************
    Convert current vec3 to glm vec3

    \return glm::vec3
    - glm version of vec3
    *******************************************************************************/
    glm::vec3 ToGLM() const;
  };

  /*!*****************************************************************************
  Add 2 vec3

  \param const Vec3&
  - First vec3
  
  \param const Vec3&
  - Second vec3

  \return Vec3
  - Vec3 of current operation
  *******************************************************************************/
  Vec3 operator+(const Vec3&, const Vec3&);

  /*!*****************************************************************************
  Subtract left side vec3 from right side vec3

  \param const Vec3&
  - First vec3

  \param const Vec3&
  - Second vec3

  \return Vec3
  - Vec3 of current operation
  *******************************************************************************/
  Vec3 operator-(const Vec3&, const Vec3&);

  /*!*****************************************************************************
  Multiply vec3 by a value

  \param const Vec3&
  - vec3

  \param float
  - Value to multiply by

  \return Vec3
  - Vec3 of current operation
  *******************************************************************************/
  Vec3 operator*(const Vec3&, float);
  
  /*!*****************************************************************************
  Multiply vec3 by a value

  \param float
  - Value to multiply by
  
  \param const Vec3&
  - vec3

  \return Vec3
  - Vec3 of current operation
  *******************************************************************************/
  Vec3 operator*(float, const Vec3&);

  /*!*****************************************************************************
  Divide vec3 by a value

  \param const Vec3&
  - vec3

  \param float
  - Value to divide by

  \return Vec3
  - Vec3 of current operation
  *******************************************************************************/
  Vec3 operator/(const Vec3&, float);

  /*!*****************************************************************************
  Dot product of 2 vec3

  \param const Vec3&
  - First vec3

  \param const Vec3&
  - Second vec3

  \return float
  - Result of dot product
  *******************************************************************************/
  float Dot(const Vec3&, const Vec3&);

  /*!*****************************************************************************
  Cross product of 2 vec3

  \param const Vec3&
  - First vec3

  \param const Vec3&
  - Second vec3

  \return Vec3
  - Vec3 of current operation
  *******************************************************************************/
  Vec3 Cross(const Vec3&, const Vec3&);

  /*!*****************************************************************************
  Distance of a vec3 to another

  \param const Vec3&
  - First vec3

  \param const Vec3&
  - Second vec3

  \return float
  - Distance between 2 vec3
  *******************************************************************************/
  float Distance(const Vec3&, const Vec3&);

  /*!*****************************************************************************
  Square Distance of a vec3 to another

  \param const Vec3&
  - First vec3

  \param const Vec3&
  - Second vec3

  \return float
  - Squared distance between 2 vec3
  *******************************************************************************/
  float SqDistance(const Vec3&, const Vec3&);

  /*!*****************************************************************************
  Output operator overload, prints vec3 in this format (x,y,z)

  \param std::ostream&
  - Output stream

  \param const Vec3&
  - vec3

  \return std::ostream&
  - Output stream
  *******************************************************************************/
  std::ostream& operator<<(std::ostream&, const Vec3&);
}
