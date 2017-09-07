#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>

#define SHADER_CODE(...) #__VA_ARGS__
/*
template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

#define STRMATCH_N(str1, str2, n) (!_strnicmp((str1), (str2), (n)))
#define SAFEDELETE(ptr)		do {if(ptr) { delete (ptr); (ptr) = NULL; }} while(0)
#define SAFEDELETEARRAY(ptr) do {if(ptr) { delete [] (ptr); (ptr) = NULL; }} while(0)
#define SAFERELEASE(ptr)	do {if(ptr) {(ptr)->Release(); (ptr) = NULL; }} while(0)
#define DESTROYONFAILED(hr) do { if (FAILED(hr)) {Destroy(); return false;} } while(0)
#define RETURNONFAILED(hr)	do { if (FAILED(hr)) { return false;} } while(0)
#define RETURNONNULL(ptr)	do { if (!(ptr)) { return false;} } while(0)
*/

struct TARGA_HEADER
{
	BYTE data1[12];
	WORD width;
	WORD height;
	BYTE bpp;
	BYTE data2;
};

class AutoBlendStateHolder
{
public:
	AutoBlendStateHolder() : m_dc(nullptr), m_state(nullptr), m_mask(0) {}
	AutoBlendStateHolder(ID3D11DeviceContext* dc, ID3D11BlendState* new_state = nullptr, FLOAT* new_factor = nullptr, UINT new_mask = 0xFFFFFFFF)
	{
		Save(dc, new_state, new_factor, new_mask);
	}
	virtual ~AutoBlendStateHolder() { Restore(); }

	virtual void Save(ID3D11DeviceContext* dc, ID3D11BlendState* new_state = nullptr, FLOAT* new_factor = nullptr, UINT new_mask = 0xFFFFFFFF)
	{
		m_dc = dc;
		m_state = nullptr;
		m_dc->OMGetBlendState(&m_state, m_factor, &m_mask);
		if (new_state)
		{
			float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
			if (!new_factor)
				new_factor = blendFactor;
			m_dc->OMSetBlendState(new_state, new_factor, new_mask);
		}
	}
	virtual void Restore()
	{
		m_dc->OMSetBlendState(m_state, m_factor, m_mask);
		SAFERELEASE(m_state);
		m_dc = nullptr;
	}
private:
	ID3D11DeviceContext *m_dc;
	ID3D11BlendState *m_state;
	FLOAT m_factor[4];
	UINT m_mask;
};

class AutoDSStateHolder
{
public:
	AutoDSStateHolder() : m_dc(nullptr), m_state(nullptr), m_ref(0) {}
	AutoDSStateHolder(ID3D11DeviceContext* dc, ID3D11DepthStencilState* new_state = nullptr, UINT new_ref = 1) { Save(dc, new_state, new_ref); }
	virtual ~AutoDSStateHolder() { Restore(); }
	virtual void Save(ID3D11DeviceContext* dc, ID3D11DepthStencilState* new_state = nullptr, UINT new_ref = 1)
	{
		m_dc = dc;
		m_state = nullptr;
		m_dc->OMGetDepthStencilState(&m_state, &m_ref);
		if (new_state)
		{
			m_dc->OMSetDepthStencilState(new_state, new_ref);
		}
	}
	virtual void Restore()
	{
		m_dc->OMSetDepthStencilState(m_state, m_ref);
		SAFERELEASE(m_state);
		m_dc = nullptr;
	}
private:
	ID3D11DeviceContext *m_dc;
	ID3D11DepthStencilState *m_state;
	UINT m_ref;
};

