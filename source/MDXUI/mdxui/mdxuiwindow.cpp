#include "inc.h"

#include "mdxuimodel.h"
#include "mdxuiwindow.h"
#include "mdxuicontrol.h"
#include "mdxuimanager.h"

MDXUIWindow::MDXUIWindow(MDXUIWndManager *mgr, int x, int y, int w, int h, LPWSTR caption, ULONG id, bool isTransparentPad) :
	MDXUIPanel(mgr, x, y, w, h, caption, id)
{
	m_pCurrControl = nullptr;
	m_pMessageCb = nullptr;
	m_bIsTransparentPad = isTransparentPad;
	m_bVisible = true;
	m_bMinimized = false;

	m_rc.h += m_TitleHeight;

	m_Controls.clear();

	m_TextX = 6;
	m_TextY = 3;

	if (m_bIsTransparentPad)
		m_QuadCount = 1;
	else
	{
		m_QuadCount = 10;
		//m_pText->Initialize();
		//m_pText->UpdateModel(caption, m_rc.x + m_TextX, m_rc.y + m_TextY);
		m_pText->UpdateText(caption);
		MoveText();
	}
	UpdateModelWithRect(0);
	m_bMoving = false;
}

void MDXUIWindow::Render()
{
	if (m_bVisible)
	{
		RenderModel();
		if (!m_bIsTransparentPad)
		{
			m_pText->Render();
		}
		if (!m_bMinimized)
		{
			CONTROL_ITER it = m_Controls.begin();
			for (; it != m_Controls.end(); ++it)
			{
				(*it)->Render();
			}
		}
	}
}

void MDXUIWindow::Move(int x, int y)
{
	MDXUIPanel::Move(x, y);
	CONTROL_ITER it = m_Controls.begin();
	for (; it != m_Controls.end(); ++it)
	{
		(*it)->Move(x, y);
	}
}

LRESULT MDXUIWindow::MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// Mouse messages
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_XBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
		{
			POINT pt = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			return MouseMessageProc(message, pt, wParam);
		}
	}

	return FALSE;
}

int MDXUIWindow::MouseMessageProc(UINT msg, POINT pt, ULONG_PTR wparam)
{
	if (msg == WM_MOUSEMOVE)
	{
		if (wparam == 0 || wparam == MK_LBUTTON)
		{
			// decide current control
			if (!m_pCurrControl || !m_pCurrControl->PtIn(pt))
			{
				CONTROL_ITER it = m_Controls.begin();
				for (; it != m_Controls.end(); ++it)
				{
					if (*it != m_pCurrControl)
					{
						if ((*it)->PtIn(pt))
						{
							m_pCurrControl = *it;
							break;
						}
					}
				}
			}
		}

		if (wparam == MK_LBUTTON)
		{
#if 0
			if (!m_bMoving)
				//if (PtIn(pt))
			{
				if (!m_bIsTransparentPad && PtInRect(pt, m_rc.x, m_rc.y, m_rc.w, m_TitleHeight))
				{
					m_bMoving = true;
					//	m_OffsetX = pt.x - m_Rect.left;
					//	m_OffsetY = pt.y - m_Rect.top;
					//Move(pt.x - m_MovingOffsetX, pt.y - m_MovingOffsetY);
					m_MouseMovingX = pt.x;
					m_MouseMovingY = pt.y;
				}
			}
			else
#endif
				if (m_bMoving)
				{
					Move(pt.x - m_MouseMovingX, pt.y - m_MouseMovingY);
					m_MouseMovingX = pt.x;
					m_MouseMovingY = pt.y;
					return 1;
				}
		}
	}
	else if (msg == WM_LBUTTONDOWN)
	{
		//if (PtInRect(pt, m_TitleRect))
		//m_State = DXUI_BUTTON_PRESSED;
		//UpdateModel(1, 100, 100, 260, 140);

		//if (!m_bMoving && PtInRect(pt, m_TitleRect))
		//{
		//m_bMoving = true;
		//m_OffsetX = pt.x - m_Rect.left;
		//m_OffsetY = pt.y - m_Rect.top;
		//}

		TEXQUAD *quad = nullptr;
		WINDOWCOORD *qc = &(g_TexLayouter->WindowUV[0]);
		int w = qc->Quad[9].w;
		int h = qc->Quad[9].h;
		int x = m_rc.x + m_rc.w - w - 4;
		int y = m_rc.y - 4;
		if (PtInRect(pt, x, y, w, h))
		{
			m_bMinimized = !m_bMinimized;
			UpdateModelWithRect(-2);
		}

		if (!m_bMoving)
			//if (PtIn(pt))
		{
			if (!m_bIsTransparentPad && PtInRect(pt, m_rc.x, m_rc.y, m_rc.w, m_TitleHeight))
			{
				m_bMoving = true;
				//	m_OffsetX = pt.x - m_Rect.left;
				//	m_OffsetY = pt.y - m_Rect.top;
				//Move(pt.x - m_MovingOffsetX, pt.y - m_MovingOffsetY);
				m_MouseMovingX = pt.x;
				m_MouseMovingY = pt.y;
				return 0;
			}
		}
	}
	else if (msg == WM_LBUTTONUP)
	{
		//m_State = DXUI_BUTTON_NORMAL;
		//UpdateModel(0, 100, 100, 260, 140);
		if (m_bMoving)
			m_bMoving = false;
	}

	// if window is minimized, don't send any messages to controls.
	if (m_bMinimized)
		return 0;

	int bMsgHandled = 0; // no need further processing

	if (m_pCurrControl)
	{
		bMsgHandled = m_pCurrControl->MouseMessageProc(msg, pt, wparam);
		if (bMsgHandled)
			return 1;
	}
	CONTROL_ITER it = m_Controls.begin();
	for (; it != m_Controls.end(); ++it)
	{
		if (*it != m_pCurrControl)
		{
			bMsgHandled = (*it)->MouseMessageProc(msg, pt, wparam);
			if (bMsgHandled)
				return 1;
		}
	}
	return 0;
}

