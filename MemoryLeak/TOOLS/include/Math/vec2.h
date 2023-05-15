/*!*****************************************************************************
\file vec2.h
\author Jazz Teoh Yu Jue, Chen Jia Wen
\par DP email: j.teoh\@digipen.edu, c.jiawen\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
This file contains the implementation of 2D vector and it's operations
*******************************************************************************/
#pragma once
#include <iostream>
#include <glm/glm.hpp>

namespace Math {
  struct Vec3;

  struct Vec2 {
    union {
      float x;
      float r;
    };
    union {
      float y;
      float g;
    };

    
    // Default destructor
    ~Vec2();
    
    /*!*****************************************************************************
    Parametrized constructor

    \param float
    - x value

    \param float
    - y value
    *******************************************************************************/
    Vec2(float, float);
    
    /*!*****************************************************************************
    Default constructor, initialises all values to be indicated value, or 0 by
    default

    \param float
    - Value to initialise all numbers of vector to. 0 by default
    *******************************************************************************/
    Vec2(float = 0);
    
    /*!*****************************************************************************
    Copy Constructor

    \param const Vec2&
    - Another vec2 to copy to current vec2
    *******************************************************************************/
    Vec2(const Vec2&);
    
    /*!*****************************************************************************
    Copy assignment operator

    \param const glm::vec2&
    - Vec2 to copy to current vec2
    *******************************************************************************/
    Vec2& operator=(const Vec2&);

    /*!*****************************************************************************
    Explicit conversion constructor

    \param const glm::vec2&
    - Vector2 from glm
    *******************************************************************************/
    explicit Vec2(glm::vec2);
    
    /*!*****************************************************************************
    Explicit conversion constructor.
    x and y values are replaced by vec3's x and y value.

    \param const Vec2&
    - Vec3 to insert into vec2
    *******************************************************************************/
    explicit Vec2(const Vec3&);
    
    /*!*****************************************************************************
    Operator overload for +=

    \param const Vec2&
    - Vector 2 to add to current one

    \return Vec2&
    - Reference to current Vec2
    *******************************************************************************/
    Vec2& operator+=(const Vec2&);
    
    /*!*****************************************************************************
    Operator overload for -=

    \param const Vec2&
    - Vector 2 to subtract from current one

    \return Vec2&
    - Reference to current Vec2
    *******************************************************************************/
    Vec2& operator-=(const Vec2&);
    
    /*!*****************************************************************************
    Operator overload for *=

    \param float
    - Value to multiply to current Vec2

    \return Vec2&
    - Reference to current Vec2
    *******************************************************************************/
    Vec2& operator*=(float);
    
    /*!*****************************************************************************
    Operator overload for /=

    \param float
    - Value to divide current Vec2 by

    \return Vec2&
    - Reference to current Vec2
    *******************************************************************************/
    Vec2& operator/=(float);
    
    /*!*****************************************************************************
    Operator overload for ==
    
    \param const Vec2&
    - Vec2 to compare with

    \return bool
    - True if both vec2 are equal, otherwise false
    *******************************************************************************/
    bool operator==(const Vec2&);
    
    /*!*****************************************************************************
    Operator overload for -. Negate all values of current Vec2

    \return Vec2
    Negated version of current Vec2
    *******************************************************************************/
    Vec2 operator-() const;

    float& operator[](int);

    /*!*****************************************************************************
    Normalise current Vec2

    \return Vec2&
    - Reference to current Vec2
    *******************************************************************************/
    Vec2& Normalize();
    
    /*!*****************************************************************************
    Return value of normalised Vec2

    \return Vec2
    - Normalised version of current Vec2
    *******************************************************************************/
    Vec2 Normalized() const;
    
    /*!*****************************************************************************
    Get magnitude of Vec2

    \return
    - Magnitude of current Vec2
    *******************************************************************************/
    float Magnitude() const;
    
    /*!*****************************************************************************
    Get squared magnitude of current Vec2

    \return
    - Squared magnitude of current Vec2
    *******************************************************************************/
    float SqMagnitude() const;
    
    /*!*****************************************************************************
    Convert current Vec2 to glm Vec2

    \return glm::Vec2
    - glm version of Vec2
    *******************************************************************************/
    glm::vec2 ToGLM() const;
  };
  
  /*!*****************************************************************************
  Add 2 Vec2

  \param const Vec2&
  - First Vec2

  \param const Vec2&
  - Second Vec2

  \return Vec2
  - Vec2 of current operation
  *******************************************************************************/
  Vec2 operator+(const Vec2&, const Vec2&);
  
  /*!*****************************************************************************
  Subtract left side Vec2 from right side Vec2

  \param const Vec2&
  - First Vec2

  \param const Vec2&
  - Second Vec2

  \return Vec2
  - Vec2 of current operation
  *******************************************************************************/
  Vec2 operator-(const Vec2&, const Vec2&);
  
  /*!*****************************************************************************
  Multiply Vec2 by a value

  \param const Vec2&
  - Vec2

  \param float
  - Value to multiply by

  \return Vec2
  - Vec2 of current operation
  *******************************************************************************/
  Vec2 operator*(const Vec2&, float);
  
  /*!*****************************************************************************
  Multiply Vec2 by a value

  \param float
  - Value to multiply by

  \param const Vec2&
  - Vec2

  \return Vec2
  - Vec2 of current operation
  *******************************************************************************/
  Vec2 operator*(float, const Vec2&);
  
  /*!*****************************************************************************
  Divide Vec2 by a value

  \param const Vec2&
  - Vec2

  \param float
  - Value to divide by

  \return Vec2
  - Vec2 of current operation
  *******************************************************************************/
  Vec2 operator/(const Vec2&, float);
  
  /*!*****************************************************************************
  Dot product of 2 Vec2

  \param const Vec2&
  - First Vec2

  \param const Vec2&
  - Second Vec2

  \return float
  - Result of dot product
  *******************************************************************************/
  float Dot(const Vec2&, const Vec2&);

  /*!*****************************************************************************
  Cross product of 2 Vec2

  \param const Vec2&
  - First Vec2

  \param const Vec2&
  - Second Vec2

  \return float
  - Magnitude of orthogonal vector along z-axis
  *******************************************************************************/
  float Cross(const Vec2&, const Vec2&);

  Vec2 Cross(const Vec2&, float);

  Vec2 Cross(float, const Vec2&);
  
  /*!*****************************************************************************
  Distance of a Vec2 to another

  \param const Vec2&
  - First Vec2

  \param const Vec2&
  - Second Vec2

  \return float
  - Distance between 2 Vec2
  *******************************************************************************/
  float Distance(const Vec2&, const Vec2&);
  
  /*!*****************************************************************************
  Square Distance of a Vec2 to another

  \param const Vec2&
  - First Vec2

  \param const Vec2&
  - Second Vec2

  \return float
  - Squared distance between 2 Vec2
  *******************************************************************************/
  float SqDistance(const Vec2&, const Vec2&);
  
  /*!*****************************************************************************
  Output operator overload, prints Vec2 in this format (x,y,z)

  \param std::ostream&
  - Output stream

  \param const Vec2&
  - Vec2

  \return std::ostream&
  - Output stream
  *******************************************************************************/
  std::ostream& operator<<(std::ostream&, const Vec2&);
  
}

