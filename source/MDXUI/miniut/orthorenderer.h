#pragma once

#include "creator.h"
#include "muttex.h"
#include "mutshader.h"
#include "XMWrapper.h"

using namespace WrappedXM;

#define USE_LOCAL_RASTER_STATE	1

class OrthoModel
{
protected:
	struct ORTHO_VERTEX
	{
		WXMVECTOR3 position;
		WXMVECTOR2 texture;
	};

	inline ORTHO_VERTEX InitVertex(float x, float y, float z, WXMVECTOR2& uv)
	{
		ORTHO_VERTEX vtx = {};
		vtx.position = WXMVECTOR3(x, y, z);
		vtx.texture = uv;
		return vtx;
	}

	inline ORTHO_VERTEX InitVertex(float x, float y, float z, float u, float v)
	{
		return InitVertex(x, y, z, WXMVECTOR2(u, v));
		//ORTHO_VERTEX vtx = {};
		//vtx.position = WXMVECTOR3(x, y, z);
		//vtx.texture = WXMVECTOR2(u, v);
		//return vtx;
	}

public:
	OrthoModel():
		m_pVb(nullptr),
		m_pIb(nullptr),
		m_pVertices(nullptr),
		m_pIndices(nullptr),
		m_QuadCount(0),
		m_pImmeDC(nullptr)
	{
	}
	OrthoModel(ID3D11Device* dev, int w, int h):
		m_pVb(nullptr),
		m_pIb(nullptr),
		m_pVertices(nullptr),
		m_pIndices(nullptr),
		m_QuadCount(0),
		m_pImmeDC(nullptr),
		m_pDevice(dev),
		m_wScreen(w),
		m_hScreen(h)
	{
//		Initialize(m_pDevice, w, h);
	}

	virtual ~OrthoModel() { Destroy(); }
	virtual void Initialize(ID3D11Device* dev, int w, int h)
	{
		m_pDevice = dev;
		//if (m_pDevice)
		//	m_pDevice->GetImmediateContext(&m_pImmeDC);

		m_wScreen = w;
		m_hScreen = h;
	}

	virtual void InitQuadVertices(ORTHO_VERTEX*& vertices, float x, float y, float w, float h, float u1, float v1, float u2, float v2)
	{
		// First triangle in quad.
		*vertices++ = InitVertex(x, y, 0.0f, u1, v1);// Bottom right.
		*vertices++ = InitVertex(x + w, y - h, 0.0f, u2, v2);// Bottom left.
		*vertices++ = InitVertex(x, y - h, 0.0f, u1, v2);// Top left.
		// Second triangle in quad.
		*vertices++ = InitVertex(x, y, 0.0f, u1, v1);// Bottom right.
		*vertices++ = InitVertex(x + w, y, 0.0f, u2, v1);// Top left.
		*vertices++ = InitVertex(x + w, y - h, 0.0f, u2, v2);// Top right.
	}
protected:
	virtual void Destroy()
	{
		SAFERELEASE(m_pVb);
		SAFERELEASE(m_pIb);
		SAFEDELETEARRAY(m_pVertices);
		SAFEDELETEARRAY(m_pIndices);
		m_QuadCount = 0;
		SAFERELEASE(m_pImmeDC);
	}
	virtual bool CreateModel();
	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pImmeDC;
	ID3D11Buffer *m_pVb;
	ID3D11Buffer *m_pIb;
	ORTHO_VERTEX *m_pVertices;
	ULONG *m_pIndices;
	int m_QuadCount;
	int m_VertexCount;
	int m_IndexCount;
	int m_wScreen; // screen width
	int m_hScreen; // screen height
//	int m_PosX;
//	int m_PosY;
};

class OrthoRenderer
{
public:
	OrthoRenderer(ID3D11Device* dev, int w, int h) :m_pDevice(dev), m_pTexList(nullptr)
#if USE_LOCAL_RASTER_STATE
		,m_pImmeDC(nullptr/*dc*/), 
		m_pRasterSolidState(nullptr),	m_pDepthDisabledState(nullptr), 
		/*m_pBlendDisabledState(nullptr), */m_pBlendEnabledState(nullptr)
#endif
	{
#if USE_LOCAL_RASTER_STATE
		if (m_pDevice)
			m_pDevice->GetImmediateContext(&m_pImmeDC);
#endif
		InitMatrix(w, h);
		InitRenderer();
	}
	virtual ~OrthoRenderer()
	{
		SAFEDELETE(m_pTexList);
#if USE_LOCAL_RASTER_STATE
		SAFERELEASE(m_pRasterSolidState);
		SAFERELEASE(m_pBlendEnabledState);
//		SAFERELEASE(m_pBlendDisabledState);
		SAFERELEASE(m_pDepthDisabledState);
		SAFERELEASE(m_pImmeDC);
#endif
	}
//	virtual void Render(int indexCount, WXMVECTOR4* color) = 0;
	virtual void Render(int indexCount)
	{
#if USE_LOCAL_RASTER_STATE
		//AutoBlendStateHolder bsh(m_pImmeDC);
		//AutoDSStateHolder dssh(m_pImmeDC);
		//AutoRasterStateHolder rsh(m_pImmeDC, m_pRasterState);
		//EnableAlphaBlendState();
		//DisableDepthState();
#endif
	}
protected:
	virtual void InitMatrix(int w, int h)
	{
		m_MatView = WXMMATRIX::CreateIdentity();

		// Create an orthographic projection matrix for 2D rendering.
		float znear = 0.f - 1.f;// -1.f;
		float zfar = 1000.f;
		m_MatOrtho = WXMMATRIX::CreateOrthographicLH((float)w, (float)h, znear, zfar);
	}

