//=============================================================================
//
// デバッグ表示
// Author : 有田明玄
//
//=============================================================================

#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

//インクルード
#include"main.h"

#define MAX_DBTEXT	(1024)

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CApplication;

class CDebugProc
{
public:
	CDebugProc();
	~CDebugProc();
	void Init();
	void Uninit();
	static void Print(const char *pFormat, /*可変引数*/...);
	static void Draw();
private:
	static LPD3DXFONT m_pFont;
	static char m_aStr[MAX_DBTEXT];	//配列サイズ超えないように気を付けて　std::string使ってもいい

};

#endif

