//=============================================================================
//
// �I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//�C���N���[�h
#include"main.h"
#include"Object2D.h"

//�O���錾

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



