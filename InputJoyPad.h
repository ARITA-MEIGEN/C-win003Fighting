#ifndef _INPUTJOYPAD_H_
#define _INPUTJOYPAD_H_	//二重インクルード防止用

#include "main.h"

//マクロ定義
#define NUM_KEY_MAX		(256)					//キーの最大数
#define DEADZONE		(20000.0f)				//スティックの認識する量


class CInputJoyPad
{
public:
	//キーの種類
	typedef enum
	{
		JOYKEY_UP = 0,	//十字キー(上)
		JOYKEY_DOWN,	//十字キー(下)
		JOYKEY_LEFT,	//十字キー(左)
		JOYKEY_RIGHT,	//十字キー(右)
		JOYKEY_START,	//スタートボタン
		JOYKEY_BACK,	//バックボタン
		JOYKEY_LPUSH,	//Lスティック押し込み
		JOYKEY_RPUSH,	//Rスティック押し込み
		JOYKEY_LB,		//Lボタン
		JOYKEY_RB,		//Rボタン
		JOYKEY_NONE1,	//反応なし
		JOYKEY_NONE2,	//反応なし
		JOYKEY_A,		//Aボタン
		JOYKEY_B,		//Bボタン
		JOYKEY_X,		//Xボタン
		JOYKEY_Y,		//Yボタン
		JOYKEY_LSTICK,	//左スティック
		JOYKEY_RSTICK,	//右スティック
	}JOYKEY;

	CInputJoyPad();
	~CInputJoyPad();
	HRESULT Init();	//初期化
	void Uninit();									//終了
	void Update();									//更新

	bool GetJoypadPress(JOYKEY key);						//キーボードプレス情報を取得
	bool GetJoypadTrigger(JOYKEY key);						//キーボードトリガー情報を取得
	bool GetJoypadRelease(JOYKEY key);						//キーボードリリース情報を取得
	D3DXVECTOR3 GetJoypadStick(JOYKEY key);					//ジョイパッドのスティック情報
	void Joypadvibration(int nTime, int nStrength);			//ジョイパッドの振動設定

private:
	//メンバ変数
	XINPUT_STATE m_joyKeyState;						//ジョイパッドのプレス情報
	XINPUT_STATE m_joyKeyStateTrigger;				//ジョイパッドのトリガー情報
	XINPUT_STATE m_joyKeyStateRelease;				//ジョイパッドのリリース情報
	D3DXVECTOR3 m_JoyStickPos;						//ジョイスティック
	XINPUT_VIBRATION m_joyMoter;					//振動の設定
	int m_nTime;									//振動の時間
	int m_nStrength;								//振動の強さ
};

#endif
