#pragma once

#include "mdxuicontrolbase.h"
#include "mdxuicontrol.h"

class MDXUIWindow : public MDXUIPanel
{
	typedef std::vector<MDXUIPanel*>::iterator CONTROL_ITER;
public:

	MDXUIWindow(MDXUIWndManager *mgr, int x, int y, int w, int h, LPWSTR caption, ULONG id, bool isPad = false);
	virtual ~MDXUIWindow()
	{
		CONTROL_ITER it = m_Controls.begin();
		for (; it != m_Controls.end(); ++it)
		{
			SAFEDELETE(*it);
		}
		m_Controls.clear();
	}

	virtual void CreateStatic(int x, int y, LPWSTR caption = nullptr, ULONG id = 0)
	{
		MDXUIStatic *sta = new MDXUIStatic(m_pManager, m_rc.x + x, m_rc.y + m_TitleHeight + y, caption, id);
		AddControl(sta);
	}
	virtual MDXUIButton* CreateButton(int x, int y, int w, int h, LPWSTR caption = nullptr, ULONG id = 0)
	{
		MDXUIButton *btn = new MDXUIButton(m_pManager, m_rc.x + x, m_rc.y + m_TitleHeight + y, w, h, caption, id);
		AddControl(btn);
		return btn;
	}
	virtual MDXUISlider* CreateSlider(int x, int y, int w, LPWSTR text = nullptr, ULONG id = 0)
	{
		MDXUISlider *sld = new MDXUISlider(m_pManager, m_rc.x + x, m_rc.y + m_TitleHeight + y, w, text, id);
		AddControl(sld);
		return sld;
	}
	virtual void CreateCheckbox(int x, int y, LPWSTR caption = nullptr, ULONG id = 0)
	{
		MDXUICheckbox *chk = new MDXUICheckbox(m_pManager, m_rc.x + x, m_rc.y + m_TitleHeight + y, caption, id);
		AddControl(chk);
	}
	virtual MDXUICombobox* CreateCombobox(int x, int y, int w, LPWSTR text = nullptr, ULONG id = 0)
	{
		MDXUICombobox *cmb = new MDXUICombobox(m_pManager, m_rc.x + x, m_rc.y + m_TitleHeight + y, w, text, id);
		AddControl(cmb);
		return cmb;
	}

	virtual void Render();

	virtual LRESULT MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int MouseMessageProc(UINT msg, POINT pt, ULONG_PTR wparam = 0);

	// internal use
	virtual MDXUIControl* FindControl(int id)
	{
		CONTROL_ITER it = m_Controls.begin();
		for (; it != m_Controls.end(); ++it)
		{
			if ((*it)->GetControlId() == id)
			{
				return *it;
			}
		}
		return nullptr;
	}
protected:
	virtual void AddControl(MDXUIPanel* ctl) { ctl->SetCb(m_pMessageCb); m_Controls.push_back(ctl); }
	virtual void Move(int x, int y);

	virtual void BuildVertexArray(int UIindex, void* ptr, float drawX, float drawY, float drawX2, float drawY2);
	RECT m_TitleRect;
	bool m_bMinimized;
	bool m_bMoving;
	int m_MouseMovingX;
	int m_MouseMovingY;
	const int m_TitleHeight = 20;

	MDXUIPanel *m_pCurrControl;
	std::vector<MDXUIPanel *> m_Controls;
	bool m_bIsTransparentPad; // is a pad, no window title bar, no window rect, only a rect(with color , or even transparent)
};
