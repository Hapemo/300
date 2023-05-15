/*!*****************************************************************************
\file mat32.h
\author Jazz Teoh Yu Jue, Chen Jia Wen
\par DP email: j.teoh\@digipen.edu, c.jiawen\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
This file contains the implementation of 3x2 matrix and it's operations.
3x2 matrix is useful in 2D aspect because during the homogenenous transformation
calculation of 2D space, the last row is not needed since it's always 0,0,1.
Doing computation with it results in wasted processing speed and storage memory
*******************************************************************************/
#pragma once

#include <iostream>
#include <vector>
#include "math.h"
#include "vec2.h"

namespace Math {
  struct Mat3;

  struct Mat32 { // Mat32 works the same as Mat3. Since the last column is always (0,0,1), it's removed for optimisation purposes.
    std::vector<std::vector<float>> mtx;

  public:
    
    /*!*****************************************************************************
    Parametrized constructor with individual values.

    \param float
    - x1

    \param float
    - y1

    \param float
    - x2

    \param float
    - y2

    \param float
    - x3
    
    \param float
    - y3
    *******************************************************************************/
    Mat32(float, float, float, float, float, float);
    
    // Default destructor
    ~Mat32() {};

    /*!*****************************************************************************
    Parametrized constructor with columns of vector 2.

    \param const Vec2&
    - First column

    \param const Vec2&
    - Second column

    \param const Vec2&
    - Third column
    *******************************************************************************/
    Mat32(const Vec2&, const Vec2&, const Vec2&);

    /*!*****************************************************************************
    Copy constructor

    \param const Mat3&
    - Matrix 3 to copy into current matrix
    *******************************************************************************/
    Mat32(const Mat32&);
    
    /*!*****************************************************************************
    Copy assignment operator

    \param const Mat32&
    - 3x2 matrix to copy into current matrix
    *******************************************************************************/
    Mat32& operator=(const Mat32&);

    /*!*****************************************************************************
    Copy assignment operator

    \param const Mat3&
    - 3x3 matrix to copy into current matrix
    *******************************************************************************/
    explicit Mat32(const Mat3&);
    
    /*!*****************************************************************************
    Parametrized constructor with identity 2d matrix and translate.
    x1 and y2 will be replaced with first floating value. x3 and y3 will be replaced
    with the 2 other floating values
    
    \param float
    - floating value to replace x1 and y2

    \param float
    - floating value to replace x3

    \param float
    - floating value to replace y3
    *******************************************************************************/
    Mat32(float = 1.f, float = 0.f, float = 0.f);
    
    /*!*****************************************************************************
    Bracket operator overload to access a row in a matrix

    \param short
    - Row in the matrix to access

    \return std::vector<float>&
    - reference to indicated row of matrix
    *******************************************************************************/
    std::vector<float>& operator[](short _i) { return mtx[_i]; }
    
    /*!*****************************************************************************
    Bracket operator overload to access a row in a matrix (Const version)

    \param short
    - Row in the matrix to access

    \return const std::vector<float>&
    - reference to indicated row of matrix
    *******************************************************************************/
    const std::vector<float>& operator[](short _i) const { return mtx[_i]; }
  private:

    /*!*****************************************************************************
    Initialise the vectors in the matrix, should be called in constructors

    \param short
    - Row in the matrix to access

    \return std::vector<float>&
    - reference to indicated row of matrix
    *******************************************************************************/
    void Init();
  };

  /*!*****************************************************************************
  Operator overload for * to do vector matrix multiplication

  /param const Mat32&
  - Matrix

  /param const Vec2&
  - Vector 2

  \return Vec2
  - Result after vector matrix multiplication
  *******************************************************************************/
  Vec2 operator*(const Mat32&, const Vec2&);
  
  /*!*****************************************************************************
  Operator overload for * to do vector matrix multiplication

  /param const Vec2&
  - Vector 2

  /param const Mat32&
  - Matrix

  \return Vec2
  - Result after vector matrix multiplication
  *******************************************************************************/
  Vec2 operator*(const Vec2&, const Mat32&);
  
  /*!*****************************************************************************
  Operator overload for * to do matrix matrix multiplication. Multiply the left
  side matrix by right side matrix

  /param const Mat32&
  - First matrix

  /param const Mat32&
  - Second matrix

  \return Mat32
  - Result after vector matrix multiplication
  *******************************************************************************/
  Mat32 operator*(const Mat32&, const Mat32&);
  

  /*!*****************************************************************************
  Output operator overloading to print matrix. Printing in this format,
  | x1 , x2 , x3 |
  | y1 , y2 , y3 |

  /param std::ostream&
  - Outstream

  /param const Mat32&
  - Matrix

  \return std::ostream&
  - Outstream
  *******************************************************************************/
  std::ostream& operator<<(std::ostream&, const Mat32&);
}

