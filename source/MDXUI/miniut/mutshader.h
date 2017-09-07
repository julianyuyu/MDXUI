#pragma once

#include "inc.h"
#include "creator.h"
#include "XMWrapper.h"

//using namespace DirectX;
using namespace WrappedXM;

class MUTShader
{
	//
	// !!!!const buffer must be same as the one in *.ps/*.vs !!!!
	//
	struct CB_MATRIX
	{
		WXMMATRIX world;
		WXMMATRIX view;
		WXMMATRIX projection;
	};

public:
	MUTShader(ID3D11Device* dev) : m_pDevice(dev), m_pImmeDC(nullptr) { Initialize(); }
	virtual ~MUTShader() { Destroy(); }

	virtual bool CreateVSAndLayout(LPWSTR file, LPSTR code, LPSTR vsmain, LPSTR *SemanticTable = nullptr, int layoutCount = 0);
	virtual bool AppendShader(LPWSTR file, LPSTR code, LPSTR shadermain, LPSTR profile = "ps_5_0", LPD3D_SHADER_MACRO macro = nullptr, LPD3DINCLUDE inc = nullptr);
	virtual bool CreateLayout(LPSTR* SemanticTable)
	{
		//int
		return false;
	}

	virtual bool CreateResource(int* cbSizeTable = nullptr, int cbCount = 0);
	//	virtual void SetVS(int idx) {}
	virtual void SetHS(int idx) { if (!m_HSList.empty()) m_pHS = m_HSList.at(idx); }
	virtual void SetDS(int idx) { if (!m_DSList.empty()) m_pDS = m_DSList.at(idx); }
	virtual void SetGS(int idx) { if (!m_GSList.empty()) m_pGS = m_GSList.at(idx); }
	virtual void SetPS(int idx) { if (!m_PSList.empty()) m_pPS = m_PSList.at(idx); }

	virtual void Render(UINT idxCount, WXMMATRIX* mat_world, WXMMATRIX* mat_view, WXMMATRIX* mat_projection, void* renderInput = nullptr,
		ID3D11ShaderResourceView** texarray = nullptr, int texcount = 0, UINT* texSlotArray = nullptr, int slotCount = 0)
	{
		if (!UpdateShaderMatrix(mat_world, mat_view, mat_projection)) return;
		if (!UpdateShaderResource(renderInput)) return;
		if (!UpdatePSTexture(texarray, texcount, texSlotArray, slotCount)) return;
		Draw(idxCount);
	}

protected:
	virtual void Initialize()
	{
		if (m_pDevice)
			m_pDevice->GetImmediateContext(&m_pImmeDC);
		//		m_bHasHsDs = false;
		m_bUseSamplerStateClamp = false;
		m_vsCbIndex = 0;
		//		m_LayoutCount = 0;
		m_pSamplerState = nullptr;
		m_pSamplerStateClamp = nullptr;
		m_pVS = nullptr;
		m_pGS = nullptr;
		m_pHS = nullptr;
		m_pDS = nullptr;
		m_pPS = nullptr;
		m_layout = nullptr;
		m_pCbMatrix = nullptr;
		m_HSList.clear();
		m_DSList.clear();
		m_GSList.clear();
		m_PSList.clear();
		m_CBList.clear();
	}

	virtual void Destroy()
	{
		SAFERELEASE(m_pVS);
		SAFERELEASE(m_pGS);
		SAFERELEASE(m_layout);
		SAFERELEASE(m_pSamplerStateClamp);
		SAFERELEASE(m_pSamplerState);
		SAFERELEASE(m_pCbMatrix);

		SafeReleaseList(m_HSList);
		SafeReleaseList(m_DSList);
		SafeReleaseList(m_GSList);
		SafeReleaseList(m_PSList);
		m_pHS = nullptr;
		m_pDS = nullptr;
		m_pGS = nullptr;
		m_pPS = nullptr;

		SafeReleaseList(m_CBList);
	}

	virtual void SetSampler();
	//	virtual bool CreateShaderCode(LPSTR vscode, LPSTR vsmain, LPSTR pscode, LPSTR psmain, int layoutCount = 2, bool bColorShader = false);
	//	virtual bool CreateShader(LPWSTR vsname, LPSTR vsmain, LPWSTR psname, LPSTR psmain, int layoutCount = 2, bool bColorShader = false);// { return false; }
	virtual bool UpdateShaderMatrix(WXMMATRIX* worldMatrix, WXMMATRIX* viewMatrix, WXMMATRIX* projectionMatrix);
	virtual bool UpdatePSTexture(ID3D11ShaderResourceView** texarray = nullptr, int texcount = 0, UINT* texGroupArray = nullptr, int GroupCount = 0);
	virtual bool UpdateShaderResource(void* renderInput) { return true; }
	virtual void SetPipeline();
	virtual void DrawIndexed(UINT idxCount);
	virtual void Draw(UINT vtxCount);

	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pImmeDC;

	//	int m_LayoutCount;
	ID3D11SamplerState *m_pSamplerState;
	ID3D11SamplerState* m_pSamplerStateClamp;
	ID3D11Buffer *m_pCbMatrix;// Const buffer for matrix
	UINT m_vsCbIndex;
	//	bool m_bHasHsDs;
	bool m_bUseSamplerStateClamp;

	ID3D11InputLayout  *m_layout;
	ID3D11VertexShader *m_pVS; // current VS (support 1 vs for now)
	ID3D11GeometryShader *m_pGS; // current GS
	ID3D11HullShader   *m_pHS; // current HS
	ID3D11DomainShader *m_pDS; // current DS
	ID3D11PixelShader  *m_pPS; // current PS

	// for multi shader support.
	std::vector<ID3D11HullShader *> m_HSList;
	std::vector<ID3D11DomainShader *> m_DSList;
	std::vector<ID3D11GeometryShader *> m_GSList;
	std::vector<ID3D11PixelShader *> m_PSList;

	std::vector<ID3D11Buffer *> m_CBList;
};
