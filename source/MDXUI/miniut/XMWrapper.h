#pragma once

#include <memory.h>
#include <DirectXMath.h>

using namespace DirectX;

#if 1
//namespace DirectX{
namespace WrappedXM
{
	struct WXMMATRIX;

#if 1
	// 2D vector
	struct WXMVECTOR2 : public XMFLOAT2
	{
		WXMVECTOR2() : XMFLOAT2(0.f, 0.f) {}
		explicit WXMVECTOR2(float x) : XMFLOAT2(x, x) {}
		WXMVECTOR2(float _x, float _y) : XMFLOAT2(_x, _y) {}
		explicit WXMVECTOR2(_In_reads_(2) const float *pArray) : XMFLOAT2(pArray) {}
		WXMVECTOR2(FXMVECTOR V) { XMStoreFloat2(this, V); }
		WXMVECTOR2(const WXMVECTOR2& V)
		{
			XMVECTOR x = XMLoadFloat2(&V);
			XMStoreFloat2(this, x);
		}
		operator XMVECTOR() const { return XMLoadFloat2(this); }

		// Comparision operators
		bool operator == (const WXMVECTOR2& V) const;
		bool operator != (const WXMVECTOR2& V) const;

		// Assignment operators
		WXMVECTOR2& operator= (const WXMVECTOR2& V) { x = V.x; y = V.y; return *this; }
		WXMVECTOR2& operator+= (const WXMVECTOR2& V);
		WXMVECTOR2& operator-= (const WXMVECTOR2& V);
		WXMVECTOR2& operator*= (const WXMVECTOR2& V);
		WXMVECTOR2& operator*= (float S);
		WXMVECTOR2& operator/= (float S);

		// Urnary operators
		WXMVECTOR2 operator+ () const { return *this; }
		WXMVECTOR2 operator- () const { return WXMVECTOR2(-x, -y); }

		// Vector operations
		bool InBounds(const WXMVECTOR2& Bounds) const;
		float Length() const;
		float LengthSquared() const;
		float Dot(const WXMVECTOR2& V) const;
		WXMVECTOR2 Cross(const WXMVECTOR2& V) const;
//		void Cross(const WXMVECTOR2& V, WXMVECTOR2& result) const;
		void Normalize();
//		void Normalize(WXMVECTOR2& result) const;
		void Clamp(const WXMVECTOR2& vmin, const WXMVECTOR2& vmax);
//		void Clamp(const WXMVECTOR2& vmin, const WXMVECTOR2& vmax, WXMVECTOR2& result) const;
#if 0
		// Static functions
		static float Distance(const WXMVECTOR2& v1, const WXMVECTOR2& v2);
		static float DistanceSquared(const WXMVECTOR2& v1, const WXMVECTOR2& v2);

		static void Min(const WXMVECTOR2& v1, const WXMVECTOR2& v2, WXMVECTOR2& result);
		static WXMVECTOR2 Min(const WXMVECTOR2& v1, const WXMVECTOR2& v2);

		static void Max(const WXMVECTOR2& v1, const WXMVECTOR2& v2, WXMVECTOR2& result);
		static WXMVECTOR2 Max(const WXMVECTOR2& v1, const WXMVECTOR2& v2);

		static void Lerp(const WXMVECTOR2& v1, const WXMVECTOR2& v2, float t, WXMVECTOR2& result);
		static WXMVECTOR2 Lerp(const WXMVECTOR2& v1, const WXMVECTOR2& v2, float t);

		static void SmoothStep(const WXMVECTOR2& v1, const WXMVECTOR2& v2, float t, WXMVECTOR2& result);
		static WXMVECTOR2 SmoothStep(const WXMVECTOR2& v1, const WXMVECTOR2& v2, float t);

		static void Barycentric(const WXMVECTOR2& v1, const WXMVECTOR2& v2, const WXMVECTOR2& v3, float f, float g, WXMVECTOR2& result);
		static WXMVECTOR2 Barycentric(const WXMVECTOR2& v1, const WXMVECTOR2& v2, const WXMVECTOR2& v3, float f, float g);

		static void CatmullRom(const WXMVECTOR2& v1, const WXMVECTOR2& v2, const WXMVECTOR2& v3, const WXMVECTOR2& v4, float t, WXMVECTOR2& result);
		static WXMVECTOR2 CatmullRom(const WXMVECTOR2& v1, const WXMVECTOR2& v2, const WXMVECTOR2& v3, const WXMVECTOR2& v4, float t);

