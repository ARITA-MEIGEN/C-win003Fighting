//=============================================================================
//
// �I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//�C���N���[�h
#include"main.h"
#include"Application.h"
#include"Object2D.h"

//�O���錾

class CResult
{
public:
	CResult();
	~CResult();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	//�Q�b�^�[
	static CObject2D*GetBg() { return m_pBg; };
private:
	static CObject2D*m_pBg;
};

#endif // !_OBJECT_H_



