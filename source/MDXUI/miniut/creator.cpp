#include "inc.h"
#include "creator.h"
#include "ddsloader.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3dx10.lib")
//#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "XMWrapper.h"
//using namespace DirectX;
using namespace WrappedXM;

void* ShaderCreator::Create(LPWSTR filename, LPSTR shadermain, LPSTR profile, LPD3D_SHADER_MACRO macro, LPD3DINCLUDE inc)
{
	ID3D10Blob* errMsg = nullptr;
	ID3D10Blob* blob = nullptr;

//	HRESULT hr = D3DX11CompileFromFile(filename, NULL, NULL, shadermain, profile, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &blob, &errMsg, NULL);
	HRESULT hr = D3DCompileFromFile(filename, macro, inc, shadermain, profile, D3DCOMPILE_ENABLE_STRICTNESS, 0, &blob, &errMsg);
//	HRESULT hr = E_FAIL;
	if (FAILED(hr))
	{
		OutputError(errMsg, filename);
		SAFERELEASE(errMsg);
		return nullptr;
	}

	void *ptr = nullptr;

	SAFERELEASE(m_pBlob);
	m_pBlob = blob;

	m_ByteCodePtr = blob->GetBufferPointer();
	m_ByteCodeLength = blob->GetBufferSize();
	if (STRMATCH_N(profile, "vs", 2))
	{
		hr = m_pDevice->CreateVertexShader(m_ByteCodePtr, m_ByteCodeLength, NULL, (ID3D11VertexShader **)&ptr);
	}
	else if (STRMATCH_N(profile, "ps", 2))
	{
		hr = m_pDevice->CreatePixelShader(m_ByteCodePtr, m_ByteCodeLength, NULL, (ID3D11PixelShader **)&ptr);
	}
	else if (STRMATCH_N(profile, "hs", 2))
	{
		hr = m_pDevice->CreateHullShader(m_ByteCodePtr, m_ByteCodeLength, NULL, (ID3D11HullShader **)&ptr);
	}
	else if (STRMATCH_N(profile, "ds", 2))
	{
		hr = m_pDevice->CreateDomainShader(m_ByteCodePtr, m_ByteCodeLength, NULL, (ID3D11DomainShader **)&ptr);
	}
	else
	{
		hr = E_FAIL;
	}
//	SAFERELEASE(blob);
	RETURNONFAILED(hr);

	return ptr;
}

void* ShaderCreator::CreateSource(LPSTR sourcecode, LPSTR shadermain, LPSTR profile, LPD3D_SHADER_MACRO macro, LPD3DINCLUDE inc)
{
	ID3D10Blob* errMsg = nullptr;
	ID3D10Blob* blob = nullptr;

	int len = (int)strlen(sourcecode);
	HRESULT hr = D3DCompile(sourcecode, len, NULL, macro, inc, shadermain, profile, D3DCOMPILE_ENABLE_STRICTNESS, 0, &blob, &errMsg);
	if (FAILED(hr))
	{
		OutputError(errMsg, L"shader source");
		SAFERELEASE(errMsg);
		return nullptr;
	}

	void *ptr = nullptr;
	SAFERELEASE(m_pBlob);
	m_pBlob = blob;
	m_ByteCodePtr = blob->GetBufferPointer();
	m_ByteCodeLength = blob->GetBufferSize();
	if (STRMATCH_N(profile, "vs", 2))
	{
		hr = m_pDevice->CreateVertexShader(m_ByteCodePtr, m_ByteCodeLength, NULL, (ID3D11VertexShader **)&ptr);
	}
	else if (STRMATCH_N(profile, "ps", 2))
	{
		hr = m_pDevice->CreatePixelShader(m_ByteCodePtr, m_ByteCodeLength, NULL, (ID3D11PixelShader **)&ptr);
	}
	else if (STRMATCH_N(profile, "hs", 2))
	{
		hr = m_pDevice->CreateHullShader(m_ByteCodePtr, m_ByteCodeLength, NULL, (ID3D11HullShader **)&ptr);
	}
	else if (STRMATCH_N(profile, "ds", 2))
	{
		hr = m_pDevice->CreateDomainShader(m_ByteCodePtr, m_ByteCodeLength, NULL, (ID3D11DomainShader **)&ptr);
	}
	else
	{
		hr = E_FAIL;
	}
//	SAFERELEASE(blob);
	RETURNONFAILED(hr);

	return ptr;
}

