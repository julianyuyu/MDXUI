#include "inc.h"
#include "orthorenderer.h"

const PSTR OrthoVSCode = SHADER_CODE
(
	cbuffer MatrixBuffer
	{
		matrix viewMatrix;
		matrix projMatrix;
	};

	struct VS_INPUT
	{
		float4 pos : POSITION;
		float2 tex : TEXCOORD0;
	};

	struct PS_INPUT
	{
		float4 pos : SV_POSITION;
		float2 tex : TEXCOORD0;
	};

	PS_INPUT OrthoVSMain(VS_INPUT input)
	{
		PS_INPUT output = (PS_INPUT)0;

		input.pos.w = 1.0f;

		output.pos = mul(input.pos, viewMatrix);
		output.pos = mul(output.pos, projMatrix);

		output.tex = input.tex;
		return output;
	}
);

const PSTR OrthoPSCode = SHADER_CODE
(
	Texture2D    texOrtho;
	SamplerState samplerOrtho;

	struct PS_INPUT
	{
		float4 pos : SV_POSITION;
		float2 tex : TEXCOORD0;
	};

	float4 OrthoPSMain(PS_INPUT input) : SV_TARGET
	{
		return texOrtho.Sample(samplerOrtho, input.tex);
	}
);


bool OrthoModel::CreateModel()
{
	m_VertexCount = 6 * m_QuadCount;
	m_IndexCount  = m_VertexCount;

	SAFERELEASE(m_pVb);
	SAFERELEASE(m_pIb);
	SAFEDELETEARRAY(m_pVertices);
	SAFEDELETEARRAY(m_pIndices);

	if (m_pDevice && !m_pImmeDC)
		m_pDevice->GetImmediateContext(&m_pImmeDC);

	// Create a CPU vertex data buffer
	m_pVertices = new ORTHO_VERTEX[m_VertexCount];
	if (!m_pVertices)
	{
		return false;
	}

	// Create a CPU index data buffer
	m_pIndices = new ULONG[m_IndexCount];
	if (!m_pIndices)
	{
		return false;
	}

	memset(m_pVertices, 0, (sizeof(ORTHO_VERTEX) * m_VertexCount));

	for (int i = 0; i<m_IndexCount; i++)
	{
		m_pIndices[i] = i;
	}

	BufferCreator ctr(m_pDevice);
//	SAFERELEASE(m_pVb);
//	SAFERELEASE(m_pIb);
	m_pVb = ctr.Create(sizeof(ORTHO_VERTEX), m_VertexCount, D3D11_BIND_VERTEX_BUFFER, m_pVertices, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	m_pIb = ctr.Create(sizeof(ULONG), m_IndexCount, D3D11_BIND_INDEX_BUFFER, m_pIndices);

//	SAFEDELETEARRAY(vertices);
//	SAFEDELETEARRAY(indices);

	return true;
}

void OrthoShader::DrawIndexed(UINT idxCount)
{
	m_pImmeDC->IASetInputLayout(m_layout);

	m_pImmeDC->VSSetShader(m_pVS, NULL, 0);
	m_pImmeDC->HSSetShader(nullptr, NULL, 0);
	m_pImmeDC->DSSetShader(nullptr, NULL, 0);
	m_pImmeDC->GSSetShader(nullptr, NULL, 0);
	m_pImmeDC->PSSetShader(m_pPS, NULL, 0);

	m_pImmeDC->PSSetSamplers(0, 1, &m_pSamplerState);
	m_pImmeDC->DrawIndexed(idxCount, 0, 0);
}

bool OrthoShader::Create(LPWSTR vsname, LPSTR vscode, LPSTR vsmain, LPWSTR psname, LPSTR pscode, LPSTR psmain)
{
	// create vs
	ShaderCreator sctr(m_pDevice);
	if ((vsname && vscode))
		return false;
	else if ((!vsname && !vscode))
		m_pVS = (ID3D11VertexShader *)sctr.CreateSource(OrthoVSCode, "OrthoVSMain", "vs_5_0");
	else if (vscode)
		m_pVS = (ID3D11VertexShader *)sctr.CreateSource(vscode, vsmain, "vs_5_0");
	else if (vsname)
		m_pVS = (ID3D11VertexShader *)sctr.Create(vsname, vsmain, "vs_5_0");

	RETURNONNULL(m_pVS);

	// create layout
	LayoutCreator lctr(2, m_pDevice);
	lctr.Desc(0, "POSITION", 0, 0, DXGI_FORMAT_R32G32B32_FLOAT);
	lctr.Desc(1, "TEXCOORD", 0, D3D11_APPEND_ALIGNED_ELEMENT, DXGI_FORMAT_R32G32_FLOAT);

	m_layout = lctr.Create(sctr.ByteCodePtr(), (UINT)sctr.ByteCodeLength());
	RETURNONNULL(m_layout);

	// create ps
	if (psname && pscode)
		return false;
	else if (!psname && !pscode)
		m_pPS = (ID3D11PixelShader *)sctr.CreateSource(OrthoPSCode, "OrthoPSMain", "ps_5_0");
	else if (pscode)
		m_pPS = (ID3D11PixelShader *)sctr.CreateSource(pscode, psmain, "ps_5_0");
	else if (psname)
		m_pPS = (ID3D11PixelShader *)sctr.Create(psname, psmain, "ps_5_0");

	RETURNONNULL(m_pPS);

	// create cb and sampler
	if (!CreateResource()) return false;
	return true;
}

bool OrthoShader::CreateResource()
{
	SamplerCreator sctr(m_pDevice);
	m_pSamplerState = sctr.Create(D3D11_FILTER_MIN_MAG_MIP_POINT);
	RETURNONNULL(m_pSamplerState);

	BufferCreator ctr(m_pDevice);
	m_pCbMatrix = ctr.CreateCB(sizeof(ORTHO_CB_MATRIX));
	RETURNONNULL(m_pCbMatrix);

	return true;
}

bool OrthoShader::UpdateShaderResource(WXMMATRIX* mat_view, WXMMATRIX* mat_projection)
{
	WXMMATRIX mat_v, mat_p;
	mat_v = mat_view->Transpose();
	mat_p = mat_projection->Transpose();

	ResWriter rwtr(m_pImmeDC);
	ORTHO_CB_MATRIX cbo = { mat_v, mat_p };
	rwtr.Copy(&cbo, sizeof(ORTHO_CB_MATRIX), m_pCbMatrix);
	int vsCbIndex = 0;
	m_pImmeDC->VSSetConstantBuffers(vsCbIndex, 1, &m_pCbMatrix);
	return true;
}
