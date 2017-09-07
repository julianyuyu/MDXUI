#include "inc.h"

#include "mdxui.h"
#include "mdxuishader.h"
#include "mdxuimodel.h"
#include "mdxuicontrol.h"
#include "mdxuiwindow.h"
#include "mdxuimanager.h"

#define MDXUI_CHECKBOX_W	29
#define MDXUI_CHECKBOX_H	29
#define MDXUI_COMBOBOX_H	28
#define MDXUI_SLIDER_H	25
#define MDXUI_SLIDER_THUMB_W	25
#define MDXUI_SLIDER_THUMB_H	30
#define MDXUI_STATIC_W	1	/* just for creation */
#define MDXUI_STATIC_H	20


MDXUIPanel::MDXUIPanel(MDXUIWndManager* mgr, int x, int y, int w, int h, LPWSTR caption, ULONG id)
	: MDXUIControl(id), MDXUIModel()
{
	m_pManager = mgr;
	OrthoModel::Initialize(mgr->D3D11Device(), mgr->ScreenWidth(), mgr->ScreenHeight());
	MDXUIModel::Initialize(mgr->UIRenderer());
	SetRect(x, y, w, h);
//	UpdateModelWithRect(MDXUI_UIACTION_NONE);

	m_pText = new FontText(m_pDevice, m_wScreen, m_hScreen, mgr->UIFontRenderer());
//	m_pText->UpdateModel(caption, 0, 0);
}

MDXUIStatic::MDXUIStatic(MDXUIWndManager *mgr, int x, int y, LPWSTR caption, ULONG id) :
	MDXUIPanel(mgr, x, y, MDXUI_STATIC_W, MDXUI_STATIC_H, caption, id)
{
	m_bVisible = true;
	m_QuadCount = 1;
	//m_pText->Initialize(m_rc.x, m_rc.y);
	m_pText->CenteredDisplay(0, MDXUI_STATIC_H);
	//m_pText->UpdateModel(caption/*, m_rc.x, m_rc.y*/);
	m_pText->UpdateText(caption);
	MoveText();

	UpdateModel(MDXUI_UIACTION_NONE, x, y);
}

void MDXUIStatic::BuildVertexArray(int UIindex, void* ptr, float drawX, float drawY, float drawX2, float drawY2)
{
	ORTHO_VERTEX* vertices = (ORTHO_VERTEX*)ptr;

	if (UIindex == -1)
	{
		// moving
		for (int i = 0; i < m_VertexCount; i++)
		{
			vertices[i].position.x += m_MovingOffsetX;
			vertices[i].position.y -= m_MovingOffsetY;
		}
	}
	else
	{
		// actually no Tex is need, here we just use transparent UV to leverage exist code.
		TEXCOORD *tc = &g_TexLayouter->TransparentUV;
		InitQuadVertices(vertices, drawX, drawX, 1, 1, tc->u1, tc->v1, tc->u2, tc->v2);
		return;
	}
}

MDXUICheckbox::MDXUICheckbox(MDXUIWndManager *mgr, int x, int y, LPWSTR caption, ULONG id) :
	MDXUIPanel(mgr, x, y, MDXUI_CHECKBOX_W, MDXUI_CHECKBOX_H, caption, id), m_bClicking(false)
{
	m_bVisible = true;
	m_QuadCount = 1;
	m_State = MDXUI_STATE_NORMAL;

	//m_TextX = m_rc.w + 1;
	//m_TextY = 0;

	//m_pText->Initialize(m_rc.x + m_TextX, m_rc.y + m_TextY);
	//m_pText->UpdateModel(caption/*, m_rc.x + m_TxtOffsetX, m_rc.y+ m_TxtOffsetY*/);
	m_TextX = m_rc.w + 1;
	m_pText->CenteredDisplay(0, MDXUI_CHECKBOX_H);
	m_pText->UpdateText(caption);
	MoveText();

	UpdateModel(MDXUI_UIACTION_NONE, x, y);
}

