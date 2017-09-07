#pragma once

#include "inc.h"
#include "mdxuitex.h"
#include "mdxuirenderer.h"
#include "fonttext.h"

class MDXUIModel : public OrthoModel
{
public:
	MDXUIModel() : m_pText(nullptr), m_TextX(0), m_TextY(0) {}
	MDXUIModel(ID3D11Device* dev, int w, int h, MDXUIRenderer* r)
	:OrthoModel(dev, w, h), m_pText(nullptr), m_TextX(0), m_TextY(0)
	{
		Initialize(r);
	}
	virtual ~MDXUIModel()
	{
		SAFEDELETE(g_TexLayouter);
		SAFEDELETE(m_pText);
	}
	virtual void Initialize(MDXUIRenderer* r)
	{
		m_pText = nullptr;
		if (!g_TexLayouter)
			g_TexLayouter = new MDXUITexLayouter;
		g_TexLayouter->CalcLayout();

		m_pDXUIRenderer = r;
	}

	virtual void RenderModel();
	virtual bool UpdateModel(int UIindex, POINT pt1, POINT pt2)
	{
		return UpdateModel(UIindex, pt1.x, pt1.y, pt2.x, pt2.y);
	}
	virtual bool UpdateModel(int UIindex = 0, int x1 = -1, int y1 = -1, int posX2 = -1, int posY2= -1);

protected:
//	virtual void InitQuadVertices(ORTHO_VERTEX*& vertices, float x, float y, float w, float h, float u1, float v1, float u2, float v2)
//	{
//		*vertices++ = InitVertex(x, y, 0.0f, u1, v1);// Bottom right.
//		*vertices++ = InitVertex(x + w, y - h, 0.0f, u2, v2);// Bottom left.
//		*vertices++ = InitVertex(x, y - h, 0.0f, u1, v2);// Top left.
//		*vertices++ = InitVertex(x, y, 0.0f, u1, v1);// Bottom right.
//		*vertices++ = InitVertex(x + w, y, 0.0f, u2, v1);// Top left.
//		*vertices++ = InitVertex(x + w, y - h, 0.0f, u2, v2);// Top right.
//	}
protected:
	virtual void BuildVertexArray(int UIindex, void* ptr, float drawX, float drawY, float drawX2, float drawY2) = 0;
	static MDXUITexLayouter *g_TexLayouter;
	MDXUIRenderer *m_pDXUIRenderer;
	FontText *m_pText;
	int m_TextX;
	int m_TextY;
};
