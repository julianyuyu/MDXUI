#include <windows.h>
#include <d3d11.h>
#include "resource.h"

#include "inc.h"
#include "mdxui.h"

#pragma comment(lib, "mdxui.lib")

#pragma comment(linker, \
	"\"/manifestdependency:type='Win32' "\
	"name='Microsoft.Windows.Common-Controls' "\
	"version='6.0.0.0' "\
	"processorArchitecture='*' "\
	"publicKeyToken='6595b64144ccf1df' "\
	"language='*'\"")

#define MAX_LOADSTRING 100
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
ID3D11Device*           g_pd3dDevice = NULL;
ID3D11DeviceContext*    g_pImmediateContext = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;
ID3D11DepthStencilView *g_pDepthStencilView = nullptr;

MDXUI *g_MDXUI = nullptr;

HRESULT InitDevice();
void CleanupDevice();
void CreateMDXUI(ID3D11Device* dev, int width, int height);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
void Render();
void Resize(bool fullscreen);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MDXUISAMPLE, szWindowClass, MAX_LOADSTRING);

	g_hInst = hInstance;
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MDXUISAMPLE));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_MDXUISAMPLE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MDXUISAMPLE));
	RegisterClassEx(&wcex);

	g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if (!g_hWnd)
		return 0;

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MDXUISAMPLE));

    if (FAILED(InitDevice()))
    {
        CleanupDevice();
        return 0;
    }

    MSG msg = {};
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
        else
        {
            Render();
        }
    }

    CleanupDevice();
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int i = 0;
    PAINTSTRUCT ps;
    HDC hdc;
	if (g_MDXUI && g_MDXUI->MessageProc(hWnd, message, wParam, lParam))
		return 0;

	static bool fullscreen = false;
    switch(message)
	{
	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_F11:
			Resize(!fullscreen);
			break;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SIZE:
		Resize(false);
		break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;
	D3D_FEATURE_LEVEL realLevel;
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

    hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels),
                                       D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &realLevel, &g_pImmediateContext);

    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
        return hr;

    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView);
    pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	TextureCreator tctr(g_pd3dDevice);
	g_pDepthStencilView = tctr.CreateDepthStencil(nullptr,width, height);
    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView);

	D3D11_VIEWPORT vp = {};
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports(1, &vp);

	CreateMDXUI(g_pd3dDevice, width, height);

    return S_OK;
}

#define ID_ADAPTER_BUTTON	100
#define ID_ADAPTER_CHKBOX	101
#define ID_ADAPTER_COMBOX	102
#define ID_ADAPTER_SLIDER	103
#define ID_BUTTON	201
#define ID_COMBOX	202
#define ID_CHKBOX	203
#define ID_SLIDER	204
#define ID_MISC_BUTTON	301
#define ID_MISC_CHKBOX	302
#define ID_MISC_SLIDER	303

ULONG CALLBACK EventCb(UINT message, UINT ControlId, ULONG_PTR wParam, ULONG_PTR lParam)
{
	switch (ControlId)
	{
	case ID_BUTTON:
		MessageBoxW(nullptr, L"button", L"info", MB_OK);
		break;
	case ID_COMBOX:
		if (wParam == MDN_CB_CHANGED)
			MessageBoxW(nullptr, L"combobox", L"info 2", MB_OK);
		break;
	case ID_CHKBOX:
			if (wParam == MDN_BN_CLICKED)
				//if ()
				MessageBoxW(nullptr, L"checkbox", L"info 3", MB_OK);
			break;
	case 3:
		break;
	}
	return 0;
}