int MDXUICheckbox::MouseMessageProc(UINT msg, POINT pt, ULONG_PTR wparam)
{
	if (m_rc.PtIn(pt))
	{
		//POINT pt1 = { m_rc.x, m_rc.y };
		//POINT pt2 = { m_rc.x + m_rc.w, m_rc.y + m_rc.h };
		//m_bClicking = false;
		if (msg == WM_LBUTTONDOWN)
		{
			if (m_rc.PtIn(pt))
			{
				if (!m_bClicking)
					m_bClicking = true;
				//m_State = DXUI_BUTTON_PRESSED;
				//UpdateModel(1, pt1, pt2);
				//UpdateModelWithRect(MDXUI_UIACTION_BUTTON_PRESSED);
			}
		}
		else if (msg == WM_LBUTTONUP)
		{
			if (m_bClicking)
			{
				m_bClicking = false;
				if (m_rc.PtIn(pt))
				{
					if (m_State == MDXUI_STATE_NORMAL)
					{

						m_State = MDXUI_STATE_PRESSED;
						UpdateModelWithRect(MDXUI_UIACTION_BUTTON_PRESSED);
					}
					else if (m_State == MDXUI_STATE_PRESSED)
					{
						m_State = MDXUI_STATE_NORMAL;
						UpdateModelWithRect(MDXUI_UIACTION_NONE);
					}

					m_pMessageCb(MDM_COMMAND, m_ControlId, (ULONG_PTR)MDN_CB_CHANGED, (ULONG_PTR)m_State);
					return 1;
				}
			}
		}
	}
	//UpdateModel(0, m_rc.x, m_rc.y);
	return 0;
}

void MDXUICheckbox::BuildVertexArray(int UIindex, void* ptr, float drawX, float drawY, float drawX2, float drawY2)
{
	ORTHO_VERTEX* vertices = (ORTHO_VERTEX*)ptr;

	if (UIindex == MDXUI_UIACTION_MOVING)
	{
		// moving
		for (int i = 0; i < m_VertexCount; i++)
		{
			vertices[i].position.x += m_MovingOffsetX;
			vertices[i].position.y -= m_MovingOffsetY;
		}
	}
	else
	{
		int idx = 0;
		if (UIindex == MDXUI_UIACTION_NONE)
			idx = 0;
		else if (UIindex == MDXUI_UIACTION_BUTTON_PRESSED)
			idx = 1;
		float u1, v1, u2, v2;
		float x, y, w, h;
		TEXQUAD *quad = nullptr;
		CHKBOXCOORD *qc = &(g_TexLayouter->ChkboxUV[idx]);
		for (int i = 0; i < m_QuadCount; i++)
		{
			quad = &(qc->Quad);
			u1 = quad->u1;
			v1 = quad->v1;
			u2 = quad->u2;
			v2 = quad->v2;

			w = (float)quad->w;
			h = (float)quad->h;
			x = drawX;
			y = drawY;

			InitQuadVertices(vertices, x, y, w, h, u1, v1, u2, v2);
		}
	}
}

MDXUIButton::MDXUIButton(MDXUIWndManager *mgr, int x, int y, int w, int h, LPWSTR caption, ULONG id) :
	MDXUIPanel(mgr, x, y, w, h, caption, id)
{
	m_Type = MDXUI_BUTTON;
	m_bVisible = true;
	m_QuadCount = 9;

	UpdateModelWithRect(MDXUI_UIACTION_NONE);
	//m_pText->UpdateModel(caption, m_rc.x, m_rc.y, w, h);

	m_pText->CenteredDisplay(w, h);
	m_pText->UpdateText(caption);
	MoveText();
	//SetText(caption);
}

