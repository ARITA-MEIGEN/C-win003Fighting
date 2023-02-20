//=============================================================================
//
// メイン
// Author : 有田明玄
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_						//多重インクルード防止
#define DIRECTINPUT_VERSION     0x0800	// DirectInputのバージョン指定

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "dinput.h"			//入力処理に必要



//*****************************************************************************
// ライブラリーリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dInput8.lib")		//入力処理に必要
#pragma comment (lib, "xinput.lib")		//Xinput処理に必要

//前方宣言
class CApplication;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int GetFPS(void);


#endif // !_MAIN_H_


