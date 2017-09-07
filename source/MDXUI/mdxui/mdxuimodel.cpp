
#include "inc.h"
#include "creator.h"
#include "mdxuimodel.h"

MDXUITexLayouter *MDXUIModel::g_TexLayouter = nullptr;

bool MDXUIModel::UpdateModel(int UIindex, int posX, int posY, int posX2, int posY2)
{
//	ORTHO_VERTEX* vertices;
	float drawX=0.f, drawY=0.f, drawX2=0.f, drawY2=0.f;

	if (!m_pVb || !m_pIb)
	{
		CreateModel();
	}

	// Create the vertex array.
//	vertices = new ORTHO_VERTEX[m_VertexCount];
//	if (!vertices)
//	{
//		return false;
//	}

	// Initialize vertex array to zeros at first.
//	memset(m_pVertices, 0, (sizeof(ORTHO_VERTEX) * m_VertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_wScreen / 2) * -1) + posX);
	drawY = (float)((m_hScreen / 2) - posY);
	if (posX2 != -1 && posY2 != -1)
	{
		drawX2 = (float)(((m_wScreen / 2) * -1) + posX2);
		drawY2 = (float)((m_hScreen / 2) - posY2);
	}

	BuildVertexArray(UIindex, (void*)m_pVertices, drawX, drawY, drawX2, drawY2);
	ResWriter rwtr(m_pImmeDC);
	rwtr.Copy(m_pVertices, sizeof(ORTHO_VERTEX)* m_VertexCount, m_pVb);

//	SAFEDELETEARRAY(vertices);

	return true;
}

void MDXUIModel::RenderModel()
{
	UINT stride = sizeof(ORTHO_VERTEX);
	UINT offset = 0;

	// Render the UI Model buffers.
	m_pImmeDC->IASetVertexBuffers(0, 1, &m_pVb, &stride, &offset);
	m_pImmeDC->IASetIndexBuffer(m_pIb, DXGI_FORMAT_R32_UINT, 0);
	m_pImmeDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (m_pDXUIRenderer)
		m_pDXUIRenderer->Render(m_IndexCount);
}