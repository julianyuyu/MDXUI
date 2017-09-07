#include "inc.h"

#include "mdxui.h"
#include "mdxuishader.h"
#include "creator.h"

MDXUIWndManager *MDXUI::s_pManager = nullptr;

LRESULT MDXUI::MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return s_pManager->MessageProc(hWnd, message, wParam, lParam);
}

bool MDXUI::Resize()
{
	return true;
}

#if GLOBAL_RASTER_STATE
void MDXUI::CreateRenderState()
{
	StateCreator stctr(m_pDevice11);
	m_pRasterSolidState = stctr.CreateRasterState(D3D11_FILL_SOLID);
	m_pDepthDisabledState = stctr.CreateDepthStencilState(false, true);
	m_pABlendEnabledState = stctr.CreateBlendState(0, true, D3D11_BLEND_SRC_ALPHA);
}
#endif

void MDXUI::Render()
{
#if GLOBAL_RASTER_STATE
	AutoBlendStateHolder  bsh(m_pImmeDC, m_pABlendEnabledState);
	AutoDSStateHolder	  dsh(m_pImmeDC, m_pDepthDisabledState);
	AutoRasterStateHolder rsh(m_pImmeDC, m_pRasterSolidState);
#if 0
	EnableAlphaBlendState();
	DisableDepthState();
#endif
#endif
	// render each window in window Manager
	s_pManager->Render();
}
