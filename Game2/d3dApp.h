//=======================================================================================
// d3dApp.h by Frank Luna (C) 2008 All Rights Reserved.
//
// Simple Direct3D demo application class.  
// Make sure you link: d3d10.lib d3dx10d.lib dxerr.lib dxguid.lib.
// Link d3dx10.lib for release mode builds instead of d3dx10d.lib.
//=======================================================================================


#ifndef D3DAPP_H
#define D3DAPP_H


#include "d3dUtil.h"
#include "GameTimer.h"
#include "input.h"
#include "graphics.h"
#include "Text.h"
#include "audio.h"
#include <WinError.h>
#include <string>
#include <sstream>



class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();

	HINSTANCE getAppInst();
	HWND      getMainWnd();

	int run();

	// Framework methods.  Derived client class overrides these methods to 
	// implement specific application requirements.

	virtual void initApp();
	virtual void onResize();// reset projection/etc
	virtual void updateScene(float dt);
	virtual void drawScene(); 
	// Return pointer to Audio.
    Audio* getAudio()       {return audio;}
	virtual LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	void setCursorShow(bool show);

	POINT MousePos;
	POINT PlayerPos;

protected:
	void initMainWindow();
	void initDirect3D();

	void togglePause();
	void toggleMouseCapture();

	bool mouseCaptured;

	HCURSOR normalCursor;
	
protected:

	HINSTANCE mhAppInst;
	HWND      mhMainWnd;
	HRESULT   hr;
	bool      mAppPaused;
	bool      mMinimized;
	bool      mMaximized;
	bool      mResizing;

	bool showCursor;

	GameTimer mTimer;
	Input* input;
	Audio* audio;
	Text timesNew;
	Text arial;
	Text roboto;

	std::wstring mFrameStats;
 
	ID3D10Device*    md3dDevice;
	IDXGISwapChain*  mSwapChain;
	ID3D10Texture2D* mDepthStencilBuffer;
	ID3D10RenderTargetView* mRenderTargetView;
	ID3D10DepthStencilView* mDepthStencilView;
	ID3DX10Font* mFont;

	// Derived class should set these in derived constructor to customize starting values.
	std::wstring mMainWndCaption;
	D3D10_DRIVER_TYPE md3dDriverType;
	D3DXCOLOR mClearColor;
	int mClientWidth;
	int mClientHeight;

	RECT ScreenRect;
	RECT MouseRect;

	

	HWND DesktopWindow;
	HWND MyWindow;

};




#endif // D3DAPP_H