void ShaderCreator::OutputError(ID3D10Blob* errMsg, PWSTR filename)
{
	if (!errMsg)
	{
		MessageBox(NULL, filename, L"Missing Shader File", MB_OK);
	}
	else
	{
		LPSTR msg = (LPSTR)(errMsg->GetBufferPointer());
		SIZE_T len = errMsg->GetBufferSize();

		FILE *file = NULL;
		fopen_s(&file, "shader_error.txt", "a+t");
		fwprintf_s(file, L"\n%s\n", filename);
		fwrite(msg, 1, len, file);
		fprintf_s(file, "\n");
		//fprintf_s(file, "%s\n\n", msg);
		fclose(file);

		MessageBox(NULL, L"compiling error, Check shader_error.txt for log.", filename, MB_OK);
	}
}

void LayoutCreator::Desc(int index, PSTR semanticname, UINT semanticidx, UINT alignedoffset, DXGI_FORMAT fmt, UINT slot, D3D11_INPUT_CLASSIFICATION slotclass, UINT instanceStep)
{
	m_descs[index].SemanticName = semanticname;
	m_descs[index].SemanticIndex = semanticidx;
	m_descs[index].Format = fmt;
	m_descs[index].InputSlot = slot;
	m_descs[index].AlignedByteOffset = alignedoffset;
	m_descs[index].InputSlotClass = slotclass;
	m_descs[index].InstanceDataStepRate = instanceStep;
}

ID3D11InputLayout* LayoutCreator::Create(LPVOID vsptr, UINT vssize, ID3D11Device* d)
{
	if (d)
	{
		m_pDevice = d;
	}

	ID3D11InputLayout *layout = nullptr;
	m_pDevice->CreateInputLayout(m_descs, m_Count, vsptr, vssize, &layout);
	return layout;
}

ID3D11Buffer* BufferCreator::Create(UINT size, UINT count, D3D11_BIND_FLAG type, void* ptr, D3D11_USAGE usage, UINT access)
{
	if (!size || !count)
	{
		return nullptr;
	}

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = usage;
	desc.ByteWidth = size * count;
	desc.BindFlags = type;
	desc.CPUAccessFlags = access;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subdata = {};
	D3D11_SUBRESOURCE_DATA *sub = nullptr;
	if (ptr)
	{
		subdata.pSysMem = ptr;
		subdata.SysMemPitch = 0;
		subdata.SysMemSlicePitch = 0;
		sub = &subdata;
	}

	ID3D11Buffer *buffer = nullptr;
	HRESULT hr = m_pDevice->CreateBuffer(&desc, sub, &buffer);
	if (FAILED(hr))
	{
		return nullptr;
	}
	return buffer;
}

int TextureCreator::CheckFileType(PWSTR filename)
{
	FILE_TYPE t = NONE;
	PWSTR ext = wcsrchr(filename, L'.');
	if (ext)
	{
		ext++;
		if (0 == _wcsicmp(ext, L"bmp"))
		{
			t = BMP;
		}
		else if (0 == _wcsicmp(ext, L"tga"))
		{
			t = TGA;
		}
		else if (0 == _wcsicmp(ext, L"dds"))
		{
			t = DDS;
		}
	}

	if (t == NONE)
	{
		FILE* f;
		BITMAPFILEHEADER bmpHdr;
		DWORD magic;
		if (0 != _wfopen_s(&f, filename, L"rb"))
		{
			return t;
		}

		if (fread(&magic, 1, sizeof(DWORD), f))
		{
			if (magic == DirectX::DDS_MAGIC)
			{
				t = DDS;
			}
			fseek(f, 0, SEEK_SET);
		}

		if (t == NONE && fread(&bmpHdr, 1, sizeof(BITMAPFILEHEADER), f))
		{
			if (bmpHdr.bfType == (('B') | ('M') << 8))
			{
				t = BMP;
			}
			fseek(f, 0, SEEK_SET);
		}
		fclose(f);
	}

	return t;
}

