#pragma once

/****************************************************************************
*
* WXMVECTOR2
*
****************************************************************************/

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

inline bool WXMVECTOR2::operator == (const WXMVECTOR2& V) const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	return XMVector2Equal(v1, v2);
}

inline bool WXMVECTOR2::operator != (const WXMVECTOR2& V) const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	return XMVector2NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

inline WXMVECTOR2& WXMVECTOR2::operator+= (const WXMVECTOR2& V)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	XMVECTOR X = XMVectorAdd(v1, v2);
	XMStoreFloat2(this, X);
	return *this;
}

inline WXMVECTOR2& WXMVECTOR2::operator-= (const WXMVECTOR2& V)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	XMVECTOR X = XMVectorSubtract(v1, v2);
	XMStoreFloat2(this, X);
	return *this;
}

inline WXMVECTOR2& WXMVECTOR2::operator*= (const WXMVECTOR2& V)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	XMVECTOR X = XMVectorMultiply(v1, v2);
	XMStoreFloat2(this, X);
	return *this;
}

inline WXMVECTOR2& WXMVECTOR2::operator*= (float S)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR X = XMVectorScale(v1, S);
	XMStoreFloat2(this, X);
	return *this;
}

inline WXMVECTOR2& WXMVECTOR2::operator/= (float S)
{
	using namespace DirectX;
	assert(S != 0.0f);
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR X = XMVectorScale(v1, 1.f / S);
	XMStoreFloat2(this, X);
	return *this;
}

inline bool WXMVECTOR2::InBounds(const WXMVECTOR2& Bounds) const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&Bounds);
	return XMVector2InBounds(v1, v2);
}

inline float WXMVECTOR2::Length() const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR X = XMVector2Length(v1);
	return XMVectorGetX(X);
}

inline float WXMVECTOR2::LengthSquared() const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR X = XMVector2LengthSq(v1);
	return XMVectorGetX(X);
}

inline float WXMVECTOR2::Dot(const WXMVECTOR2& V) const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	XMVECTOR X = XMVector2Dot(v1, v2);
	return XMVectorGetX(X);
}

inline WXMVECTOR2 WXMVECTOR2::Cross(const WXMVECTOR2& V) const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	XMVECTOR R = XMVector2Cross(v1, v2);

	WXMVECTOR2 result;
	XMStoreFloat2(&result, R);
	return result;
}

inline void WXMVECTOR2::Normalize()
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR X = XMVector2Normalize(v1);
	XMStoreFloat2(this, X);
}

inline WXMVECTOR2 WXMVECTOR2::Transform(const WXMVECTOR2& v, const WXMMATRIX& m)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector2TransformCoord(v1, M);

	WXMVECTOR2 result;
	XMStoreFloat2(&result, X);
	return result;
}

inline void WXMVECTOR2::Clamp(const WXMVECTOR2& vmin, const WXMVECTOR2& vmax)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&vmin);
	XMVECTOR v3 = XMLoadFloat2(&vmax);
	XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat2(this, X);
}

/****************************************************************************
*
* WXMVECTOR3
*
****************************************************************************/

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

inline bool WXMVECTOR3::operator == (const WXMVECTOR3& V) const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	return XMVector3Equal(v1, v2);
}

inline bool WXMVECTOR3::operator != (const WXMVECTOR3& V) const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	return XMVector3NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

inline WXMVECTOR3& WXMVECTOR3::operator+= (const WXMVECTOR3& V)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorAdd(v1, v2);
	XMStoreFloat3(this, X);
	return *this;
}

inline WXMVECTOR3& WXMVECTOR3::operator-= (const WXMVECTOR3& V)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorSubtract(v1, v2);
	XMStoreFloat3(this, X);
	return *this;
}

inline WXMVECTOR3& WXMVECTOR3::operator*= (const WXMVECTOR3& V)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorMultiply(v1, v2);
	XMStoreFloat3(this, X);
	return *this;
}

inline WXMVECTOR3& WXMVECTOR3::operator*= (float S)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVectorScale(v1, S);
	XMStoreFloat3(this, X);
	return *this;
}

