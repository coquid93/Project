#pragma once

#include <cmath>
#include <stdio.h>

#ifndef SIGN
#define SIGN(a)				( ((a)>0) ? 1 : -1 )
#endif
#ifndef MIN
#define MIN(a,b)			( ((a)<(b)) ? (a) : (b) )
#endif
#ifndef MAX
#define MAX(a,b)			( ((a)>(b)) ? (a) : (b) )
#endif
#ifndef ABS
#define ABS(x)				( ((x)>=0.0) ? (x) :-(x) )
#endif
#ifndef ABSMIN
#define ABSMIN(a,b)			( ((a*a)<(b*b)) ? (a) : (b) )
#endif
#ifndef ABSMAX
#define ABSMAX(a,b)			( ((a*a)>(b*b)) ? (a) : (b) )
#endif
#ifndef SQ
#define SQ(x)				( (x)*(x) )
#endif
#ifndef CB
#define CB(x)				( (x)*(x)*(x) )
#endif
#ifndef SWAP
#define SWAP(a,a0)			{ double *tmp = a; a = a0; a0 = tmp; }
#endif
#ifndef IS_TOO_SMALL
#define IS_TOO_SMALL(x)		((x)>-1.0e-10 && (x)<1.0e-10)
#endif
#ifndef TRIM
#define TRIM(x,a,b)			{ if((x)<(a)) (x) = (a); else if((x)>(b)) (x) = (b); }
#endif
#ifndef TRIML
#define TRIML(x,a)			{ if((x)<(a)) (x) = (a); }
#endif
#ifndef TRIMR
#define TRIMR(x,b)			{ if((x)>(b)) (x) = (b); }
#endif
#ifndef SET_TRIM
#define SET_TRIM(x,x0,a,b)	{ x=x0; if((x)<(a)) (x) = (a); else if((x)>(b)) (x) = (b); }
#endif
#ifndef SET_TRIML
#define SET_TRIML(x,x0,a)	{ x=x0; if((x)<(a)) (x) = (a); }
#endif
#ifndef SET_TRIMR
#define SET_TRIMR(x,x0,b)	{ x=x0; if((x)>(b)) (x) = (b); }
#endif
namespace respace {

///////////////////////////////////////////////////////////////////////////////
// Math class declaration
// - Static methods are provided for the use of Math::Sin(ang)
// - Random Number Generator(RNG) recommended by George Marsaglia.
// - Gaussian deviates by Box-Muller transformation method.
// - Fast trigonometric functions provided by Eberly.
///////////////////////////////////////////////////////////////////////////////
template <typename T> class Math{
public:
    static const T      pi;
    static const T      twopi;
    static const T      halfpi;
    static const T      inv_pi;
    static const T      inv_twopi;

    static const double inf_d;
    static const float  inf_f;
    static const double eps_d;
    static const float  eps_f;
    static const float  max_flt;
    static const int    max_int;
    static const int    min_int;
    static const unsigned int   max_uint;
public:
    // Random number generator
    static void     SetSeed(unsigned int w=1, unsigned int z=1, unsigned int jsr=1, unsigned int jcong=1);
    static unsigned int     RandInt();
    static T        Rand0_1();      // [0,1)
    static T        Rand_11();      // [-1,1)
    static T        Rand(T a, T b); // [a,b)
    static T        RandGaussian(T mean=0, T variance=1); // Gauss deviated data 
    static T        RandGaussian_11();
    static void     Write4Diehard();

    // Perlin Noise (Improved Noise by Ken Perlin (Siggraph 2002)
    // input range [0, 10] guarantee noise-like shape
    // input range [0, 100] guarantee white-noise-like shape
    // output value in [-1,1]. The (75%) lies in [-0.5,0.5]
    static T        Noise (T x, T y, T z);

    // Misc.
    static T        MaxSquare(const T v00, const T v10, const T v01, const T v11);
    static T        MinSquare(const T v00, const T v10, const T v01, const T v11);
    static T        MaxCube(  const T v000, const T v100,
                              const T v010, const T v110, 
                              const T v001, const T v101, 
                              const T v011, const T v111);
    static T        MinCube(  const T v000, const T v100,
                              const T v010, const T v110,
                              const T v001, const T v101, 
                              const T v011, const T v111);
    static T        Lerp(const T y0, const T y1, const T x0, const T x1, T x) { return y0 + (y1-y0)*(x-x0)/(x1-x0); }
    static T        Lerp(T a, T b, T t) { return (1-t)*a + t*b; }
    static T        Bilerp(   const T v00, const T v10,
                              const T v01, const T v11,
                              T fx, T fy);
    static T        Trilerp(  const T v000, const T v100,
                              const T v010, const T v110,
                              const T v001, const T v101,
                              const T v011, const T v111, 
                              T fx, T fy, T fz);
    static T        Quadlerp( const T v0000, const T v1000, 
                              const T v0100, const T v1100, 
                              const T v0010, const T v1010, 
                              const T v0110, const T v1110, 
                              const T v0001, const T v1001, 
                              const T v0101, const T v1101, 
                              const T v0011, const T v1011, 
                              const T v0111, const T v1111, 
                              T fx, T fy, T fz, T ft);
    static T        Pow2(int p);
    static T        Bias(T t, T bias);
    static T        Gain(T t, T gain);
    static T        SmoothStep(T t, T lo, T hi);
    static T        Clamp(T t, T lo, T hi);
    static T        Ceil(T t);
    static T        AdjustAngle(T rad, T lo=0, T hi = 8.0*atan(1.0) /*2*pi*/);
    static T        Pythagoras(T a, T b); // (a^2 + b^2)^1/2
    static T        Gaussian(T mu, T sigma, T x);
    static T        GaussianCoeff(T mu, T sigma); // 1/sigma*sqrt(2*pi)

    // Trigonometric functions
    static T        Sin(T ang);     // The input must be in [0,pi/2]. //  max error = 1.9e-08, speed up = 2.8
    static T        Cos(T ang);     // The input must be in [0,pi/2]. //  max error = 6.5e-09, speed up = 2.8
    static T        Tan(T ang);     // The input must be in [0,pi/4]. //  max error = 1.9e-08, speed up = 3.4
    static T        SinInv(T val);  // The input must be in [0,1].    //  max error = 1.4e-07, speed up = 5.5
    static T        CosInv(T val);  // The input must be in [0,1].    //  max error = 1.4e-07, speed up = 5.7   
    static T        TanInv(T val);  // The input must be in [-1,1].   //  max error = 2.3e-08, speed up = 1.8
    static T        ExpNeg(T val);  // Fast approximations to exp(-x).  The input x must be in [0,infinity).
                                    //  max error = 0.00000025, speed up = 17.3

private:
    static unsigned int     seed_w;
    static unsigned int     seed_z;
    static unsigned int     seed_jsr;
    static unsigned int     seed_jcong;
    static unsigned int     perm[257];      // 256+1
    static T        _Fade(T t);
    static T        _Grad(unsigned int hash, T x, T y, T z);
};

///////////////////////////////////////////////////////////////////////////////
// Math inline definition
///////////////////////////////////////////////////////////////////////////////
// 32 bit unsigned integer [0, 4294967295]
// the same as unsigned long
// Default Seeds
#define SEED_W      362436069u
#define SEED_Z      521288629u
#define SEED_SHR    123456789u
#define SEED_CONG   380116160u

// MWC(Multiply-With-Carrier)
// x(n) = a*x(n-1) + (carry mod 2^16): x(n-1) masked to be <= 65535
// MWC returns value made by (the bottom half of seed_w + the top half of seed_z)
#define SEED_WNEW   ((seed_w=18000*(seed_w&65535)+(seed_w>>16))&65535)
#define SEED_ZNEW   ((seed_z=30903*(seed_z&65535)+(seed_z>>16))<<16)
#define RNG_MWC     (SEED_WNEW + SEED_ZNEW)

// SHR3(3-Shift-Register)
#define RNG_SHR3    (seed_jsr=(seed_jsr=(seed_jsr=seed_jsr^(seed_jsr<<17))^(seed_jsr>>13))^(seed_jsr<<5))

// CONG(Congruential)
#define RNG_CONG    (seed_jcong=69069*seed_jcong+1234567)

// KISS(Keep-It-Simple-Stupid)
#define RNG_KISS    ((RNG_MWC^RNG_CONG)+RNG_SHR3)

// Static variables are loaded once when application starts.
// NOTE/TODO: Unless this class is templated, this produces link errors ?!
template <typename T> unsigned int Math<T>::seed_w      = SEED_W;
template <typename T> unsigned int Math<T>::seed_z      = SEED_Z;
template <typename T> unsigned int Math<T>::seed_jsr    = SEED_SHR;
template <typename T> unsigned int Math<T>::seed_jcong  = SEED_CONG;
template <typename T> unsigned int Math<T>::perm[] = {
    151,160,137, 91, 90, 15,131, 13,201, 95, 96, 53,194,233,  7,225,140, 36,103, 30,
    69,142,  8, 99, 37,240, 21, 10, 23,190,  6,148,247,120,234, 75,  0, 26,197, 62,
    94,252,219,203,117, 35, 11, 32, 57,177, 33, 88,237,149, 56, 87,174, 20,125,136,
    171,168, 68,175, 74,165, 71,134,139, 48, 27,166, 77,146,158,231, 83,111,229,122,
    60,211,133,230,220,105, 92, 41, 55, 46,245, 40,244,102,143, 54, 65, 25, 63,161,
    1,216, 80, 73,209, 76,132,187,208, 89, 18,169,200,196,135,130,116,188,159, 86,
    164,100,109,198,173,186,  3, 64, 52,217,226,250,124,123,  5,202, 38,147,118,126,
    255, 82, 85,212,207,206, 59,227, 47, 16, 58, 17,182,189, 28, 42,223,183,170,213,
    119,248,152,  2, 44,154,163, 70,221,153,101,155,167, 43,172,  9,129, 22, 39,253,
    19, 98,108,110, 79,113,224,232,178,185,112,104,218,246, 97,228,251, 34,242,193,
    238,210,144, 12,191,179,162,241, 81, 51,145,235,249, 14,239,107, 49,192,214, 31,
    181,199,106,157,184, 84,204,176,115,121, 50, 45,127,  4,150,254,138,236,205, 93,
    222,114, 67, 29, 24, 72,243,141,128,195, 78, 66,215, 61,156,180, 
    151
};
template <typename T> 
inline T Math<T>::_Fade ( T t ) {
    // Interpolation curve
    // f'(1)=f''(1)=f'''(1)=0
    return (t*t*t*(t*(t*6-15)+10));
}
template <typename T>
inline T Math<T>::_Grad ( unsigned int hash, T x, T y, T z ) {
    // Compute dot product of (x,y,z) and 12+4 directions.
    // (1,1,0),(-1,1,0),(1,-1,0),(-1,-1,0) (x+y),(-x+y),(x-y),(-x-y)
    // (1,0,1),(-1,0,1),(1,0,-1),(-1,0,-1) (x+z),(-x+z),(x-z),(-x-z)
    // (0,1,1),(0,-1,1),(0,1,-1),(0,-1,-1) (y+z),(-y,z),(y-z),(-y-z)
    // (1,1,0),(-1,1,0),(0,-1,1),(0,-1,-1) (4 additions)
    unsigned int h = hash & 15; // 15=0000[1111]. This restricts hash to [0~15]
    T u = (h<8) ? x : y;
    T v = (h<4) ? y : ((h==12||h==14) ? x : z);
    return ((h&1)==0 ? u :-u) + ((h&2)==0 ? v : -v);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T> 
inline void Math<T>::SetSeed(unsigned int w, unsigned int z, unsigned int jsr, unsigned int jcong){
    // The same seed produces the same pattern of random number.
    // Nothing is initiated if zero.
    seed_w      = (w     != 0) ? w     : SEED_W;
    seed_z      = (z     != 0) ? z     : SEED_Z;
    seed_jsr    = (jsr   != 0) ? jsr   : SEED_SHR;
    seed_jcong  = (jcong != 0) ? jcong : SEED_CONG;
}

template <typename T> 
inline unsigned int Math<T>::RandInt(){
    //return RNG_KISS;  // 10 times longer period than RNG_MWC
    return RNG_MWC;
}

template <typename T> 
inline T Math<T>::Rand0_1(){
    // [0,4294967295(=2^32-1)] for unsigned int(long)
    // for open interval (RNG_KISS+1)/(2^32+1). 
    // However use ((RNG_KISS)/(2^32)).
    //return (T)((RandInt() + 1)*((T)2.328306435996595e-10));
    return (T)(RandInt()*((T)2.328306436538696e-10));
}

template <typename T> 
inline T Math<T>::Rand_11(){
    // [0,4294967295(=2^32-1)] for unsigned int(long)
    // [–2147483648, 2147483647(=2^31-1)] for int(long)
    // for open interval ((int)(RNG_KISS+1))/(2^31+1). 
    // However use ((int)RNG_KISS)/(2^31).
    return (T)((int)RandInt())*((T)4.656612873077393e-10);
}

template <typename T> 
inline T Math<T>::Rand(T min, T max){
    return (T)( min + (max-min)*Rand0_1() );
}

template <typename T> 
inline T Math<T>::RandGaussian(T mean, T variance){
    // Box-Muller transformation method produces 2 Gaussian distributions.
    // So, we return one and save the other for the next time.
    static bool iset=false; // flag says we have the saved one. static variable.
    static T gset;          // static variable.

    if(!iset){
        T v1,v2,Rsqr,temp;
        // Take v1, v2 in unit circle
        do{
            v1 = Rand_11();
            v2 = Rand_11();
            Rsqr = SQ(v1) + SQ(v2);
        }while(Rsqr >= 1.0 || Rsqr == 0);
        
        // Box-Muller transformation formula gives normal distribution.
        // Then modify to have mean, variance we want.
        temp = sqrt((T)-2.0*log(Rsqr)/Rsqr);
        gset = v1*temp*variance + mean; // v1 = (x-mean/variance)
        iset = true;

        return v2*temp*variance + mean;

    }else{
        iset = false;       // we used the saved one.
        return gset;
    }
}

template <typename T> 
inline T Math<T>::RandGaussian_11(){
    T x = RandGaussian();
    // 99.99%
    if(x > 4)           x = 4;
    else if(x < -4)     x = -4;
    return (T) 0.25*x;
}

template <typename T> 
inline void Math<T>::Write4Diehard(){
    unsigned int i,j;
    FILE *fp;
    if(fopen_s(&fp,"test.bin","wb") == 0){
        for(i=0; i<3000000; i++){
            j = RandInt();
            fwrite(&j, sizeof(unsigned int), 1, fp);
        }
        fclose(fp);
    }
}

template <typename T>
inline T Math<T>::Noise(T x, T y, T z) {
    x = ABS(x); y = ABS(y); z = ABS(z);
    int X = ((int)x) & 255;
    int Y = ((int)y) & 255;
    int Z = ((int)z) & 255;
    x -= (int)x; y -= (int)y; z -= (int)z;      // floor
    T u = _Fade(x), v = _Fade(y), w = _Fade(z); // interpolation
    int A  = (perm[X  ]+Y) & 255;   // Hash coordinates of the 8 cube corners.
    int AA = (perm[A  ]+Z) & 255;   // Note we need (256+1) size array perm.
    int AB = (perm[A+1]+Z) & 255;
    int B  = (perm[X+1]+Y) & 255;
    int BA = (perm[B  ]+Z) & 255;
    int BB = (perm[B+1]+Z) & 255;
    // Blended result from 8 corners of cube
    return  Lerp(Lerp(Lerp(_Grad(perm[AA  ],x,y  ,z  ),_Grad(perm[BA  ],x-1,y  ,z  ),u),
                      Lerp(_Grad(perm[AB  ],x,y-1,z  ),_Grad(perm[BB  ],x-1,y-1,z  ),u), v),
                 Lerp(Lerp(_Grad(perm[AA+1],x,y  ,z-1),_Grad(perm[BA+1],x-1,y  ,z-1),u),
                      Lerp(_Grad(perm[AB+1],x,y-1,z-1),_Grad(perm[BB+1],x-1,y-1,z-1),u), v),
                 w );
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
inline T Math<T>::MaxSquare(const T v00, const T v10, const T v01, const T v11)
{
  return MAX( MAX(v00, v10), MAX(v01, v11) );
}

template <typename T>
inline T Math<T>::MinSquare(const T v00, const T v10, const T v01, const T v11)
{
  return MIN( MIN(v00, v10), MIN(v01, v11) );
}

template <typename T>
inline T Math<T>::MaxCube(const T v000, const T v100, const T v010, const T v110, const T v001, const T v101, const T v011, const T v111)
{
  return MAX ( MAX( MAX(v000, v100), MAX(v010, v110) ), MAX( MAX(v001, v101), MAX(v011, v111) ) );
}

template <typename T>
inline T Math<T>::MinCube(const T v000, const T v100, const T v010, const T v110, const T v001, const T v101, const T v011, const T v111)
{
  return MIN ( MIN( MIN(v000, v100), MIN(v010, v110) ), MIN( MIN(v001, v101), MIN(v011, v111) ) );
}

template <typename T> 
inline T Math<T>::Bilerp(const T v00, const T v10, 
                         const T v01, const T v11,
                         T fx, T fy)
{
  return Lerp(Lerp(v00,v10, fx),
              Lerp(v01,v11, fx),
              fy);
}
template <typename T>
inline T Math<T>::Trilerp(const T v000, const T v100,
                          const T v010, const T v110,
                          const T v001, const T v101,
                          const T v011, const T v111,
                          T fx, T fy, T fz)
{
  return Lerp(Bilerp(v000, v100, v010, v110, fx, fy), 
              Bilerp(v001, v101, v011, v111, fx, fy), 
              fz);
}
template <typename T>
inline T Math<T>::Quadlerp(const T v0000, const T v1000, 
                           const T v0100, const T v1100,
                           const T v0010, const T v1010,
                           const T v0110, const T v1110,
                           const T v0001, const T v1001,
                           const T v0101, const T v1101,
                           const T v0011, const T v1011,
                           const T v0111, const T v1111,
                           T fx, T fy, T fz, T ft)
{
  return Lerp(Trilerp(v0000, v1000, v0100, v1100, v0010, v1010, v0110, v1110, fx, fy, fz), 
              Trilerp(v0001, v1001, v0101, v1101, v0011, v1011, v0111, v1111, fx, fy, fz), 
              ft);
}
///////////////////////////////////////////////////////////////////////////////
// BIAS(t;a) = t^(-log2(a))                           [__/ a=0.05,  /^^a=0.95]
// GAIN(t;a) = 1/2*bias(2*t,a) or 1-1/2*bias(2-2*t,a) [_/^^a=0.05, __/ a=0.95]
// Here, fast alternative to Perlin's bias and gain function.     /
// See Graphics Gems. pp401-403.
template <typename T> 
inline T Math<T>::Bias(T t, T a){ // a: bias
    if(t < 0) t=0; 
    if(t > 1) t=1;
    T aInv = (T) 1.0/a;
    //return pow(t, -1.442695*log(a));
    return t/((aInv-2)*(1-t)+1);
}

template <typename T> 
inline T Math<T>::Gain(T t, T a){ // a: gain
    if(t < 0) t=0; 
    if(t > 1) t=1;
    T aInv = (T) 1.0/a;
    if(t < 0.5){
        return 0.5*Bias(2*t,a);
    }else{
        return 1-0.5*Bias(2-2*t,a);
    }
}

template <typename T>
inline T Math<T>::SmoothStep(T t, T lo, T hi){
    T v = (t - lo)/(hi - lo);
    if ( v > 1.0 )      return (T) 1.0;
    else if ( v < 0.0 ) return (T) 0.0;
    return (T) ( 3.0 - 2.0*t )*t*t;
}

template <typename T>
inline T Math<T>::Clamp(T t, T lo, T hi){
    if(t > hi)      return hi;
    else if(t < lo) return lo;
    return t;
}

template <typename T>
inline T Math<T>::Ceil(T t){
    return (T) ceil(t);
}

template <typename T> 
inline T Math<T>::AdjustAngle(T rad, T lo, T hi){
    if (rad < lo) {
        do { rad += 8.0*atan(1.0); /*2*pi*/ } while (rad < lo);
    } else if (rad > hi) {
        do { rad -= 8.0*atan(1.0); /*2*pi*/ } while (rad > hi);
    }
    return rad;
}

template <typename T> 
inline T Math<T>::Pythagoras(T a, T b){
    // Compute (a^2 + b^2)^1/2 without destructive underflow or overflow
    T absa = ABS(a), absb = ABS(b);
    return (absa > absb) ? absa*sqrt(1.0 + SQ(absb/absa)) : (absb == 0.0) ? 0.0 : (absb*sqrt(1.0 + SQ(absa/absb)));
}

template <typename T>
inline T Math<T>::Gaussian(T mu, T sigma, T x){
    return (T) exp( -(T) 0.5*SQ(x-mu) / (T) SQ(sigma) );
}

template <typename T>
inline T Math<T>::GaussianCoeff(T mu, T sigma){
    return (T) 1.0 / (T) (sigma * sqrt( 8.0*atan(1.0) /*2*pi*/ ));
}

///////////////////////////////////////////////////////////////////////////////
template <typename T> 
inline T Math<T>::Sin(T ang){
    T angSqr = ang*ang;
    T result = -(T)2.39e-08;    
    result *= angSqr;
    result += (T)2.7526e-06;
    result *= angSqr;
    result -= (T)1.98409e-04;
    result *= angSqr;
    result += (T)8.3333315e-03;
    result *= angSqr;
    result -= (T)1.666666664e-01;
    result *= angSqr;
    result += (T)1.0;
    result *= ang;
    return result;
}

template <typename T> 
inline T Math<T>::Cos(T ang){
    T angSqr = ang*ang;
    T result = -(T)2.605e-07;
    result *= angSqr;
    result += (T)2.47609e-05;
    result *= angSqr;
    result -= (T)1.3888397e-03;
    result *= angSqr;
    result += (T)4.16666418e-02;
    result *= angSqr;
    result -= (T)4.999999963e-01;
    result *= angSqr;
    result += (T)1.0;
    return result;
}

template <typename T> 
inline T Math<T>::Tan(T ang){
    T angSqr = ang*ang;
    T result = (T)9.5168091e-03;
    result *= angSqr;
    result += (T)2.900525e-03;
    result *= angSqr;
    result += (T)2.45650893e-02;
    result *= angSqr;
    result += (T)5.33740603e-02;
    result *= angSqr;
    result += (T)1.333923995e-01;
    result *= angSqr;
    result += (T)3.333314036e-01;
    result *= angSqr;
    result += (T)1.0;
    result *= ang;
    return result;
}

template <typename T> 
inline T Math<T>::SinInv(T val){
    T root = (T)sqrt(abs((T)1.0)-val);
    T result = -(T)0.0012624911;
    result *= val;
    result += (T)0.0066700901;
    result *= val;
    result -= (T)0.0170881256;
    result *= val;
    result += (T)0.0308918810;
    result *= val;
    result -= (T)0.0501743046;
    result *= val;
    result += (T)0.0889789874;
    result *= val;
    result -= (T)0.2145988016;
    result *= val;
    result += (T)1.5707963050;
    result = 2.0*atan(1.0) /*pi/2*/ - root*result;
    return result;
}

template <typename T> 
inline T Math<T>::CosInv(T val){
    T root = (T)sqrt(abs((T)1.0)-val);
    T result = -(T)0.0012624911;
    result *= val;
    result += (T)0.0066700901;
    result *= val;
    result -= (T)0.0170881256;
    result *= val;
    result += (T)0.0308918810;
    result *= val;
    result -= (T)0.0501743046;
    result *= val;
    result += (T)0.0889789874;
    result *= val;
    result -= (T)0.2145988016;
    result *= val;
    result += (T)1.5707963050;
    result *= root;
    return result;
}

template <typename T> 
inline T Math<T>::TanInv(T val){
    T valSqr = val*val;
    T result = (T)0.0028662257;
    result *= valSqr;
    result -= (T)0.0161657367;
    result *= valSqr;
    result += (T)0.0429096138;
    result *= valSqr;
    result -= (T)0.0752896400;
    result *= valSqr;
    result += (T)0.1065626393;
    result *= valSqr;
    result -= (T)0.1420889944;
    result *= valSqr;
    result += (T)0.1999355085;
    result *= valSqr;
    result -= (T)0.3333314528;
    result *= valSqr;
    result += (T)1.0;
    result *= val;
    return result;
}

template <typename T> 
inline T Math<T>::ExpNeg(T val){
    T result = (T)0.0000006906;
    result *= val;
    result += (T)0.0000054302;
    result *= val;
    result += (T)0.0001715620;
    result *= val;
    result += (T)0.0025913712;
    result *= val;
    result += (T)0.0312575832;
    result *= val;
    result += (T)0.2499986842;
    result *= val;
    result += (T)1.0;
    result *= result;
    result *= result;
    result = ((T)1.0)/result;
    return result;
}

template <typename T>
inline T Math<T>::Pow2(int p){
    switch (p) {
        case -20: return 9.53674e-07;
        case -19: return 1.90735e-06;
        case -18: return 3.8147e-06;
        case -17: return 7.62939e-06;
        case -16: return 1.52588e-05;
        case -15: return 3.05176e-05;
        case -14: return 6.10352e-05;
        case -13: return 0.0001220703125;
        case -12: return 0.000244140625;
        case -11: return 0.00048828125;
        case -10: return 0.0009765625;
        case -9: return 0.001953125;
        case -8: return 0.00390625;
        case -7: return 0.0078125;
        case -6: return 0.015625;
        case -5: return 0.03125;
        case -4: return 0.0625;
        case -3: return 0.125;
        case -2: return 0.25;
        case -1: return 0.5;
        case 0: return 1;
        case 1: return 2;
        case 2: return 4;
        case 3: return 8;
        case 4: return 16;
        case 5: return 32;
        case 6: return 64;
        case 7: return 128;
        case 8: return 256;
        case 9: return 512;
        case 10: return 1024;
        case 11: return 2048;
        case 12: return 4096;
        case 13: return 8192;
        case 14: return 16384;
        case 15: return 32768;
        case 16: return 65536;
        case 17: return 131072;
        case 18: return 262144;
        case 19: return 524288;
        case 20: return 1048576;
        default:
            T ret = 1;
            if (ABS(p) == p)
                for (int i=0; i<ABS(p); i++) ret *= 2.0;
            else
                for (int i=0; i<ABS(p); i++) ret *= 0.5;
            return ret;
    }
}

///////////////////////////////////////////////////////////////////////////////

template <typename T> const T       Math<T>::pi =         static_cast<T>(3.14159265358979323846);       // No! ( 4.0*atan(1.0) );
template <typename T> const T       Math<T>::halfpi =     static_cast<T>(1.57079632679489661923);       // No! ( 2.0*atan(1.0) );
template <typename T> const T       Math<T>::twopi =      static_cast<T>(2.0*3.14159265358979323846);   // No! ( 8.0*atan(1.0) );
template <typename T> const T       Math<T>::inv_pi =     static_cast<T>(1.0/3.14159265358979323846);   // No! ( 1.0/(4.0*atan(1.0)) );
template <typename T> const T       Math<T>::inv_twopi =  static_cast<T>(0.5/3.14159265358979323846);   // No! ( 1.0/(8.0*atan(1.0)) );
template <typename T> const double  Math<T>::inf_d =      1.7976931348623158e+308;
template <typename T> const double  Math<T>::eps_d =      2.2204460492503131e-016;
template <typename T> const float   Math<T>::inf_f =      3.402823460e+38F; // 3.402823466e+38F;
template <typename T> const float   Math<T>::eps_f =      1.192092896e-07F;
template <typename T> const float   Math<T>::max_flt =    3.402823460e+38F; // 3.402823466e+38F;
template <typename T> const int     Math<T>::max_int =    2147483647;
template <typename T> const int     Math<T>::min_int =    (-2147483647-1);
template <typename T> const unsigned int    Math<T>::max_uint =   0xffffffffU;

///////////////////////////////////////////////////////////////////////////////
typedef Math<float>     Mathf;
typedef Math<double>    Mathd;

}; // end namespace re