BYTE* TextureCreator::LoadFile(PWSTR filename, UINT bpp, UINT& width, UINT& height, FILE_TYPE* ptype)
{
	FILE* f;
	int imageSize = 0;
	int imageBPP = 0;
	BYTE* image = nullptr;
	BYTE* data = nullptr;

	FILE_TYPE type = (FILE_TYPE)CheckFileType(filename);
	if (ptype)
		*ptype = type;

	if (type == DDS)
	{
#if 0
		// get binary.
		FILE* ftxt = nullptr;
		if (0 != _wfopen_s(&ftxt, L"tex.txt", L"wt"))
		{
			return nullptr;
		}
		if (0 != _wfopen_s(&f, filename, L"rb"))
		{
			return nullptr;
		}
		int fileSize;
		if ((fseek(f, 0, SEEK_END) < 0) ||
			((fileSize = ftell(f)) < 0) ||
			(fseek(f, 0, SEEK_SET) < 0))
		{
			return nullptr;
		}
		BYTE *ptr = new BYTE[fileSize];

		if (!fread(ptr, 1, fileSize, f))
		{
			return nullptr;
		}
		int count = fileSize >> 2;
		DWORD *ptr2 = (DWORD *)ptr;
		for (int i = 0; i < count; i++)
		{
			if (!(i % 8))
			{
				fprintf_s(ftxt, "\n");
				fprintf_s(ftxt, "    ");
			}
			fprintf_s(ftxt, "0x%08X, ", *ptr2++);
		}
		SAFEDELETEARRAY(ptr);
		fclose(ftxt);
		fclose(f);
#endif
		return nullptr;
	}
	else if (type == TGA)
	{
		TARGA_HEADER targaHdr;

		if (0 != _wfopen_s(&f, filename, L"rb"))
		{
			return nullptr;
		}
		if (!fread(&targaHdr, 1, sizeof(TARGA_HEADER), f))
		{
			return nullptr;
		}

		width = (int)targaHdr.width;
		height = (int)targaHdr.height;
		//bpp = 4; // target bpp

		imageBPP = (int)(targaHdr.bpp/8);

		// Check that it is neither 32 bit nor 24 bit.
		if (imageBPP != 3 && imageBPP != 4)
		{
			return nullptr;
		}

		// Calculate the size of the 32 bit image data.
		imageSize = width * height * imageBPP;

		// Allocate memory for the targa image data.
		image = new BYTE[imageSize];
		if (!image)
		{
			return nullptr;
		}

		if (imageSize != fread(image, 1, imageSize, f))
		{
			return nullptr;
		}

		data = new BYTE[width * height * bpp];
		if (!data)
		{
			return nullptr;
		}

		int imageStride = width * imageBPP;
		BYTE* ptr = image + imageStride * (height - 1);
		BYTE* src = ptr;
		BYTE* dst = data;

		// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
		if (imageBPP == 3)
		{
			for (UINT j = 0; j < height; j++)
			{
				src = ptr - imageStride * j;
				for (UINT i = 0; i < width; i++)
				{
					int k = i * 3;
					*dst++ = src[k + 2];  // Red.
					*dst++ = src[k + 1];  // Green.
					*dst++ = src[k + 0];  // Blue
					*dst++ = 0xFF;  // Alpha
				}
			}
		}
		else if (imageBPP == 4)
		{
			for (UINT j = 0; j < height; j++)
			{
				ptr = src - imageStride * j;
				for (UINT i = 0; i < width; i++)
				{
					int k = i * 4;
					*dst++ = ptr[k + 2];  // Red.
					*dst++ = ptr[k + 1];  // Green.
					*dst++ = ptr[k + 0];  // Blue
					*dst++ = ptr[k + 3];  // Alpha
				}
			}
		}
		SAFEDELETE(image);
		fclose(f);
	}
	else if (type == BMP)
	{
		if (0 != _wfopen_s(&f, filename, L"rb"))
		{
			return nullptr;
		}
		BITMAPFILEHEADER fhdr;
		BITMAPINFOHEADER info;
		fread(&fhdr, 1, sizeof(BITMAPFILEHEADER), f);
		fread(&info, 1, sizeof(BITMAPINFOHEADER), f);
		if (info.biCompression != BI_RGB || (info.biBitCount != 24 && info.biBitCount != 32))
		{
			return nullptr;
		}

		width = (int)info.biWidth;
		height = (int)info.biHeight;
		//bpp = 4; // target bpp
		imageBPP = (int)(info.biBitCount / 8);
		// Calculate the size of the 32 bit image data.

		imageSize = width * height * imageBPP;

		// Allocate memory for the targa image data.
		image = new BYTE[imageSize];
		if (!image)
		{
			return nullptr;
		}

		if (imageSize != fread(image, 1, imageSize, f))
		{
			return nullptr;
		}

		int imageStride = width * imageBPP;

		// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
		if (imageBPP == 3)
		{
			data = new BYTE[width * height * bpp];
			BYTE* src = image;
			BYTE* dst = data;
			for (UINT j = 0; j < height; j++)
			{
				src = src + imageStride * j;
				for (UINT i = 0; i < width; i++)
				{
					*dst++ = *src++;  // Red.
					*dst++ = *src++;  // Green.
					*dst++ = *src++;  // Blue
					*dst++ = 0xFF;  // Alpha
				}
			}
			SAFEDELETE(image);
		}
		else if (imageBPP == 4)
		{
			data = image;
		}
		fclose(f);
	}


	return data;
}

