#include "inc.h"
#include "mutshader.h"

void MUTShader::SetPipeline()
{
	// Set the vertex input layout.
	m_pImmeDC->IASetInputLayout(m_layout);

	// Set shaders that will be used for rendering.
	m_pImmeDC->VSSetShader(m_pVS, NULL, 0);
	if (m_pHS) m_pImmeDC->HSSetShader(m_pHS, NULL, 0);
	if (m_pDS) m_pImmeDC->DSSetShader(m_pDS, NULL, 0);
	if (m_pGS) m_pImmeDC->GSSetShader(m_pGS, NULL, 0);
	m_pImmeDC->PSSetShader(m_pPS, NULL, 0);
}

void MUTShader::DrawIndexed(UINT idxCount)
{
	SetPipeline();
	SetSampler();
	m_pImmeDC->DrawIndexed(idxCount, 0, 0);
}

void MUTShader::Draw(UINT vtxCount)
{
	SetPipeline();
	SetSampler();
	m_pImmeDC->Draw(vtxCount, 0);
}

void MUTShader::SetSampler()
{
	if (!m_bUseSamplerStateClamp)
		m_pImmeDC->PSSetSamplers(0, 1, &m_pSamplerState);
	else
		m_pImmeDC->PSSetSamplers(0, 1, &m_pSamplerStateClamp);
}

bool MUTShader::CreateResource(int* cbSizeTable, int cbCount)
{
	SamplerCreator sctr(m_pDevice);
	m_pSamplerState = sctr.Create(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP);
	RETURNONNULL(m_pSamplerState);
	m_pSamplerStateClamp = sctr.Create(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP);
	RETURNONNULL(m_pSamplerStateClamp);

	BufferCreator ctr(m_pDevice);
	m_pCbMatrix = ctr.CreateCB(sizeof(CB_MATRIX));
	RETURNONNULL(m_pCbMatrix);

	if (cbSizeTable && cbCount)
	{
		ID3D11Buffer *cb;
		for (int i = 0; i < cbCount; i++)
		{
			cb = ctr.CreateCB(cbSizeTable[i]);
			m_CBList.push_back(cb);
		}
	}
	return true;
}

bool MUTShader::AppendShader(LPWSTR file, LPSTR code, LPSTR shadermain, LPSTR profile, LPD3D_SHADER_MACRO macro, LPD3DINCLUDE inc)
{
	void *ptr = nullptr;
	ShaderCreator sctr(m_pDevice);
	if ((file && code) || (!file && !code))
		return false;
	else if (file)
		ptr = sctr.Create(file, shadermain, profile, macro, inc);
	else if (code)
		ptr = sctr.CreateSource(code, shadermain, profile, macro, inc);
	RETURNONNULL(ptr);

	if (STRMATCH_N(profile, "vs", 2))
	{
		m_pVS = (ID3D11VertexShader *)ptr;
	}
	else if (STRMATCH_N(profile, "ps", 2))
	{
		m_pPS = (ID3D11PixelShader *)ptr;
		m_PSList.push_back(m_pPS);
	}
	else if (STRMATCH_N(profile, "hs", 2))
	{
		m_pHS = (ID3D11HullShader *)ptr;
		m_HSList.push_back(m_pHS);
	}
	else if (STRMATCH_N(profile, "ds", 2))
	{
		m_pDS = (ID3D11DomainShader *)ptr;
		m_DSList.push_back(m_pDS);
	}
	return true;
}

