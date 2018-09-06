#pragma once

#include "Math/Math.h"

namespace respace {

///////////////////////////////////////////////////////////////////////////////
// Vec2 class declaration
///////////////////////////////////////////////////////////////////////////////
template <typename T> class Vec2 {
public:
    // open privilege for the use of glVertexfv, v.x, etc.
    union{
        struct{ T x, y; };
        T val[2];
    };

public:
    // Constructors
    Vec2();
    Vec2(T d);
    Vec2(T d0, T d1);
    Vec2(const Vec2 &src);
    Vec2(const T src[2]);

    // Setter methods
    void Set(T d);
    void Set(T d0, T d1);
    void Set(const Vec2 &src);
    void Set(const T src[2]);

    // Overloading operators
    T &operator[](int i);       // Getter by reference (no const for later setter use)
    T  operator[](int i) const; // Getter by value
    T &operator()(int i);       // Getter by reference (no const for later setter use)
    T  operator()(int i) const; // Getter by value

    Vec2 &operator=(T d);
    Vec2 &operator=(const Vec2 &src);
    Vec2 &operator=(const T src[2]);

    Vec2 &operator+=(T d);
    Vec2 &operator-=(T d);
    Vec2 &operator*=(T d);
    Vec2 &operator/=(T d);
    Vec2 &operator+=(const Vec2 &src);
    Vec2 &operator-=(const Vec2 &src);
    
    // Basic methods
    void Clear(){ val[0] = val[1] = (T) 0;}
    bool IsEqual(const Vec2 &src) const;
    void Normalize();
    T    L1Norm() const;
    T    L2Norm() const;
    T    LInfNorm() const;
    T    Mag() const;
    T    SqrMag() const;

    // Friends functions (LHS and RHS arguments are both possible)
    // Notice for usage: const Vec2 and scalar value
    template <typename S> friend Vec2<S> operator-(const Vec2<S> &a);
    template <typename S> friend Vec2<S> operator-(const Vec2<S> &a,S b);
    template <typename S> friend Vec2<S> operator-(S a,const Vec2<S> &b);
    template <typename S> friend Vec2<S> operator-(const Vec2<S> &a,const Vec2<S> &b);
    template <typename S> friend Vec2<S> operator+(const Vec2<S> &a,S b);
    template <typename S> friend Vec2<S> operator+(S a,const Vec2<S> &b);
    template <typename S> friend Vec2<S> operator+(const Vec2<S> &a,const Vec2<S> &b);
    template <typename S> friend Vec2<S> operator*(const Vec2<S> &a,S b);
    template <typename S> friend Vec2<S> operator*(S a,const Vec2<S> &b);
    template <typename S> friend Vec2<S> operator/(const Vec2<S> &a,S b);
    template <typename S> friend T Mag(const Vec2<S> &a);
    template <typename S> friend T SqrMag(const Vec2<S> &a);
    template <typename S> friend T Dot(const Vec2<S> &a,const Vec2<S> &b);
    template <typename S> friend T Cross(const Vec2<S> &a,const Vec2<S> &b);
    //template <typename S> friend std::ostream &operator<<(std::ostream &str, Vec2<S> &vec);
};

///////////////////////////////////////////////////////////////////////////////
// Vec2 class inline definition
///////////////////////////////////////////////////////////////////////////////
// Constructors
template <typename T> inline Vec2<T>::Vec2() {
    Set((T) 0);
}
template <typename T> inline Vec2<T>::Vec2(T d) {
    Set(d);
}
template <typename T> inline Vec2<T>::Vec2(T d0,T d1) {
    Set(d0,d1);
}
template <typename T> inline Vec2<T>::Vec2(const Vec2 &src) {
    Set(src);
}
template <typename T> inline Vec2<T>::Vec2(const T src[2]) {
    Set(src);
}
//-----------------------------------------------------------------------------
// Setters
template <typename T> inline void Vec2<T>::Set(T d) {
    val[0] = val[1] = d;
} 
template <typename T> inline void Vec2<T>::Set(T d0, T d1) {
    val[0] = d0;
    val[1] = d1;
}
template <typename T> inline void Vec2<T>::Set(const Vec2 &src) {
    val[0] = src[0];
    val[1] = src[1];
} 
template <typename T> inline void Vec2<T>::Set(const T src[2]) {
    val[0] = src[0];
    val[1] = src[1];
}
//-----------------------------------------------------------------------------
// Overloading operators
template <typename T> inline T &Vec2<T>::operator[](int i) {

    return val[i];
}
template <typename T> inline T  Vec2<T>::operator[](int i) const {

    return val[i];
}
template <typename T> inline T &Vec2<T>::operator()(int i) {

    return val[i];
}
template <typename T> inline T  Vec2<T>::operator()(int i) const {

    return val[i];
}
//-----------------------------------------------------------------------------
template <typename T> inline Vec2<T> &Vec2<T>::operator=(T d) {
    Set(d);
    return (*this);
}
template <typename T> inline Vec2<T> &Vec2<T>::operator=(const Vec2 &src) {
    Set(src);
    return (*this);
}
template <typename T> inline Vec2<T> &Vec2<T>::operator=(const T src[2]) {
    Set(src);
    return (*this);
}
//-----------------------------------------------------------------------------
template <typename T> inline Vec2<T> &Vec2<T>::operator+=(T d) {
    val[0] += d;
    val[1] += d;
    return (*this);
}   
template <typename T> inline Vec2<T> &Vec2<T>::operator-=(T d) {
    val[0] -= d;
    val[1] -= d;
    return (*this);
}
template <typename T> inline Vec2<T> &Vec2<T>::operator*=(T d) {
    val[0] *= d;
    val[1] *= d;
    return (*this);
}
template <typename T> inline Vec2<T> &Vec2<T>::operator/=(T d) {
    val[0] /= d;
    val[1] /= d;
    return (*this);
}
template <typename T> inline Vec2<T> &Vec2<T>::operator+=(const Vec2 &src) {
    val[0] += src[0];
    val[1] += src[1];
    return (*this);
}
template <typename T> inline Vec2<T> &Vec2<T>::operator-=(const Vec2 &src) {
    val[0] -= src[0];
    val[1] -= src[1];
    return (*this);
}
//-----------------------------------------------------------------------------
// Methods
template <typename T> inline bool Vec2<T>::IsEqual(const Vec2 &src) const {
    return ( (val[0] == src[0]) && (val[1] == src[1]) );
}
template <typename T> inline void Vec2<T>::Normalize() {
    register T m = Mag();
    if (m != 0) (*this) /= m;
}
template <typename T> inline T Vec2<T>::L1Norm() const {
    return ( ABS(val[0]) + ABS(val[1]) );
}
template <typename T> inline T Vec2<T>::L2Norm() const {
    return (T)sqrt( SQ(val[0]) + SQ(val[1]) );
}
template <typename T> inline T Vec2<T>::LInfNorm() const {
    return MAX(ABS(val[0]), ABS(val[1]));
}
template <typename T> inline T Vec2<T>::Mag() const {
    return (T)sqrt( SQ(val[0]) + SQ(val[1]) );
}
template <typename T> inline T Vec2<T>::SqrMag() const {
    return ( SQ(val[0]) + SQ(val[1]) );
}
//-----------------------------------------------------------------------------
// Friend functions (inline should be specified)
template <typename T> inline Vec2<T> operator-(const Vec2<T> &a) {
    return Vec2<T>(-a[0],-a[1]);
}
template <typename T> inline Vec2<T> operator-(const Vec2<T> &a,T b) {
    return Vec2<T>(a[0] - b, a[1] - b);
}
template <typename T> inline Vec2<T> operator-(T a,const Vec2<T> &b) {
    return Vec2<T>(a - b[0], a - b[1]);
}
template <typename T> inline Vec2<T> operator-(const Vec2<T> &a,const Vec2<T> &b) {
    return Vec2<T>(a[0] - b[0], a[1] - b[1]);
}
template <typename T> inline Vec2<T> operator+(const Vec2<T> &a,T b) {
    return Vec2<T>(a[0] + b, a[1] + b);
}
template <typename T> inline Vec2<T> operator+(T a,const Vec2<T> &b) {
    return Vec2<T>(a + b[0], a + b[1]);
}
template <typename T> inline Vec2<T> operator*(const Vec2<T> &a,T b) {
    return Vec2<T>(a[0] * b, a[1] * b);
}
template <typename T> inline Vec2<T> operator+(const Vec2<T> &a,const Vec2<T> &b) {
    return Vec2<T>(a[0] + b[0], a[1] + b[1]);
}
template <typename T> inline Vec2<T> operator*(T a,const Vec2<T> &b) {
    return Vec2<T>(a * b[0], a * b[1]);
}
template <typename T> inline Vec2<T> operator/(const Vec2<T> &a,T b) {
    return Vec2<T>(a[0] / b, a[1] / b);
}
template <typename T> inline T Mag(const Vec2<T> &a) {
    return a.Mag();
}
template <typename T> inline T SqrMag(const Vec2<T> &a) {
    return a.SqrMag();
}
template <typename T> inline T Dot(const Vec2<T> &a,const Vec2<T> &b) {
    return (a[0] * b[0] + a[1] * b[1]);
}
template <typename T> inline T Cross(const Vec2<T> &a,const Vec2<T> &b) {
    return (a[0] * b[1] - b[0] * a[1]);
}
//template <typename T> inline std::ostream &operator<<(std::ostream &str, Vec2<T> &vec){
//  str << "[" << vec[0] << ", " << vec[1] << "]";
//  return str;
//}

///////////////////////////////////////////////////////////////////////////////
typedef Vec2<int>       Vec2i;
typedef Vec2<unsigned int>      Vec2ui;
typedef Vec2<float>     Vec2f;
typedef Vec2<double>    Vec2d;
//typedef Vec2<real>      Vec2r;


}; // end namespace re