		static void Hermite(const WXMVECTOR2& v1, const WXMVECTOR2& t1, const WXMVECTOR2& v2, const WXMVECTOR2& t2, float t, WXMVECTOR2& result);
		static WXMVECTOR2 Hermite(const WXMVECTOR2& v1, const WXMVECTOR2& t1, const WXMVECTOR2& v2, const WXMVECTOR2& t2, float t);

		static void Reflect(const WXMVECTOR2& ivec, const WXMVECTOR2& nvec, WXMVECTOR2& result);
		static WXMVECTOR2 Reflect(const WXMVECTOR2& ivec, const WXMVECTOR2& nvec);

		static void Refract(const WXMVECTOR2& ivec, const WXMVECTOR2& nvec, float refractionIndex, WXMVECTOR2& result);
		static WXMVECTOR2 Refract(const WXMVECTOR2& ivec, const WXMVECTOR2& nvec, float refractionIndex);

		static void Transform(const WXMVECTOR2& v, const Quaternion& quat, WXMVECTOR2& result);
		static WXMVECTOR2 Transform(const WXMVECTOR2& v, const Quaternion& quat);

		static void Transform(const WXMVECTOR2& v, const WXMMATRIX& m, WXMVECTOR2& result);
		static WXMVECTOR2 Transform(const WXMVECTOR2& v, const WXMMATRIX& m);
		static void Transform(_In_reads_(count) const WXMVECTOR2* varray, size_t count, const WXMMATRIX& m, _Out_writes_(count) WXMVECTOR2* resultArray);

		static void Transform(const WXMVECTOR2& v, const WXMMATRIX& m, Vector4& result);
		static void Transform(_In_reads_(count) const WXMVECTOR2* varray, size_t count, const WXMMATRIX& m, _Out_writes_(count) Vector4* resultArray);

		static void TransformNormal(const WXMVECTOR2& v, const WXMMATRIX& m, WXMVECTOR2& result);
		static WXMVECTOR2 TransformNormal(const WXMVECTOR2& v, const WXMMATRIX& m);
		static void TransformNormal(_In_reads_(count) const WXMVECTOR2* varray, size_t count, const WXMMATRIX& m, _Out_writes_(count) WXMVECTOR2* resultArray);
#endif
		static WXMVECTOR2 Transform(const WXMVECTOR2& v, const WXMMATRIX& m);

		// Constants
		//static const WXMVECTOR2 Zero;
		//static const WXMVECTOR2 One;
		//static const WXMVECTOR2 UnitX;
		//static const WXMVECTOR2 UnitY;
	};
#endif

	// 3D vector
	struct WXMVECTOR3 : public XMFLOAT3
	{
		WXMVECTOR3() : XMFLOAT3(0.f, 0.f, 0.f) {}
		explicit WXMVECTOR3(float x) : XMFLOAT3(x, x, x) {}
		WXMVECTOR3(float _x, float _y, float _z) : XMFLOAT3(_x, _y, _z) {}
		explicit WXMVECTOR3(_In_reads_(3) const float *pArray) : XMFLOAT3(pArray) {}
		WXMVECTOR3(FXMVECTOR V) { XMStoreFloat3(this, V); }
		WXMVECTOR3(const WXMVECTOR3& V)
		{
			XMVECTOR x = XMLoadFloat3(&V);
			XMStoreFloat3(this, x);
		}

		operator XMVECTOR() const { return XMLoadFloat3(this); }

		// Comparision operators
		bool operator == (const WXMVECTOR3& V) const;
		bool operator != (const WXMVECTOR3& V) const;

		// Assignment operators
		WXMVECTOR3& operator= (const WXMVECTOR3& V) { x = V.x; y = V.y; z = V.z; return *this; }
		WXMVECTOR3& operator+= (const WXMVECTOR3& V);
		WXMVECTOR3& operator-= (const WXMVECTOR3& V);
		WXMVECTOR3& operator*= (const WXMVECTOR3& V);
		WXMVECTOR3& operator*= (float S);
		WXMVECTOR3& operator/= (float S);

