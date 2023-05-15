/*!*****************************************************************************
\file mat32.cpp
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
#include "mat32.h"
#include "mat3.h"
#include <iomanip>

namespace Math {
  void Mat32::Init() {
    mtx.resize(2);
    for (auto& row : mtx)
      row.resize(3);
  }

  Mat32::Mat32(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3) {
    Init();
    mtx[0][0] = _x1;
    mtx[0][1] = _x2;
    mtx[0][2] = _x3;
    mtx[1][0] = _y1;
    mtx[1][1] = _y2;
    mtx[1][2] = _y3;
  }

  Mat32::Mat32(const Vec2& _col1, const Vec2& _col2, const Vec2& _col3) {
    Init();
    mtx[0][0] = _col1.x;
    mtx[1][0] = _col1.y;
    mtx[0][1] = _col2.x;
    mtx[1][1] = _col2.y;
    mtx[0][2] = _col3.x;
    mtx[1][2] = _col3.y;
  }

  Mat32::Mat32(float _i, float _x, float _y) {
    Init();
    mtx[0][0] = mtx[1][1] = _i;
    mtx[0][1] = mtx[1][0] = 0;
    mtx[0][2] = _x;
    mtx[1][2] = _y;
  }

  Mat32::Mat32(const Mat3& _mat) {
    Init();
    mtx[0][0] = _mat.mMtx2[0];
    mtx[0][1] = _mat.mMtx2[1];
    mtx[0][2] = _mat.mMtx2[2];
    mtx[1][0] = _mat.mMtx2[3];
    mtx[1][1] = _mat.mMtx2[4];
    mtx[1][2] = _mat.mMtx2[5];
  }

  Mat32::Mat32(const Mat32& _mat) {
    Init();
    *this = _mat;
  }

  Mat32& Mat32::operator=(const Mat32& _mat) {
    this->mtx = _mat.mtx;
    return *this;
  }

  Vec2 operator*(const Mat32& _m, const Vec2& _v) {
    float x{ _m[0][0] * _v.x + _m[0][1] * _v.y + _m[0][2] };
    float y{ _m[1][0] * _v.x + _m[1][1] * _v.y + _m[1][2] };
    return { x, y };
  }

  Vec2 operator*(const Vec2& _v, const Mat32& _m) {
    return operator*(_m, _v);
  }

  Mat32 operator*(const Mat32& _m1, const Mat32& _m2) {
    Mat32 temp{ _m1 };
    temp[0][0] = _m1[0][0] * _m2[0][0] + _m1[0][1] * _m2[1][0];
    temp[0][1] = _m1[0][0] * _m2[0][1] + _m1[0][1] * _m2[1][1];
    temp[0][2] = _m1[0][0] * _m2[0][2] + _m1[0][1] * _m2[1][2] + _m1[0][2];
    temp[1][0] = _m1[1][0] * _m2[0][0] + _m1[1][1] * _m2[1][0];
    temp[1][1] = _m1[1][0] * _m2[0][1] + _m1[1][1] * _m2[1][1];
    temp[1][2] = _m1[1][0] * _m2[0][2] + _m1[1][1] * _m2[1][2] + _m1[1][2];
    return temp;
  }

  std::ostream& operator<<(std::ostream& _out, const Mat32& _m) {
    _out << std::fixed << std::setprecision(7);
    _out << "| " << _m[0][0] << " , " << _m[0][1] << " , " << _m[0][2] << " |" << '\n';
    _out << "| " << _m[1][0] << " , " << _m[1][1] << " , " << _m[1][2] << " |" << '\n';
    return _out;
  }
}