ID3D11ShaderResourceView* TextureCreator::CreateFromFile(PWSTR filename, bool autogen/* = false*/)
{
	ID3D11ShaderResourceView *t = nullptr;
#if 0
	D3DX11CreateShaderResourceViewFromFile(m_pDevice, filename, NULL, NULL, &t, NULL);
#else
	UINT width, height, bpp;
	BYTE *data;

	bpp = 4; // DXGI_FORMAT_R8G8B8A8_UNORM
	FILE_TYPE type = NONE;
	// Load the targa image data into memory.
	data = LoadFile(filename, bpp, width, height, &type);

	if (!data && type == DDS)
	{
		ID3D11Resource *tex;
		DirectX::CreateDDSTextureFromFile(m_pDevice, filename, &tex, &t);
		SAFERELEASE(tex);
		return t;
	}

	ID3D11Texture2D *tex = nullptr;
	if (!autogen)
		tex = CreateTex2D(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, bpp, data, D3D11_BIND_SHADER_RESOURCE, autogen);
#if 1
	else
	{
		tex = CreateTex2D(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, bpp, nullptr, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, autogen);
		// Copy the targa image data into the texture.
		m_dc->UpdateSubresource(tex, 0, NULL, data, width * bpp, 0);
	}
#endif
	SAFEDELETE(data);

	t = CreateOnlySRV2D(tex, DXGI_FORMAT_R8G8B8A8_UNORM);

	if (autogen)
	{
		// Generate mipmaps for this texture.
		m_dc->GenerateMips(t);
	}
	SAFERELEASE(tex);
#endif

	return t;
}

ID3D11ShaderResourceView* TextureCreator::CreateFromMemory(void * data, int size, int width, int height, int bpp, bool autogen/* = false*/)
{
	ID3D11ShaderResourceView *t = nullptr;

	if (data && (*(const uint32_t*)(data) == DDS_MAGIC))
	{
		ID3D11Resource *tex;
		DirectX::CreateDDSTextureFromMemory(m_pDevice, (const uint8_t *)data, (size_t)size, &tex, &t);
		SAFERELEASE(tex);
		return t;
	}

	ID3D11Texture2D *tex = nullptr;
	if (!autogen)
		tex = CreateTex2D(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, bpp, data, D3D11_BIND_SHADER_RESOURCE, autogen);
#if 1
	else
	{
		tex = CreateTex2D(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, bpp, nullptr, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, autogen);
		// Copy the targa image data into the texture.
		m_dc->UpdateSubresource(tex, 0, NULL, data, width * bpp, 0);
	}
#endif
	SAFEDELETE(data);

	t = CreateOnlySRV2D(tex, DXGI_FORMAT_R8G8B8A8_UNORM);

	if (autogen)
	{
		// Generate mipmaps for this texture.
		m_dc->GenerateMips(t);
	}
	SAFERELEASE(tex);

	return t;
}

ID3D11Texture2D* TextureCreator::CreateTex2D(UINT w, UINT h, DXGI_FORMAT f, UINT bytePP, void* dataptr, UINT type, bool autogen /*= false*/)
{
	D3D11_TEXTURE2D_DESC d = {};
	d.Width = w;
	d.Height = h;
	d.MipLevels = 1;
	d.ArraySize = 1;
	d.Format = f;
	d.SampleDesc.Count = 1;
	d.SampleDesc.Quality = 0;
	d.Usage = D3D11_USAGE_DEFAULT;
	d.BindFlags = type;
	d.CPUAccessFlags = 0;
	d.MiscFlags = 0;

	if (autogen)
	{
		d.MipLevels = 0;
		d.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}

	D3D11_SUBRESOURCE_DATA subdata = {};
	D3D11_SUBRESOURCE_DATA *sub = nullptr;
	if (dataptr)
	{
		subdata.pSysMem = dataptr;
		subdata.SysMemPitch = w * bytePP;
		subdata.SysMemSlicePitch = 0;
		sub = &subdata;
	}

	ID3D11Texture2D *tex = nullptr;
	HRESULT hr = m_pDevice->CreateTexture2D(&d, sub, &tex);
	return tex;
}