		// Urnary operators
		WXMVECTOR3 operator+ () const { return *this; }
		WXMVECTOR3 operator- () const;
		// Vector operations
		bool InBounds(const WXMVECTOR3& Bounds) const;
		float Length() const;
		float LengthSquared() const;

		float Dot(const WXMVECTOR3& V) const;
//		void Cross(const WXMVECTOR3& V, WXMVECTOR3& result) const;
		WXMVECTOR3 Cross(const WXMVECTOR3& V) const;

		void Normalize();
//		void Normalize(WXMVECTOR3& result) const;

		void Clamp(const WXMVECTOR3& vmin, const WXMVECTOR3& vmax);
//		void Clamp(const WXMVECTOR3& vmin, const WXMVECTOR3& vmax, WXMVECTOR3& result) const;

		// Static functions
		static WXMVECTOR3 Transform(const WXMVECTOR3& v, const WXMMATRIX& m);
	};

	// Binary operators
	//WXMVECTOR3 operator+ (const WXMVECTOR3& V1, const WXMVECTOR3& V2);
	//WXMVECTOR3 operator- (const WXMVECTOR3& V1, const WXMVECTOR3& V2);
	//WXMVECTOR3 operator* (const WXMVECTOR3& V1, const WXMVECTOR3& V2);
	//WXMVECTOR3 operator* (const WXMVECTOR3& V, float S);
	//WXMVECTOR3 operator/ (const WXMVECTOR3& V1, const WXMVECTOR3& V2);
	//WXMVECTOR3 operator* (float S, const WXMVECTOR3& V);


	//------------------------------------------------------------------------------
	// 4D vector
	struct WXMVECTOR4 : public XMFLOAT4
	{
		WXMVECTOR4() : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
		explicit WXMVECTOR4(float x) : XMFLOAT4(x, x, x, x) {}
		WXMVECTOR4(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		explicit WXMVECTOR4(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
		WXMVECTOR4(FXMVECTOR V) { XMStoreFloat4(this, V); }
		WXMVECTOR4(const WXMVECTOR4& V)
		{
			XMVECTOR x = XMLoadFloat4(&V);
			XMStoreFloat4(this, x);
		}
		operator XMVECTOR() const { return XMLoadFloat4(this); }

		// Comparision operators
		bool operator == (const WXMVECTOR4& V) const;
		bool operator != (const WXMVECTOR4& V) const;

		// Assignment operators
		WXMVECTOR4& operator= (const WXMVECTOR4& V) { x = V.x; y = V.y; z = V.z; w = V.w; return *this; }
		WXMVECTOR4& operator+= (const WXMVECTOR4& V);
		WXMVECTOR4& operator-= (const WXMVECTOR4& V);
		WXMVECTOR4& operator*= (const WXMVECTOR4& V);
		WXMVECTOR4& operator*= (float S);
		WXMVECTOR4& operator/= (float S);

		// Urnary operators
		WXMVECTOR4 operator+ () const { return *this; }
		WXMVECTOR4 operator- () const;
		// Vector operations
		bool InBounds(const WXMVECTOR4& Bounds) const;
		float Length() const;
		float LengthSquared() const;
		float Dot(const WXMVECTOR4& V) const;
//		void Cross(const WXMVECTOR4& v1, const WXMVECTOR4& v2, WXMVECTOR4& result) const;
		WXMVECTOR4 Cross(const WXMVECTOR4& v1, const WXMVECTOR4& v2) const;

		void Normalize();
//		void Normalize(WXMVECTOR4& result) const;

		void Clamp(const WXMVECTOR4& vmin, const WXMVECTOR4& vmax);
//		void Clamp(const WXMVECTOR4& vmin, const WXMVECTOR4& vmax, WXMVECTOR4& result) const;

#if 0
		// Static functions
		static float Distance(const Vector4& v1, const Vector4& v2);
		static float DistanceSquared(const Vector4& v1, const Vector4& v2);

		static void Min(const Vector4& v1, const Vector4& v2, Vector4& result);
		static Vector4 Min(const Vector4& v1, const Vector4& v2);

		static void Max(const Vector4& v1, const Vector4& v2, Vector4& result);
		static Vector4 Max(const Vector4& v1, const Vector4& v2);

		static void Lerp(const Vector4& v1, const Vector4& v2, float t, Vector4& result);
		static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t);

		static void SmoothStep(const Vector4& v1, const Vector4& v2, float t, Vector4& result);
		static Vector4 SmoothStep(const Vector4& v1, const Vector4& v2, float t);

		static void Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g, Vector4& result);
		static Vector4 Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g);

