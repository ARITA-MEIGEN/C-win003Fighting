//=============================================================================
//
// レンダー
// Author : 有田明玄
//
//=============================================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_

//インクルード
#include"main.h"

//前方宣言
class CObject2D;
class CPlayer;

//マクロ定義
class CRenderer
{
public:



	//メンバ関数
	CRenderer();
	~CRenderer();
	 HRESULT Init(HWND hWnd, bool bWindow);
	 void Uninit();
	 void Update();
	 void Draw();
	 void DrawFPS();
	 LPDIRECT3DDEVICE9 GetDevice(void);/* {return g_pD3DDevice}*/

private:
	//メンバ変数
	LPDIRECT3D9 g_pD3D;								// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 g_pD3DDevice;					// Deviceオブジェクト
	LPD3DXFONT g_pFont;								// フォント

};

#endif // !_RENDERER_H_
