/*!*****************************************************************************
\file mat3.cpp
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

#include <iomanip>
#include "mat3.h"
#include "mat32.h"

namespace Math {
  Mat3::Mat3(float _x1, float _y1, float _z1, float _x2, float _y2, float _z2, float _x3, float _y3, float _z3) {
    mMtx2[0] = _x1;
    mMtx2[1] = _y1;
    mMtx2[2] = _z1;
    mMtx2[3] = _x2;
    mMtx2[4] = _y2;
    mMtx2[5] = _z2;
    mMtx2[6] = _x3;
    mMtx2[7] = _y3;
    mMtx2[8] = _z3;
  }

  Mat3::Mat3(const Vec3& _vct1, const Vec3& _vct2, const Vec3& _vct3) {
    mMtx2[0] = _vct1.x;
    mMtx2[1] = _vct1.y;
    mMtx2[2] = _vct1.z;
    mMtx2[3] = _vct2.x;
    mMtx2[4] = _vct2.y;
    mMtx2[5] = _vct2.z;
    mMtx2[6] = _vct3.x;
    mMtx2[7] = _vct3.y;
    mMtx2[8] = _vct3.z;
  }

  Mat3::Mat3(const Mat3& _mat) {
    for (int i = 0; i < 9; ++i) {
      mMtx2[i] = _mat.mMtx2[i];
    }
  }

  Mat3::Mat3(float _i) {
    mMtx2[0] = mMtx2[4] = mMtx2[8] = _i;
    mMtx2[1] = mMtx2[2] = mMtx2[3] = mMtx2[5] = mMtx2[6] = mMtx2[7] = 0;
  }

  Mat3::Mat3(const Mat32& _mat) {
    mMtx1[0][0] = _mat[0][0];
    mMtx1[0][1] = _mat[0][1];
    mMtx1[0][2] = _mat[0][2];
    mMtx1[1][0] = _mat[1][0];
    mMtx1[1][1] = _mat[1][1];
    mMtx1[1][2] = _mat[1][2];
    mMtx1[2][0] = 0;
    mMtx1[2][1] = 0;
    mMtx1[2][2] = 1;
  }

  Mat3& Mat3::operator=(const Mat3& _mat) {
    for (int i = 0; i < 9; ++i) {
      this->mMtx2[i] = _mat.mMtx2[i];
    }
    return *this;
  }

  Mat3& Mat3::Transpose() {
    Mat3 temp = *this;
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j) {
        temp.mMtx1[j][i] = mMtx1[i][j];
      }
    return *this = temp;
  }

  Mat3 Mat3::Transposed() const {
    Mat3 temp = *this;
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j) {
        temp.mMtx1[j][i] = mMtx1[i][j];
      }
    return temp;
  }

  Mat3::~Mat3() {}
  Vec3 operator*(const Mat3& _lhs, const Vec3& _rhs) {
    float x = _lhs.mMtx2[0] * _rhs.x + _lhs.mMtx2[3] * _rhs.y + _lhs.mMtx2[6] * _rhs.z;
    float y = _lhs.mMtx2[1] * _rhs.x + _lhs.mMtx2[4] * _rhs.y + _lhs.mMtx2[7] * _rhs.z;
    float z = _lhs.mMtx2[2] * _rhs.x + _lhs.mMtx2[5] * _rhs.y + _lhs.mMtx2[8] * _rhs.z;
    return { x, y, z };
  }

  Vec3 operator*(const Vec3& lhs, const Mat3& rhs) {
    return operator*(rhs, lhs);
  }

  Mat3 operator*(const Mat3& _lhs, const Mat3& _rhs) {
    Mat3 temp = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++) {
        temp.mMtx1[i][j] = 0;
        for (int k = 0; k < 3; k++) {
          temp.mMtx1[i][j] += _lhs.mMtx1[i][k] * _rhs.mMtx1[k][j];
        }
      }
    return temp;
  }

  std::ostream& operator<<(std::ostream& _out, const Mat3& _m) {
    _out << std::fixed << std::setprecision(7);
    _out << "| " << _m[0][0] << " , " << _m[0][1] << " , " << _m[0][2] << " |" << '\n';
    _out << "| " << _m[1][0] << " , " << _m[1][1] << " , " << _m[1][2] << " |" << '\n';
    _out << "| " << _m[2][0] << " , " << _m[2][1] << " , " << _m[2][2] << " |" << '\n';
    return _out;
  }
}