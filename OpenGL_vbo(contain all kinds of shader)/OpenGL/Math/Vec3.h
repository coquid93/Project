#pragma once

#include "Math/Math.h"

namespace respace {

///////////////////////////////////////////////////////////////////////////////
// Vec3 class declaration
///////////////////////////////////////////////////////////////////////////////
template <typename T> class Vec3 {
public:
    // open privilege for the use of glVertexfv, v.x, etc.
    union{
        struct { T x, y, z; };
        T val[3];
    };

public:
    // Constructors
    Vec3();
    Vec3(T d);
    Vec3(T d0, T d1, T d2);
    Vec3(const Vec3 &src);
    Vec3(const T src[3]);

    // Setter methods
    void Set(T d);
    void Set(T d0, T d1, T d2);
    void Set(const Vec3 &src);
    void Set(const T src[3]);

    // Overloading operators
    T &operator[](int i);
    T  operator[](int i) const;
    T &operator()(int i);
    T  operator()(int i) const;

    Vec3 &operator=(T d);
    Vec3 &operator=(const Vec3 &src);
    Vec3 &operator=(const T src[3]);

    Vec3 &operator+=(T d);
    Vec3 &operator-=(T d);
    Vec3 &operator*=(T d);
    Vec3 &operator/=(T d);
    Vec3 &operator+=(const Vec3 &src);
    Vec3 &operator-=(const Vec3 &src);

    // Basic methods
    void Clear(){val[0] = val[1] = val[2] = (T) 0;}
    bool IsEqual(const Vec3 &src) const;
    void Normalize();
    T   L1Norm() const;
    T   L2Norm() const;
    T   LInfNorm() const;
    T   Mag() const;
    T   SqrMag() const;

    // for the use of Scene graph implementation
    void Interpolate(const Vec3 &a, const Vec3 &b, const T t){
        *this = (1-t)*a + t*b;
    }
    
    // Friends functions (LHS and RHS arguments are both possible)
    template <typename S> friend Vec3<S> operator-(const Vec3<S> &a);
    template <typename S> friend Vec3<S> operator-(const Vec3<S> &a,S b);
    template <typename S> friend Vec3<S> operator-(S a,const Vec3<S> &b);
    template <typename S> friend Vec3<S> operator-(const Vec3<S> &a,const Vec3<S> &b);
    template <typename S> friend Vec3<S> operator+(const Vec3<S> &a,S b);
    template <typename S> friend Vec3<S> operator+(S a,const Vec3<S> &b);
    template <typename S> friend Vec3<S> operator+(const Vec3<S> &a,const Vec3<S> &b);
    template <typename S> friend Vec3<S> operator*(const Vec3<S> &a,S b);
    template <typename S> friend Vec3<S> operator*(S a,const Vec3<S> &b);
    template <typename S> friend Vec3<S> operator/(const Vec3<S> &a,S b);
    template <typename S> friend S Mag(const Vec3<S> &a);
    template <typename S> friend S SqrMag(const Vec3<S> &a);
    template <typename S> friend S Dot(const Vec3<S> &a,const Vec3<S> &b);
    template <typename S> friend Vec3<S> Cross(const Vec3<S> &a,const Vec3<S> &b);
    template <typename S> friend S DotCross(const Vec3<S> &a,const Vec3<S> &b,const Vec3<S> &c);
    //template <typename S> friend std::ostream &operator<<(std::ostream &str, Vec3<T> &vec);

};

///////////////////////////////////////////////////////////////////////////////
// Vec3 class inline definition
///////////////////////////////////////////////////////////////////////////////
// Constructors
template <typename T> inline Vec3<T>::Vec3() {
    Set((T) 0);
}
template <typename T> inline Vec3<T>::Vec3(T d) {
    Set(d);
}
template <typename T> inline Vec3<T>::Vec3(T d0,T d1,T d2) {
    Set(d0,d1,d2);
}
template <typename T> inline Vec3<T>::Vec3( const Vec3 &src ) {
    Set(src);
}
template <typename T> inline Vec3<T>::Vec3(const T src[3]) {
    Set(src);
}
//-----------------------------------------------------------------------------
// Setters
template <typename T> inline void Vec3<T>::Set(T d) {
    val[0] = d;
    val[1] = d;
    val[2] = d;
}
template <typename T> inline void Vec3<T>::Set(T d0, T d1, T d2) {
    val[0] = d0;
    val[1] = d1;
    val[2] = d2;
}
template <typename T> inline void Vec3<T>::Set(const Vec3 &src) {
    val[0] = src[0];
    val[1] = src[1];
    val[2] = src[2];
}
template <typename T> inline void Vec3<T>::Set(const T src[3]) {
    val[0] = src[0];
    val[1] = src[1];
    val[2] = src[2];
}
//-----------------------------------------------------------------------------
// Overloading operators
template <typename T> inline T &Vec3<T>::operator[](int i) {

    return val[i];
}
template <typename T> inline T  Vec3<T>::operator[](int i) const {

    return val[i];
} 
template <typename T> inline T &Vec3<T>::operator()(int i) {

    return val[i];
}
template <typename T> inline T  Vec3<T>::operator()(int i) const {

    return val[i];
}
//-----------------------------------------------------------------------------
template <typename T> inline Vec3<T> &Vec3<T>::operator=(T d) {
    Set(d);
    return (*this);
}
template <typename T> inline Vec3<T> &Vec3<T>::operator=(const Vec3 &src) {
    Set(src);
    return (*this);
}
template <typename T> inline Vec3<T> &Vec3<T>::operator=(const T src[3]) {
    Set(src);
    return (*this);
}
//-----------------------------------------------------------------------------
template <typename T> inline Vec3<T> &Vec3<T>::operator+=(T d) {
    val[0] += d;
    val[1] += d;
    val[2] += d;
    return (*this);
}
template <typename T> inline Vec3<T> &Vec3<T>::operator-=(T d) {
    val[0] -= d;
    val[1] -= d;
    val[2] -= d;
    return (*this);
}
template <typename T> inline Vec3<T> &Vec3<T>::operator*=(T d) {
    val[0] *= d;
    val[1] *= d;
    val[2] *= d;
    return (*this);
}
template <typename T> inline Vec3<T> &Vec3<T>::operator/=(T d) {
    val[0] /= d;
    val[1] /= d;
    val[2] /= d;
    return (*this);
}
template <typename T> inline Vec3<T> &Vec3<T>::operator+=(const Vec3 &src) {
    val[0] += src[0];
    val[1] += src[1];
    val[2] += src[2];
    return (*this);
}
template <typename T> inline Vec3<T> &Vec3<T>::operator-=(const Vec3 &src) {
    val[0] -= src[0];
    val[1] -= src[1];
    val[2] -= src[2];
    return (*this);
}
//-----------------------------------------------------------------------------
// Methods
template <typename T> inline bool Vec3<T>::IsEqual(const Vec3 &src) const {
    return ( (val[0] == src[0]) && (val[1] == src[1]) && (val[2] == src[2]) );
}
template <typename T> inline void Vec3<T>::Normalize() {
    register T m = Mag();
    if (m != 0) (*this) /= m;
}
template <typename T> inline T Vec3<T>::L1Norm() const {
    return ABS(val[0]) + ABS(val[1]) + ABS(val[2]);
}
template <typename T> inline T Vec3<T>::L2Norm() const {
    return sqrt( SQ(val[0]) + SQ(val[1]) + SQ(val[2]) );
} 
template <typename T> inline T Vec3<T>::LInfNorm() const {
    return MAX( MAX(ABS(val[0]), ABS(val[1])), ABS(val[2]) );
}
template <typename T> inline T Vec3<T>::Mag() const {
    return sqrt( SQ(val[0]) + SQ(val[1]) + SQ(val[2]) );
}
template <typename T> inline T Vec3<T>::SqrMag() const {
    return ( SQ(val[0]) + SQ(val[1]) + SQ(val[2]) );
}
//-----------------------------------------------------------------------------
// Friend functions
template <typename T> inline Vec3<T> operator-(const Vec3<T> &a) {
    return Vec3<T>(-a[0],-a[1],-a[2]);
}
template <typename T> inline Vec3<T> operator-(const Vec3<T> &a,T b) {
    return Vec3<T>(a[0] - b, a[1] - b, a[2] - b);
}
template <typename T> inline Vec3<T> operator-(T a,const Vec3<T> &b) {
    return Vec3<T>(a - b[0], a - b[1], a - b[2]);
}
template <typename T> inline Vec3<T> operator-(const Vec3<T> &a,const Vec3<T> &b) {
    return Vec3<T>(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}
template <typename T> inline Vec3<T> operator+(const Vec3<T> &a,T b) {
    return Vec3<T>(a[0] + b, a[1] + b, a[2] + b);
}
template <typename T> inline Vec3<T> operator+(T a,const Vec3<T> &b) {
    return Vec3<T>(a + b[0], a + b[1], a + b[2]);
}
template <typename T> inline Vec3<T> operator+(const Vec3<T> &a,const Vec3<T> &b) {
    return Vec3<T>(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}
template <typename T> inline Vec3<T> operator*(const Vec3<T> &a,T b) {
    return Vec3<T>(a[0] * b, a[1] * b, a[2] * b);
}
template <typename T> inline Vec3<T> operator*(T a,const Vec3<T> &b) {
    return Vec3<T>(a * b[0], a * b[1], a * b[2]);
}
template <typename T> inline Vec3<T> operator/(const Vec3<T> &a,T b) {
    return Vec3<T>(a[0] / b, a[1] / b, a[2] / b);
}
template <typename T> inline T Mag(const Vec3<T> &a) {
    return a.Mag();
}
template <typename T> inline T SqrMag(const Vec3<T> &a) {
    return a.SqrMag();
}
template <typename T> inline T Dot(const Vec3<T> &a,const Vec3<T> &b) {
    return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}
template <typename T> inline Vec3<T> Cross(const Vec3<T> &a,const Vec3<T> &b) {
    return Vec3<T>(a[1] * b[2] - b[1] * a[2], a[2] * b[0] - b[2] * a[0], a[0] * b[1] - b[0] * a[1]);
}
template <typename T> inline T DotCross(const Vec3<T> &a,const Vec3<T> &b,const Vec3<T> &c) {
    return Dot(Cross(a,b),c);
}
//template <typename T> inline std::ostream &operator<<(std::ostream &str, Vec3<T> &vec){
//  str << "[" << vec[0] << ", " << vec[1] << ", " << vec[2] << "]";
//  return str;
//}

///////////////////////////////////////////////////////////////////////////////
typedef Vec3<int>       Vec3i;
typedef Vec3<unsigned int>      Vec3ui;
typedef Vec3<float>     Vec3f;
typedef Vec3<double>    Vec3d;
//typedef Vec3<real>      Vec3r;


}; // end namespace re

