
#include "inc.h"
#include "mdxuimanager.h"

MDXUIRenderer *MDXUIWndManager::s_pUIRenderer = nullptr;
FontRenderer *MDXUIWndManager::s_pFontRenderer = nullptr;

MDXUIWindow* MDXUIWndManager::NewWindow(int x, int y, int w, int h, LPWSTR caption/* = nullptr*/, int id/* = 0*/)
{
	MDXUIWindow* wnd = new MDXUIWindow(this, x, y, w, h, caption, id, false);
	wnd->SetCb(m_pMessageCb);
	m_WindowList.push_back(wnd);

	return wnd;
}

MDXUIWindow* MDXUIWndManager::CreateDefaultWindow()
{
	MDXUIWindow* wnd = new MDXUIWindow(this, 0, 0, m_ScreenWidth, m_ScreenHeight, nullptr, 0, true);
	wnd->SetCb(m_pMessageCb);
	m_WindowList.push_back(wnd);
	m_pDefaultWnd = wnd;
	return wnd;
}

MDXUIControl* MDXUIWndManager::TraversallyFindControl(int id, MDXUIWindow* parent/*=nullptr*/)
{
	MDXUIWindow *wnd = (MDXUIWindow *)parent;
	MDXUIControl *ctl = nullptr;

	if (wnd)
	{
		return wnd->FindControl(id);
	}
	else
	{
		// check default window first
		ctl = m_pDefaultWnd->FindControl(id);
		if (ctl)
			return ctl;

		WINDOW_ITER it = m_WindowList.begin();
		for (; it != m_WindowList.end(); ++it)
		{
			if ((*it) != m_pDefaultWnd)
			{
				ctl = (*it)->FindControl(id);
				if (ctl)
					return ctl;
			}
		}
	}

	return ctl;
}

LRESULT MDXUIWndManager::MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_SIZE)
	{
		// should handled it here
		//return 1;
	}

	if (message == WM_LBUTTONDOWN)
	{
		int xPos = (int)(short)LOWORD(lParam);
		int yPos = (int)(short)HIWORD(lParam);

		// update m_pCurrWnd

		if (!m_pCurrWnd || !m_pCurrWnd->PtIn(xPos, yPos))
		{
			WINDOW_ITER it = m_WindowList.begin();
			for (; it != m_WindowList.end(); ++it)
			{
				if (*it == m_pDefaultWnd)
				{
					// default window should not be set as current window.
					continue;
				}
				if (*it != m_pCurrWnd)
				{
					if ((*it)->PtIn(xPos, yPos))
					{
						m_pCurrWnd = *it;
						break;
					}
				}
			}
			if (it == m_WindowList.end())
			{
				m_pCurrWnd = nullptr;
			}
		}
	}

	int bMsgHandled = 0; // no need further processing
	if (m_pCurrWnd)
	{
		bMsgHandled = m_pCurrWnd->MessageProc(hWnd, message, wParam, lParam);
		if (bMsgHandled)
			return 1;
	}
#if 1
	else
	{
		bMsgHandled = m_pDefaultWnd->MessageProc(hWnd, message, wParam, lParam);
		if (bMsgHandled)
			return 1;
	}
#else
	WINDOW_ITER it = m_WindowList.begin();
	for (; it != m_WindowList.end(); ++it)
	{
		if (*it != m_pCurrWnd)
		{
			bMsgHandled = (*it)->MessageProc(hWnd, message, wParam, lParam);
			if (bMsgHandled)
				return 1;
		}
	}
#endif
	return 0;
}
