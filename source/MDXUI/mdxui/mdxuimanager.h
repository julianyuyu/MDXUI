#pragma once
#include "inc.h"
#include <d3d11.h>
#include "mdxuiwindow.h"
#include "mdxuicontrol.h"
#include "fontrenderer.h"
#include "mdxuirenderer.h"

typedef ULONG(CALLBACK *MDXUI_MESSAGECB)(UINT message, UINT ControlId, ULONG_PTR wParam, ULONG_PTR lParam);


class MDXUIWndManager
{
	typedef std::vector<MDXUIWindow*>::iterator	WINDOW_ITER;
public:
	MDXUIWndManager(ID3D11Device* dev, ULONG ww, ULONG hh, MDXUI_MESSAGECB cb = nullptr) :
		m_pDevice11(dev), m_ScreenWidth(ww), m_ScreenHeight(hh), m_pMessageCb(cb)
	{
		m_pCurrWnd = nullptr;
		m_WindowList.clear();
		if (!s_pUIRenderer)
		{
			s_pUIRenderer = new MDXUIRenderer(m_pDevice11, m_ScreenWidth, m_ScreenHeight);
			s_pUIRenderer->Create();
		}
		if (!s_pFontRenderer)
		{
			s_pFontRenderer = new FontRenderer(m_pDevice11, m_ScreenWidth, m_ScreenHeight);
			s_pFontRenderer->Create();
		}
	}
	virtual ~MDXUIWndManager()
	{
		WINDOW_ITER it = m_WindowList.begin();
		for (; it != m_WindowList.end(); ++it)
		{
			SAFEDELETE(*it);
		}
		m_WindowList.clear();

		SAFEDELETE(s_pFontRenderer);
		SAFEDELETE(s_pUIRenderer);
	}

	virtual LRESULT MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void SetCb(MDXUI_MESSAGECB cb)
	{
		// TODO: loop all the windows and update their CB
		m_pMessageCb = cb;
	}

	virtual void Render()
	{
		//		AutoBlendStateHolder bsh(m_pImmeDC);
		//		AutoDSStateHolder dssh(m_pImmeDC);
		//		AutoRasterStateHolder rsh(m_pImmeDC, m_pRasterState);
		//		SetAlphaBlendState(true);
		//		SetDepthState(false);

		WINDOW_ITER it = m_WindowList.begin();
		for (; it != m_WindowList.end(); ++it)
		{
			if ((*it) != m_pCurrWnd)
			{
				(*it)->Render();
			}
		}

		if (m_pCurrWnd)
		{
			m_pCurrWnd->Render();
		}
	}

	virtual MDXUIWindow* NewWindow(int x, int y, int w, int h, LPWSTR caption = nullptr, int id = 0);

	virtual MDXUIWindow* CreateDefaultWindow();

	virtual ID3D11Device* D3D11Device() { return m_pDevice11; }
	virtual ULONG ScreenWidth() { return m_ScreenWidth; }
	virtual ULONG ScreenHeight() { return m_ScreenHeight; }
	virtual MDXUIRenderer* UIRenderer() { return s_pUIRenderer; }
	virtual FontRenderer* UIFontRenderer() { return s_pFontRenderer; }

	// internal user for MDXUI
	virtual MDXUIControl* TraversallyFindControl(int id, MDXUIWindow* parent=nullptr);

protected:
	ID3D11Device* m_pDevice11;
	MDXUI_MESSAGECB m_pMessageCb;
	static FontRenderer *s_pFontRenderer;
	static MDXUIRenderer *s_pUIRenderer;
	ULONG m_ScreenWidth;
	ULONG m_ScreenHeight;
	std::vector<MDXUIWindow *> m_WindowList;
	MDXUIWindow *m_pCurrWnd;
	MDXUIWindow *m_pDefaultWnd;
};