void MDXUIWindow::BuildVertexArray(int UIIndex, void* ptr, float drawX, float drawY, float drawX2, float drawY2)
{
	ORTHO_VERTEX* vertices = (ORTHO_VERTEX*)ptr;

	float u1, v1, u2, v2;
	float x, y, w, h;

	if (m_bIsTransparentPad && m_QuadCount == 1)
	{
		TEXCOORD *tc = &g_TexLayouter->TransparentUV;
		InitQuadVertices(vertices, drawX, drawX, drawX2 - drawX, drawY2 - drawY, tc->u1, tc->v1, tc->u2, tc->v2);
		return;
	}

	if (UIIndex == -1)
	{
		// moving
		for (int i = 0; i < m_VertexCount; i++)
		{
			vertices[i].position.x += m_MovingOffsetX;
			vertices[i].position.y -= m_MovingOffsetY;
		}
	}
	else
	{/*
	 if (UIIndex == -2 && m_bMinimized)
	 {
	 // minimized

	 for (int i = 0; i < m_QuadCount; i++)
	 {

	 }
	 if ()
	 w = (float)quad->w;
	 h = (float)quad->h;
	 x = drawX2 - w - 4;
	 y = drawY - 4;
	 }
	 else
	 */
		{
			int ui_idx = 0;
			if (UIIndex == -2)
				ui_idx = 0;

			TEXQUAD *quad = nullptr;
			WINDOWCOORD *qc = &(g_TexLayouter->WindowUV[ui_idx]);
			float w_left = (float)qc->Quad[0].w;
			float w_right = (float)qc->Quad[2].w;
			float h_top = (float)qc->Quad[0].h;
			float h_bottom = (float)qc->Quad[6].h;
			float w_mid = (drawX2 - drawX) - w_left - w_right;
			float h_mid = (drawY - drawY2) - h_top - h_bottom;

			for (int i = 0; i < m_QuadCount; i++)
			{
				quad = &(qc->Quad[i]);
				u1 = quad->u1;
				v1 = quad->v1;
				u2 = quad->u2;
				v2 = quad->v2;
				//if (i != 0 && i != 7)
				//	continue;
				if (i == 3 || i == 4 || i == 5)
				{
					h = h_mid;
					y = drawY - h_top;
				}
				if (i == 1 || i == 4 || i == 7)
				{
					w = w_mid;
					x = drawX + w_left;
				}
				if (i == 3)
				{
					w = w_left;
					x = drawX;
				}
				if (i == 5)
				{
					w = w_right;
					x = drawX2 - w_right;
				}
				if (i == 1)
				{
					h = h_top;
					y = drawY;
				}
				if (i == 7)
				{
					h = h_bottom;
					y = drawY2 + h_bottom;
				}
				if (i == 0 || i == 2 || i == 6 || i == 8)
				{
					w = (float)quad->w;
					h = (float)quad->h;
					if (i == 0 || i == 6)
						x = drawX;
					if (i == 2 || i == 8)
						x = drawX2 - w_right;
					if (i == 0 || i == 2)
						y = drawY;
					if (i == 6 || i == 8)
						y = drawY2 + h_bottom;
				}
				if (i == 9)
				{
					w = (float)quad->w;
					h = (float)quad->h;
					x = drawX2 - w - 4;
					y = drawY - 4;
				}


				if (UIIndex == -2 && m_bMinimized)
				{
					if (i > 2 && i< 9)
					{
						u1 = g_TexLayouter->TransparentUV.u1;
						v1 = g_TexLayouter->TransparentUV.v1;
						u2 = g_TexLayouter->TransparentUV.u2;
						v2 = g_TexLayouter->TransparentUV.v2;
					}
				}

				InitQuadVertices(vertices, x, y, w, h, u1, v1, u2, v2);
			}
		}
	}
}