inline WXMVECTOR3& WXMVECTOR3::operator/= (float S)
{
	using namespace DirectX;
	assert(S != 0.0f);
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVectorScale(v1, 1.f / S);
	XMStoreFloat3(this, X);
	return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------
inline WXMVECTOR3 WXMVECTOR3::operator- () const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVectorNegate(v1);
	WXMVECTOR3 R;
	XMStoreFloat3(&R, X);
	return R;
}
#if 0
//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------
inline WXMVECTOR3 operator+ (const WXMVECTOR3& V1, const WXMVECTOR3& V2)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V1);
	XMVECTOR v2 = XMLoadFloat3(&V2);
	XMVECTOR X = XMVectorAdd(v1, v2);
	WXMVECTOR3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline WXMVECTOR3 operator- (const WXMVECTOR3& V1, const WXMVECTOR3& V2)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V1);
	XMVECTOR v2 = XMLoadFloat3(&V2);
	XMVECTOR X = XMVectorSubtract(v1, v2);
	WXMVECTOR3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline WXMVECTOR3 operator* (const WXMVECTOR3& V1, const WXMVECTOR3& V2)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V1);
	XMVECTOR v2 = XMLoadFloat3(&V2);
	XMVECTOR X = XMVectorMultiply(v1, v2);
	WXMVECTOR3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline WXMVECTOR3 operator* (const WXMVECTOR3& V, float S)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorScale(v1, S);
	WXMVECTOR3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline WXMVECTOR3 operator/ (const WXMVECTOR3& V1, const WXMVECTOR3& V2)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V1);
	XMVECTOR v2 = XMLoadFloat3(&V2);
	XMVECTOR X = XMVectorDivide(v1, v2);
	WXMVECTOR3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline WXMVECTOR3 operator* (float S, const WXMVECTOR3& V)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorScale(v1, S);
	WXMVECTOR3 R;
	XMStoreFloat3(&R, X);
	return R;
}
#endif

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------
inline bool WXMVECTOR3::InBounds(const WXMVECTOR3& Bounds) const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&Bounds);
	return XMVector3InBounds(v1, v2);
}

inline float WXMVECTOR3::Length() const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVector3Length(v1);
	return XMVectorGetX(X);
}

inline float WXMVECTOR3::LengthSquared() const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVector3LengthSq(v1);
	return XMVectorGetX(X);
}

inline float WXMVECTOR3::Dot(const WXMVECTOR3& V) const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	XMVECTOR X = XMVector3Dot(v1, v2);
	return XMVectorGetX(X);
}

inline WXMVECTOR3 WXMVECTOR3::Cross(const WXMVECTOR3& V) const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	XMVECTOR R = XMVector3Cross(v1, v2);

	WXMVECTOR3 result;
	XMStoreFloat3(&result, R);
	return result;
}

inline void WXMVECTOR3::Normalize()
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVector3Normalize(v1);
	XMStoreFloat3(this, X);
}

inline void WXMVECTOR3::Clamp(const WXMVECTOR3& vmin, const WXMVECTOR3& vmax)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&vmin);
	XMVECTOR v3 = XMLoadFloat3(&vmax);
	XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat3(this, X);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------
inline WXMVECTOR3 WXMVECTOR3::Transform(const WXMVECTOR3& v, const WXMMATRIX& m)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector3TransformCoord(v1, M);

	WXMVECTOR3 result;
	XMStoreFloat3(&result, X);
	return result;
}

/****************************************************************************
*
* WXMVECTOR4
*
****************************************************************************/
inline bool WXMVECTOR4::operator == (const WXMVECTOR4& V) const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&V);
	return XMVector4Equal(v1, v2);
}

inline bool WXMVECTOR4::operator != (const WXMVECTOR4& V) const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&V);
	return XMVector4NotEqual(v1, v2);
}

inline WXMVECTOR4& WXMVECTOR4::operator+= (const WXMVECTOR4& V)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&V);
	XMVECTOR X = XMVectorAdd(v1, v2);
	XMStoreFloat4(this, X);
	return *this;
}

inline WXMVECTOR4& WXMVECTOR4::operator-= (const WXMVECTOR4& V)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&V);
	XMVECTOR X = XMVectorSubtract(v1, v2);
	XMStoreFloat4(this, X);
	return *this;
}

inline WXMVECTOR4& WXMVECTOR4::operator*= (const WXMVECTOR4& V)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&V);
	XMVECTOR X = XMVectorMultiply(v1, v2);
	XMStoreFloat4(this, X);
	return *this;
}

inline WXMVECTOR4& WXMVECTOR4::operator*= (float S)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR X = XMVectorScale(v1, S);
	XMStoreFloat4(this, X);
	return *this;
}

inline WXMVECTOR4& WXMVECTOR4::operator/= (float S)
{
	using namespace DirectX;
	assert(S != 0.0f);
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR X = XMVectorScale(v1, 1.f / S);
	XMStoreFloat4(this, X);
	return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

inline WXMVECTOR4 WXMVECTOR4::operator- () const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR X = XMVectorNegate(v1);
	WXMVECTOR4 R;
	XMStoreFloat4(&R, X);
	return R;
}
#if 0
//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

inline WXMVECTOR4 operator+ (const WXMVECTOR4& V1, const WXMVECTOR4& V2)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&V1);
	XMVECTOR v2 = XMLoadFloat4(&V2);
	XMVECTOR X = XMVectorAdd(v1, v2);
	WXMVECTOR4 R;
	XMStoreFloat4(&R, X);
	return R;
}

