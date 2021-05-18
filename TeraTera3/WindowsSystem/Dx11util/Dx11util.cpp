
#include "DX11util.h"

CDirectXGraphics g_DXGrobj; // ＤＩＲＥＣＴＸ　ＧＲＡＰＨＩＣＳ　オブジェクト
							// 定数バッファ定義

CDirectXGraphics *GetDX11Obj()
{
	return &g_DXGrobj;
}

ID3D11Device *GetDX11Device()
{
	return g_DXGrobj.GetDXDevice();
}

ID3D11DeviceContext *GetDX11DeviceContext()
{
	return g_DXGrobj.GetImmediateContext();
}

bool DX11Init(HWND hwnd, int width, int height, bool fullscreen)
{
	bool sts;

	sts = g_DXGrobj.Init(hwnd, width, height, fullscreen);
	if (!sts)
	{
		MessageBox(hwnd, "DX11 init error", "error", MB_OK);
		return false;
	}
	return true;
}

void DX11Uninit()
{

	// 解放処理
	g_DXGrobj.Exit();
}

void DX11BeforeRender(float ClearColor[])
{
	// ターゲットバッファクリア
	g_DXGrobj.GetImmediateContext()->ClearRenderTargetView(g_DXGrobj.GetRenderTargetView(), ClearColor);
	// Zバッファ(深度バッファ)クリア
	g_DXGrobj.GetImmediateContext()->ClearDepthStencilView(g_DXGrobj.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX11AfterRender()
{
	// レンダリングされたイメージを表示。
	g_DXGrobj.GetSwapChain()->Present(0, 0);
}

int DX11GetScreenWidth()
{
	return g_DXGrobj.GetWidth();
}

int DX11GetScreenHeight()
{
	return g_DXGrobj.GetHeight();
}

void TurnOffZbuffer()
{
	g_DXGrobj.TurnOffZbuffer();
}

void TurnOnZbuffer()
{
	g_DXGrobj.TurnOnZBuffer();
}

void TurnOnAlphablend()
{
	g_DXGrobj.TurnOnAlphaBlending();
}

void TurnOffAlphablend()
{
	g_DXGrobj.TurnOffAlphaBlending();
}

void TurnWire()
{
	g_DXGrobj.TurnWire();
}

void TurnSolid()
{
	g_DXGrobj.TurnSolid();
}
