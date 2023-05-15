/*!*****************************************************************************
\file vec3.cpp
\author Jazz Teoh Yu Jue, Chen Jia Wen
\par DP email: j.teoh\@digipen.edu, c.jiawen\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
This file contains the implementation of 3D vector and it's operations.
*******************************************************************************/
#include "vec3.h"
#include "vec2.h"

namespace Math {
  Vec3::Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

  Vec3::Vec3(float _i) : x(_i), y(_i), z(_i) {}

  Vec3::Vec3(const glm::vec3& _v) : x(_v.x), y(_v.y), z(_v.z) {}
  Vec3::Vec3(const Vec3& _vct) : x(_vct.x), y(_vct.y), z(_vct.z) {}
  Vec3::Vec3(const Vec2& _v) : x(_v.x), y(_v.y), z(1) {}

  Vec3& Vec3::operator+=(const Vec3& _rhs) {
    x += _rhs.x;
    y += _rhs.y;
    z += _rhs.z;
    return *this;
  }

  Vec3& Vec3::operator-=(const Vec3& _rhs) {
    x -= _rhs.x;
    y -= _rhs.y;
    z -= _rhs.z;
    return *this;
  }

  Vec3& Vec3::operator*=(float _rhs) {
    x *= _rhs;
    y *= _rhs;
    z *= _rhs;
    return *this;
  }

  Vec3& Vec3::operator/=(float _rhs) {
    x /= _rhs;
    y /= _rhs;
    z /= _rhs;
    return *this;
  }

  Vec3& Vec3::operator=(const Vec3& _rhs) {
    x = _rhs.x;
    y = _rhs.y;
    z = _rhs.z;
    return *this;
  }

  Vec3 Vec3::operator-() const {
    return { -x, -y, -z };
  }

  Vec3& Vec3::Normalize() {
    return *this /= this->Magnitude();
  }

  Vec3 Vec3::Normalized() const {
    return *this / this->Magnitude();
  }

  float Vec3::Magnitude() const {
    return sqrtf(x * x + y * y + z * z);
  }

  float Vec3::SqMagnitude() const {
    return x * x + y * y + z * z;
  }

  glm::vec3 Vec3::ToGLM() const
  {
      return glm::vec3(this->x, this->y, this->z);
  }

  Vec3 operator+(const Vec3& _lhs, const Vec3& _rhs) {
    Vec3 temp = _lhs;
    return temp += _rhs;
  }

  Vec3 operator-(const Vec3& _lhs, const Vec3& _rhs) {
    Vec3 temp = _lhs;
    return temp -= _rhs;
  }

  Vec3 operator*(const Vec3& _lhs, float _rhs) {
    Vec3 temp = _lhs;
    return temp *= _rhs;
  }

  Vec3 operator*(float _lhs, const Vec3& _rhs) {
    return operator*(_rhs, _lhs);
  }

  Vec3 operator/(const Vec3& _lhs, float _rhs) {
    Vec3 temp = _lhs;
    return temp /= _rhs;
  }

  float Dot(const Vec3& _lhs, const Vec3& _rhs) {
    return _lhs.x * _rhs.x + _lhs.y * _rhs.y + _lhs.z * _rhs.z;
  }

  Vec3 Cross(const Vec3& v1, const Vec3& v2) {
    Vec3 v{};
    v.x = v1.y * v2.z - v2.y * v1.z;
    v.y = v1.z * v2.x - v2.z * v1.x;
    v.z = v1.x * v2.y - v2.x * v1.y;
    return v;
  }

  float Distance(const Vec3& _lhs, const Vec3& _rhs) {
    return (_lhs-_rhs).Magnitude();
  }

  float SqDistance(const Vec3& _lhs, const Vec3& _rhs) {
    return (_lhs - _rhs).SqMagnitude();
  }

  std::ostream& operator<<(std::ostream& os, const Vec3& _rhs) {
    os << '(' << _rhs.x << ", " << _rhs.y << ", " << _rhs.z << ')';
    return os;
  }

}