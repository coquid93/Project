#pragma once

#include "Math/Math.h"

namespace respace {

///////////////////////////////////////////////////////////////////////////////
// Vec4 class declaration
///////////////////////////////////////////////////////////////////////////////
template <typename T> class Vec4 {
public:
    // open privilege for the use of glVertexfv, v.x, etc.
    union{
        struct { T x, y, z, w; };
        T val[4];
    };

public:
    // Constructors
    Vec4();
    Vec4(T d);
    Vec4(T d0, T d1, T d2, T d3);
    Vec4(const Vec4 &src);
    Vec4(const T src[4]);

    // Setter methods
    void Set(T d);
    void Set(T d0, T d1, T d2, T d3);
    void Set(const Vec4 &src);
    void Set(const T src[4]);
    void SetHSV (T h, T s, T v);            // HSL color model
    void GetHSV (T &h, T &s, T &v) const;   // HSL color model

    // Overloading operators
    T &operator[](int i);
    T  operator[](int i) const;
    T &operator()(int i);
    T  operator()(int i) const;

    Vec4 &operator=(T d);
    Vec4 &operator=(const Vec4 &src);
    Vec4 &operator=(const T src[4]);

    Vec4 &operator+=(T d);
    Vec4 &operator-=(T d);
    Vec4 &operator*=(T d);
    Vec4 &operator/=(T d);
    Vec4 &operator+=(const Vec4 &src);
    Vec4 &operator-=(const Vec4 &src);

    // Basic methods
    void Clear(){val[0] = val[1] = val[2] = val[3] = (T) 0;}
    bool IsEqual(const Vec4 &src) const;
    void Normalize();
    T   L1Norm() const;
    T   L2Norm() const;
    T   LInfNorm() const;
    T   Mag() const;
    T   SqrMag() const;

    // for the use of Scene graph implementation
    void Interpolate(const Vec4 &a, const Vec4 &b, const T t){
        *this = (1-t)*a + t*b;
    }
    
    // Friends functions (LHS and RHS arguments are both possible)
    template <typename S> friend Vec4<S> operator-(const Vec4<S> &a);
    template <typename S> friend Vec4<S> operator-(const Vec4<S> &a,S b);
    template <typename S> friend Vec4<S> operator-(S a,const Vec4<S> &b);
    template <typename S> friend Vec4<S> operator-(const Vec4<S> &a,const Vec4<S> &b);
    template <typename S> friend Vec4<S> operator+(const Vec4<S> &a,S b);
    template <typename S> friend Vec4<S> operator+(S a,const Vec4<S> &b);
    template <typename S> friend Vec4<S> operator+(const Vec4<S> &a,const Vec4<S> &b);
    template <typename S> friend Vec4<S> operator*(const Vec4<S> &a,S b);
    template <typename S> friend Vec4<S> operator*(S a,const Vec4<S> &b);
    template <typename S> friend Vec4<S> operator/(const Vec4<S> &a,S b);
    template <typename S> friend S Mag(const Vec4<S> &a);
    template <typename S> friend S SqrMag(const Vec4<S> &a);
    template <typename S> friend S Dot(const Vec4<S> &a,const Vec4<S> &b);
    //template <typename S> friend std::ostream &operator<<(std::ostream &str, Vec4<T> &vec);

};

///////////////////////////////////////////////////////////////////////////////
// Vec4 class inline definition
///////////////////////////////////////////////////////////////////////////////
// Constructors
template <typename T> inline Vec4<T>::Vec4() {
    Set((T) 0);
}
template <typename T> inline Vec4<T>::Vec4(T d) {
    Set(d);
}
template <typename T> inline Vec4<T>::Vec4(T d0,T d1,T d2,T d3) {
    Set(d0,d1,d2,d3);
}
template <typename T> inline Vec4<T>::Vec4( const Vec4 &src ) {
    Set(src);
}
template <typename T> inline Vec4<T>::Vec4(const T src[4]) {
    Set(src);
}
//-----------------------------------------------------------------------------
// Setters
template <typename T> inline void Vec4<T>::Set(T d) {
    val[0] = d;
    val[1] = d;
    val[2] = d;
    val[3] = d;
}
template <typename T> inline void Vec4<T>::Set(T d0, T d1, T d2, T d3) {
    val[0] = d0;
    val[1] = d1;
    val[2] = d2;
    val[3] = d3;
}
template <typename T> inline void Vec4<T>::Set(const Vec4 &src) {
    val[0] = src[0];
    val[1] = src[1];
    val[2] = src[2];
    val[3] = src[3];
}
template <typename T> inline void Vec4<T>::Set(const T src[4]) {
    val[0] = src[0];
    val[1] = src[1];
    val[2] = src[2];
    val[3] = src[3];
}
#define UNDEFINED_HUE 361
template <typename T> inline void Vec4<T>::SetHSV(T h, T s, T v){
    // Truncate
    if(s < 0)		s = (T) 0;
    else if(s > 1)	s = (T) 1;
    if(v < 0)		v = (T) 0;
    else if(v > 1)	v = (T) 1;
    if(h != UNDEFINED_HUE){
        if(h<0)			{ while(h<0)	h += 360; }
        else if(h>360)	{ while(h>360)	h -= 360; }
    }

    // Hue
    if(s == (T) 0){
        val[0] = val[1] = val[2] = v;
    }else{
        if(h == (T) 360) h = 0;
        h /= (T) 60;
        int	hi	= (int) h;	    // ¦¦h/60¦¥mod 6
        T	f = (T) h - hi;		// floating point
        T	p = (T) v*(1 - s);
        T	q = (T) v*(1 - f*s);
        T	t = (T) v*(1 - (1-f)*s);

        switch(hi){
            case 0: val[0] = v; val[1] = t; val[2] = p; break;
            case 1: val[0] = q; val[1] = v; val[2] = p; break;
            case 2: val[0] = p; val[1] = v; val[2] = t; break;
            case 3: val[0] = p; val[1] = q; val[2] = v; break;
            case 4: val[0] = t; val[1] = p; val[2] = v; break;
            case 5: val[0] = v; val[1] = p; val[2] = q; break;
        }
    }
}
template <typename T> inline void Vec4<T>::GetHSV(T &h, T &s, T &v) const {
    T max = (T) MAX(MAX(val[0],val[1]), val[2]);
    T min = (T) MIN(MIN(val[0],val[1]), val[2]);

    // saturation & value in [0,1]
    s = (max == (T) 0) ? (T) 0 : ((max-min)/max);
    v = max;

    // hue in [0,360] or UNDEFINED_HUE
    T R = val[0], G = val[1], B = val[2];
    if(s == (T) 0){
        h = UNDEFINED_HUE;
    }else{
        if(R == max)		h = (G-B)/(max-min);
        else if(G == max)	h = 2 + (B-R)/(max-min);
        else if(B == max)	h = 4 + (R-G)/(max-min);
        h *= 60;

        if(h < (T) 0)		h += 360;
    }
}
#undef UNDEFINED_HUE

//-----------------------------------------------------------------------------
// Overloading operators
template <typename T> inline T &Vec4<T>::operator[](int i) {

    return val[i];
}
template <typename T> inline T  Vec4<T>::operator[](int i) const {

    return val[i];
} 
template <typename T> inline T &Vec4<T>::operator()(int i) {

    return val[i];
}
template <typename T> inline T  Vec4<T>::operator()(int i) const {

    return val[i];
}
//-----------------------------------------------------------------------------
template <typename T> inline Vec4<T> &Vec4<T>::operator=(T d) {
    Set(d);
    return (*this);
}
template <typename T> inline Vec4<T> &Vec4<T>::operator=(const Vec4 &src) {
    Set(src);
    return (*this);
}
template <typename T> inline Vec4<T> &Vec4<T>::operator=(const T src[4]) {
    Set(src);
    return (*this);
}
//-----------------------------------------------------------------------------
template <typename T> inline Vec4<T> &Vec4<T>::operator+=(T d) {
    val[0] += d;
    val[1] += d;
    val[2] += d;
    val[3] += d;
    return (*this);
}
template <typename T> inline Vec4<T> &Vec4<T>::operator-=(T d) {
    val[0] -= d;
    val[1] -= d;
    val[2] -= d;
    val[3] -= d;
    return (*this);
}
template <typename T> inline Vec4<T> &Vec4<T>::operator*=(T d) {
    val[0] *= d;
    val[1] *= d;
    val[2] *= d;
    val[3] *= d;
    return (*this);
}
template <typename T> inline Vec4<T> &Vec4<T>::operator/=(T d) {
    val[0] /= d;
    val[1] /= d;
    val[2] /= d;
    val[3] /= d;
    return (*this);
}
template <typename T> inline Vec4<T> &Vec4<T>::operator+=(const Vec4 &src) {
    val[0] += src[0];
    val[1] += src[1];
    val[2] += src[2];
    val[3] += src[3];
    return (*this);
}
template <typename T> inline Vec4<T> &Vec4<T>::operator-=(const Vec4 &src) {
    val[0] -= src[0];
    val[1] -= src[1];
    val[2] -= src[2];
    val[3] -= src[3];
    return (*this);
}
//-----------------------------------------------------------------------------
// Methods
template <typename T> inline bool Vec4<T>::IsEqual(const Vec4 &src) const {
    return ( (val[0] == src[0]) && (val[1] == src[1]) && (val[2] == src[2]) && (val[3] == src[3]) );
}
template <typename T> inline void Vec4<T>::Normalize() {
    register T m = Mag();
    if (m != 0) (*this) /= m;
}
template <typename T> inline T Vec4<T>::L1Norm() const {
    return ABS(val[0]) + ABS(val[1]) + ABS(val[2]) + ABS(val[3]);
}
template <typename T> inline T Vec4<T>::L2Norm() const {
    return sqrt( SQ(val[0]) + SQ(val[1]) + SQ(val[2]) + SQ(val[3]) );
} 
template <typename T> inline T Vec4<T>::LInfNorm() const {
    return MAX( MAX( MAX(ABS(val[0]), ABS(val[1])), ABS(val[2]) ), ABS(val[3]) );
}
template <typename T> inline T Vec4<T>::Mag() const {
    return sqrt( SqrMag() );
}
template <typename T> inline T Vec4<T>::SqrMag() const {
    return ( SQ(val[0]) + SQ(val[1]) + SQ(val[2]) + SQ(val[3]) );
}
//-----------------------------------------------------------------------------
// Friend functions
template <typename T> inline Vec4<T> operator-(const Vec4<T> &a) {
    return Vec4<T>(-a[0],-a[1],-a[2],-a[3]);
}
template <typename T> inline Vec4<T> operator-(const Vec4<T> &a,T b) {
    return Vec4<T>(a[0] - b, a[1] - b, a[2] - b, a[3] - b);
}
template <typename T> inline Vec4<T> operator-(T a,const Vec4<T> &b) {
    return Vec4<T>(a - b[0], a - b[1], a - b[2], a - b[3]);
}
template <typename T> inline Vec4<T> operator-(const Vec4<T> &a,const Vec4<T> &b) {
    return Vec4<T>(a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]);
}
template <typename T> inline Vec4<T> operator+(const Vec4<T> &a,T b) {
    return Vec4<T>(a[0] + b, a[1] + b, a[2] + b, a[3] + b);
}
template <typename T> inline Vec4<T> operator+(T a,const Vec4<T> &b) {
    return Vec4<T>(a + b[0], a + b[1], a + b[2], a + b[3]);
}
template <typename T> inline Vec4<T> operator+(const Vec4<T> &a,const Vec4<T> &b) {
    return Vec4<T>(a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]);
}
template <typename T> inline Vec4<T> operator*(const Vec4<T> &a,T b) {
    return Vec4<T>(a[0] * b, a[1] * b, a[2] * b, a[3] * b);
}
template <typename T> inline Vec4<T> operator*(T a,const Vec4<T> &b) {
    return Vec4<T>(a * b[0], a * b[1], a * b[2], a * b[3]);
}
template <typename T> inline Vec4<T> operator/(const Vec4<T> &a,T b) {
    return Vec4<T>(a[0] / b, a[1] / b, a[2] / b, a[3] / b);
}
template <typename T> inline T Mag(const Vec4<T> &a) {
    return a.Mag();
}
template <typename T> inline T SqrMag(const Vec4<T> &a) {
    return a.SqrMag();
}
template <typename T> inline T Dot(const Vec4<T> &a,const Vec4<T> &b) {
    return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]);
}
//template <typename T> inline std::ostream &operator<<(std::ostream &str, Vec4<T> &vec){
//  str << "[" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << val[3] << "]";
//  return str;
//}

///////////////////////////////////////////////////////////////////////////////
typedef Vec4<int>       Vec4i;
typedef Vec4<unsigned int>      Vec4ui;
typedef Vec4<float>     Vec4f;
typedef Vec4<double>    Vec4d;
//typedef Vec4<real>      Vec4r;


}; // end namespace re

