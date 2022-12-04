//=============================================================================
//
// �A�v���P�[�V����
// Author : �L�c����
//
//=============================================================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include"main.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CRenderer;
class CInputKeyboard;
class CInputJoyPad;
class CDebugProc;
class CTitle;
class CGame;
class CResult;
class CFade;

//�}�N����`
#define SCREEN_WIDTH	(1280)		// �X�N���[���̕�
#define SCREEN_HEIGHT	(720)		// �X�N���[���̍���

// �v���g�^�C�v�錾
class CApplication
{
public:
	enum MODE
	{//�Q�[�����[�h
		MODE_TITLE = 0,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX
	};

	//�����o�֐�
	CApplication();
	~CApplication();
	HRESULT Init(HWND hWnd, bool bWindow,HINSTANCE hInstance);
	void Uninit();
	void Update();
	void Draw();
	//�Q�b�^�[
	static CRenderer*GetRenderer();
	static CInputKeyboard*GetInputKeyboard();
	static CInputJoyPad*GetInputJoypad();
	static CDebugProc * GetDebugProc();
	static CFade*GetFade();
	//void DrawStatas();
	static void SetMode(MODE mode);


private:
	//�����o�ϐ�
	static CRenderer*m_pRenderer;
	static CInputKeyboard*m_pInputKeyboard;
	static CInputJoyPad*m_pInputJoyPad;
	static CDebugProc*m_pDebugProc;
	LPD3DXFONT m_pFont;						//�t�H���g�ւ̃|�C���g
	static MODE m_mode;
	static CFade* m_pFade;

	//���
	static CTitle*m_pTitle;
	static CGame*m_pGame;
	static CResult*m_pResult;
};

#endif // !_APPLICATION_H_