class AutoRasterStateHolder
{
public:
	AutoRasterStateHolder() : m_dc(nullptr), m_state(nullptr) {}
	AutoRasterStateHolder(ID3D11DeviceContext* dc, ID3D11RasterizerState* new_state = nullptr) { Save(dc, new_state); }
	virtual ~AutoRasterStateHolder() { Restore(); }
	virtual void Save(ID3D11DeviceContext* dc, ID3D11RasterizerState* new_state = nullptr)
	{
		m_dc = dc;
		m_state = nullptr;
		m_dc->RSGetState(&m_state);
		if (new_state)
		{
			m_dc->RSSetState(new_state);
		}
	}
	virtual void Restore()
	{
		m_dc->RSSetState(m_state);
		SAFERELEASE(m_state);
		m_dc = nullptr;
	}
private:
	ID3D11DeviceContext *m_dc;
	ID3D11RasterizerState *m_state;
};

class ResMapper
{
public:
	ResMapper(ID3D11DeviceContext* dc, ID3D11Buffer* cb, D3D11_MAP flag = D3D11_MAP_WRITE_DISCARD)
	{
		m_dc = dc;
		m_cb = cb;
		D3D11_MAPPED_SUBRESOURCE Mapped;
		HRESULT hr = m_dc->Map(m_cb, 0, flag, 0, &Mapped);
		if (SUCCEEDED(hr)) m_ptr = Mapped.pData;
		else m_ptr = nullptr;
	}
	virtual ~ResMapper() { Close(); }
	virtual void Close() { if (m_ptr) { m_dc->Unmap(m_cb, 0); m_ptr = nullptr; } }
	virtual void* Ptr() { return m_ptr; }
private:
	ID3D11DeviceContext* m_dc;
	ID3D11Buffer* m_cb;
	void *m_ptr;
};

class ResWriter
{
public:
	ResWriter(ID3D11DeviceContext* dc /*= nullptr*/ /*must set DC*/, ID3D11Buffer* cb = nullptr) { m_dc = dc; m_cb = cb; }
	virtual ~ResWriter() {}

	virtual bool Copy(void* ptr, int size, ID3D11Buffer* cb = nullptr, D3D11_MAP flag = D3D11_MAP_WRITE_DISCARD)
	{
		if (cb)
			m_cb = cb;

		if (m_dc == nullptr || m_cb == nullptr)
			return false;

		D3D11_MAPPED_SUBRESOURCE Mapped;
		HRESULT hr = m_dc->Map(m_cb, 0, flag, 0, &Mapped);
		RETURNONFAILED(hr);
		//ZeroMemory(Mapped.pData, size);
		CopyMemory(Mapped.pData, ptr, size);
		m_dc->Unmap(m_cb, 0);
		return true;
	}
private:
	ID3D11DeviceContext* m_dc;
	ID3D11Buffer* m_cb;
};


template <typename T> class TCbWriter
{
public:
	TCbWriter() {}
	TCbWriter(ID3D11DeviceContext* dc /*= nullptr*/ /*must set DC*/, ID3D11Buffer* cb = nullptr) { m_dc = dc; m_cb = cb; }
	virtual ~TCbWriter() {}

	virtual bool Copy(T* ptr, ID3D11Buffer* cb = nullptr, D3D11_MAP flag = D3D11_MAP_WRITE_DISCARD)
	{
		if (cb)
			m_cb = cb;

		if (m_dc == nullptr || m_cb == nullptr)
			return false;

		D3D11_MAPPED_SUBRESOURCE Mapped;
		HRESULT hr = m_dc->Map(m_cb, 0, flag, 0, &Mapped);
		RETURNONFAILED(hr);
		T *dst = (T *)Mapped.pData;
		*dst = *ptr;
		m_dc->Unmap(m_cb, 0);
		return true;
	}
private:
	ID3D11DeviceContext* m_dc;
	ID3D11Buffer* m_cb;
};