inline WXMVECTOR4 operator- (const WXMVECTOR4& V1, const WXMVECTOR4& V2)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&V1);
	XMVECTOR v2 = XMLoadFloat4(&V2);
	XMVECTOR X = XMVectorSubtract(v1, v2);
	WXMVECTOR4 R;
	XMStoreFloat4(&R, X);
	return R;
}

inline WXMVECTOR4 operator* (const WXMVECTOR4& V1, const WXMVECTOR4& V2)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&V1);
	XMVECTOR v2 = XMLoadFloat4(&V2);
	XMVECTOR X = XMVectorMultiply(v1, v2);
	WXMVECTOR4 R;
	XMStoreFloat4(&R, X);
	return R;
}

inline WXMVECTOR4 operator* (const WXMVECTOR4& V, float S)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&V);
	XMVECTOR X = XMVectorScale(v1, S);
	WXMVECTOR4 R;
	XMStoreFloat4(&R, X);
	return R;
}

inline WXMVECTOR4 operator/ (const WXMVECTOR4& V1, const WXMVECTOR4& V2)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&V1);
	XMVECTOR v2 = XMLoadFloat4(&V2);
	XMVECTOR X = XMVectorDivide(v1, v2);
	WXMVECTOR4 R;
	XMStoreFloat4(&R, X);
	return R;
}

inline WXMVECTOR4 operator* (float S, const WXMVECTOR4& V)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&V);
	XMVECTOR X = XMVectorScale(v1, S);
	WXMVECTOR4 R;
	XMStoreFloat4(&R, X);
	return R;
}
#endif

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

inline bool WXMVECTOR4::InBounds(const WXMVECTOR4& Bounds) const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&Bounds);
	return XMVector4InBounds(v1, v2);
}

inline float WXMVECTOR4::Length() const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR X = XMVector4Length(v1);
	return XMVectorGetX(X);
}

inline float WXMVECTOR4::LengthSquared() const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR X = XMVector4LengthSq(v1);
	return XMVectorGetX(X);
}

inline float WXMVECTOR4::Dot(const WXMVECTOR4& V) const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&V);
	XMVECTOR X = XMVector4Dot(v1, v2);
	return XMVectorGetX(X);
}
//
//inline void WXMVECTOR4::Cross(const WXMVECTOR4& v1, const WXMVECTOR4& v2, WXMVECTOR4& result) const
//{
//	using namespace DirectX;
//	XMVECTOR x1 = XMLoadFloat4(this);
//	XMVECTOR x2 = XMLoadFloat4(&v1);
//	XMVECTOR x3 = XMLoadFloat4(&v2);
//	XMVECTOR R = XMVector4Cross(x1, x2, x3);
//	XMStoreFloat4(&result, R);
//}

inline WXMVECTOR4 WXMVECTOR4::Cross(const WXMVECTOR4& v1, const WXMVECTOR4& v2) const
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(this);
	XMVECTOR x2 = XMLoadFloat4(&v1);
	XMVECTOR x3 = XMLoadFloat4(&v2);
	XMVECTOR R = XMVector4Cross(x1, x2, x3);

	WXMVECTOR4 result;
	XMStoreFloat4(&result, R);
	return result;
}

inline void WXMVECTOR4::Normalize()
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR X = XMVector4Normalize(v1);
	XMStoreFloat4(this, X);
}
//
//inline void WXMVECTOR4::Normalize(WXMVECTOR4& result) const
//{
//	using namespace DirectX;
//	XMVECTOR v1 = XMLoadFloat4(this);
//	XMVECTOR X = XMVector4Normalize(v1);
//	XMStoreFloat4(&result, X);
//}

inline void WXMVECTOR4::Clamp(const WXMVECTOR4& vmin, const WXMVECTOR4& vmax)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&vmin);
	XMVECTOR v3 = XMLoadFloat4(&vmax);
	XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat4(this, X);
}
//
//inline void WXMVECTOR4::Clamp(const WXMVECTOR4& vmin, const WXMVECTOR4& vmax, WXMVECTOR4& result) const
//{
//	using namespace DirectX;
//	XMVECTOR v1 = XMLoadFloat4(this);
//	XMVECTOR v2 = XMLoadFloat4(&vmin);
//	XMVECTOR v3 = XMLoadFloat4(&vmax);
//	XMVECTOR X = XMVectorClamp(v1, v2, v3);
//	XMStoreFloat4(&result, X);
//}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline WXMVECTOR4 WXMVECTOR4::Transform(const WXMVECTOR4& v, const WXMMATRIX& m)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector4Transform(v1, M);

	WXMVECTOR4 result;
	XMStoreFloat4(&result, X);
	return result;
}


/****************************************************************************
*
* WXMMATRIX
*
****************************************************************************/

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