ID3D11ShaderResourceView* TextureCreator::Create2D(ID3D11Texture2D** pptex, UINT w, UINT h, DXGI_FORMAT f/*= DXGI_FORMAT_R8G8B8A8_UNORM*/, 
										UINT bytePP/*= 4*/, void* dataptr/*= nullptr*/, UINT type/*= D3D11_BIND_SHADER_RESOURCE*/, UINT miplevels/*=-1*/)
{
//	if (!pptex)
//		return nullptr;

	bool bNewCreatedTex  = false;

	// Create RT tex if null
	ID3D11Texture2D *tex = nullptr;

	if (pptex && *pptex)
	{
		tex = *pptex;
	}
	else //(!pptex || *pptex == nullptr)
	{
		bNewCreatedTex = true;
		tex = CreateTex2D(w, h, f, bytePP, dataptr, type);
		if (!tex)
			return nullptr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC vd = {};
	vd.Format = f;
	vd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	vd.Texture2D.MipLevels = miplevels/*-1*/;

	ID3D11ShaderResourceView *v = nullptr;
	HRESULT hr = m_pDevice->CreateShaderResourceView(tex, &vd, &v);

	if (bNewCreatedTex)
	{
		if (pptex)
		{
			*pptex = tex;
		}
		else
		{
			SAFERELEASE(tex);
		}
	}
	return v;
}

ID3D11RenderTargetView* TextureCreator::CreateRT(ID3D11Texture2D** pptex, UINT w, UINT h, DXGI_FORMAT f /*= DXGI_FORMAT_R32G32B32A32_FLOAT*/,
												 UINT type/* = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/)
{
	//if (!pptex)
	//	return nullptr;
	bool bNewCreatedTex = false;

	// Create RT tex if null
	ID3D11Texture2D *tex = nullptr;

	if (pptex && *pptex)
	{
		tex = *pptex;
	}
	else //(!pptex || *pptex == nullptr)
	{
		bNewCreatedTex = true;
		tex = CreateTex2D(w, h, f, 0, nullptr, type);
		if (!tex)
			return nullptr;
	}

	// render target view.
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = f;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	ID3D11RenderTargetView* rtv = nullptr;
	HRESULT hr = m_pDevice->CreateRenderTargetView(tex, &rtvDesc, &rtv);

	if (bNewCreatedTex)
	{
		if (pptex)
		{
			*pptex = tex;
		}
		else
		{
			SAFERELEASE(tex);
		}
	}
	return rtv;
}

ID3D11DepthStencilView* TextureCreator::CreateDepthStencil(ID3D11Texture2D** pptex, UINT w, UINT h, DXGI_FORMAT f /*= DXGI_FORMAT_D24_UNORM_S8_UINT*/,
	UINT type/* = D3D11_BIND_DEPTH_STENCIL*/)
{
//	if (!pptex)
//		return nullptr;
	bool bNewCreatedTex = false;

	// Create tex if null
	ID3D11Texture2D *tex = nullptr;

	if (pptex && *pptex)
	{
		tex = *pptex;
	}
	else //(!pptex || *pptex == nullptr)
	{
		bNewCreatedTex = true;
		tex = CreateTex2D(w, h, f, 0, nullptr, type);
		if (!tex)
			return nullptr;
	}

	// render target view.
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = f;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	ID3D11DepthStencilView* dsv = nullptr;
	HRESULT hr = m_pDevice->CreateDepthStencilView(tex, &dsvDesc, &dsv);

	if (bNewCreatedTex)
	{
		if (pptex)
		{
			*pptex = tex;
		}
		else
		{
			SAFERELEASE(tex);
		}
	}
	return dsv;
}

ID3D11ShaderResourceView* TextureCreator::CreateOnlySRV2D(ID3D11Texture2D* tex, DXGI_FORMAT f/* = DXGI_FORMAT_R8G8B8A8_UNORM*/, UINT miplevels /*=-1*/)
{
	return tex ? Create2D(&tex, 0, 0, f, 0, nullptr, 0, miplevels) : nullptr;
}

ID3D11RenderTargetView* TextureCreator::CreateOnlyRTV(ID3D11Texture2D* tex, DXGI_FORMAT f/* = DXGI_FORMAT_R32G32B32A32_FLOAT*/)
{
	return tex ? CreateRT(&tex, 0, 0, f) : nullptr;
}

ID3D11DepthStencilView* TextureCreator::CreateOnlyDSV(ID3D11Texture2D* tex, DXGI_FORMAT f/* = DXGI_FORMAT_D24_UNORM_S8_UINT*/)
{
	return tex ? CreateDepthStencil(&tex, 0, 0, f) : nullptr;
}

ID3D11DepthStencilState* StateCreator::CreateDepthStencilState(bool d_enable/* = true*/, bool s_enable/* = true*/, 
	D3D11_DEPTH_WRITE_MASK d_wmask/* = D3D11_DEPTH_WRITE_MASK_ALL*/, D3D11_COMPARISON_FUNC d_func/*= D3D11_COMPARISON_LESS*/,
	UINT8 s_rmask/* = D3D11_DEFAULT_STENCIL_READ_MASK*/, UINT s_wmask/*=D3D11_DEFAULT_STENCIL_WRITE_MASK*/)
{
	D3D11_DEPTH_STENCIL_DESC desc = {};
	desc.DepthEnable = d_enable;
	desc.DepthWriteMask = d_wmask;
	desc.DepthFunc = d_func;
	desc.StencilEnable = s_enable;
	desc.StencilReadMask = s_rmask;
	desc.StencilWriteMask = s_wmask;

	// Stencil op for front-facing.
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil op for back-facing.
	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	ID3D11DepthStencilState *dss = nullptr;
	m_pDevice->CreateDepthStencilState(&desc, &dss);
	return dss;
}

ID3D11BlendState* StateCreator::CreateBlendState(int RTindex, bool enable, 
	D3D11_BLEND src/* = D3D11_BLEND_ONE*/, D3D11_BLEND dst/* = D3D11_BLEND_INV_SRC_ALPHA*/, D3D11_BLEND_OP op/* = D3D11_BLEND_OP_ADD*/,
	D3D11_BLEND srcAlpha/* = D3D11_BLEND_ONE*/, D3D11_BLEND dstAlpha/* = D3D11_BLEND_ZERO*/, D3D11_BLEND_OP opAlpha/* = D3D11_BLEND_OP_ADD*/)
{
	D3D11_BLEND_DESC desc = {};

	const D3D11_RENDER_TARGET_BLEND_DESC DefaultRTBlendDesc =
	{
		FALSE,
		D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
		D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
		D3D11_COLOR_WRITE_ENABLE_ALL,
	};
	desc.AlphaToCoverageEnable = FALSE;
	desc.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		desc.RenderTarget[i] = DefaultRTBlendDesc;

	desc.RenderTarget[RTindex].BlendEnable = enable ? TRUE : FALSE;
	desc.RenderTarget[RTindex].SrcBlend = src;
	desc.RenderTarget[RTindex].DestBlend = dst;
	desc.RenderTarget[RTindex].BlendOp = op;
	desc.RenderTarget[RTindex].SrcBlendAlpha = srcAlpha;
	desc.RenderTarget[RTindex].DestBlendAlpha = dstAlpha;
	desc.RenderTarget[RTindex].BlendOpAlpha = opAlpha;
	desc.RenderTarget[RTindex].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11BlendState *bs = nullptr;
	m_pDevice->CreateBlendState(&desc, &bs);
	return bs;
}

ID3D11RasterizerState* StateCreator::CreateRasterState(D3D11_FILL_MODE fill/* = D3D11_FILL_SOLID*/, D3D11_CULL_MODE cull/* = D3D11_CULL_BACK*/,
		bool DepthClipEnable/* = true*/, bool ScissorEnable/* = false*/, bool MultisampleEnable/* = false*/)
{
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = cull;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = DepthClipEnable;
	rasterDesc.FillMode = fill;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = MultisampleEnable;
	rasterDesc.ScissorEnable = ScissorEnable;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	ID3D11RasterizerState *rs = nullptr;
	m_pDevice->CreateRasterizerState(&rasterDesc, &rs);
	return rs;
}