class LayoutCreator
{
public:
	LayoutCreator(int descCount, ID3D11Device* d = nullptr) : m_Count(descCount), m_pDevice(d) { m_descs = new D3D11_INPUT_ELEMENT_DESC[descCount]; }
	virtual ~LayoutCreator() { SAFEDELETEARRAY(m_descs); }
	virtual void Desc(int index, PSTR semanticname, UINT semanticidx = 0, UINT alignedoffset = 0, DXGI_FORMAT fmt = DXGI_FORMAT_R32G32B32A32_FLOAT,
			UINT slot=0, D3D11_INPUT_CLASSIFICATION slotclass = D3D11_INPUT_PER_VERTEX_DATA, UINT instanceStep = 0);
	virtual ID3D11InputLayout* Create(LPVOID vsptr, UINT vssize, ID3D11Device* d = nullptr);
private:
	int m_Count;
	ID3D11Device* m_pDevice;
	D3D11_INPUT_ELEMENT_DESC* m_descs;
};

class ShaderCreator
{
public:
	ShaderCreator(ID3D11Device* d) : m_pDevice(d), m_pBlob(nullptr), m_ByteCodePtr(nullptr), m_ByteCodeLength(0) {}
	virtual ~ShaderCreator() { SAFERELEASE(m_pBlob); }
	virtual void* Create(LPWSTR filename, LPSTR shadermain, LPSTR profile, LPD3D_SHADER_MACRO macro = nullptr, LPD3DINCLUDE inc = nullptr);
	virtual void* CreateSource(LPSTR sourcecode, LPSTR shadermain, LPSTR profile, LPD3D_SHADER_MACRO macro = nullptr, LPD3DINCLUDE inc = nullptr);
	virtual void OutputError(ID3D10Blob* errMsg, PWSTR filename);
	virtual void* CreateVS5_0(LPWSTR f, LPSTR s) { return Create(f, s, "vs_5_0"); }
	virtual void* CreatePS5_0(LPWSTR f, LPSTR s) { return Create(f, s, "ps_5_0"); }
	//virtual ID3D10Blob* GetBlob() { return m_pBlob; }
	virtual void* ByteCodePtr() { return m_ByteCodePtr; }
	virtual SIZE_T ByteCodeLength() { return m_ByteCodeLength; }
private:
	ID3D11Device *m_pDevice;
	ID3D10Blob *m_pBlob;
	void *m_ByteCodePtr;
	SIZE_T m_ByteCodeLength;
};

class BufferCreator
{
public:
	BufferCreator(ID3D11Device* d) : m_pDevice(d) {}
	virtual ~BufferCreator() {}
	virtual ID3D11Buffer* Create(UINT size, UINT count, D3D11_BIND_FLAG type = D3D11_BIND_VERTEX_BUFFER,
		void* data = nullptr, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT access = 0);
	virtual ID3D11Buffer* CreateCB(UINT ConstBufferSize, UINT ConstBufferCount = 1)
	{
		return Create(ConstBufferSize, ConstBufferCount, D3D11_BIND_CONSTANT_BUFFER, nullptr, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	}
private:
	ID3D11Device* m_pDevice;
};

class TextureCreator
{
	enum FILE_TYPE
	{
		NONE = 0,
		BMP = 1,
		TGA = 2,
		DDS = 3,
	};
public:
	TextureCreator(ID3D11Device* d, ID3D11DeviceContext* dc = nullptr) : m_pDevice(d), m_dc(dc) {}
	virtual ~TextureCreator() {}
	ID3D11ShaderResourceView* CreateFromMemory(void * data, int size, int width = 0, int height = 0, int bpp = 4, bool autogen = false/*, ID3D11DeviceContext* dc*//* = nullptr*/);
	ID3D11ShaderResourceView* CreateFromFile(PWSTR filename, bool autogen = false/*, ID3D11DeviceContext* dc = nullptr*/);
	ID3D11ShaderResourceView* Create2D(ID3D11Texture2D** pptex, UINT w, UINT h,	DXGI_FORMAT f = DXGI_FORMAT_R8G8B8A8_UNORM, 
						UINT bytePP = 4, void* dataptr=nullptr, UINT type = D3D11_BIND_SHADER_RESOURCE, UINT miplevels =-1);

