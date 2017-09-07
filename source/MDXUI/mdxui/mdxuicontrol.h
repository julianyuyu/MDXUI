
#ifndef _MDXUICONTROL_H_
#define _MDXUICONTROL_H_

#pragma once

#include "mdxuicontrolbase.h"

class MDXUIStatic : public MDXUIPanel
{
public:
	MDXUIStatic(MDXUIWndManager *mgr, int x, int y, LPWSTR caption, ULONG id);
	virtual ~MDXUIStatic() {}
//	virtual int MouseMessageProc(UINT msg, POINT pt, ULONG_PTR wparam = 0) { return 0; }
protected:
	virtual void BuildVertexArray(int UIindex, void* ptr, float drawX, float drawY, float drawX2, float drawY2);
};

class MDXUICheckbox : public MDXUIPanel
{
public:
	MDXUICheckbox(MDXUIWndManager *mgr, int x, int y, LPWSTR caption, ULONG id);
	virtual ~MDXUICheckbox() {}
	virtual bool GetState() { return (m_State == MDXUI_STATE_PRESSED); }
	virtual int MouseMessageProc(UINT msg, POINT pt, ULONG_PTR wparam = 0);
protected:

	virtual void BuildVertexArray(int UIindex, void* ptr, float drawX, float drawY, float drawX2, float drawY2);
	bool m_bClicking;
};

class MDXUIButton : public MDXUIPanel
{
public:
	MDXUIButton(MDXUIWndManager *mgr, int x, int y, int w, int h, LPWSTR Caption, ULONG id);
	virtual ~MDXUIButton() {}
	virtual int MouseMessageProc(UINT msg, POINT pt, ULONG_PTR wparam = 0);
	virtual void SetText(LPWSTR String) {}
	virtual void SetEnable(bool in_bEnabled) {}

protected:
	virtual void BuildVertexArray(int UIindex, void* ptr, float drawX, float drawY, float drawX2, float drawY2);
};

class MDXUICombobox : public MDXUIPanel
{
	struct ITEM {
		int idx;
		LPWSTR txt;
		FontText *Font;
	};
	typedef std::vector<ITEM *>::iterator ITEM_ITER;
public:
	MDXUICombobox(MDXUIWndManager *mgr, int x, int y, int w, LPWSTR Caption, ULONG id);
	virtual ~MDXUICombobox()
	{
		ITEM_ITER it = m_Items.begin();
		for (; it != m_Items.end(); ++it)
		{
			SAFEDELETE((*it)->Font);
			SAFEDELETE((*it));
		}
		m_Items.clear();
	}
	virtual int MouseMessageProc(UINT msg, POINT pt, ULONG_PTR wparam=0);

	virtual void Move(int x, int y);
	virtual void SetText(LPWSTR String) {}
	virtual void SetEnable(bool in_bEnabled) {}
	virtual int AddString(LPWSTR String);
	virtual int FindString(LPWSTR String) { return 0; }
	virtual int GetCurrent() { return m_Current; }
	virtual int SetCurrent() { return 0; }
	virtual void Render()
	{
		if (m_bVisible)
		{
			RenderModel();
			m_pText->Render();

			if (m_State == MDXUI_STATE_PRESSED)
			{
				ITEM_ITER it = m_Items.begin();
				for (; it != m_Items.end(); ++it)
				{
					(*it)->Font->Render();
				}
			}
		}
	}
protected:
	virtual void BuildVertexArray(int UIindex, void* ptr, float drawX, float drawY, float drawX2, float drawY2);

	void MoveSubItemText(FontText* text, int idx)
	{
		text->Move(m_rc.x + m_TextX, m_rc.y + m_rc.h + m_ItemH * idx + m_TextY);
	}
	bool m_bClicking;
//	bool m_bScrolling;
	bool m_bDropping; // is dropping down (no need, check if m_State == PRESSED)
	int m_Current; // current item index

	const int m_ItemH = 30;
	std::vector<ITEM *> m_Items;
};

class MDXUISlider : public MDXUIPanel
{
public:
	MDXUISlider(MDXUIWndManager *mgr, int x, int y, int w, LPWSTR Caption, ULONG id);
	virtual ~MDXUISlider() {}
	virtual int MouseMessageProc(UINT msg, POINT pt, ULONG_PTR wparam=0);

	virtual void SetPosition(int pos) { m_Position = pos; }
	virtual int  GetPosition() { return m_Position; }
	virtual int  GetValue() { return m_MinRange + m_Position / (m_MaxRange - m_MinRange); }
	virtual void SetRange(int min, int max, int step=1) { m_MinRange = min; m_MaxRange = max; }
	virtual void SetText(LPWSTR String) {}
	virtual void SetEnable(bool in_bEnabled) {}

protected:
	virtual void Sliding(POINT pt);
	virtual int CalcButtonLeftPosition()
	{
		int w_btn = m_HandleW;
		int w_tray = m_rc.w - w_btn;
		int x_btn = m_rc.x + /*w_btn/2 + */(int)((float)w_tray * (float)m_Position / (float)(m_MaxRange - m_MinRange));
		return x_btn;
	}
	virtual void BuildVertexArray(int UIindex, void* ptr, float drawX, float drawY, float drawX2, float drawY2);

	// const value
	int m_HandleH;
	int m_HandleW;
	//const int cHandleH = 30;
	
//	const int m_SliderW = 25;
	const int m_TxtH = 24;
	int m_MinRange;
	int m_MaxRange;
	int m_Position;
	bool m_bSliding;
	bool m_bClicking;
	int m_MouseMovingX;
	int m_MouseMovingY;
};

#endif /*_MDXUICONTROL_H_*/
