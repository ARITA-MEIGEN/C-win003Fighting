//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// 戦闘BGM
	SOUND_LABEL_BGM_BATTLE001,			// 戦闘BGM
	SOUND_LABEL_BGM_RESULT,			// 戦闘BGM
	SOUND_LABEL_SE_LATK_HIT,			// 弱
	SOUND_LABEL_SE_MATK_HIT,			// 中
	SOUND_LABEL_SE_HATK_HIT,			// 強
	SOUND_LABEL_SE_GUARD,				// 強
	SOUND_LABEL_SE_FIRE,				// 炎SE
	SOUND_LABEL_SE_SPECIAL,				// 必殺技
	SOUND_LABEL_SE_START,				// 開始
	SOUND_LABEL_SE_BACKSTEP,			// バックステップ
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
