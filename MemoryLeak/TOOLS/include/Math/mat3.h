/*!*****************************************************************************
\file mat3.h
\author Jazz Teoh Yu Jue, Chen Jia Wen
\par DP email: j.teoh\@digipen.edu, c.jiawen\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
This file contains the implementation of 3x3 matrix and it's operations.
The matrix is structured with row major method as such,
x1, x2, x3
y1, y2, y3
z1, z2, z3

*******************************************************************************/
#pragma once
#include <iostream>
#include "math.h"
#include "vec3.h"

namespace Math {
  struct Mat32;

  struct Mat3 {
    union {
      float mMtx1[3][3];
      float mMtx2[9];
    };

  public:
    /*!*****************************************************************************
    Parametrized constructor with individual values.

    \param float 
    - x1
    
    \param float 
    - y1
    
    \param float 
    - z1
    
    \param float 
    - x2
    
    \param float 
    - y2
    
    \param float 
    - z2
    
    \param float 
    - x3
    
    \param float 
    - y3
    
    \param float 
    - z3
    *******************************************************************************/
    Mat3(float, float, float, float, float, float, float, float, float);
    
    /*!*****************************************************************************
    Parametrized constructor with rows of vector 3.

    \param const Vec3&
    - First row

    \param const Vec3&
    - Second row

    \param const Vec3&
    - Third row
    *******************************************************************************/
    Mat3(const Vec3&, const Vec3&, const Vec3&);

    /*!*****************************************************************************
    Copy constructor

    \param const Mat3&
    - Matrix 3 to copy into current matrix
    *******************************************************************************/
    Mat3(const Mat3&);

    /*!*****************************************************************************
    Copy assignment operator

    \param const Mat3&
    - 3x3 matrix to copy into current matrix
    *******************************************************************************/
    Mat3& operator=(const Mat3&);

    /*!*****************************************************************************
    Default constructor with single value making up identity matrix. By default
    it's 1. If specified floating value, it will replace the 1 values of the 
    identity matrix with it

    \param float
    - Values to replace the center diagonal stripe of values. By default 1.
    *******************************************************************************/
    Mat3(float = 1);

    /*!*****************************************************************************
    Explicit copy constructor from Mat32. Copy all values of Mat32 over. Replace the 
    last row with 0,0,1.

    \param const Mat32&
    - 3x2 matrix to copy into current matrix
    *******************************************************************************/
    explicit Mat3(const Mat32&);

    /*!*****************************************************************************
    Bracket operator overload to access a value in a matrix

    \param short
    - Index to a spot in the matrix 

    \return float*
    - Pointer to a spot in matrix
    *******************************************************************************/
    float* operator[](short _idx) { return mMtx1[_idx]; }
    
    /*!*****************************************************************************
    Bracket operator overload to access a value in a matrix. (Const version)

    \param short
    - Index to a spot in the matrix

    \return const float*
    - Pointer to a spot in matrix
    *******************************************************************************/
    const float * operator[](short _idx) const { return mMtx1[_idx]; }
    
    /*!*****************************************************************************
    Transpose current matrix
    
    \return Mat&
    - Current matrix after transposed
    *******************************************************************************/
    Mat3& Transpose();
    
    /*!*****************************************************************************
    Return transposed version of current matrix
    
    \return Mat3
    - Transposed version of current matrix
    *******************************************************************************/
    Mat3 Transposed() const;
    ~Mat3();
  };

  /*!*****************************************************************************
  Operator overload for * to do vector matrix multiplication

  /param const Mat3&
  - Matrix

  /param const Vec3&
  - Vector 3

  \return Vec3
  - Result after vector matrix multiplication
  *******************************************************************************/
  Vec3 operator*(const Mat3&, const Vec3&);

  /*!*****************************************************************************
  Operator overload for * to do vector matrix multiplication

  /param const Vec3&
  - Vector 3

  /param const Mat3&
  - Matrix

  \return Vec3
  - Result after vector matrix multiplication
  *******************************************************************************/
  Vec3 operator*(const Vec3&, const Mat3&);

  /*!*****************************************************************************
  Operator overload for * to do matrix matrix multiplication. Multiply the left
  side matrix by right side matrix

  /param const Mat3&
  - First matrix

  /param const Mat3&
  - Second matrix

  \return Mat3
  - Result after vector matrix multiplication
  *******************************************************************************/
  Mat3 operator*(const Mat3&, const Mat3&);

  /*!*****************************************************************************
  Output operator overloading to print matrix. Printing in this format,
  | x1 , x2 , x3 |
  | y1 , y2 , y3 |
  | z1 , z2 , z3 |

  /param std::ostream&
  - Outstream

  /param const Mat3&
  - Matrix

  \return std::ostream&
  - Outstream
  *******************************************************************************/
  std::ostream& operator<<(std::ostream&, const Mat3&);
}