		static void CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t, Vector4& result);
		static Vector4 CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t);

		static void Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t, Vector4& result);
		static Vector4 Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t);

		static void Reflect(const Vector4& ivec, const Vector4& nvec, Vector4& result);
		static Vector4 Reflect(const Vector4& ivec, const Vector4& nvec);

		static void Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex, Vector4& result);
		static Vector4 Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex);
#endif
		//static void Transform(const Vector2& v, const Quaternion& quat, Vector4& result);
		//static Vector4 Transform(const Vector2& v, const Quaternion& quat);

		//static void Transform(const Vector3& v, const Quaternion& quat, Vector4& result);
		//static Vector4 Transform(const Vector3& v, const Quaternion& quat);

		//static void Transform(const Vector4& v, const Quaternion& quat, Vector4& result);
		//static Vector4 Transform(const Vector4& v, const Quaternion& quat);

		//static void Transform(const Vector4& v, const Matrix& m, Vector4& result);
		static WXMVECTOR4 Transform(const WXMVECTOR4& v, const WXMMATRIX& m);
//		static void Transform(_In_reads_(count) const Vector4* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector4* resultArray);

		// Constants
		static const WXMVECTOR4 Zero;
		static const WXMVECTOR4 One;
		static const WXMVECTOR4 UnitX;
		static const WXMVECTOR4 UnitY;
		static const WXMVECTOR4 UnitZ;
		static const WXMVECTOR4 UnitW;
	};

	// Binary operators
	//WXMVECTOR4 operator+ (const WXMVECTOR4& V1, const WXMVECTOR4& V2);
	//WXMVECTOR4 operator- (const WXMVECTOR4& V1, const WXMVECTOR4& V2);
	//WXMVECTOR4 operator* (const WXMVECTOR4& V1, const WXMVECTOR4& V2);
	//WXMVECTOR4 operator* (const WXMVECTOR4& V, float S);
	//WXMVECTOR4 operator/ (const WXMVECTOR4& V1, const WXMVECTOR4& V2);
	//WXMVECTOR4 operator* (float S, const WXMVECTOR4& V);


	//------------------------------------------------------------------------------
	// 4x4 WXMMATRIX (assumes right-handed cooordinates)
	struct WXMMATRIX : public XMFLOAT4X4
	{
		WXMMATRIX() : XMFLOAT4X4(
			1.f, 0, 0, 0,
			0, 1.f, 0, 0,
			0, 0, 1.f, 0,
			0, 0, 0, 1.f) {}

		WXMMATRIX(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) : XMFLOAT4X4(
				m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33) {}
#if 1
		explicit WXMMATRIX(const WXMVECTOR3& r0, const WXMVECTOR3& r1, const WXMVECTOR3& r2) : XMFLOAT4X4(
			r0.x, r0.y, r0.z, 0,
			r1.x, r1.y, r1.z, 0,
			r2.x, r2.y, r2.z, 0,
			0, 0, 0, 1.f) {}
		explicit WXMMATRIX(const WXMVECTOR4& r0, const WXMVECTOR4& r1, const WXMVECTOR4& r2, const WXMVECTOR4& r3) : XMFLOAT4X4(
			r0.x, r0.y, r0.z, r0.w,
			r1.x, r1.y, r1.z, r1.w,
			r2.x, r2.y, r2.z, r2.w,
			r3.x, r3.y, r3.z, r3.w) {}
#endif

		explicit WXMMATRIX(_In_reads_(16) const float *pArray) : XMFLOAT4X4(pArray) {}
		WXMMATRIX(CXMMATRIX M) { XMStoreFloat4x4(this, M); }
		WXMMATRIX(const WXMMATRIX& M)
		{
			using namespace DirectX;
			XMMATRIX x = XMLoadFloat4x4(&M);
			XMStoreFloat4x4(this, x);
		}

		operator XMMATRIX() const
		{
			return XMLoadFloat4x4(this);
		}

		// Comparision operators
		bool operator == (const WXMMATRIX& M) const;
		bool operator != (const WXMMATRIX& M) const;

		// Assignment operators
		WXMMATRIX& operator= (const WXMMATRIX& M) { memcpy_s(this, sizeof(float) * 16, &M, sizeof(float) * 16); return *this; }
		WXMMATRIX& operator+= (const WXMMATRIX& M);
		WXMMATRIX& operator-= (const WXMMATRIX& M);
		WXMMATRIX& operator*= (const WXMMATRIX& M);
		WXMMATRIX& operator/= (const WXMMATRIX& M);
		WXMMATRIX& operator*= (float S);
		WXMMATRIX& operator/= (float S);
		// Element-wise divide

		// Urnary operators
		WXMMATRIX operator+ () const { return *this; }
		WXMMATRIX operator- () const;
#if 0
		// Properties
		Vector3 Up() const { return Vector3(_21, _22, _23); }
		void Up(const Vector3& v) { _21 = v.x; _22 = v.y; _23 = v.z; }

		Vector3 Down() const { return Vector3(-_21, -_22, -_23); }
		void Down(const Vector3& v) { _21 = -v.x; _22 = -v.y; _23 = -v.z; }

		Vector3 Right() const { return Vector3(_11, _12, _13); }
		void Right(const Vector3& v) { _11 = v.x; _12 = v.y; _13 = v.z; }

		Vector3 Left() const { return Vector3(-_11, -_12, -_13); }
		void Left(const Vector3& v) { _11 = -v.x; _12 = -v.y; _13 = -v.z; }

		Vector3 Forward() const { return Vector3(-_31, -_32, -_33); }
		void Forward(const Vector3& v) { _31 = -v.x; _32 = -v.y; _33 = -v.z; }

		Vector3 Backward() const { return Vector3(_31, _32, _33); }
		void Backward(const Vector3& v) { _31 = v.x; _32 = v.y; _33 = v.z; }

		Vector3 Translation() const { return Vector3(_41, _42, _43); }
		void Translation(const Vector3& v) { _41 = v.x; _42 = v.y; _43 = v.z; }

		// WXMMATRIX operations
		bool Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation);