	virtual bool InitRenderer()
	{
		if (m_pDevice)
		{
#if USE_LOCAL_RASTER_STATE
			StateCreator stctr(m_pDevice);
			m_pRasterSolidState = stctr.CreateRasterState(D3D11_FILL_SOLID);
			m_pDepthDisabledState = stctr.CreateDepthStencilState(false, true);
			m_pBlendEnabledState = stctr.CreateBlendState(0, true, D3D11_BLEND_SRC_ALPHA);
//			m_pBlendDisabledState = stctr.CreateBlendState(0, false, D3D11_BLEND_SRC_ALPHA);
#endif
			return true;
		}
		return false;
	}
#if USE_LOCAL_RASTER_STATE
	virtual void DisableDepthState()
	{
		//m_pImmeDC->OMSetDepthStencilState(enable ? m_pDepthEnabledState : m_pDepthDisabledState, 1);
		m_pImmeDC->OMSetDepthStencilState(m_pDepthDisabledState, 1);
	}

	virtual void EnableAlphaBlendState()
	{
		float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
		// Turn on/off the alpha blending.
		//m_pImmeDC->OMSetBlendState(enable ? m_pABlendEnabledState : m_pABlendDisabledState, blendFactor, 0xffffffff);
		m_pImmeDC->OMSetBlendState(m_pBlendEnabledState, blendFactor, 0xffffffff);
	}
	ID3D11DeviceContext* m_pImmeDC;
#endif
	ID3D11Device* m_pDevice;
	MUTTexList *m_pTexList;

//	WXMMATRIX m_MatWorld;
	WXMMATRIX m_MatOrtho;
	WXMMATRIX m_MatView;

#if USE_LOCAL_RASTER_STATE
	ID3D11RasterizerState* m_pRasterSolidState;
	ID3D11DepthStencilState *m_pDepthDisabledState;
	ID3D11BlendState *m_pBlendEnabledState;
//	ID3D11BlendState *m_pBlendDisabledState;
#endif
};

class OrthoShader
{
	struct ORTHO_CB_MATRIX
	{
		WXMMATRIX view;
		WXMMATRIX projection;
	};
public:
	OrthoShader(ID3D11Device* dev) : m_pDevice(dev), m_pImmeDC(nullptr), m_pVS(nullptr), m_pPS(nullptr),
		m_layout(nullptr), m_pSamplerState(nullptr), m_pCbMatrix(nullptr)
	{
		if (m_pDevice)
			m_pDevice->GetImmediateContext(&m_pImmeDC);
	}
	virtual ~OrthoShader() { Destroy(); }
	virtual bool Create(LPWSTR vsname, LPSTR vscode, LPSTR vsmain, LPWSTR psname, LPSTR pscode, LPSTR psmain);
	virtual void Render(UINT idxCount, WXMMATRIX* mat_view, WXMMATRIX* mat_projection, ID3D11ShaderResourceView* tex)
	{
		if (!UpdateShaderResource(mat_view, mat_projection)) return;
		if (tex) m_pImmeDC->PSSetShaderResources(0, 1, &tex);
		DrawIndexed(idxCount);
	}

protected:
	virtual void Destroy()
	{
		SAFERELEASE(m_pVS);
		SAFERELEASE(m_pPS);
		SAFERELEASE(m_layout);
		SAFERELEASE(m_pSamplerState);
		SAFERELEASE(m_pCbMatrix);
		SAFERELEASE(m_pImmeDC);
	}

	virtual bool CreateResource();
	virtual bool UpdateShaderResource(WXMMATRIX* viewMatrix, WXMMATRIX* projectionMatrix);
	virtual void DrawIndexed(UINT idxCount);


	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pImmeDC;

	ID3D11VertexShader *m_pVS;
	ID3D11PixelShader  *m_pPS;
	ID3D11InputLayout  *m_layout;
	ID3D11SamplerState *m_pSamplerState;
	ID3D11Buffer *m_pCbMatrix;// Const buffer for matrix
};
