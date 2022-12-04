//=============================================================================
//
// アプリケーション
// Author : 有田明玄
//
//=============================================================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include"main.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CRenderer;
class CInputKeyboard;
class CInputJoyPad;
class CDebugProc;
class CTitle;
class CGame;
class CResult;
class CFade;

//マクロ定義
#define SCREEN_WIDTH	(1280)		// スクリーンの幅
#define SCREEN_HEIGHT	(720)		// スクリーンの高さ

// プロトタイプ宣言
class CApplication
{
public:
	enum MODE
	{//ゲームモード
		MODE_TITLE = 0,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX
	};

	//メンバ関数
	CApplication();
	~CApplication();
	HRESULT Init(HWND hWnd, bool bWindow,HINSTANCE hInstance);
	void Uninit();
	void Update();
	void Draw();
	//ゲッター
	static CRenderer*GetRenderer();
	static CInputKeyboard*GetInputKeyboard();
	static CInputJoyPad*GetInputJoypad();
	static CDebugProc * GetDebugProc();
	static CFade*GetFade();
	//void DrawStatas();
	static void SetMode(MODE mode);


private:
	//メンバ変数
	static CRenderer*m_pRenderer;
	static CInputKeyboard*m_pInputKeyboard;
	static CInputJoyPad*m_pInputJoyPad;
	static CDebugProc*m_pDebugProc;
	LPD3DXFONT m_pFont;						//フォントへのポイント
	static MODE m_mode;
	static CFade* m_pFade;

	//画面
	static CTitle*m_pTitle;
	static CGame*m_pGame;
	static CResult*m_pResult;
};

#endif // !_APPLICATION_H_