	ID3D11RenderTargetView* CreateRT(ID3D11Texture2D** pptex, UINT w, UINT h, DXGI_FORMAT f = DXGI_FORMAT_R32G32B32A32_FLOAT,
						UINT type = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	ID3D11DepthStencilView* CreateDepthStencil(ID3D11Texture2D** pptex, UINT w, UINT h, DXGI_FORMAT f = DXGI_FORMAT_D24_UNORM_S8_UINT,
		UINT type = D3D11_BIND_DEPTH_STENCIL);
	ID3D11RenderTargetView* CreateOnlyRTV(ID3D11Texture2D* tex, DXGI_FORMAT f = DXGI_FORMAT_R32G32B32A32_FLOAT);
	ID3D11DepthStencilView* CreateOnlyDSV(ID3D11Texture2D* tex, DXGI_FORMAT f = DXGI_FORMAT_D24_UNORM_S8_UINT);
	ID3D11ShaderResourceView* CreateOnlySRV2D(ID3D11Texture2D* tex, DXGI_FORMAT f = DXGI_FORMAT_R8G8B8A8_UNORM, UINT miplevels =-1);
private:
	ID3D11Texture2D* CreateTex2D(UINT w, UINT h, DXGI_FORMAT f, UINT bytePP, void* dataptr, UINT type, bool autogen = false);
	BYTE* LoadFile(PWSTR filename, UINT bpp, UINT& width, UINT& height, FILE_TYPE* ptype = nullptr);
	int CheckFileType(PWSTR filename);
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_dc;
};

class SamplerCreator
{
public:
	SamplerCreator(ID3D11Device* d) { m_pDevice = d; }
	virtual ~SamplerCreator() {}
	ID3D11SamplerState* Create(D3D11_FILTER f = D3D11_FILTER_MIN_MAG_MIP_LINEAR, 
							   D3D11_TEXTURE_ADDRESS_MODE au = D3D11_TEXTURE_ADDRESS_WRAP,
							   D3D11_TEXTURE_ADDRESS_MODE av = D3D11_TEXTURE_ADDRESS_WRAP,
							   D3D11_TEXTURE_ADDRESS_MODE aw = D3D11_TEXTURE_ADDRESS_WRAP)
	{
		// Create texture sampler state.
		D3D11_SAMPLER_DESC desc = {};
		desc.Filter = f;
		desc.AddressU = au;
		desc.AddressV = av;
		desc.AddressW = aw;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.BorderColor[0] = 0;
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 0;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		ID3D11SamplerState *s = nullptr;
		m_pDevice->CreateSamplerState(&desc, &s);
		return s;
	}
private:
	ID3D11Device* m_pDevice;
};

class StateCreator
{
public:
	StateCreator(ID3D11Device* d) { m_pDevice = d; }
	virtual ~StateCreator() {}
	ID3D11DepthStencilState* CreateDepthStencilState(bool d_enable = true, bool s_enable = true,
		D3D11_DEPTH_WRITE_MASK d_wmask = D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_FUNC d_func = D3D11_COMPARISON_LESS,
		UINT8 s_rmask = D3D11_DEFAULT_STENCIL_READ_MASK, UINT s_wmask = D3D11_DEFAULT_STENCIL_WRITE_MASK);
	ID3D11BlendState* CreateBlendState(int RTindex, bool enable,
		D3D11_BLEND src = D3D11_BLEND_ONE, D3D11_BLEND dst = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP op = D3D11_BLEND_OP_ADD,
		D3D11_BLEND srcAlpha = D3D11_BLEND_ONE, D3D11_BLEND dstAlpha = D3D11_BLEND_ZERO, D3D11_BLEND_OP opAlpha = D3D11_BLEND_OP_ADD);

	ID3D11RasterizerState* CreateRasterState(D3D11_FILL_MODE fill = D3D11_FILL_SOLID, D3D11_CULL_MODE cull = D3D11_CULL_BACK,
		bool DepthClipEnable = true, bool ScissorEnable = false, bool MultisampleEnable = false);
private:
	ID3D11Device* m_pDevice;
};