#endif

		WXMMATRIX Transpose() const;
		void Transpose(WXMMATRIX& result) const;

		WXMMATRIX Invert() const;
		void Invert(WXMMATRIX& result) const;

		float Determinant() const;
#if 0
		// Static functions
		static WXMMATRIX CreateBillboard(const Vector3& object, const Vector3& cameraPosition, const Vector3& cameraUp, _In_opt_ const Vector3* cameraForward = nullptr);

		static WXMMATRIX CreateConstrainedBillboard(const Vector3& object, const Vector3& cameraPosition, const Vector3& rotateAxis,
			_In_opt_ const Vector3* cameraForward = nullptr, _In_opt_ const Vector3* objectForward = nullptr);

		static WXMMATRIX CreateTranslation(const Vector3& position);
		static WXMMATRIX CreateTranslation(float x, float y, float z);

		static WXMMATRIX CreateScale(const Vector3& scales);
		static WXMMATRIX CreateScale(float xs, float ys, float zs);
		static WXMMATRIX CreateScale(float scale);

		static WXMMATRIX CreateRotationX(float radians);
		static WXMMATRIX CreateRotationY(float radians);
		static WXMMATRIX CreateRotationZ(float radians);

		static WXMMATRIX CreateFromAxisAngle(const Vector3& axis, float angle);

		static WXMMATRIX CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane);
		static WXMMATRIX CreatePerspective(float width, float height, float nearPlane, float farPlane);
		static WXMMATRIX CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane);
		static WXMMATRIX CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane);
		static WXMMATRIX CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane);

		static WXMMATRIX CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up);
		static WXMMATRIX CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up);
#endif
		static WXMMATRIX CreateTranslation(const WXMVECTOR3& position);
		static WXMMATRIX CreateTranslation(float x, float y, float z);
		static WXMMATRIX CreatePerspectiveFovLH(float fov, float aspectRatio, float nearPlane, float farPlane);
		static WXMMATRIX CreatePerspectiveLH(float width, float height, float nearPlane, float farPlane);
		static WXMMATRIX CreateOrthographicLH(float width, float height, float zNearPlane, float zFarPlane);
		static WXMMATRIX CreateLookAtLH(const WXMVECTOR3& position, const WXMVECTOR3& target, const WXMVECTOR3& up);
		static WXMMATRIX CreateLookAtRH(const WXMVECTOR3& position, const WXMVECTOR3& target, const WXMVECTOR3& up);
		static WXMMATRIX CreateIdentity();
		static WXMMATRIX CreateRotationRollPitchYaw(float pitch, float yaw, float roll);