inline bool WXMMATRIX::operator == (const WXMMATRIX& M) const
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

	XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	return (XMVector4Equal(x1, y1)
		&& XMVector4Equal(x2, y2)
		&& XMVector4Equal(x3, y3)
		&& XMVector4Equal(x4, y4)) != 0;
}

inline bool WXMMATRIX::operator != (const WXMMATRIX& M) const
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

	XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	return (XMVector4NotEqual(x1, y1)
		|| XMVector4NotEqual(x2, y2)
		|| XMVector4NotEqual(x3, y3)
		|| XMVector4NotEqual(x4, y4)) != 0;
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

inline WXMMATRIX& WXMMATRIX::operator+= (const WXMMATRIX& M)
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_41));

	XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	x1 = XMVectorAdd(x1, y1);
	x2 = XMVectorAdd(x2, y2);
	x3 = XMVectorAdd(x3, y3);
	x4 = XMVectorAdd(x4, y4);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

inline WXMMATRIX& WXMMATRIX::operator-= (const WXMMATRIX& M)
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_41));

	XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	x1 = XMVectorSubtract(x1, y1);
	x2 = XMVectorSubtract(x2, y2);
	x3 = XMVectorSubtract(x3, y3);
	x4 = XMVectorSubtract(x4, y4);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

inline WXMMATRIX& WXMMATRIX::operator*= (const WXMMATRIX& M)
{
	using namespace DirectX;
	XMMATRIX M1 = XMLoadFloat4x4(this);
	XMMATRIX M2 = XMLoadFloat4x4(&M);
	XMMATRIX X = XMMatrixMultiply(M1, M2);
	XMStoreFloat4x4(this, X);
	return *this;
}

inline WXMMATRIX& WXMMATRIX::operator/= (const WXMMATRIX& M)
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_41));

	XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	x1 = XMVectorDivide(x1, y1);
	x2 = XMVectorDivide(x2, y2);
	x3 = XMVectorDivide(x3, y3);
	x4 = XMVectorDivide(x4, y4);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

inline WXMMATRIX& WXMMATRIX::operator*= (float S)
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_41));

	x1 = XMVectorScale(x1, S);
	x2 = XMVectorScale(x2, S);
	x3 = XMVectorScale(x3, S);
	x4 = XMVectorScale(x4, S);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

inline WXMMATRIX& WXMMATRIX::operator/= (float S)
{
	using namespace DirectX;
	assert(S != 0.f);
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_41));

	float rs = 1.f / S;

	x1 = XMVectorScale(x1, rs);
	x2 = XMVectorScale(x2, rs);
	x3 = XMVectorScale(x3, rs);
	x4 = XMVectorScale(x4, rs);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

inline WXMMATRIX WXMMATRIX::operator- () const
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
	XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

	v1 = XMVectorNegate(v1);
	v2 = XMVectorNegate(v2);
	v3 = XMVectorNegate(v3);
	v4 = XMVectorNegate(v4);

	WXMMATRIX R;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), v1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), v2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), v3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), v4);
	return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

inline WXMMATRIX operator+ (const WXMMATRIX& M1, const WXMMATRIX& M2)
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

	XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
	XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
	XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
	XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

	x1 = XMVectorAdd(x1, y1);
	x2 = XMVectorAdd(x2, y2);
	x3 = XMVectorAdd(x3, y3);
	x4 = XMVectorAdd(x4, y4);

	WXMMATRIX R;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
	return R;
}

inline WXMMATRIX operator- (const WXMMATRIX& M1, const WXMMATRIX& M2)
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

	XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
	XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
	XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
	XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

	x1 = XMVectorSubtract(x1, y1);
	x2 = XMVectorSubtract(x2, y2);
	x3 = XMVectorSubtract(x3, y3);
	x4 = XMVectorSubtract(x4, y4);

	WXMMATRIX R;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
	return R;
}

inline WXMMATRIX operator* (const WXMMATRIX& M1, const WXMMATRIX& M2)
{
	using namespace DirectX;
	XMMATRIX m1 = XMLoadFloat4x4(&M1);
	XMMATRIX m2 = XMLoadFloat4x4(&M2);
	XMMATRIX X = XMMatrixMultiply(m1, m2);

	WXMMATRIX R;
	XMStoreFloat4x4(&R, X);
	return R;
}

inline WXMMATRIX operator* (const WXMMATRIX& M, float S)
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	x1 = XMVectorScale(x1, S);
	x2 = XMVectorScale(x2, S);
	x3 = XMVectorScale(x3, S);
	x4 = XMVectorScale(x4, S);

	WXMMATRIX R;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
	return R;
}

inline WXMMATRIX operator/ (const WXMMATRIX& M, float S)
{
	using namespace DirectX;
	assert(S != 0.f);

	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	float rs = 1.f / S;

	x1 = XMVectorScale(x1, rs);
	x2 = XMVectorScale(x2, rs);
	x3 = XMVectorScale(x3, rs);
	x4 = XMVectorScale(x4, rs);

	WXMMATRIX R;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
	return R;
}

