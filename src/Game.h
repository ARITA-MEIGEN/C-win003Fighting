//=============================================================================
//
// �I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//�C���N���[�h
#include"Application.h"

#define MAX_PLAYER	(2)
#define STAGE_WIDTH	(350)	//��������[�܂ł̒���
#define END_TIMER	(120)	//�v���C���[������ł���I���܂ł̎���


//�O���錾
class CPlayer;
class CLife;				//�̗̓Q�[�W
class CCamera;
class CLight;
class CFloor;
class CTimer;
class CUI;

class CGame
{
public:
	enum GAME
	{
		GAME_NONE = 0,
		GAME_START,
		GAME_ROUND1,
		GAME_ROUND2,
		GAME_END
	};

	CGame();
	~CGame();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	//�Z�b�^�[
	static void SetGame(GAME gamestate);

	//�Q�b�^�[
	static GAME GetGame();
	static CCamera * GetCamera();
	static CFloor*GetFloor();
	static CLight*GetLight();
	static bool GetDebugCamera() { return bDebugCamera; };
	static CPlayer*GetPlayer(int number) { return m_pPlayer[number]; };
	static CTimer*GetTimer() { return m_pTimer; };

private:
	static	CPlayer*m_pPlayer[MAX_PLAYER];
	static	CLife*m_Life;				//�̗̓Q�[�W
	static	GAME m_gamestate;			//�Q�[���̏��
	static	CCamera*m_pCamera;
	static	CLight*m_pLight;
	static	CFloor*m_pFloor;
	static  bool bDebugCamera;
	static	CTimer*m_pTimer;
	static	CUI*m_pUI;
	int		m_Timer;

};

#endif