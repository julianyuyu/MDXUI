
#ifndef _FONTTEXT_H_
#define _FONTTEXT_H_

#include "fontrenderer.h"

#define NEW_VERTEX_OFFSET_METHOD		1

class FontText : public OrthoModel
{
public:
	FontText(ID3D11Device* dev, int screen_w, int screen_h, FontRenderer* r, int x = 0, int y = 0, WXMVECTOR4* c= nullptr, WXMVECTOR4* cshadow = nullptr)
		: OrthoModel(dev, screen_w, screen_h), m_FontRenderer(r),
#if !NEW_VERTEX_OFFSET_METHOD
		m_OffsetX(0), m_OffsetY(0),
#endif
		m_TxtLength(0), m_MaxLength(0), m_pVb2(nullptr), m_pIb2(nullptr), m_PosX(0), m_PosY(0), m_ParentRectW(0), m_ParentRectH(0)
	{
		Initialize(x, y, c, cshadow);
	}
	FontText(FontText& ref):
		OrthoModel(ref.m_pDevice, ref.m_wScreen, ref.m_hScreen), m_FontRenderer(ref.m_FontRenderer),
#if !NEW_VERTEX_OFFSET_METHOD
		m_OffsetX(0), m_OffsetY(0),
#endif
		m_TxtLength(0), m_MaxLength(0), m_pVb2(nullptr), m_pIb2(nullptr), m_PosX(0), m_PosY(0), m_ParentRectW(0), m_ParentRectH(0)
	{
		Initialize(ref.m_PosX, ref.m_PosY, &ref.m_pixelColor, &ref.m_pixelColorShadow);
	}
	virtual ~FontText()
	{
		Destroy();
	}
	virtual void Destroy()
	{
		SAFERELEASE(m_pVb2);
		SAFERELEASE(m_pIb2);
	}

	virtual void CenteredDisplay(int rect_w = 0, int rect_h = 0)
	{
		m_ParentRectW = rect_w;
		m_ParentRectH = rect_h;
	}

	virtual void Initialize(int x = 0, int y = 0, WXMVECTOR4* c = nullptr, WXMVECTOR4* cshadow = nullptr)
	{
		m_bShadow = false;
		m_pixelColor = WXMVECTOR4(1.f, 1.f, 1.f, 1.0f);
		m_pixelColorShadow = WXMVECTOR4(0.f, 0.0f, 0.4f, 1.f);

		m_PosX = x;
		m_PosY = y;
		if (c)
			m_pixelColor = *c;
		if (cshadow)
		{
			m_pixelColorShadow = *cshadow;
			m_bShadow = true;
		}
	}

	virtual void Render(WXMVECTOR4* color = nullptr, WXMVECTOR4* shadow_color = nullptr);
	virtual void Move(int new_x, int new_y);
	virtual void UpdateText(PWSTR text);
//	virtual bool UpdateModel(PWSTR txt, int new_x=-1, int new_y=-1, int rect_w = 0, int rect_h = 0);
//	virtual int TextHeight() { return m_Width; }
//	virtual int TextWidth() { return m_Height; }
	virtual void TextSize(float& w, float& h) { w = m_Width; h = m_Height; };
private:
	virtual bool CreateModel();
	virtual void BuildVertexArray(void*, PWSTR, float, float);
	virtual void OffsetVertexPosition(void* vertices, int count, float x, float y);
private:
	FontRenderer *m_FontRenderer;
	ID3D11Buffer *m_pVb2;
	ID3D11Buffer *m_pIb2;

	int m_TxtLength; // current text length, (string length)
	int m_MaxLength; // max text length
	int m_PosX;
	int m_PosY;
	int m_ParentRectW; // for centered display
	int m_ParentRectH;
	float m_Width;  // total width of text, texture width of the string, not string length
	float m_Height; // total height of text
#if !NEW_VERTEX_OFFSET_METHOD
	float m_OffsetX;
	float m_OffsetY;
#endif
	WXMVECTOR4 m_pixelColor;
	WXMVECTOR4 m_pixelColorShadow;
	bool m_bShadow;
};

#endif