int MDXUIButton::MouseMessageProc(UINT msg, POINT pt, ULONG_PTR wparam)
{
	if (m_rc.PtIn(pt))
	{
		//POINT pt1 = { m_rc.x, m_rc.y };
		//POINT pt2 = { m_rc.x + m_rc.w, m_rc.y + m_rc.h };
		if (msg == WM_LBUTTONDOWN)
		{
			m_State = MDXUI_STATE_PRESSED;
			//UpdateModel(1, pt1, pt2);
			UpdateModelWithRect(MDXUI_UIACTION_BUTTON_PRESSED);
		}
		else if (msg == WM_LBUTTONUP)
		{
			m_State = MDXUI_STATE_NORMAL;
			//UpdateModel(0, 100, 100, 260, 140);
			UpdateModelWithRect(MDXUI_UIACTION_NONE);
			m_pMessageCb(MDM_COMMAND, m_ControlId, 0, 0);
			return 1;
		}
	}
	//UpdateModel(0, 100, 100, 260, 140);
	return 0;
}

void MDXUIButton::BuildVertexArray(int UIIndex, void* ptr, float drawX, float drawY, float drawX2, float drawY2)
{
	ORTHO_VERTEX* vertices = (ORTHO_VERTEX*)ptr;

	if (UIIndex == MDXUI_UIACTION_MOVING)
	{
		// moving
		for (int i = 0; i < m_VertexCount; i++)
		{
			vertices[i].position.x += m_MovingOffsetX;
			vertices[i].position.y -= m_MovingOffsetY;
		}
	}
	else
	{
		float u1, v1, u2, v2;
		float x, y, w, h;

		int idx = 0;
		if (UIIndex == MDXUI_UIACTION_BUTTON_PRESSED)
			idx = 1;

		// button
		TEXQUAD *quad = nullptr;
		BUTTONCOORD *qc = &(g_TexLayouter->ButtonUV[idx]);
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

			InitQuadVertices(vertices, x, y, w, h, u1, v1, u2, v2);
		}
	}
}

MDXUICombobox::MDXUICombobox(MDXUIWndManager *mgr, int x, int y, int w, LPWSTR caption, ULONG id) :
	MDXUIPanel(mgr, x, y, w, MDXUI_COMBOBOX_H, caption, id)
{
	m_bVisible = true;
	m_QuadCount = 11;
	m_Type = MDXUI_COMBOBOX;
	m_Items.clear();
	m_Current = 0;
	UpdateModelWithRect(MDXUI_UIACTION_NONE);
	//m_pText->UpdateModel(caption, m_rc.x + m_TextX, m_rc.y, 0, MDXUI_COMBOBOX_H);
	m_TextX = 4;
	m_pText->CenteredDisplay(0, MDXUI_COMBOBOX_H);
	m_pText->UpdateText(caption);
	MoveText();

	m_State = MDXUI_STATE_NORMAL;
	//UpdateModel(0, 300, 100, 460, 126);
}

int MDXUICombobox::AddString(LPWSTR str)
{
	FontText *font = new FontText(*m_pText);
	//font->UpdateModel(str, m_rc.x + m_TextX, m_rc.y + m_rc.h + m_ItemH * (int)m_Items.size() + m_TextY);

	font->CenteredDisplay(0, m_ItemH);
	font->UpdateText(str);
	//font->Move(m_rc.x + m_TextX, m_rc.y + m_rc.h + m_ItemH * (int)m_Items.size() + m_TextY);
	MoveSubItemText(font, (int)m_Items.size());

	ITEM *item = new ITEM{ 0, str, font };
	m_Items.push_back(item);
	return (int)m_Items.size() - 1; // index
}

