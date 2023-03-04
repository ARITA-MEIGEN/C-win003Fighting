//=============================================================================
//
// オブジェクト
// Author : 有田明玄
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//インクルード
#include"main.h"
#include"Application.h"
#include"Object2D.h"

//前方宣言

class CResult
{
public:
	CResult();
	~CResult();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	//ゲッター
	static CObject2D*GetBg() { return m_pBg; };
private:
	static CObject2D*m_pBg;
};

#endif // !_OBJECT_H_