void CreateMDXUI(ID3D11Device* dev, int width, int height)
{
	SAFEDELETE(g_MDXUI);
	g_MDXUI = new MDXUI(dev, width, height, EventCb);
	
	g_MDXUI->CreateStatic(nullptr, 100, 20, L"中文 Static 1, 相对坐标(100,100)");
	g_MDXUI->CreateButton(nullptr, 100, 100, 160, 40, L"中文按钮 Button 1", ID_BUTTON);
	g_MDXUI->CreateSlider(nullptr, 100, 300, 160, L"中文 Slider 1", ID_SLIDER);
	g_MDXUI->CreateCheckbox(nullptr, 100, 200, L"中文 CheckBox 1", ID_CHKBOX);
	MDXUICombobox *cmb0 = g_MDXUI->CreateCombobox(nullptr, 300, 100, 160, L"中文 ComboBox 1", ID_COMBOX);
	cmb0->AddString(L"item 1");
	cmb0->AddString(L"item 2");
	cmb0->AddString(L"item 3");

	MDXUIWindow *wnd = g_MDXUI->NewWindow(500, 60, 400, 260, L"MDXUI 中文窗口 Window 1");
	wnd->CreateStatic(20, 20, L"MDXUI Static");
	wnd->CreateButton(20, 50, 100, 30, L"Push Button", ID_ADAPTER_BUTTON);
	wnd->CreateSlider(20, 150, 160, L"Slider 比如调整亮度", ID_ADAPTER_SLIDER);
	wnd->CreateCheckbox(20, 100, L"CheckBox 2 in 窗口 1", ID_ADAPTER_CHKBOX);
	//wnd->CreateStatic(20, 300, L"中文static3");
	MDXUICombobox *cmb = wnd->CreateCombobox(180, 50, 180, L"中文 ComboBox 2", ID_ADAPTER_COMBOX);
	cmb->AddString(L"中文 item 1 in 窗口 1");
	cmb->AddString(L"中文 item 2 in 窗口 1");
	cmb->AddString(L"中文 item 3 in 窗口 1");
	cmb->AddString(L"中文 item 4 in 窗口 1");
	cmb->AddString(L"中文 item 5 in 窗口 1");

	MDXUIWindow *wnd2 = g_MDXUI->NewWindow(500, 380, 300, 200, L"MDXUI 中文窗口 Window 2");
	wnd2->CreateStatic(20, 20, L"中文 Static 3");
	wnd2->CreateButton(20, 60, 160, 30, L"中文按钮 Button 3", ID_MISC_BUTTON);
	wnd2->CreateCheckbox(20, 120, L"中文 CheckBox 3", ID_MISC_CHKBOX);
	wnd2->CreateSlider(20, 160, 160, L"中文 Slider 3", ID_MISC_SLIDER);

}

void Render()
{
    // Just clear the backbuffer
    float ClearColor[4] = { 0.0f, 0.3f, 0.6f, 1.0f }; //red,green,blue,alpha
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	g_MDXUI->Render();

    g_pSwapChain->Present(0, 0);
}

void Resize(bool fullscreen)
{
	if (!g_pd3dDevice || !g_pSwapChain)
	{
		return;
	}
	SAFEDELETE(g_MDXUI);
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	static int ww = 0;
	static int hh = 0;
	if (!fullscreen)
	{
		ww = w;
		hh = h;
	}
	if (fullscreen)
	{
		w = GetSystemMetrics(SM_CXSCREEN);
		h = GetSystemMetrics(SM_CYSCREEN);
	}

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = w;
	sd.BufferDesc.Height = h;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = fullscreen;

	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;
	UINT Flags = 0;
	if (fullscreen)
	{
		Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		g_pSwapChain->SetFullscreenState(fullscreen, nullptr);
	}

	SAFERELEASE(g_pRenderTargetView);
	SAFERELEASE(g_pDepthStencilView);
	g_pSwapChain->ResizeBuffers(sd.BufferCount, w, h, sd.BufferDesc.Format, Flags);
	ID3D11Texture2D* pBackBuffer = NULL;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	SAFERELEASE(pBackBuffer);

	TextureCreator tctr(g_pd3dDevice);
	g_pDepthStencilView = tctr.CreateDepthStencil(nullptr, w, h);
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)w;
	vp.Height = (FLOAT)h;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	CreateMDXUI(g_pd3dDevice, w, h);
}

void CleanupDevice()
{
	SAFEDELETE(g_MDXUI);

    if(g_pImmediateContext)
		g_pImmediateContext->ClearState();

	SAFERELEASE(g_pRenderTargetView);
	SAFERELEASE(g_pSwapChain);
	SAFERELEASE(g_pImmediateContext);
	SAFERELEASE(g_pd3dDevice);
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