bool MUTShader::CreateVSAndLayout(LPWSTR file, LPSTR code, LPSTR vsmain, LPSTR *SemanticTable, int layoutCount)
{
	//	CreateResource();
	ShaderCreator sctr(m_pDevice);
	if ((file && code) || (!file && !code))
		return false;
	else if (file)
		m_pVS = (ID3D11VertexShader *)sctr.Create(file, vsmain, "vs_5_0");
	else if (code)
		m_pVS = (ID3D11VertexShader *)sctr.CreateSource(code, vsmain, "vs_5_0");
	RETURNONNULL(m_pVS);

	LPSTR tbl[] = { "POSITION", "TEXCOORD" };
	if (!SemanticTable && !layoutCount)
	{
		SemanticTable = tbl;
		layoutCount = 2;
	}

	LayoutCreator lctr(layoutCount, m_pDevice);
	for (int i = 0; i < layoutCount; i++)
	{
		DXGI_FORMAT fmt = DXGI_FORMAT_R32G32B32_FLOAT;
		if (STRMATCH(SemanticTable[i], "TEXCOORD"))
			fmt = DXGI_FORMAT_R32G32_FLOAT;
		else if (STRMATCH(SemanticTable[i], "COLOR"))
		{
			fmt = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		lctr.Desc(i, SemanticTable[i], 0, (i == 0) ? 0 : D3D11_APPEND_ALIGNED_ELEMENT, fmt);
	}

	m_layout = lctr.Create(sctr.ByteCodePtr(), (UINT)sctr.ByteCodeLength());
	RETURNONNULL(m_layout);

	return true;
}

bool MUTShader::UpdatePSTexture(ID3D11ShaderResourceView** texarray/* = nullptr*/, int texcount/* = 0*/, UINT* texGroupArray/* = nullptr*/, int GroupCount/* = 0*/)
{
	// Set shader texture resource in the pixel shader.
	if (!texarray && !texcount && !texGroupArray && !GroupCount)
		return true; // no texture.

	if ((!texGroupArray && GroupCount) || (texGroupArray && !GroupCount))
		return false;

	if (!texarray) return false;

	if (!texGroupArray || !GroupCount)
	{
		m_pImmeDC->PSSetShaderResources(0, texcount, texarray);
	}
	else
	{
		ID3D11ShaderResourceView** CurrentArray = nullptr;

		int TexCountInCurrentGroup = 0;
		int GlobalIndex = 0;
		for (int i = 0; i < GroupCount; i++)
		{
			TexCountInCurrentGroup = texGroupArray[i];
			CurrentArray = new ID3D11ShaderResourceView*[TexCountInCurrentGroup];
			for (int k = 0; k < TexCountInCurrentGroup; k++)
			{
				CurrentArray[k] = texarray[GlobalIndex];
				GlobalIndex++;
			}
			m_pImmeDC->PSSetShaderResources(i, TexCountInCurrentGroup, CurrentArray);
			//CurrentArray += TexCountInCurrentGroup;
			SAFEDELETEARRAY(CurrentArray);
		}
	}
	return true;
}

bool MUTShader::UpdateShaderMatrix(WXMMATRIX* mat_world, WXMMATRIX* mat_view, WXMMATRIX* mat_projection)
{
	// Transpose the matrices to prepare them for the shader.
	//D3DXMATRIX mat_w, mat_v, mat_p;
	//D3DXMatrixTranspose(&mat_w, mat_world);
	//D3DXMatrixTranspose(&mat_v, mat_view);
	//D3DXMatrixTranspose(&mat_p, mat_projection);

	ResWriter rwtr(m_pImmeDC);

	//CB_MATRIX cbm = { mat_w, mat_v, mat_p };
	CB_MATRIX cbm = {};
	mat_world->Transpose(cbm.world);
	mat_view->Transpose(cbm.view);
	mat_projection->Transpose(cbm.projection);
	rwtr.Copy(&cbm, sizeof(CB_MATRIX), m_pCbMatrix);

	if (!m_pHS && !m_pDS)
	{
		// if has HullShader/DomainShader, need set m_pCbMatrix to DS
		m_pImmeDC->VSSetConstantBuffers(m_vsCbIndex, 1, &m_pCbMatrix);
		//m_vsCbIndex++; // for next VS CB
	}
	else if (m_pHS && m_pDS)
	{
		m_pImmeDC->DSSetConstantBuffers(0, 1, &m_pCbMatrix);
	}
	else
		return false;

	return true;
}
