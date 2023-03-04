//=============================================================================
//
// オブジェクト
// Author : 有田明玄
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//インクルード
#include"main.h"
#include"Object2D.h"

//前方宣言

class CUI
{
public:
	explicit CUI(int nPriority = 0);
	~CUI();

	HRESULT Init();
	void Uninit();
	void Update();
	static CUI*Create();

	int GetTimer() { return m_timer; };

private:
	static CObject2D*m_pStart[2];
	int m_timer;
};

#endif // !_OBJECT_H_



