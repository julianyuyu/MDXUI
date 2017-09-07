#pragma once

#include <d3d11_1.h>

namespace DirectX
{
	const uint32_t DDS_MAGIC = 0x20534444; // "DDS "

	enum DDS_ALPHA_MODE
	{
		DDS_ALPHA_MODE_UNKNOWN = 0,
		DDS_ALPHA_MODE_STRAIGHT = 1,
		DDS_ALPHA_MODE_PREMULTIPLIED = 2,
		DDS_ALPHA_MODE_OPAQUE = 3,
		DDS_ALPHA_MODE_CUSTOM = 4,
	};

	HRESULT CreateDDSTextureFromMemory(ID3D11Device* d3dDevice,
		const uint8_t* ddsData,
		size_t ddsDataSize,
		ID3D11Resource** texture,
		ID3D11ShaderResourceView** textureView,
		size_t maxsize = 0,
		DDS_ALPHA_MODE* alphaMode = nullptr);

	HRESULT CreateDDSTextureFromMemory(ID3D11Device* d3dDevice,
		ID3D11DeviceContext* d3dContext,
		const uint8_t* ddsData,
		size_t ddsDataSize,
		ID3D11Resource** texture,
		ID3D11ShaderResourceView** textureView,
		size_t maxsize = 0,
		DDS_ALPHA_MODE* alphaMode = nullptr);

	HRESULT CreateDDSTextureFromMemoryEx(ID3D11Device* d3dDevice,
		const uint8_t* ddsData,
		size_t ddsDataSize,
		size_t maxsize,
		D3D11_USAGE usage,
		unsigned int bindFlags,
		unsigned int cpuAccessFlags,
		unsigned int miscFlags,
		bool forceSRGB,
		ID3D11Resource** texture,
		ID3D11ShaderResourceView** textureView,
		DDS_ALPHA_MODE* alphaMode = nullptr);

	HRESULT CreateDDSTextureFromMemoryEx(ID3D11Device* d3dDevice,
		ID3D11DeviceContext* d3dContext,
		const uint8_t* ddsData,
		size_t ddsDataSize,
		size_t maxsize,
		D3D11_USAGE usage,
		unsigned int bindFlags,
		unsigned int cpuAccessFlags,
		unsigned int miscFlags,
		bool forceSRGB,
		ID3D11Resource** texture,
		ID3D11ShaderResourceView** textureView,
		DDS_ALPHA_MODE* alphaMode = nullptr);


	// Standard version
	HRESULT __cdecl CreateDDSTextureFromFile(_In_ ID3D11Device* d3dDevice,
		_In_z_ const wchar_t* szFileName,
		_Outptr_opt_ ID3D11Resource** texture,
		_Outptr_opt_ ID3D11ShaderResourceView** textureView,
		_In_ size_t maxsize = 0,
		_Out_opt_ DDS_ALPHA_MODE* alphaMode = nullptr
		);

	// Standard version with optional auto-gen mipmap support
	HRESULT __cdecl CreateDDSTextureFromFile(_In_ ID3D11Device* d3dDevice,
		_In_opt_ ID3D11DeviceContext* d3dContext,
		_In_z_ const wchar_t* szFileName,
		_Outptr_opt_ ID3D11Resource** texture,
		_Outptr_opt_ ID3D11ShaderResourceView** textureView,
		_In_ size_t maxsize = 0,
		_Out_opt_ DDS_ALPHA_MODE* alphaMode = nullptr
		);

	// Extended version
	HRESULT __cdecl CreateDDSTextureFromFileEx(_In_ ID3D11Device* d3dDevice,
		_In_z_ const wchar_t* szFileName,
		_In_ size_t maxsize,
		_In_ D3D11_USAGE usage,
		_In_ unsigned int bindFlags,
		_In_ unsigned int cpuAccessFlags,
		_In_ unsigned int miscFlags,
		_In_ bool forceSRGB,
		_Outptr_opt_ ID3D11Resource** texture,
		_Outptr_opt_ ID3D11ShaderResourceView** textureView,
		_Out_opt_ DDS_ALPHA_MODE* alphaMode = nullptr
		);

	// Extended version with optional auto-gen mipmap support
	HRESULT __cdecl CreateDDSTextureFromFileEx(_In_ ID3D11Device* d3dDevice,
		_In_opt_ ID3D11DeviceContext* d3dContext,
		_In_z_ const wchar_t* szFileName,
		_In_ size_t maxsize,
		_In_ D3D11_USAGE usage,
		_In_ unsigned int bindFlags,
		_In_ unsigned int cpuAccessFlags,
		_In_ unsigned int miscFlags,
		_In_ bool forceSRGB,
		_Outptr_opt_ ID3D11Resource** texture,
		_Outptr_opt_ ID3D11ShaderResourceView** textureView,
		_Out_opt_ DDS_ALPHA_MODE* alphaMode = nullptr
		);

}; // namespace DirectX
