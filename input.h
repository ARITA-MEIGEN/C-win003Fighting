//=============================================================================
//
// 入力処理 [input.h]
// Author : KOZUNA HIROHITO
// 
//=============================================================================

#ifndef _INPUT_H_		//このマクロ定義がされなかったら
#define _INPUT_H_		//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "inputkeydata.h"
#include "DirectInput.h"

//----------------------------------------------------------------------------
//前方宣言
//----------------------------------------------------------------------------
class CInputKeyboard;
class CInputJoyPad;
class CInputMouse;

//----------------------------------------------------------------------------
//クラス定義
//----------------------------------------------------------------------------
class CInput
{
public:

	CInput();
	~CInput();

	//*アプリケーション処理に書くやつ
	static CInput *Create();							//入力処理系のクリエイト、Initの前に書く
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		//入力処理全部の初期化
	void Uninit();										//入力処理全部の終了処理
	void Update();										//入力処理全部の更新処理
														//*

														//*インプットが必要な時呼び出す
	static CInput *GetKey() { return m_pInput; }		//プレイやトリガーなどのアドレスの取得

	bool Press(STAN_DART_INPUT_KEY key);				//総合プレス
	bool Trigger(STAN_DART_INPUT_KEY key);				//総合トリガー
	bool Press(int nKey);								//キーボードプレス
	bool Trigger(int nkey);								//キーボードトリガー
	bool Release(int nkey);								//キーボードリリース
	bool Press(DirectJoypad key, int nNum = 0);			//ジョイパットプレス
	bool Trigger(DirectJoypad key, int nNum = 0);		//ジョイパットトリガー

	D3DXVECTOR3 VectorMoveKey(int nNum);								//十字キー式のベクトル取得
	D3DXVECTOR3 VectorMoveKeyAll();												//十字キー式のベクトル取得
	D3DXVECTOR3 VectorMoveJoyStick(int nNum = 0, bool bleftandright = false);	//ジョイスティックのベクトル取得
	D3DXVECTOR3 VectorMoveJoyStickAll(bool bleftandright = false); //ジョイスティックのベクトル取得

	InputType GetOldInputType() { return m_nOldInputType; }		//最後に触ったデバイス
		
private:

	CInputKeyboard *m_pKeyboard;	//キーボードの情報
	CInputJoyPad *m_pJoyPad;		//ジョイパッドの情報
	CInputMouse *m_pMouse;			//マウスの情報
	static CInput *m_pInput;		//このクラスの情報
	InputType m_nOldInputType;		//最後に触ったデバイス
};

#endif