inline WXMMATRIX operator/ (const WXMMATRIX& M1, const WXMMATRIX& M2)
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

	XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
	XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
	XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
	XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

	x1 = XMVectorDivide(x1, y1);
	x2 = XMVectorDivide(x2, y2);
	x3 = XMVectorDivide(x3, y3);
	x4 = XMVectorDivide(x4, y4);

	WXMMATRIX R;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
	return R;
}

inline WXMMATRIX operator* (float S, const WXMMATRIX& M)
{
	using namespace DirectX;

	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	x1 = XMVectorScale(x1, S);
	x2 = XMVectorScale(x2, S);
	x3 = XMVectorScale(x3, S);
	x4 = XMVectorScale(x4, S);

	WXMMATRIX R;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
	return R;
}

//------------------------------------------------------------------------------
// WXMMATRIX operations
//------------------------------------------------------------------------------
#if 0
inline bool WXMMATRIX::Decompose(WXMVECTOR3& scale, Quaternion& rotation, WXMVECTOR3& translation)
{
	using namespace DirectX;

	XMVECTOR s, r, t;

	if (!XMMatrixDecompose(&s, &r, &t, *this))
		return false;

	XMStoreFloat3(&scale, s);
	XMStoreFloat4(&rotation, r);
	XMStoreFloat3(&translation, t);

	return true;
}
#endif

inline WXMMATRIX WXMMATRIX::Transpose() const
{
	using namespace DirectX;
	XMMATRIX M = XMLoadFloat4x4(this);
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixTranspose(M));
	return R;
}

inline void WXMMATRIX::Transpose(WXMMATRIX& result) const
{
	using namespace DirectX;
	XMMATRIX M = XMLoadFloat4x4(this);
	XMStoreFloat4x4(&result, XMMatrixTranspose(M));
}

inline WXMMATRIX WXMMATRIX::Invert() const
{
	using namespace DirectX;
	XMMATRIX M = XMLoadFloat4x4(this);
	WXMMATRIX R;
	XMVECTOR det;
	XMStoreFloat4x4(&R, XMMatrixInverse(&det, M));
	return R;
}

inline void WXMMATRIX::Invert(WXMMATRIX& result) const
{
	using namespace DirectX;
	XMMATRIX M = XMLoadFloat4x4(this);
	XMVECTOR det;
	XMStoreFloat4x4(&result, XMMatrixInverse(&det, M));
}

