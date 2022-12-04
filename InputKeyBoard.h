#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_	//二重インクルード防止用

#include "main.h"
//マクロ定義
#define NUM_KEY_MAX		(256)					//キーの最大数

class CInputKeyboard
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化
	void Uninit();									//終了
	void Update();									//更新
	bool GetPress(int nKey);						//キーボードプレス情報を取得
	bool GetTrigger(int nKey);						//キーボードトリガー情報を取得
	bool GetRelease(int nKey);						//キーボードリリース情報を取得

private:
	//メンバ変数
	BYTE m_aKeyState[NUM_KEY_MAX];					//キーボードのプレス情報
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];			//キーボードのトリガー情報
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];			//キーボードのリリース情報
	BYTE m_aKeyStateRepeat[NUM_KEY_MAX];			//キーボードのリピート情報

protected:
	//メンバ変数
	static LPDIRECTINPUT8 m_pInput;						//DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice;						//入力デバイス(キーボード)へのポインタ
};

#endif