int MDXUICombobox::MouseMessageProc(UINT msg, POINT pt, ULONG_PTR wparam)
{
	if (msg == WM_LBUTTONDOWN)
	{
		int EachH = m_ItemH;
		int DropH = m_Items.size() * EachH;

		UIRECT rcDropped(m_rc.x, m_rc.y, +m_rc.x + m_rc.w, m_rc.y + m_rc.h + DropH);

		if ((!m_bDropping && PtInRect(pt, m_rc)) ||
			(m_bDropping &&PtInRect(pt, rcDropped)))
		{
			m_bClicking = true;
		}
	}
	else if (msg == WM_MOUSEMOVE)
	{
		//if (wparam == MK_LBUTTON)
		{
			//if (m_bScrolling)

			if (m_State == MDXUI_STATE_PRESSED)
			{
				int EachH = m_ItemH;
				int DropH = m_Items.size() * EachH;
				//Sliding(pt);
				if (pt.x > m_rc.x && pt.x < m_rc.x + m_rc.w)
				{
					int y_item0 = m_rc.y + m_rc.h;
					int count = m_Items.size();
					int i = 0;
					for (i = 0; i < count; i++)
					{
						if (pt.y > y_item0 + EachH*i && pt.y < y_item0 + EachH*i + EachH)
						{
							break;
						}
					}
					if (i < count)
					{
						m_Current = i;
						UpdateModelWithRect(MDXUI_UIACTION_CMBBOX_SCROLLING);
					}
				}
			}
		}
	}
	else if (msg == WM_LBUTTONUP)
	{
		if (m_bClicking)
		{
			m_bClicking = false;
			//int DropH = 80; // height of dropped rect
			int BtnH = 26, BtnW = 26;
			int BoxH = m_rc.h;
			int Btn_OffsetY = (BoxH - BtnH) / 2;
			//int 
			int EachH = m_ItemH;
			int DropH = m_Items.size() * EachH;
			UIRECT rcBtn(m_rc.x + m_rc.w - BtnW, m_rc.y + Btn_OffsetY, BtnW, BtnH);
			POINT pt1 = { m_rc.x, m_rc.y };
			POINT pt2 = { m_rc.x + m_rc.w, m_rc.y + m_rc.h };
			POINT pt2_dropped = { pt2.x, pt2.y + DropH };

			if( ((m_State == MDXUI_STATE_NORMAL) && rcBtn.PtIn(pt)) )
			{
				m_bDropping = true;
				UpdateModel(MDXUI_UIACTION_NONE, pt1, pt2_dropped);
				m_State = MDXUI_STATE_PRESSED;
			}
			else if ((m_State == MDXUI_STATE_PRESSED) && PtInRect(pt, m_rc.x, m_rc.y, m_rc.w, m_rc.h+ DropH))
			{
				if (rcBtn.PtIn(pt))
				{
					// end with no change
					UpdateModel(MDXUI_UIACTION_NONE, pt1, pt2);
					MoveText();
					m_State = MDXUI_STATE_NORMAL;
				}
#if 0
				else
				{
				//	UpdateModel(0, pt1, pt2_dropped);
					if (pt.x > m_rc.x && pt.x < m_rc.x + m_rc.w)
					{
						int y_item0 = m_rc.y + m_rc.h;
						int count = m_Items.size();
						for (int i = 0; i < count; i++)
						{
							if (pt.y > y_item0 + EachH*i && pt.y < y_item0 + EachH*i + EachH)
							{
								break;
							}
						}
					}
				}
#endif
				else if (pt.y > pt2.y && pt.y < pt2_dropped.y)
				{
					// end with new item

					UpdateModel(MDXUI_UIACTION_NONE, pt1, pt2);
					//m_pText->CenteredDisplay(0, m_rc.h);
					m_pText->UpdateText(m_Items.at(m_Current)->txt);
					MoveText();
					m_State = MDXUI_STATE_NORMAL;
					m_pMessageCb(MDM_COMMAND, m_ControlId, (ULONG_PTR)MDN_CB_CHANGED, 0);
					return 1;
				}
			}

			// if mouse-button-down in control, but button-up not in control, we should not handle in here, but in window-manager,
			// for we should cancel all the controls which "button-down in control, but button-up not in control"
#if 0
			/*
			else
			{
				UpdateModel(0, pt1, pt2);
				m_State = MDXUI_STATE_NORMAL;
				m_pMessageCb(MDM_COMMAND, m_ControlId, 0, 0);
				return 1;
			}*/
#else
			// here we just close the dropped list, but not handle the message.
			else
			{
				UpdateModel(MDXUI_UIACTION_NONE, pt1, pt2);
				m_State = MDXUI_STATE_NORMAL;
				//m_pMessageCb(MDM_COMMAND, m_ControlId, 0, 0);
				//return 1;
			}
#endif
		}
	}
	return 0;
}