inline float WXMMATRIX::Determinant() const
{
	using namespace DirectX;
	XMMATRIX M = XMLoadFloat4x4(this);
	return XMVectorGetX(XMMatrixDeterminant(M));
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------
#if 0
_Use_decl_annotations_
inline WXMMATRIX WXMMATRIX::CreateBillboard(const WXMVECTOR3& object, const WXMVECTOR3& cameraPosition, const WXMVECTOR3& cameraUp, const WXMVECTOR3* cameraForward)
{
	using namespace DirectX;
	XMVECTOR O = XMLoadFloat3(&object);
	XMVECTOR C = XMLoadFloat3(&cameraPosition);
	XMVECTOR Z = XMVectorSubtract(O, C);

	XMVECTOR N = XMVector3LengthSq(Z);
	if (XMVector3Less(N, g_XMEpsilon))
	{
		if (cameraForward)
		{
			XMVECTOR F = XMLoadFloat3(cameraForward);
			Z = XMVectorNegate(F);
		}
		else
			Z = g_XMNegIdentityR2;
	}
	else
	{
		Z = XMVector3Normalize(Z);
	}

	XMVECTOR up = XMLoadFloat3(&cameraUp);
	XMVECTOR X = XMVector3Cross(up, Z);
	X = XMVector3Normalize(X);

	XMVECTOR Y = XMVector3Cross(Z, X);

	XMMATRIX M;
	M.r[0] = X;
	M.r[1] = Y;
	M.r[2] = Z;
	M.r[3] = XMVectorSetW(O, 1.f);

	WXMMATRIX R;
	XMStoreFloat4x4(&R, M);
	return R;
}

_Use_decl_annotations_
inline WXMMATRIX WXMMATRIX::CreateConstrainedBillboard(const WXMVECTOR3& object, const WXMVECTOR3& cameraPosition, const WXMVECTOR3& rotateAxis,
	const WXMVECTOR3* cameraForward, const WXMVECTOR3* objectForward)
{
	using namespace DirectX;

	static const XMVECTORF32 s_minAngle = { 0.99825467075f, 0.99825467075f, 0.99825467075f, 0.99825467075f }; // 1.0 - XMConvertToRadians( 0.1f );

	XMVECTOR O = XMLoadFloat3(&object);
	XMVECTOR C = XMLoadFloat3(&cameraPosition);
	XMVECTOR faceDir = XMVectorSubtract(O, C);

	XMVECTOR N = XMVector3LengthSq(faceDir);
	if (XMVector3Less(N, g_XMEpsilon))
	{
		if (cameraForward)
		{
			XMVECTOR F = XMLoadFloat3(cameraForward);
			faceDir = XMVectorNegate(F);
		}
		else
			faceDir = g_XMNegIdentityR2;
	}
	else
	{
		faceDir = XMVector3Normalize(faceDir);
	}

	XMVECTOR Y = XMLoadFloat3(&rotateAxis);
	XMVECTOR X, Z;

	XMVECTOR dot = XMVectorAbs(XMVector3Dot(Y, faceDir));
	if (XMVector3Greater(dot, s_minAngle))
	{
		if (objectForward)
		{
			Z = XMLoadFloat3(objectForward);
			dot = XMVectorAbs(XMVector3Dot(Y, Z));
			if (XMVector3Greater(dot, s_minAngle))
			{
				dot = XMVectorAbs(XMVector3Dot(Y, g_XMNegIdentityR2));
				Z = (XMVector3Greater(dot, s_minAngle)) ? g_XMIdentityR0 : g_XMNegIdentityR2;
			}
		}
		else
		{
			dot = XMVectorAbs(XMVector3Dot(Y, g_XMNegIdentityR2));
			Z = (XMVector3Greater(dot, s_minAngle)) ? g_XMIdentityR0 : g_XMNegIdentityR2;
		}

		X = XMVector3Cross(Y, Z);
		X = XMVector3Normalize(X);

		Z = XMVector3Cross(X, Y);
		Z = XMVector3Normalize(Z);
	}
	else
	{
		X = XMVector3Cross(Y, faceDir);
		X = XMVector3Normalize(X);

		Z = XMVector3Cross(X, Y);
		Z = XMVector3Normalize(Z);
	}

	XMMATRIX M;
	M.r[0] = X;
	M.r[1] = Y;
	M.r[2] = Z;
	M.r[3] = XMVectorSetW(O, 1.f);

	WXMMATRIX R;
	XMStoreFloat4x4(&R, M);
	return R;
}
#endif

inline WXMMATRIX WXMMATRIX::CreateTranslation(const WXMVECTOR3& position)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixTranslation(position.x, position.y, position.z));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateTranslation(float x, float y, float z)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixTranslation(x, y, z));
	return R;
}
#if 0
inline WXMMATRIX WXMMATRIX::CreateScale(const WXMVECTOR3& scales)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixScaling(scales.x, scales.y, scales.z));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateScale(float xs, float ys, float zs)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixScaling(xs, ys, zs));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateScale(float scale)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixScaling(scale, scale, scale));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateRotationX(float radians)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixRotationX(radians));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateRotationY(float radians)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixRotationY(radians));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateRotationZ(float radians)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixRotationZ(radians));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateFromAxisAngle(const WXMVECTOR3& axis, float angle)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMVECTOR a = XMLoadFloat3(&axis);
	XMStoreFloat4x4(&R, XMMatrixRotationAxis(a, angle));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixPerspectiveFovRH(fov, aspectRatio, nearPlane, farPlane));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreatePerspective(float width, float height, float nearPlane, float farPlane)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixPerspectiveRH(width, height, nearPlane, farPlane));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixPerspectiveOffCenterRH(left, right, bottom, top, nearPlane, farPlane));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixOrthographicRH(width, height, zNearPlane, zFarPlane));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixOrthographicOffCenterRH(left, right, bottom, top, zNearPlane, zFarPlane));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateLookAt(const WXMVECTOR3& eye, const WXMVECTOR3& target, const WXMVECTOR3& up)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMVECTOR eyev = XMLoadFloat3(&eye);
	XMVECTOR targetv = XMLoadFloat3(&target);
	XMVECTOR upv = XMLoadFloat3(&up);
	XMStoreFloat4x4(&R, XMMatrixLookAtRH(eyev, targetv, upv));
	return R;
}
#endif

inline WXMMATRIX WXMMATRIX::CreatePerspectiveFovLH(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreatePerspectiveLH(float width, float height, float nearPlane, float farPlane)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixPerspectiveLH(width, height, nearPlane, farPlane));
	return R;
}
inline WXMMATRIX WXMMATRIX::CreateOrthographicLH(float width, float height, float zNearPlane, float zFarPlane)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixOrthographicLH(width, height, zNearPlane, zFarPlane));
//	XMStoreFloat4x4(&R, XMMatrixOrthographicOffCenterLH(0.f, width, 0.f, height, zNearPlane, zFarPlane));

