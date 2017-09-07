#pragma once

#include "mdxuimodel.h"

typedef ULONG(CALLBACK *MDXUI_MESSAGECB)(UINT message, UINT ControlId, ULONG_PTR wParam, ULONG_PTR lParam);


class MDXUIWndManager;


enum MDXUI_CONTROL_TYPE
{
	MDXUI_CONTROL_NONE = 0,
	MDXUI_STATIC,
	MDXUI_BUTTON,
	MDXUI_CHECKBOX,
	MDXUI_COMBOBOX,
	MDXUI_SLIDER,
	MDXUI_PANEL,
//	MDXUI_WINDOW,
};

enum MDXUI_CONTROL_STATE
{
	MDXUI_STATE_NORMAL = 0,
	MDXUI_STATE_PRESSED,
	MDXUI_STATE_DISABLED,
};

enum MDXUI_UIACTION
{
	MDXUI_UIACTION_NONE = 0, // no action, just normal rendering.
	MDXUI_UIACTION_MOVING,
	MDXUI_UIACTION_BUTTON_PRESSED,
	MDXUI_UIACTION_SLIDER_SLIDING,
	MDXUI_UIACTION_CMBBOX_SCROLLING,
};

struct UIRECT
{
	int x, y, w, h;

	//UIRECT() {}
	~UIRECT() {}
	UIRECT(int xx = 0, int yy = 0, int ww = 0, int hh = 0)
	{
		Set(xx, yy, ww, hh);
	}
	void Set(int xx = 0, int yy = 0, int ww = 0, int hh = 0)
	{
		x = xx; y = yy; w = ww; h = hh;
	}
	bool PtIn(int xx, int yy)
	{
		return (xx >= x && xx <= x + w && yy >= y && yy <= y + h);
	}
	bool PtIn(POINT pt)
	{
		return PtIn(pt.x, pt.y);
	}

	void Move(int xx, int yy)
	{
		x += xx; y += yy;
	}
};

inline bool PtInRect(POINT pt, int x, int y, int w, int h)
{
	return (pt.x >= x && pt.x <= x + w && pt.y >= y && pt.y <= y + h);
}

inline bool PtInRect(POINT pt, UIRECT rc)
{
	return rc.PtIn(pt);
}

class MDXUIControl
{
public:
	MDXUIControl(int id) : m_ControlId(id), m_pMessageCb(nullptr) {}
	virtual ~MDXUIControl() {}
	virtual void SetRect(int x, int y, int w, int h)
	{
		//::SetRect(&m_Rect, x, y, x+w, y+h);
		m_rc.Set(x, y, w, h);
	}

	virtual void SetCb(MDXUI_MESSAGECB cb) { m_pMessageCb = cb; }
	virtual int MouseMessageProc(UINT msg, POINT pt, ULONG_PTR wparam = 0) { return 0; }

	virtual bool GetState() { return false; }
	virtual int GetControlId() { return m_ControlId; }
	//	virtual MDXUI_CONTROL_TYPE GetType() { return MDXUI_CONTROL_NONE; }

	virtual void SetEnable(bool in_bEnabled) {}
	virtual bool IsEnabled() { return true; }

	virtual bool PtIn(int x, int y) { return m_rc.PtIn(POINT{ x, y }); }
	//virtual bool PtIn(int x, int y) { POINT pt = { x, y }; return m_rc.PtIn(pt); }
	virtual bool PtIn(POINT pt) { return m_rc.PtIn(pt); }
protected:

	MDXUI_MESSAGECB		m_pMessageCb;
	bool				m_bVisible;
	ULONG				m_ControlId;
	MDXUIWndManager	   *m_pManager;
	UIRECT				m_rc;
	float m_MovingOffsetX;
	float m_MovingOffsetY;
	MDXUI_CONTROL_TYPE	m_Type;
	MDXUI_CONTROL_STATE	m_State;
};

class MDXUIPanel : public MDXUIControl, public MDXUIModel
{
public:
	MDXUIPanel(MDXUIWndManager *mgr, int x, int y, int w, int h, LPWSTR Caption, ULONG id);
	virtual ~MDXUIPanel() {}
	virtual int MouseMessageProc(UINT msg, POINT pt, ULONG_PTR wparam = 0) { return 0; }
	virtual void Render()
	{
		if (m_bVisible)
		{
			RenderModel();
			m_pText->Render();
		}
	}
	virtual void Move(int x, int y)
	{
		m_MovingOffsetX = float(x/* - x0*/);
		m_MovingOffsetY = float(/*y0*/  y);
		m_rc.Move(x, y);
		UpdateModelWithRect(MDXUI_UIACTION_MOVING);
		//m_pText->UpdateModel(nullptr, m_rc.x + m_TextX, m_rc.y + m_TextY);
		//m_pText->Move(m_rc.x + m_TextX, m_rc.y + m_TextY);
		MoveText();
	}
protected:
	virtual void MoveText()
	{
		if (m_pText)
			m_pText->Move(m_rc.x + m_TextX, m_rc.y + m_TextY);
	}
	virtual void BuildVertexArray(int UIindex, void* ptr, float drawX, float drawY, float drawX2, float drawY2) {}
	virtual bool UpdateModelWithRect(int UIindex)
	{
		return UpdateModel(UIindex, m_rc.x, m_rc.y, m_rc.x + m_rc.w, m_rc.y + m_rc.h);
	}
};