void MDXUICombobox::Move(int x, int y)
{
	m_MovingOffsetX = float(x/* - x0*/);
	m_MovingOffsetY = float(/*y0*/  y);

#if 0
	if (m_State == MDXUI_STATE_NORMAL)
	{

		m_rc.Move(x, y);
		ITEM_ITER it = m_Items.begin();
		int i = 0;
		for (; it != m_Items.end(); ++it, ++i)
		{
			(*it)->Font->UpdateModel(nullptr, m_rc.x + m_TxtOffsetX, m_rc.y + m_rc.h + m_ItemH * i + m_TxtOffsetY);
		}
	}
	else if (m_State == MDXUI_STATE_PRESSED)
#endif
	{

		//int EachH = m_ItemH;
		//int DropH = m_Items.size() * EachH;
		////POINT pt1 = { m_rc.x, m_rc.y };
		////POINT pt2 = { m_rc.x + m_rc.w, m_rc.y + m_rc.h };
		////POINT pt2_dropped = { pt2.x, pt2.y + DropH };
		//UIRECT rc = { m_rc.x, m_rc.y,  m_rc.w, m_rc.y + DropH };

		m_rc.Move(x, y);
		ITEM_ITER it = m_Items.begin();
		int i = 0;
		for (; it != m_Items.end(); ++it, ++i)
		{
			//(*it)->Font->UpdateModel(nullptr, m_rc.x + m_TextX, m_rc.y + m_rc.h + m_ItemH * i + m_TextY);
			//font->Move(m_rc.x + m_TextX, m_rc.y + m_rc.h + m_ItemH * (int)m_Items.size() + m_TextY);
			MoveSubItemText((*it)->Font, i);
		}
	}
	UpdateModelWithRect(MDXUI_UIACTION_MOVING);
	//m_pText->UpdateModel(nullptr, m_rc.x + m_TextX, m_rc.y + m_TextY);
	MoveText();
}

void MDXUICombobox::BuildVertexArray(int UIindex, void* ptr, float drawX, float drawY, float drawX2, float drawY2)
{
	ORTHO_VERTEX* vertices = (ORTHO_VERTEX*)ptr;

	if (UIindex == MDXUI_UIACTION_MOVING)
	{
		// moving
		for (int i = 0; i < m_VertexCount; i++)
		{
			vertices[i].position.x += m_MovingOffsetX;
			vertices[i].position.y -= m_MovingOffsetY;
		}
	}
	else if (UIindex == MDXUI_UIACTION_CMBBOX_SCROLLING)
	{
		// scrolling
		int idx = m_Current;

		int EachH = m_ItemH;
		int HightLight_Offset = 2;
		//int DropH = m_Items.size() * EachH;
		//Sliding(pt);
		//if (pt.x > m_rc.x && pt.x < m_rc.x + m_rc.w)
		{
			//int y_item0 = m_rc.y + m_rc.h;
			//int curr_x = m_rc.x;
			//int curr_y = y_item0 + EachH * m_Current;
			//int curr_w = m_rc.w;
			//int curr_h = EachH;
			//int count = m_Items.size();

			ORTHO_VERTEX* vtx = vertices+ 10*6;
			CMBBOXCOORD *qc = &(g_TexLayouter->CmbboxUV[0]);
			TEXQUAD *quad = &(qc->Quad[10]);

			float u1 = quad->u1;
			float v1 = quad->v1;
			float u2 = quad->u2;
			float v2 = quad->v2;
			float x = drawX + HightLight_Offset;
			float y = drawY- (float)(m_rc.h + EachH * m_Current) - HightLight_Offset;
			float w = drawX2 - drawX - HightLight_Offset *2;
			float h = (float)EachH - HightLight_Offset * 2;

			InitQuadVertices(vtx, x, y, w, h, u1, v1, u2, v2);
		}
	}
	else
	{
		float u1, v1, u2, v2;
		float x, y, w, h;

		// button
		TEXQUAD *quad = nullptr;
		CMBBOXCOORD *qc = &(g_TexLayouter->CmbboxUV[0]);
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
			if (i == 10)
			{
				//continue;
				// this is for selected item.
				// set as transparent tex by default.
				x = 0.f;
				y = 0.f;
				w = 1.f;
				h = 1.f;
				u1 = g_TexLayouter->TransparentUV.u1;
				v1 = g_TexLayouter->TransparentUV.v1;
				u2 = g_TexLayouter->TransparentUV.u2;
				v2 = g_TexLayouter->TransparentUV.v2;
			}
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
			if (i == 0 || i == 2 || i == 6 || i == 8 || i == 9)
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

				if (i == 9)
				{
					// button
					x = drawX2 - w - 3;
					y = drawY - 3;
				}
			}
			InitQuadVertices(vertices, x, y, w, h, u1, v1, u2, v2);
		}
	}
}