#if 0
		static WXMMATRIX CreateFromQuaternion(const Quaternion& quat);

		static WXMMATRIX CreateFromYawPitchRoll(float yaw, float pitch, float roll);

		static WXMMATRIX CreateShadow(const Vector3& lightDir, const Plane& plane);

		static WXMMATRIX CreateReflection(const Plane& plane);

		static void Lerp(const WXMMATRIX& M1, const WXMMATRIX& M2, float t, WXMMATRIX& result);
		static WXMMATRIX Lerp(const WXMMATRIX& M1, const WXMMATRIX& M2, float t);

		static void Transform(const WXMMATRIX& M, const Quaternion& rotation, WXMMATRIX& result);
		static WXMMATRIX Transform(const WXMMATRIX& M, const Quaternion& rotation);
#endif
		// Constants
//		static const WXMMATRIX Identity;
	};

	// Binary operators
	WXMMATRIX operator+ (const WXMMATRIX& M1, const WXMMATRIX& M2);
	WXMMATRIX operator- (const WXMMATRIX& M1, const WXMMATRIX& M2);
	WXMMATRIX operator* (const WXMMATRIX& M1, const WXMMATRIX& M2);
	WXMMATRIX operator* (const WXMMATRIX& M, float S);
	WXMMATRIX operator/ (const WXMMATRIX& M, float S);
	WXMMATRIX operator/ (const WXMMATRIX& M1, const WXMMATRIX& M2);
	// Element-wise divide
	WXMMATRIX operator* (float S, const WXMMATRIX& M);

	//-----------------------------------------------------------------------------
	// Plane
	struct WXMPLANE : public XMFLOAT4
	{
		WXMPLANE() : XMFLOAT4(0.f, 1.f, 0.f, 0.f) {}
		WXMPLANE(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		WXMPLANE(const WXMVECTOR3& normal, float d) : XMFLOAT4(normal.x, normal.y, normal.z, d) {}
		WXMPLANE(const WXMVECTOR3& point1, const WXMVECTOR3& point2, const WXMVECTOR3& point3);
		WXMPLANE(const WXMVECTOR3& point, const WXMVECTOR3& normal);
		explicit WXMPLANE(const WXMVECTOR4& v) : XMFLOAT4(v.x, v.y, v.z, v.w) {}
		explicit WXMPLANE(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
		WXMPLANE(FXMVECTOR V) { XMStoreFloat4(this, V); }

		operator XMVECTOR() const { return XMLoadFloat4(this); }

		// Comparision operators
		bool operator == (const WXMPLANE& p) const;
		bool operator != (const WXMPLANE& p) const;

		// Assignment operators
		WXMPLANE& operator= (const WXMPLANE& p) { x = p.x; y = p.y; z = p.z; w = p.w; return *this; }

		// Properties
		WXMVECTOR3 Normal() const { return WXMVECTOR3(x, y, z); }
		void Normal(const WXMVECTOR3& normal) { x = normal.x; y = normal.y; z = normal.z; }

		float D() const { return w; }
		void D(float d) { w = d; }

		// Plane operations
		void Normalize();
//		void Normalize(WXMPLANE& result) const;

		float Dot(const WXMVECTOR4& v) const;
		float DotCoordinate(const WXMVECTOR3& position) const;
		float DotNormal(const WXMVECTOR3& normal) const;

		// Static functions
//		static void Transform(const WXMPLANE& plane, const WXMMATRIX& M, WXMPLANE& result);
		static WXMPLANE Transform(const WXMPLANE& plane, const WXMMATRIX& M);

//		static void Transform(const WXMPLANE& plane, const Quaternion& rotation, WXMPLANE& result);
//		static WXMPLANE Transform(const WXMPLANE& plane, const Quaternion& rotation);
		// Input quaternion must be the inverse transpose of the transformation
	};

#include "XMWrapper.inl"

}; // namespace WrappedXM

//}; // namespace DirectX

#endif