//=================================================
// Content     (キーボードの設定)(inputKeyBoard.cpp)
// Author     : 有田明玄
//=================================================
#include "InputKeyBoard.h"

//マクロ定義
#define NUM_KEY_MAX		(256)	//キーの最大数

//静的メンバ変数宣言
 LPDIRECTINPUT8 CInputKeyboard::m_pInput;						//DirectInputオブジェクトへのポインタ

//==========================================
//キーボードのコンストラクタ
//==========================================
CInputKeyboard::CInputKeyboard()
{

}

//==========================================
//キーボードのデストラクタ
//==========================================
CInputKeyboard::~CInputKeyboard()
{

}

//==========================================
//キーボードの初期化設定
//==========================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)& m_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}


	//キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//==========================================
//キーボードの終了処理
//==========================================
void CInputKeyboard::Uninit(void)
{
	//入力デバイス(キーボード)の破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();		//キーボードへのアクセス権を放棄
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//==========================================
//キーボードの更新処理
//==========================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//キーボードの入力情報
	int nCntKey;
	//入力デバイスからデータを獲得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			//キーのトリガー情報保存
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];						
			//キーのリリース情報保存
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&m_aKeyState[nCntKey];
			//キーのプレス情報保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];																			
		}
	}
	else
	{
		m_pDevice->Acquire();	//キーのアクセス権獲得
	}
}

//==========================================
//キーボードプレス情報を取得
//==========================================
bool  CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}
//=========================================
//キーボードトリガー情報を取得
//=========================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=========================================
//キーボードリリース情報を取得
//=========================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

////=========================================
////キーボードリピート情報を取得
////=========================================
//bool CInputKeyboard::GetRepeat(int nKey)
//{
//	return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false;
//}