//	float fov = XM_PI / 4.0f;
//	float ratio = width / height;
//	XMStoreFloat4x4(&R, XMMatrixPerspectiveFovLH(fov, ratio, zNearPlane, zFarPlane));
	return R;
}
inline WXMMATRIX WXMMATRIX::CreateLookAtLH(const WXMVECTOR3& eye, const WXMVECTOR3& target, const WXMVECTOR3& up)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMVECTOR eyev = XMLoadFloat3(&eye);
	XMVECTOR targetv = XMLoadFloat3(&target);
	XMVECTOR upv = XMLoadFloat3(&up);
	XMStoreFloat4x4(&R, XMMatrixLookAtLH(eyev, targetv, upv));
	return R;
}
inline WXMMATRIX WXMMATRIX::CreateLookAtRH(const WXMVECTOR3& eye, const WXMVECTOR3& target, const WXMVECTOR3& up)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMVECTOR eyev = XMLoadFloat3(&eye);
	XMVECTOR targetv = XMLoadFloat3(&target);
	XMVECTOR upv = XMLoadFloat3(&up);
	XMStoreFloat4x4(&R, XMMatrixLookAtRH(eyev, targetv, upv));
	return R;
}
inline WXMMATRIX WXMMATRIX::CreateIdentity()
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixIdentity());
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateRotationRollPitchYaw(float pitch, float yaw, float roll)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	return R;
}
#if 0
inline WXMMATRIX WXMMATRIX::CreateWorld(const WXMVECTOR3& position, const WXMVECTOR3& forward, const WXMVECTOR3& up)
{
	using namespace DirectX;
	XMVECTOR zaxis = XMVector3Normalize(XMVectorNegate(XMLoadFloat3(&forward)));
	XMVECTOR yaxis = XMLoadFloat3(&up);
	XMVECTOR xaxis = XMVector3Normalize(XMVector3Cross(yaxis, zaxis));
	yaxis = XMVector3Cross(zaxis, xaxis);

	WXMMATRIX R;
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._11), xaxis);
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._21), yaxis);
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._31), zaxis);
	R._14 = R._24 = R._34 = 0.f;
	R._41 = position.x; R._42 = position.y; R._43 = position.z;
	R._44 = 1.f;
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateFromQuaternion(const Quaternion& rotation)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMVECTOR quatv = XMLoadFloat4(&rotation);
	XMStoreFloat4x4(&R, XMMatrixRotationQuaternion(quatv));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateFromYawPitchRoll(float yaw, float pitch, float roll)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMStoreFloat4x4(&R, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateShadow(const WXMVECTOR3& lightDir, const Plane& plane)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMVECTOR light = XMLoadFloat3(&lightDir);
	XMVECTOR planev = XMLoadFloat4(&plane);
	XMStoreFloat4x4(&R, XMMatrixShadow(planev, light));
	return R;
}

inline WXMMATRIX WXMMATRIX::CreateReflection(const Plane& plane)
{
	using namespace DirectX;
	WXMMATRIX R;
	XMVECTOR planev = XMLoadFloat4(&plane);
	XMStoreFloat4x4(&R, XMMatrixReflect(planev));
	return R;
}

inline void WXMMATRIX::Lerp(const WXMMATRIX& M1, const WXMMATRIX& M2, float t, WXMMATRIX& result)
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

	XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
	XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
	XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
	XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

	x1 = XMVectorLerp(x1, y1, t);
	x2 = XMVectorLerp(x2, y2, t);
	x3 = XMVectorLerp(x3, y3, t);
	x4 = XMVectorLerp(x4, y4, t);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._41), x4);
}

inline WXMMATRIX WXMMATRIX::Lerp(const WXMMATRIX& M1, const WXMMATRIX& M2, float t)
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

	XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
	XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
	XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
	XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

	x1 = XMVectorLerp(x1, y1, t);
	x2 = XMVectorLerp(x2, y2, t);
	x3 = XMVectorLerp(x3, y3, t);
	x4 = XMVectorLerp(x4, y4, t);

	WXMMATRIX result;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._41), x4);
	return result;
}

inline void WXMMATRIX::Transform(const WXMMATRIX& M, const Quaternion& rotation, WXMMATRIX& result)
{
	using namespace DirectX;
	XMVECTOR quatv = XMLoadFloat4(&rotation);

	XMMATRIX M0 = XMLoadFloat4x4(&M);
	XMMATRIX M1 = XMMatrixRotationQuaternion(quatv);

	XMStoreFloat4x4(&result, XMMatrixMultiply(M0, M1));
}

inline WXMMATRIX WXMMATRIX::Transform(const WXMMATRIX& M, const Quaternion& rotation)
{
	using namespace DirectX;
	XMVECTOR quatv = XMLoadFloat4(&rotation);

	XMMATRIX M0 = XMLoadFloat4x4(&M);
	XMMATRIX M1 = XMMatrixRotationQuaternion(quatv);

	WXMMATRIX result;
	XMStoreFloat4x4(&result, XMMatrixMultiply(M0, M1));
	return result;
}
#endif

