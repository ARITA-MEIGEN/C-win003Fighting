//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// �퓬BGM
	SOUND_LABEL_BGM_BATTLE001,			// �퓬BGM
	SOUND_LABEL_BGM_RESULT,			// �퓬BGM
	SOUND_LABEL_SE_LATK_HIT,			// ��
	SOUND_LABEL_SE_MATK_HIT,			// ��
	SOUND_LABEL_SE_HATK_HIT,			// ��
	SOUND_LABEL_SE_GUARD,				// ��
	SOUND_LABEL_SE_FIRE,				// ��SE
	SOUND_LABEL_SE_SPECIAL,				// �K�E�Z
	SOUND_LABEL_SE_START,				// �J�n
	SOUND_LABEL_SE_BACKSTEP,			// �o�b�N�X�e�b�v
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
