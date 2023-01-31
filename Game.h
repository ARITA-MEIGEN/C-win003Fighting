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

#define MAX_PLAYER (2)

//�O���錾
class CPlayer;
class CLife;				//�̗̓Q�[�W


class CCamera;
class CLight;
class CFloor;
class CMesh;

class CGame
{
public:
	enum GAME
	{
		GAME_NONE = 0,
		GAME_NORMAL,
		GAME_END
	};

	CGame();
	~CGame();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static void SetGame(GAME gamestate);

	//�Q�b�^�[
	static GAME GetGame();
	static CCamera * GetCamera();
	static CFloor*GetFloor();
	static CMesh*GetMesh();
	static CLight*GetLight();
	static bool GetDebugCamera() { return bDebugCamera; };
	static CPlayer*GetPlayer(int number) { return m_pPlayer[number]; };

private:
	static CPlayer*m_pPlayer[MAX_PLAYER];
	static CLife*m_Life;				//�̗̓Q�[�W
	static GAME m_gamestate;			//�Q�[���̏��

	static CCamera*m_pCamera;
	static CLight*m_pLight;
	static CFloor*m_pFloor;
	static CMesh*m_pMesh;
	static  bool bDebugCamera;

};

#endif