MDXUISlider::MDXUISlider(MDXUIWndManager *mgr, int x, int y, int w, LPWSTR caption, ULONG id) :
	MDXUIPanel(mgr, x, y, w, MDXUI_SLIDER_H, caption, id)
{
	m_bVisible = true;
	m_bSliding = false;
	m_bClicking = false;
	m_Type = MDXUI_SLIDER;
	m_HandleW = MDXUI_SLIDER_THUMB_W;
	m_HandleH = MDXUI_SLIDER_THUMB_H;
	m_MinRange = 0;
	m_MaxRange = 100;
	m_Position = 10;
	m_QuadCount = 5;

	UpdateModelWithRect(MDXUI_UIACTION_NONE);

	//m_pText->UpdateModel(caption, m_rc.x + m_TextX, m_rc.y, 0, MDXUI_SLIDER_H);
	m_TextX = m_rc.w + 8;
	//	m_rc.x += m_TextX + m_TxtH;
	m_pText->CenteredDisplay(0, MDXUI_SLIDER_H);
	m_pText->UpdateText(caption);
	MoveText();
}

/*
+----+
|    |
|  ==========
|    |
+----+
*/
void MDXUISlider::BuildVertexArray(int UIindex, void* ptr, float drawX, float drawY, float drawX2, float drawY2)
{
	ORTHO_VERTEX* vertices = (ORTHO_VERTEX*)ptr;

	if (UIindex == MDXUI_UIACTION_MOVING)
	{
		// moving
		for (int i = 0; i < m_VertexCount; i++)
		{
			vertices[i].position.x += m_MovingOffsetX;
			vertices[i].position.y -= m_MovingOffsetY;
		}
	}
	else if (UIindex == MDXUI_UIACTION_SLIDER_SLIDING)
	{
		// move button
		ORTHO_VERTEX* vtx_btn = (ORTHO_VERTEX*)vertices+(3/*m_QuadCount - 2*/)* 6;

		int w_btn = m_HandleW;
		int w_tray = m_rc.w - w_btn;
		float x0_new = drawX + (float)(w_tray * m_Position) / (float)(m_MaxRange - m_MinRange);
		float delta = x0_new - vtx_btn[0].position.x;
		for (int i = 0; i < 6; i++)
		{
			vtx_btn[i].position.x += delta;
		}
	}
	else
	{
		float u1, v1, u2, v2;
		float x, y, w, h;

		// button
		TEXQUAD *quad = nullptr;
		SLIDERCOORD *qc = &(g_TexLayouter->SliderUV[0]);
		float w_total = /*(float)m_rc.w;*/(drawX2 - drawX);
		float w_nub = (float)qc->Quad[3].w;
		float h_nub = (float)qc->Quad[3].h;
		float w_tray_total = w_total - w_nub;
		float w_tray_left = (float)qc->Quad[0].w;
		float w_tray_right = (float)qc->Quad[2].w;
		float w_tray_mid = w_tray_total - w_tray_left - w_tray_right;
		//w_tray_mid -= w_nub; // because: total width = left half nub width + w_tray + right half nub width
		float h_tray = (float)qc->Quad[0].h;
		float x_tray_offset = (float)w_nub / 2.f;
		float y_tray_offset = (float)(h_nub - h_tray) / 2.f;
		//float h_mid = (drawY - drawY2) - h_top - h_bottom;

		// update value
		m_HandleW = (int)qc->Quad[3].w;
		m_HandleH = (int)qc->Quad[3].h;

		for (int i = 0; i < m_QuadCount; i++)
		{
			quad = &(qc->Quad[i]);
			u1 = quad->u1;
			v1 = quad->v1;
			u2 = quad->u2;
			v2 = quad->v2;
			w = (float)quad->w;
			h = (float)quad->h;
			if (i == 4)
				continue; // 4 is tick
			if (i == 1 || i == 2 || i == 3 || i == 0)
			{
				if (i == 3)
				{
					//x = drawX2-w_nub;
					int range = m_MaxRange - m_MinRange;
					x = drawX + (float)(w_tray_total * m_Position) / (float)range;
					y = drawY;
				}

				if (i == 1 || i == 2 || i == 0)
				{
					y = drawY - y_tray_offset;
				}
				if (i == 0)
				{
					x = x_tray_offset + drawX;

				}
				if (i == 1)
				{
					x = x_tray_offset + drawX + w_tray_left;
					w = w_tray_mid;
				}
				if (i == 2)
				{
					x = x_tray_offset + drawX + w_tray_left + w_tray_mid;
				}
			}

			InitQuadVertices(vertices, x, y, w, h, u1, v1, u2, v2);
		}
	}
}