/****************************************************************************
*
* Plane
*
****************************************************************************/

inline WXMPLANE::WXMPLANE(const WXMVECTOR3& point1, const WXMVECTOR3& point2, const WXMVECTOR3& point3)
{
	using namespace DirectX;
	XMVECTOR P0 = XMLoadFloat3(&point1);
	XMVECTOR P1 = XMLoadFloat3(&point2);
	XMVECTOR P2 = XMLoadFloat3(&point3);
	XMStoreFloat4(this, XMPlaneFromPoints(P0, P1, P2));
}

inline WXMPLANE::WXMPLANE(const WXMVECTOR3& point, const WXMVECTOR3& normal)
{
	using namespace DirectX;
	XMVECTOR P = XMLoadFloat3(&point);
	XMVECTOR N = XMLoadFloat3(&normal);
	XMStoreFloat4(this, XMPlaneFromPointNormal(P, N));
}

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

inline bool WXMPLANE::operator == (const WXMPLANE& p) const
{
	using namespace DirectX;
	XMVECTOR p1 = XMLoadFloat4(this);
	XMVECTOR p2 = XMLoadFloat4(&p);
	return XMPlaneEqual(p1, p2);
}

inline bool WXMPLANE::operator != (const WXMPLANE& p) const
{
	using namespace DirectX;
	XMVECTOR p1 = XMLoadFloat4(this);
	XMVECTOR p2 = XMLoadFloat4(&p);
	return XMPlaneNotEqual(p1, p2);
}

//------------------------------------------------------------------------------
// Plane operations
//------------------------------------------------------------------------------

inline void WXMPLANE::Normalize()
{
	using namespace DirectX;
	XMVECTOR p = XMLoadFloat4(this);
	XMStoreFloat4(this, XMPlaneNormalize(p));
}
//
//inline void WXMPLANE::Normalize(Plane& result) const
//{
//	using namespace DirectX;
//	XMVECTOR p = XMLoadFloat4(this);
//	XMStoreFloat4(&result, XMPlaneNormalize(p));
//}

inline float WXMPLANE::Dot(const WXMVECTOR4& v) const
{
	using namespace DirectX;
	XMVECTOR p = XMLoadFloat4(this);
	XMVECTOR v0 = XMLoadFloat4(&v);
	return XMVectorGetX(XMPlaneDot(p, v0));
}

inline float WXMPLANE::DotCoordinate(const WXMVECTOR3& position) const
{
	using namespace DirectX;
	XMVECTOR p = XMLoadFloat4(this);
	XMVECTOR v0 = XMLoadFloat3(&position);
	return XMVectorGetX(XMPlaneDotCoord(p, v0));
}

inline float WXMPLANE::DotNormal(const WXMVECTOR3& normal) const
{
	using namespace DirectX;
	XMVECTOR p = XMLoadFloat4(this);
	XMVECTOR n0 = XMLoadFloat3(&normal);
	return XMVectorGetX(XMPlaneDotNormal(p, n0));
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------
//
//inline void WXMPLANE::Transform(const WXMPLANE& plane, const Matrix& M, WXMPLANE& result)
//{
//	using namespace DirectX;
//	XMVECTOR p = XMLoadFloat4(&plane);
//	XMMATRIX m0 = XMLoadFloat4x4(&M);
//	XMStoreFloat4(&result, XMPlaneTransform(p, m0));
//}

inline WXMPLANE WXMPLANE::Transform(const WXMPLANE& plane, const WXMMATRIX& M)
{
	using namespace DirectX;
	XMVECTOR p = XMLoadFloat4(&plane);
	XMMATRIX m0 = XMLoadFloat4x4(&M);

	WXMPLANE result;
	XMStoreFloat4(&result, XMPlaneTransform(p, m0));
	return result;
}
#if 0
inline void WXMPLANE::Transform(const WXMPLANE& plane, const Quaternion& rotation, Plane& result)
{
	using namespace DirectX;
	XMVECTOR p = XMLoadFloat4(&plane);
	XMVECTOR q = XMLoadFloat4(&rotation);
	XMVECTOR X = XMVector3Rotate(p, q);
	X = XMVectorSelect(p, X, g_XMSelect1110); // result.d = plane.d
	XMStoreFloat4(&result, X);
}

inline WXMPLANE WXMPLANE::Transform(const WXMPLANE& plane, const Quaternion& rotation)
{
	using namespace DirectX;
	XMVECTOR p = XMLoadFloat4(&plane);
	XMVECTOR q = XMLoadFloat4(&rotation);
	XMVECTOR X = XMVector3Rotate(p, q);
	X = XMVectorSelect(p, X, g_XMSelect1110); // result.d = plane.d

	Plane result;
	XMStoreFloat4(&result, X);
	return result;
}
#endif