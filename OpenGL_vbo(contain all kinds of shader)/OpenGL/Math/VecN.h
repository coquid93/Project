#pragma once

#include "Math/Math.h"

namespace respace {

///////////////////////////////////////////////////////////////////////////////
// VecN class declaration
///////////////////////////////////////////////////////////////////////////////
template <typename T> class VecN {
private:
    T       *m_val;
    int     m_dim;

public:
    // Constructors
    VecN();
    VecN(int N);
    VecN(int N, T d);
    VecN(int N, const T *src);
    VecN(const VecN &src);
    VecN(const VecN *src);
    ~VecN() {delete [] m_val;}

    // Setters
    void SetDim(int N);
    void Set(T d);
    void Set(int N, T d);
    void Set(int N, const T *src);
    void Set(const VecN &src);
    void Set(const VecN *src);
    void Swap(T *b, int N);
    void Swap(VecN &src);
    void CopyTo(T *b);

    // Getters
    T   Get(int i) const;
    int Dim() const { return m_dim;}
    
    // Overloading operators
    T &operator[](int i);
    T  operator[](int i) const;
    T &operator()(int i);
    T  operator()(int i) const;

    VecN &operator=(T d);
    VecN &operator=(const VecN &src);
    VecN &operator=(const VecN *src);

    VecN &operator+=(T d);
    VecN &operator-=(T d);
    VecN &operator*=(T d);
    VecN &operator/=(T d);
    VecN &operator+=(const VecN &src);
    VecN &operator-=(const VecN &src);

    // Vector operations
    // A.Plus(d);
    void Plus (T d);
    void Minus(T d);
    void Mult (T d);
    void Div  (T d);
    void Plus (const VecN &a);
    void Minus(const VecN &a);
    void Plus (const VecN &a, const VecN &b);   // set a+b
    void Minus(const VecN &a, const VecN &b);   // set a-b

    // Basic methods
    void Clear() { for(int i=0; i<m_dim; i++) m_val[i] = (T) 0; }
    bool IsEqual(const VecN &src) const;
    void Normalize();
    T    L1Norm() const;
    T    L2Norm() const;
    T    LInfNorm() const;
    T    Mag() const { return L2Norm(); }
    T    SqrMag() const;

    // Friends functions (LHS and RHS arguments are both possible)
    template <typename S> friend VecN<S> operator-(const VecN<S> &a);
    template <typename S> friend VecN<S> operator-(const VecN<S> &a, S b);
    template <typename S> friend VecN<S> operator-(S a, const VecN<S> &b);
    template <typename S> friend VecN<S> operator-(const VecN<S> &a, const VecN<S> &b);
    template <typename S> friend VecN<S> operator+(const VecN<S> &a, S b);
    template <typename S> friend VecN<S> operator+(S a, const VecN<S> &b);
    template <typename S> friend VecN<S> operator+(const VecN<S> &a, const VecN<S> &b);
    template <typename S> friend VecN<S> operator*(const VecN<S> &a, S b);
    template <typename S> friend VecN<S> operator*(S a, const VecN<S> &b);
    template <typename S> friend VecN<S> operator/(const VecN<S> &a, S b);
    template <typename S> friend S Mag(const VecN<S> &a);
    template <typename S> friend S SqrMag(const VecN<S> &a);
    template <typename S> friend S Dot(const VecN<S> &a, const VecN<S> &b);
    //template <typename S> friend std::ostream &operator<<(std::ostream &str, VecN<S> &vec);
    
};

///////////////////////////////////////////////////////////////////////////////
// VecN class inline definition
///////////////////////////////////////////////////////////////////////////////
// Constructors
template <typename T> inline VecN<T>::VecN() {
    m_dim = 0;
    m_val = NULL;
}
template <typename T> inline VecN<T>::VecN(int N) {
    m_dim = N;
    m_val = new T[N];
    Set((T) 0);
}
template <typename T> inline VecN<T>::VecN(int N, T d) {
    m_dim = N;
    m_val = new T[N];
    Set(d);
}
template <typename T> inline VecN<T>::VecN(int N, const T *src){
    m_dim = N;
    m_val = new T[N];
    memcpy(m_val, src, sizeof(T)*N);
}
template <typename T> inline VecN<T>::VecN(const VecN &src) {
    m_dim = src.m_dim;
    m_val = new T[m_dim];
    Set(src);
}
template <typename T> inline VecN<T>::VecN(const VecN *src) {
    m_dim = src->m_dim;
    m_val = new T[m_dim];
    Set(src);
}
//-----------------------------------------------------------------------------
// Setters
template <typename T> inline void VecN<T>::SetDim(int N){
    if(m_dim == N && m_val) return;

    if(m_val) delete [] m_val;
    m_val = new T[N];
    m_dim = N;
    // not initializing m_val
}
template <typename T> inline void VecN<T>::Set(T d) {
    for ( int i = 0; i < m_dim; ++i) {
        m_val[i] = d;
    }
}
template <typename T> inline void VecN<T>::Set(int N, T d){
    SetDim(N);
    for ( int i = 0; i < m_dim; ++i) {
        m_val[i] = d;
    }
}
template <typename T> inline void VecN<T>::Set(int N, const T *src){
    SetDim(N);
    memcpy(m_val, src, sizeof(T)*N);
}
template <typename T> inline void VecN<T>::Set(const VecN &src) {
    SetDim(src.Dim());
    memcpy(m_val, src.m_val, sizeof(T)*m_dim); 
    //for ( int i = 0; i < m_dim; ++i) {
    //  m_val[i] = src[i];
    //}
}
template <typename T> inline void VecN<T>::Set(const VecN *src) {
    SetDim(src->Dim());
    memcpy(m_val, src->m_val, sizeof(T)*m_dim);
    //for ( int i = 0; i < m_dim; ++i) {
    //  m_val[i] = src->Get(i);
    //}
}
template <typename T> inline void VecN<T>::Swap(T *b, int N){
    T *temp = m_val;
    m_val = b;
    b = temp;
    m_dim = N;
}
template <typename T> inline void VecN<T>::Swap(VecN &src){
    T *temp = m_val;
    m_val = src.m_val;
    src.m_val = temp;

    int n = m_dim; 
    m_dim = src.m_dim;
    src.m_dim = n;
}
template <typename T> inline void VecN<T>::CopyTo(T *b){
    memcpy(b, m_val, sizeof(T)*m_dim);
}
//-----------------------------------------------------------------------------
// Getters
template <typename T> inline T  VecN<T>::Get(int i) const {

    return m_val[i];
}
//-----------------------------------------------------------------------------
// Overloading operators
template <typename T> inline T &VecN<T>::operator[](int i) {

    return m_val[i];
}
template <typename T> inline T  VecN<T>::operator[](int i) const {

    return m_val[i];
}
template <typename T> inline T &VecN<T>::operator()(int i) {

    return m_val[i];
}
template <typename T> inline T  VecN<T>::operator()(int i) const {

    return m_val[i];
}
//-----------------------------------------------------------------------------
template <typename T> inline VecN<T> &VecN<T>::operator=(T d) {
    Set(d);
    return (*this);
}
template <typename T> inline VecN<T> &VecN<T>::operator=(const VecN &src) {
    Set(src);
    return (*this);
}
template <typename T> inline VecN<T> &VecN<T>::operator=(const VecN *src) {
    Set(src);
    return (*this);
}
//-----------------------------------------------------------------------------
template <typename T> inline VecN<T> &VecN<T>::operator+=(T d) {
    Plus(d);
    return (*this);
}
template <typename T> inline VecN<T> &VecN<T>::operator-=(T d)  {
    Minus(d);
    return (*this);
}
template <typename T> inline VecN<T> &VecN<T>::operator*=(T d) {
    Mult(d);
    return (*this);
}
template <typename T> inline VecN<T> &VecN<T>::operator/=(T d) {
    Div(d);
    return (*this);
}
template <typename T> inline VecN<T> &VecN<T>::operator+=(const VecN &src) {
    Plus(src);
    return (*this);
}
template <typename T> inline VecN<T> &VecN<T>::operator-=(const VecN &src) {
    Minus(src);
    return (*this);
}
//-----------------------------------------------------------------------------
// Vector operations
template <typename T> inline void VecN<T>::Plus(T d) {
    for (int i = 0; i < m_dim; ++i)
        m_val[i] += d;
}
template <typename T> inline void VecN<T>::Plus(const VecN &a) {

    for (int i = 0; i < m_dim; ++i)
        m_val[i] += a[i];
}
template <typename T> inline void VecN<T>::Plus(const VecN &a, const VecN &b) {

    for (int i = 0; i < m_dim; ++i)
        m_val[i] = a[i] + b[i];
}
template <typename T> inline void VecN<T>::Minus(T d) {
    for (int i = 0; i < m_dim; ++i)
        m_val[i] -= d;
}
template <typename T> inline void VecN<T>::Minus(const VecN &a) {

    for (int i = 0; i < m_dim; ++i)
        m_val[i] -= a[i];
}
template <typename T> inline void VecN<T>::Minus(const VecN &a, const VecN &b) {

    for (int i = 0; i < m_dim; ++i)
        m_val[i] = a[i] - b[i];
}
template <typename T> inline void VecN<T>::Mult(T d) {
    for (int i = 0; i < m_dim; ++i)
        m_val[i] *= d;
}
template <typename T> inline void VecN<T>::Div(T d) {
    for (int i = 0; i < m_dim; ++i)
        m_val[i] /= d;
}
//-----------------------------------------------------------------------------
// Methods
template <typename T> inline bool VecN<T>::IsEqual(const VecN &src) const {
    if( m_dim != src.m_dim ) return false;
    for(int i=0; i<m_dim; ++i){
        if( m_val[0] != src[0] ) return false;
    }
    return true;
}
template <typename T> inline void VecN<T>::Normalize() {
    register T m = L2Norm();
    if( m != 0) (*this) /= m;
}
template <typename T> inline T VecN<T>::L1Norm() const {
    T sum = (T) 0;
    for(int i = 0; i < m_dim; ++i)
        sum += ABS(m_val[0]);
    return sum;
}
template <typename T> inline T VecN<T>::L2Norm() const {
    T sum = (T) 0;
    for(int i = 0; i < m_dim; ++i)
        sum += SQ(m_val[i]);
    return sqrt(sum);
}
template <typename T> inline T VecN<T>::LInfNorm() const {
    int imax=0;
    for(int i = 0; i < m_dim; ++i){
        if( ABS(a[i]) > ABS(a[imax]) )  
            imax = i;
    }
    return ABS(a[imax]);
}
template <typename T> inline T VecN<T>::SqrMag() const {
    T sum = (T) 0;
    for(int i = 0; i < m_dim; ++i)
        sum += SQ(m_val[i]);
    return sum;
}
//-----------------------------------------------------------------------------
// Friend functions
template <typename T> inline VecN<T> operator-(const VecN<T> &a) {
    VecN<T> ans;
    int n = a.Dim();
    ans.SetDim(n);
    for( int i = 0; i < n; ++i)
        ans[i] = -a[i];
    return ans;
}
template <typename T> inline VecN<T> operator-(const VecN<T> &a, T b) {
    VecN<T> ans;
    int n = a.Dim();
    ans.SetDim(n);
    for( int i = 0; i < n; ++i)
        ans[i] = a[i] - b;
    return ans;
}
template <typename T> inline VecN<T> operator-(T a, const VecN<T> &b) {
    VecN<T> ans;
    int n = b.Dim();
    ans.SetDim(n);
    for( int i = 0; i < n; ++i)
        ans[i] = a - b[i];
    return ans;
}
template <typename T> inline VecN<T> operator-(const VecN<T> &a, const VecN<T> &b) {

    VecN<T> ans;
    int n = a.Dim();
    ans.SetDim(n);
    for( int i = 0; i < n; ++i)
        ans[i] = a[i] - b[i];
    return ans;
}
template <typename T> inline VecN<T> operator+(const VecN<T> &a, T b) {
    VecN<T> ans;
    int n = a.Dim();
    ans.SetDim(n);
    for( int i = 0; i < n; ++i)
        ans[i] = a[i] + b;
    return ans;
}
template <typename T> inline VecN<T> operator+(T a, const VecN<T> &b) {
    VecN<T> ans;
    int n = b.Dim();
    ans.SetDim(n);
    for( int i = 0; i < n; ++i)
        ans[i] = a + b[i];
    return ans;
}
template <typename T> inline VecN<T> operator+(const VecN<T> &a, const VecN<T> &b) {

    VecN<T> ans;
    int n = a.Dim();
    ans.SetDim(n);
    for( int i = 0; i < n; ++i)
        ans[i] = a[i] + b[i];
    return ans;
}
template <typename T> inline VecN<T> operator*(const VecN<T> &a, T b) {
    VecN<T> ans;
    int n = a.Dim();
    ans.SetDim(n);
    for( int i = 0; i < n; ++i)
        ans[i] = a[i]*b;
    return ans;
}
template <typename T> inline VecN<T> operator*(T a, const VecN<T> &b) {
    VecN<T> ans;
    int n = b.Dim();
    ans.SetDim(n);
    for( int i = 0; i < n; ++i)
        ans[i] = a*b[i];
    return ans;
}
template <typename T> inline VecN<T> operator/(const VecN<T> &a, T b) {
    VecN<T> ans;
    int n = a.Dim();
    ans.SetDim(n);
    for( int i = 0; i < n; ++i)
        ans[i] = a[i]/b;
    return ans;
}
template <typename T> inline T Mag(const VecN<T> &a) {
    return a.Mag();
}
template <typename T> inline T SqrMag(const VecN<T> &a) {
    return a.SqrMag();
}
template <typename T> inline T Dot(const VecN<T> &a, const VecN<T> &b) {

    int n = a.Dim();
    T sum = (T) 0;
    for (int i = 0; i < n; ++i)
        sum += a[i]*b[i];
    return sum;
}
//template <typename T> inline std::ostream &operator<<(std::ostream &str, VecN<T> &vec){
//  str << "[";
//  int N = vec.Dim();
//  for( int i=0; i<N-1; ++i) str << vec[i] << ", ";
//  str << vec[N-1]; << "]";
//  return str;
//}


///////////////////////////////////////////////////////////////////////////////
typedef VecN<int>       VecNi;
typedef VecN<uint>      VecNui;
typedef VecN<float>     VecNf;
typedef VecN<double>    VecNd;
typedef VecN<real>      VecNr;


}; // end namespace re
