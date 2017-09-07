#pragma once

#include <d3d11.h>
#include "mdxuicontrol.h"
#include "mdxuimanager.h"


#define MDM_COMMAND		0x00000001

#define MDN_BN_CLICKED	0x00000001
#define MDN_CB_CHANGED	0x00000002


#define GLOBAL_RASTER_STATE		(!USE_LOCAL_RASTER_STATE)

class MDXUI
{
public:
	//static MDXUI *g_pMDXUI;
	//static int Start(ID3D11DeviceContext *dc, ULONG ww, ULONG hh, MDXUI_MESSAGECB cb = nullptr)
	//{
	//	g_pMDXUI = new MDXUI(dc, ww, hh, cb);
	//}
	MDXUI(ID3D11Device* dev, ULONG ww, ULONG hh, MDXUI_MESSAGECB cb = nullptr) :
		m_pDevice11(dev),m_pImmeDC(nullptr), m_ScreenWidth(ww), m_ScreenHeight(hh)/*, m_pMessageCb(cb)*/
	{
#if GLOBAL_RASTER_STATE
		if (m_pDevice11)
			m_pDevice11->GetImmediateContext(&m_pImmeDC);
		CreateRenderState();
#endif
		if (!s_pManager)
		{
			s_pManager = new MDXUIWndManager(m_pDevice11, m_ScreenWidth, m_ScreenHeight, cb);
		}
		m_pDefaultWnd = s_pManager->CreateDefaultWindow();
	}
	virtual ~MDXUI()
	{
#if GLOBAL_RASTER_STATE
		SAFERELEASE(m_pRasterSolidState);
		SAFERELEASE(m_pDepthDisabledState);
		SAFERELEASE(m_pABlendEnabledState);
#endif
		// SAFEDELETE(m_pDefaultWnd); // no need to destroy here, will released by Manager.
		SAFEDELETE(s_pManager);
		SAFERELEASE(m_pImmeDC);
	}
	virtual void Stop() {}
	virtual void Reset() {}

	virtual void SetMessageCallback(MDXUI_MESSAGECB cb) {/* m_pMessageCb = cb; */s_pManager->SetCb(cb); }

	virtual MDXUIControl* GetControl(int id, MDXUIWindow* parent=nullptr)
	{
		return s_pManager->TraversallyFindControl(id, parent);
	}

	virtual LRESULT MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual bool Resize();
	virtual MDXUIWindow* NewWindow(int x, int y, int w, int h, LPWSTR caption = nullptr)
	{
		return s_pManager->NewWindow(x, y, w, h, caption);
	}
	virtual void Render();

	virtual void CreateStatic(MDXUIWindow* UIWnd, int x, int y, LPWSTR caption = nullptr, ULONG id=0)
	{
		MDXUIWindow *wnd = UIWnd;
		if (!wnd)
			wnd = m_pDefaultWnd;
		wnd->CreateStatic(x, y, caption, id);
	}
	virtual MDXUIButton* CreateButton(MDXUIWindow* UIWnd, int x, int y, int w, int h, LPWSTR caption = nullptr, ULONG id = 0)
	{
		MDXUIWindow *wnd = UIWnd;
		if (!wnd)
			wnd = m_pDefaultWnd;
		return wnd->CreateButton(x, y, w, h, caption, id);
	}
	virtual MDXUISlider* CreateSlider(MDXUIWindow* UIWnd, int x, int y, int w, LPWSTR text = nullptr, ULONG id = 0)
	{
		MDXUIWindow *wnd = UIWnd;
		if (!wnd)
			wnd = m_pDefaultWnd;
		return wnd->CreateSlider(x, y, w, text, id);
	}
	virtual void CreateCheckbox(MDXUIWindow* UIWnd, int x, int y, LPWSTR caption = nullptr, ULONG id = 0)
	{
		MDXUIWindow *wnd = UIWnd;
		if (!wnd)
			wnd = m_pDefaultWnd;
		wnd->CreateCheckbox(x, y, caption, id);
	}
	virtual MDXUICombobox* CreateCombobox(MDXUIWindow* UIWnd, int x, int y, int w, LPWSTR text = nullptr, ULONG id = 0)
	{
		MDXUIWindow *wnd = UIWnd;
		if (!wnd)
			wnd = m_pDefaultWnd;
		return wnd->CreateCombobox(x, y, w, text, id);
	}

protected:
#if GLOBAL_RASTER_STATE
	virtual void CreateRenderState();
#if 0
	virtual void DisableDepthState()
	{
		//m_pImmeDC->OMSetDepthStencilState(enable ? m_pDepthEnabledState : m_pDepthDisabledState, 1);
		m_pImmeDC->OMSetDepthStencilState(m_pDepthDisabledState, 1);
	}

	virtual void EnableAlphaBlendState()
	{
		float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
		// Turn on/off the alpha blending.
		//m_pImmeDC->OMSetBlendState(enable ? m_pABlendEnabledState : m_pABlendDisabledState, blendFactor, 0xffffffff);
		m_pImmeDC->OMSetBlendState(m_pABlendEnabledState, blendFactor, 0xffffffff);
	}
#endif
#endif
	ID3D11DeviceContext *m_pImmeDC;
	ID3D11Device *m_pDevice11;
#if GLOBAL_RASTER_STATE
	ID3D11RasterizerState* m_pRasterSolidState;
	ID3D11BlendState *m_pABlendEnabledState;
	ID3D11DepthStencilState *m_pDepthDisabledState;
#endif
	static MDXUIWndManager *s_pManager;
	ULONG m_ScreenWidth;
	ULONG m_ScreenHeight;
	MDXUIWindow *m_pDefaultWnd;
};
