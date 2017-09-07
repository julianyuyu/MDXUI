#include "inc.h"
#include "fontshader.h"

#if 0
const PSTR FontVSCode = SHADER_CODE
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

	PS_INPUT FontVSMain(VS_INPUT input)
	{
		PS_INPUT output;

		input.pos.w = 1.0f;

		output.pos = mul(input.pos, viewMatrix);
		output.pos = mul(output.pos, projMatrix);

		// output texture coordinates
		output.tex = input.tex;
		return output;
	}
);
#endif

const PSTR FontPSCode = SHADER_CODE
(
	Texture2D shaderTexture;
	SamplerState SampleType;

	cbuffer PixelBuffer
	{
		float4 pixelColor;
	};

	struct PS_INPUT
	{
		float4 pos : SV_POSITION;
		float2 tex : TEXCOORD0;
	};

	float4 FontPSMain(PS_INPUT input) : SV_TARGET
	{
		float4 color;
		// sample the texture
		color = shaderTexture.Sample(SampleType, input.tex);

		// treat black pixel as transparent.
		if (color.r == 0.0f)
		{
			color.a = 0.f;
		}
		else
		{
			//color.a = 1.0f;
			color.a = 1.f * color.r;
			color = color * pixelColor;
		}

		return color;
	}
);

bool FontShader::Create()
{
	return OrthoShader::Create(nullptr, nullptr, nullptr, nullptr, FontPSCode, "FontPSMain");
}

bool FontShader::CreateResource()
{
	OrthoShader::CreateResource();

	BufferCreator ctr(m_pDevice);
	m_pCbColor = ctr.CreateCB(sizeof(FONT_CB_COLOR));
	RETURNONNULL(m_pCbColor);
	return true;
}

bool FontShader::UpdateShaderResource(WXMMATRIX* mat_view, WXMMATRIX* mat_projection, WXMVECTOR4* color)
{
	OrthoShader::UpdateShaderResource(mat_view, mat_projection);

	ResWriter rwtr(m_pImmeDC);
	FONT_CB_COLOR cbc = { *color };
	rwtr.Copy(&cbc, sizeof(FONT_CB_COLOR), m_pCbColor);
	int psCbIndex = 0;
	m_pImmeDC->PSSetConstantBuffers(psCbIndex, 1, &m_pCbColor);
	return true;
}
