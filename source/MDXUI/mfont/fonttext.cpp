#include "inc.h"
#include "fonttext.h"
#include "creator.h"

bool FontText::CreateModel()
{
	m_QuadCount = m_MaxLength;
	OrthoModel::CreateModel();

//	if(m_bShadow)
	{
		BufferCreator ctr(m_pDevice);
		SAFERELEASE(m_pVb2);
		SAFERELEASE(m_pIb2);
		m_pVb2 = ctr.Create(sizeof(ORTHO_VERTEX), m_VertexCount, D3D11_BIND_VERTEX_BUFFER, m_pVertices, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		m_pIb2 = ctr.Create(sizeof(ULONG), m_IndexCount, D3D11_BIND_INDEX_BUFFER, m_pIndices);
	}

	return true;
}

void FontText::OffsetVertexPosition(void* ptr, int count, float x, float y)
{
	ORTHO_VERTEX* vertices = (ORTHO_VERTEX*)ptr;
	for (int i = 0; i<count; i++)
	{
		vertices[i].position.x += x;
		vertices[i].position.y -= y;
	}
}

void FontText::BuildVertexArray(void* ptr, PWSTR szText, float pos_x, float pos_y)
{
	ORTHO_VERTEX* vertices = (ORTHO_VERTEX*)ptr;

	// Get the number of letters in the sentence.
//	int len = (int)wcslen(szText);

	float u1, v1, u2, v2;
	float w = 0.f, h = 0.f;
	float m_Spacing = 6.0f; // TODO: make it adjustable
	float x = pos_x/* + m_Spacing*/;
	float y = pos_y;

	{
		PWSTR txt = szText;
		// Draw each letter onto a quad.
		for (int i = 0; i<m_TxtLength; i++)
		{
			WCHAR c = *txt++;
			if (c == L'\n')
			{
				// new line
				x = pos_x;
				y -= (h + m_Spacing); // use height of last char.
				continue;
			}
			TEXCOORD *coord = m_FontRenderer->GetCoord(c);
			if (!coord)
				continue;
			u1 = coord->u1;
			v1 = coord->v1;
			u2 = coord->u2;
			v2 = coord->v2;

			w = (float)coord->w;
			h = (float)coord->h;
			InitQuadVertices(vertices, x, y, w, h, u1, v1, u2, v2);
			x += w;// +(1 * m_Spacing);
		}
		m_Width = float(x - pos_x);
		m_Height = float(pos_y - y + h);
	}
}

void FontText::Move(int new_x, int new_y)
{
	float x_offset = float(new_x - m_PosX);
	float y_offset = float(new_y - m_PosY);
	m_PosX = new_x;
	m_PosY = new_y;

	OffsetVertexPosition(m_pVertices, m_VertexCount, x_offset, y_offset);
	ResWriter rwtr(m_pImmeDC);
	rwtr.Copy(m_pVertices, sizeof(ORTHO_VERTEX)* m_VertexCount, m_pVb);

	if (m_bShadow)
	{
		ORTHO_VERTEX* vertices = new ORTHO_VERTEX[m_VertexCount];;
		memcpy(vertices, m_pVertices, (sizeof(ORTHO_VERTEX) * m_VertexCount));

		OffsetVertexPosition(vertices, m_VertexCount, 1.f, 1.f);
		rwtr.Copy(vertices, sizeof(ORTHO_VERTEX)* m_VertexCount, m_pVb2);
		SAFEDELETEARRAY(vertices);
	}
}

void FontText::UpdateText(PWSTR text)
{
	if (text)
	{
		m_TxtLength = (int)wcslen(text);

		//if (len != m_TextLength)
		if (m_TxtLength > m_MaxLength)
		{
			m_MaxLength = m_TxtLength;
			OrthoModel::Destroy();
			Destroy();
		}
	}
	else
	{
		// if just need to move the text, call Move()
		return;
	}

	if (!m_pVb || !m_pIb)
	{
		CreateModel();
	}

	//	ORTHO_VERTEX* vertices;
	// calcthe X and Y pixel position on the screen (default pos is (0,0))
	float drawX = (float)(((m_wScreen / 2) * -1) + m_PosX);
	float drawY = (float)((m_hScreen / 2) - m_PosY);

	BuildVertexArray((void*)m_pVertices, text, drawX, drawY);

	ResWriter rwtr(m_pImmeDC);
	rwtr.Copy(m_pVertices, sizeof(ORTHO_VERTEX)* m_VertexCount, m_pVb);

#if 1
	float x_offset = 0;
	float y_offset = 0;
	if (m_ParentRectW)
	{
		x_offset += ((float)m_ParentRectW - m_Width) / 2.f;
	}
	if (m_ParentRectH)
	{
		y_offset += ((float)m_ParentRectH - m_Height) / 2.f;
	}
	OffsetVertexPosition(m_pVertices, m_VertexCount, x_offset, y_offset);
#endif

	// for text shadow, copy vertices to a second VB, with an offset
	if (m_bShadow)
	{
		ORTHO_VERTEX* vertices = new ORTHO_VERTEX[m_VertexCount];;
		memcpy(vertices, m_pVertices, (sizeof(ORTHO_VERTEX) * m_VertexCount));
		OffsetVertexPosition(vertices, m_VertexCount, 1.f, 1.f);
		rwtr.Copy(vertices, sizeof(ORTHO_VERTEX)* m_VertexCount, m_pVb2);
		SAFEDELETEARRAY(vertices);
	}

}

#if 0
bool FontText::UpdateModel(PWSTR text, int new_x, int new_y, int rect_w/* = 0*/, int rect_h/* = 0*/)
{
//	ORTHO_VERTEX* vertices;
	float drawX, drawY;
	if (text)
	{
		m_TxtLength = (int)wcslen(text);
	
		//if (len != m_TextLength)
		if (m_TxtLength > m_MaxLength)
		{
			m_MaxLength = m_TxtLength;
			OrthoModel::Destroy();
			Destroy();
		}
	}
	if (!m_pVb || !m_pIb)
	{
		CreateModel();
	}

	if (new_x != -1)
	{
#if !NEW_VERTEX_OFFSET_METHOD
		m_OffsetX = float(new_x - m_PosX);
#endif
		m_PosX = new_x;
	}

	if (new_y != -1)
	{
#if !NEW_VERTEX_OFFSET_METHOD
		m_OffsetY = float(new_y - m_PosY);
#endif
		m_PosY = new_y;
	}


//	if (red < 1.f || green < 1.f || blue < 1.f)
//		m_pixelColor = WXMVECTOR4(red, green, blue, 1.0f);

	// Create the vertex array.
//	vertices = new ORTHO_VERTEX[m_VertexCount];
//	if (!vertices)
//	{
//		return false;
//	}

	// Initialize vertex array to zeros at first.
//	memset(m_pVertices, 0, (sizeof(ORTHO_VERTEX) * m_VertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_wScreen / 2) * -1) + m_PosX);
	drawY = (float)((m_hScreen / 2) - m_PosY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	//if (text)
		BuildVertexArray((void*)m_pVertices, text, drawX, drawY);
	//else
	//{
	//	OffsetVertexPosition(m_pVertices, m_VertexCount, float(new_x - m_PosX), float(new_y - m_PosY));
	//}

	// check rect width/height, if need move tex to center of the rect, then it should be done here after BuildVertexArray, 
	// as we calc the string width/height during Vertex Building.
	{
		float x_offset = 0.f;
		float y_offset = 0.f;
		if (rect_w)
		{
			x_offset = ((float)rect_w - m_Width) / 2.f;
			m_PosX += (int)x_offset;
#if !NEW_VERTEX_OFFSET_METHOD
			m_OffsetX += x_offset;
#endif
		}
		if (rect_h)
		{
			y_offset = ((float)rect_h - m_Height) / 2.f;
			m_PosY += (int)y_offset;
#if !NEW_VERTEX_OFFSET_METHOD
			m_OffsetY += y_offset;
#endif
		}
		OffsetVertexPosition(m_pVertices, m_VertexCount, (float)x_offset, (float)y_offset);
	}

	ResWriter rwtr(m_pImmeDC);
	rwtr.Copy(m_pVertices, sizeof(ORTHO_VERTEX)* m_VertexCount, m_pVb);

	// If shadowed then do the same for the second vertex buffer but offset by two pixels on both axis.
	if (m_bShadow)
	{
		ORTHO_VERTEX* vertices = new ORTHO_VERTEX[m_VertexCount];;
		memcpy(vertices, m_pVertices, (sizeof(ORTHO_VERTEX) * m_VertexCount));

		//drawX = (float)((((m_wScreen / 2) * -1) + m_PosX) +1);
		//drawY = (float)(((m_hScreen / 2) - m_PosY) - 1);
#if NEW_VERTEX_OFFSET_METHOD
		OffsetVertexPosition(vertices, m_VertexCount, 1.f, 1.f);
#else
		m_OffsetX = 1.f;
		m_OffsetY = 1.f;
		BuildVertexArray((void*)vertices, nullptr, drawX, drawY);
#endif
		//BuildVertexArray((void*)vertices, text, drawX, drawY);
		rwtr.Copy(vertices, sizeof(ORTHO_VERTEX)* m_VertexCount, m_pVb2);
		SAFEDELETEARRAY(vertices);
	}

	return true;
}
#endif

void FontText::Render(WXMVECTOR4* color/* = nullptr*/, WXMVECTOR4* shadow_color/* = nullptr*/)
{
	UINT stride = sizeof(ORTHO_VERTEX);
	UINT offset = 0;

	// If shadowed then render the shadow text first.
	if (m_bShadow)
	{
		if (shadow_color)
		{
			m_pixelColorShadow = *shadow_color;
		}
		m_pImmeDC->IASetVertexBuffers(0, 1, &m_pVb2, &stride, &offset);
		m_pImmeDC->IASetIndexBuffer(m_pIb2, DXGI_FORMAT_R32_UINT, 0);
		m_pImmeDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_FontRenderer->Render(m_TxtLength * 6, &m_pixelColorShadow);
	}

	// Render the text buffers.
	if (color)
	{
		m_pixelColor = *color;
	}
	m_pImmeDC->IASetVertexBuffers(0, 1, &m_pVb, &stride, &offset);
	m_pImmeDC->IASetIndexBuffer(m_pIb, DXGI_FORMAT_R32_UINT, 0);
	m_pImmeDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_FontRenderer->Render(m_TxtLength * 6, &m_pixelColor);
}