void MDXUISlider::Sliding(POINT pt)
{
	int delta_x = pt.x - m_MouseMovingX;
	int w_btn = m_HandleW;
	int w_tray = m_rc.w - w_btn;
	m_Position += (int)((float)(m_MaxRange - m_MinRange) *(float)delta_x / (float)w_tray);
	if (m_Position > m_MaxRange)
		m_Position = m_MaxRange;
	if (m_Position < m_MinRange)
		m_Position = m_MinRange;
	//m_Position 
	UpdateModelWithRect(MDXUI_UIACTION_SLIDER_SLIDING);
	m_MouseMovingX = pt.x;
	m_MouseMovingY = pt.y;
	m_pMessageCb(MDM_COMMAND, m_ControlId, (ULONG_PTR)m_Position, 0);
}

int MDXUISlider::MouseMessageProc(UINT msg, POINT pt, ULONG_PTR wparam)
{
	if (msg == WM_MOUSEMOVE)
	{
		if (wparam == MK_LBUTTON)
		{
			if (m_bSliding)
			{
				Sliding(pt);
			}
		}
	}
	else if (msg == WM_LBUTTONDOWN)
	{
		if (!m_bSliding)
		{
			int w_btn = m_HandleW;
			int w_tray = m_rc.w - w_btn;
			//int x_btn = m_rc.x + /*w_btn/2 + */w_tray * m_Position / (m_MaxRange - m_MinRange);
			int x_btn = CalcButtonLeftPosition();
			UIRECT rcBtn(x_btn, m_rc.y, w_btn, m_rc.h);
			UIRECT rcTray(m_rc.x+w_btn/2, m_rc.y+ (m_HandleH - 20)/2, w_tray, 20);
			if (rcBtn.PtIn(pt))
			{
				m_bSliding = true;
				//UpdateModelWithRect(MDXUI_UIACTION_SLIDER_SLIDING);
				m_MouseMovingX = pt.x;
				m_MouseMovingY = pt.y;
			}
			if (rcTray.PtIn(pt) && !rcBtn.PtIn(pt))
			{
				m_bClicking = true;
				m_MouseMovingX = pt.x;
			}
		}
	}
	else if (msg == WM_LBUTTONUP)
	{
		if (m_bSliding)
			m_bSliding = false;
		if (m_bClicking)
		{
			m_bClicking = false;
			int w_tray = m_rc.w - m_HandleW;
			m_Position = (int)((float)(pt.x - m_rc.x- m_HandleW /2) *(float)(m_MaxRange - m_MinRange) / (float)w_tray);
			UpdateModelWithRect(MDXUI_UIACTION_SLIDER_SLIDING);
		}
	}
	return 0;
}
