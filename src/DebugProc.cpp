//=============================================================================
//
// 3Dオブジェクト
// Author : 有田明玄
//
//=============================================================================

//インクルード
#include"DebugProc.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include"Application.h"
#include"renderer.h"

//静的メンバ変数
LPD3DXFONT CDebugProc::m_pFont = nullptr;
char CDebugProc::m_aStr[1024] = {};
//=============================================================================
// コンストラクタ
//=============================================================================
CDebugProc::CDebugProc()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CDebugProc::~CDebugProc()
{

}

//=============================================================================
// 初期化
//=============================================================================
void CDebugProc::Init()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);
}

//=============================================================================
// 終了
//=============================================================================
void CDebugProc::Uninit()
{

}

//=============================================================================
// 文字列読み込み
//=============================================================================
void CDebugProc::Print(const char * pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);
	char aStr[128] = {};
	for (const char* p = pFormat; *p != '\0'; ++p)
	{
		if (*p == '%')
		{
			switch (*++p)
			{
			case 'd':
				sprintf(&aStr[0], "%d ", va_arg(args, int));
				break;
			case 'f':
				sprintf(&aStr[0], "%f ", va_arg(args, double));
				break;
			case 'c':
				sprintf(&aStr[0], "%c ", va_arg(args, char));
				break;
			case 's':
				sprintf(&aStr[0], "%s ", va_arg(args, const char*));
				break;
			default:
				assert(!"不正な変換指定");
				break;
			}
			strcat(m_aStr, aStr);
		}
		else
		{
			sprintf(&aStr[0],"%c", *p);
			strcat(m_aStr, aStr);
		}
	}
	strcat(m_aStr,"\n");

	va_end(args);
}

//=============================================================================
// 描画
//=============================================================================
void CDebugProc::Draw()
{
#ifdef _DEBUG


	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	D3DXCOLOR col = {1.0f,0.0f,0.0f,1.0f};

	// テキスト描画
	m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, col);

	//テキストリセット
	m_aStr[0] = {};
#endif // _DEBUG

}
