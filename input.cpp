//=============================================================================
//
// 入力処理 [input.cpp]
// Author1 : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "input.h"
#include "DirectInput.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"

//-----------------------------------------------------------------------------
//静的メンバ変数宣言
//-----------------------------------------------------------------------------
CInput *CInput::m_pInput = nullptr;//このクラスの情報

//*************************************************************************************
//コンストラクタ
//*************************************************************************************
CInput::CInput()
{
	m_pKeyboard = nullptr;		//キーボードの情報
	m_pJoyPad = nullptr;		//ジョイパッドの情報
	m_pMouse = nullptr;			//マウスの情報
	m_nOldInputType = INPUT_TYPE_KEYBOARD;
}

//*************************************************************************************
//デストラクタ
//*************************************************************************************
CInput::~CInput()
{
}

//*************************************************************************************
//初期化
//*************************************************************************************
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(CDirectInput::Create(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//キーボードの生成
	m_pKeyboard = new CInputKeyboard;

	//キーボードの初期化処理
	if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ジョイパッドの生成
	m_pJoyPad = new CInputJoyPad;

	//ジョイパッドの初期化処理
	if (FAILED(m_pJoyPad->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//*************************************************************************************
//終了処理
//*************************************************************************************
void CInput::Uninit()
{
	//キーボードの破棄
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	//ジョイパッドの破棄
	if (m_pJoyPad != nullptr)
	{
		m_pJoyPad->Uninit();
		delete m_pJoyPad;
		m_pJoyPad = nullptr;
	}

	//DirectInputオブジェクトの破棄
	CDirectInput::Break();

	//自己破棄
	if (m_pInput != nullptr)
	{
		delete m_pInput;
		m_pInput = nullptr;
	}
}

//*************************************************************************************
//更新処理
//*************************************************************************************
void CInput::Update()
{
	//キーボードの更新
	m_pKeyboard->Update();
	//ジョイパッドの更新
	m_pJoyPad->Update();

	//最後に触ったデバイス
	if (m_pJoyPad->GetPressAll())
	{
		m_nOldInputType = INPUT_TYPE_JOYPAD;
	}
	else if (m_pKeyboard->GetAllPress())
	{
		m_nOldInputType = INPUT_TYPE_KEYBOARD;
	}
}

//*************************************************************************************
//インプットの生成
//*************************************************************************************
CInput *CInput::Create()
{
	//Inputの自己生成
	m_pInput = new CInput;
	return m_pInput;
}

//*************************************************************************************
//プレス処理(総合)
//*************************************************************************************
bool CInput::Press(STAN_DART_INPUT_KEY key)
{
	switch (key)
	{
	case KEY_UP:
		if (m_pKeyboard->GetPress(DIK_W)
			|| m_pKeyboard->GetPress(DIK_UP)
			|| m_pJoyPad->GetPressAll(JOYPAD_UP))
		{
			return true;
		}
		break;
	case KEY_UP_LEFT:
		if ((m_pKeyboard->GetPress(DIK_W) && m_pKeyboard->GetPress(DIK_A))
			|| (m_pKeyboard->GetPress(DIK_UP) && m_pKeyboard->GetPress(DIK_LEFT))
			|| m_pJoyPad->GetPressAll(JOYPAD_UP_LEFT))
		{
			return true;
		}
		break;
	case KEY_UP_RIGHT:
		if ((m_pKeyboard->GetPress(DIK_W) && m_pKeyboard->GetPress(DIK_D))
			|| (m_pKeyboard->GetPress(DIK_UP) && m_pKeyboard->GetPress(DIK_RIGHT))
			|| m_pJoyPad->GetPressAll(JOYPAD_UP_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DOWN:
		if (m_pKeyboard->GetPress(DIK_S)
			|| m_pKeyboard->GetPress(DIK_DOWN)
			|| m_pJoyPad->GetPressAll(JOYPAD_DOWN))
		{
			return true;
		}
		break;
	case KEY_DOWN_LEFT:
		if ((m_pKeyboard->GetPress(DIK_S) && m_pKeyboard->GetPress(DIK_A))
			|| (m_pKeyboard->GetPress(DIK_DOWN) && m_pKeyboard->GetPress(DIK_LEFT))
			|| m_pJoyPad->GetPressAll(JOYPAD_DOWN_LEFT))
		{
			return true;
		}
		break;
	case KEY_DOWN_RIGHT:
		if ((m_pKeyboard->GetPress(DIK_S) && m_pKeyboard->GetPress(DIK_D))
			|| (m_pKeyboard->GetPress(DIK_DOWN) && m_pKeyboard->GetPress(DIK_RIGHT))
			|| m_pJoyPad->GetPressAll(JOYPAD_DOWN_RIGHT))
		{
			return true;
		}
		break;
	case KEY_LEFT:
		if (m_pKeyboard->GetPress(DIK_A)
			|| m_pKeyboard->GetPress(DIK_LEFT)
			|| m_pJoyPad->GetPressAll(JOYPAD_LEFT))
		{
			return true;
		}
		break;
	case KEY_RIGHT:
		if (m_pKeyboard->GetPress(DIK_D)
			|| m_pKeyboard->GetPress(DIK_RIGHT)
			|| m_pJoyPad->GetPressAll(JOYPAD_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DECISION:
		if (m_pKeyboard->GetPress(DIK_RETURN)
			|| m_pJoyPad->GetPressAll(JOYPAD_A))
		{
			return true;
		}
		break;
	case KEY_SHOT:
		if (m_pKeyboard->GetPress(DIK_SPACE)
			|| m_pJoyPad->GetPressAll(JOYPAD_R1))
		{
			return true;
		}
		break;
	case KEY_BACK:
		if (m_pKeyboard->GetPress(DIK_BACKSPACE)
			|| m_pKeyboard->GetPress(DIK_B)
			|| m_pJoyPad->GetPressAll(JOYPAD_BACK)
			|| m_pJoyPad->GetPressAll(JOYPAD_B))
		{
			return true;
		}
		break;
	case KEY_SHIFT:
		if (m_pKeyboard->GetPress(DIK_RSHIFT)
			|| m_pKeyboard->GetPress(DIK_LSHIFT)
			|| m_pJoyPad->GetPressAll(JOYPAD_L1))
		{
			return true;
		}
		break;
	case KEY_MOVE:
		if (Press(KEY_UP) || Press(KEY_DOWN) || Press(KEY_LEFT) || Press(KEY_RIGHT))
		{
			return true;
		}
		break;
	case KEY_PAUSE:
		if (m_pKeyboard->GetPress(DIK_P)
			|| m_pJoyPad->GetPressAll(JOYPAD_START))
		{
			return true;
		}
		break;
	case KEY_ALL:
		if (m_pKeyboard->GetAllPress()
			|| m_pJoyPad->GetPressAll())
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

//*************************************************************************************
//トリガー処理(総合)
//*************************************************************************************
bool CInput::Trigger(STAN_DART_INPUT_KEY key)
{
	switch (key)
	{
	case KEY_UP:
		if (m_pKeyboard->GetTrigger(DIK_W)
			|| m_pKeyboard->GetTrigger(DIK_UP)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_UP))
		{
			return true;
		}
		break;
	case KEY_UP_LEFT:
		if ((m_pKeyboard->GetTrigger(DIK_W) && m_pKeyboard->GetTrigger(DIK_A))
			|| (m_pKeyboard->GetTrigger(DIK_UP) && m_pKeyboard->GetTrigger(DIK_LEFT))
			|| m_pJoyPad->GetTriggerAll(JOYPAD_UP_LEFT))
		{
			return true;
		}
		break;
	case KEY_UP_RIGHT:
		if ((m_pKeyboard->GetTrigger(DIK_W) && m_pKeyboard->GetTrigger(DIK_D))
			|| (m_pKeyboard->GetTrigger(DIK_UP) && m_pKeyboard->GetTrigger(DIK_RIGHT))
			|| m_pJoyPad->GetTriggerAll(JOYPAD_UP_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DOWN:
		if (m_pKeyboard->GetTrigger(DIK_S)
			|| m_pKeyboard->GetTrigger(DIK_DOWN)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_DOWN))
		{
			return true;
		}
		break;
	case KEY_DOWN_LEFT:
		if ((m_pKeyboard->GetTrigger(DIK_S) && m_pKeyboard->GetTrigger(DIK_A))
			|| (m_pKeyboard->GetTrigger(DIK_DOWN) && m_pKeyboard->GetTrigger(DIK_LEFT))
			|| m_pJoyPad->GetTriggerAll(JOYPAD_DOWN_LEFT))
		{
			return true;
		}
		break;
	case KEY_DOWN_RIGHT:
		if ((m_pKeyboard->GetTrigger(DIK_S) && m_pKeyboard->GetTrigger(DIK_D))
			|| (m_pKeyboard->GetTrigger(DIK_DOWN) && m_pKeyboard->GetTrigger(DIK_RIGHT))
			|| m_pJoyPad->GetTriggerAll(JOYPAD_DOWN_RIGHT))
		{
			return true;
		}
		break;
	case KEY_LEFT:
		if (m_pKeyboard->GetTrigger(DIK_A)
			|| m_pKeyboard->GetTrigger(DIK_LEFT)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_LEFT))
		{
			return true;
		}
		break;
	case KEY_RIGHT:
		if (m_pKeyboard->GetTrigger(DIK_D)
			|| m_pKeyboard->GetTrigger(DIK_RIGHT)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DECISION:
		if (m_pKeyboard->GetTrigger(DIK_RETURN)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_A))
		{
			return true;
		}
		break;
	case KEY_SHOT:
		if (m_pKeyboard->GetTrigger(DIK_SPACE)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_R1))
		{
			return true;
		}
		break;
	case KEY_BACK:
		if (m_pKeyboard->GetTrigger(DIK_BACKSPACE)
			|| m_pKeyboard->GetTrigger(DIK_B)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_BACK)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_B))
		{
			return true;
		}
		break;
	case KEY_SHIFT:
		if (m_pKeyboard->GetTrigger(DIK_RSHIFT)
			|| m_pKeyboard->GetTrigger(DIK_LSHIFT)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_A))
		{
			return true;
		}
		break;
	case KEY_MOVE:
		if (Trigger(KEY_UP) || Trigger(KEY_DOWN) || Trigger(KEY_LEFT) || Trigger(KEY_RIGHT))
		{
			return true;
		}
		break;
	case KEY_PAUSE:
		if (m_pKeyboard->GetTrigger(DIK_P)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_START)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_HOME))
		{
			return true;
		}
		break;
	case KEY_ALL:
		if (m_pKeyboard->GetAllTrigger()
			|| m_pJoyPad->GetTriggerAll())
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

//*************************************************************************************
//プレス処理(キーボード)
//*************************************************************************************
bool CInput::Press(int nKey)
{
	return m_pKeyboard->GetPress(nKey);
}

//*************************************************************************************
//トリガー処理(キーボード)
//*************************************************************************************
bool CInput::Trigger(int nkey)
{
	return m_pKeyboard->GetTrigger(nkey);
}

//*************************************************************************************
//リリース処理(キーボード)
//*************************************************************************************
bool CInput::Release(int nkey)
{
	return m_pKeyboard->GetRelease(nkey);
}

//*************************************************************************************
//プレス処理(ジョイパッド)
//*************************************************************************************
bool CInput::Press(DirectJoypad key, int nNum)
{
	return m_pJoyPad->GetPress(key, nNum);
}

//*************************************************************************************
//トリガー処理(ジョイパッド)
//*************************************************************************************
bool CInput::Trigger(DirectJoypad key, int nNum)
{
	return m_pJoyPad->GetTrigger(key, nNum);
}

//*************************************************************************************
//十字キーのベクトル
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveKey(int nNum)
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	if (m_pJoyPad->GetJoyPadNumMax() <= nNum)
	{
		return VectorMove;
	}

	int nCheck = m_pJoyPad->GetCross(nNum);

	if (nCheck > 360 || nCheck < 0)
	{//十字キーの角度が指定外だったら０を返す
		return VectorMove;
	}

	float fRot = D3DXToRadian(nCheck);

	VectorMove.x = sinf(fRot);
	VectorMove.y = -cosf(fRot);

	//長さ１のベクトル変換
	D3DXVec3Normalize(&VectorMove, &VectorMove);

	return VectorMove;
}

D3DXVECTOR3 CInput::VectorMoveKeyAll()
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nJoyPad = m_pJoyPad->GetJoyPadNumMax();
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	for (int nCnt = 0; nCnt < nJoyPad; nCnt++)
	{
		int nCheck = m_pJoyPad->GetCross(nCnt);

		if (nCheck > 360 || nCheck < 0)
		{//十字キーの角度が指定外だったら０を返す
			return VectorMove;
		}

		float fRot = D3DXToRadian(nCheck);

		VectorMove.x = sinf(fRot);
		VectorMove.y = -cosf(fRot);

		//長さ１のベクトル変換
		D3DXVec3Normalize(&VectorMove, &VectorMove);

		if (VectorMove != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			return VectorMove;
		}

	}
	return VectorMove;
}

//*************************************************************************************
//ジョイスティックのベクトル
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveJoyStick(int nNum, bool bleftandright)
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	VectorMove = m_pJoyPad->GetJoyStickData(nNum, bleftandright);
	//長さ１のベクトル変換
	D3DXVec3Normalize(&VectorMove, &VectorMove);
	return VectorMove;
}

//*************************************************************************************
//ジョイスティックのベクトル
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveJoyStickAll(bool bleftandright)
{
	D3DXVECTOR3 VectorNull = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		VectorMove = m_pJoyPad->GetJoyStickData(nCnt, bleftandright);
		//長さ１のベクトル変換
		D3DXVec3Normalize(&VectorMove, &VectorMove);
		if (VectorNull != VectorMove)
		{
			return  VectorMove;
		}
	}

	return VectorNull;
}
