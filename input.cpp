//=============================================================================
//
// 入力処理 [input.cpp]
// Author1 : KOZUNA HIROHITO
// Author2 : YUDA KAITO
// Author3 : KARASAKI YUTO
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
CInput *CInput::m_pInput = nullptr;	// このクラスの情報

//-----------------------------------------------------------------------------
//コンストラクタ
//-----------------------------------------------------------------------------
CInput::CInput()
{
	m_pKeyboard = nullptr;	// キーボードの情報
	m_pJoyPad = nullptr;	// ジョイパッドの情報
}

//-----------------------------------------------------------------------------
//デストラクタ
//-----------------------------------------------------------------------------
CInput::~CInput()
{
}

//-----------------------------------------------------------------------------
//初期化
//-----------------------------------------------------------------------------
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

	//DirectXジョイパッドの生成
	m_pJoyPad = new CInputJoyPad;

	//DirectXジョイパッドの初期化処理
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

	//DirectXジョイパッドの破棄
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
}

//*************************************************************************************
//インプットの生成
//*************************************************************************************
CInput *CInput::Create()
{
	//Inputの自己生成
	if (m_pInput == nullptr)
	{
		m_pInput = new CInput;
	}

	return m_pInput;
}

//*************************************************************************************
// 全デバイスの入力を確認
//*************************************************************************************
bool CInput::KeyChackAll(STAN_DART_INPUT_KEY key, int type)
{
	// Standart
	auto Standart = [this, type](STAN_DART_INPUT_KEY a)
	{
		switch (type)
		{
		case 1:
			return Press(a);
			break;
		case 2:
			return Trigger(a);
			break;
		case 3:
			return Release(a);
			break;
		default:
			break;
		}
		return false;
	};

	// keyboard
	auto Keyboard = [this, type](int a)
	{
		switch (type)
		{
		case 1:
			return Press(a);
			break;
		case 2:
			return Trigger(a);
			break;
		case 3:
			return Release(a);
			break;
		default:
			break;
		}
		return false;
	};

	// Joupad
	auto Joypad = [this, type](DirectJoypad a)
	{
		switch (type)
		{
		case 1:
			return Press(a);
			break;
		case 2:
			return Trigger(a);
			break;
		case 3:
			return Release(a);
			break;
		default:
			break;
		}
		return false;
	};

	switch (key)
	{
	case KEY_UP:
		return Keyboard(DIK_W) || Keyboard(DIK_UP) || Joypad(JOYPAD_UP);
		break;
	case KEY_UP_LEFT:
		return (Keyboard(DIK_W) && Keyboard(DIK_A)) || (Keyboard(DIK_UP) && Keyboard(DIK_LEFT)) || Joypad(JOYPAD_UP_LEFT);
		break;
	case KEY_UP_RIGHT:
		return (Keyboard(DIK_W) && Keyboard(DIK_D)) || (Keyboard(DIK_UP) && Keyboard(DIK_RIGHT))|| Joypad(JOYPAD_UP_RIGHT);
		break;
	case KEY_DOWN:
		return Keyboard(DIK_S) || Keyboard(DIK_DOWN) || Joypad(JOYPAD_DOWN);
		break;
	case KEY_DOWN_LEFT:
		return (Keyboard(DIK_S) && Keyboard(DIK_A)) || (Keyboard(DIK_DOWN) && Keyboard(DIK_LEFT)) || Joypad(JOYPAD_DOWN_LEFT);
		break;
	case KEY_DOWN_RIGHT:
		return  (Keyboard(DIK_S) && Keyboard(DIK_D)) || (Keyboard(DIK_DOWN) && Keyboard(DIK_RIGHT)) || Joypad(JOYPAD_DOWN_RIGHT);
		break;
	case KEY_LEFT:
		return Keyboard(DIK_A) || Keyboard(DIK_LEFT) || Joypad(JOYPAD_LEFT);
		break;
	case KEY_RIGHT:
		return Keyboard(DIK_D) || Keyboard(DIK_RIGHT) || Joypad(JOYPAD_RIGHT);
		break;
	case KEY_DECISION:
		return Keyboard(DIK_RETURN) || Joypad(JOYPAD_A);
		break;
	case KEY_SHOT:
		return Keyboard(DIK_SPACE) || Joypad(JOYPAD_R1);
		break;
	case KEY_BACK:
		return Keyboard(DIK_BACKSPACE) || Keyboard(DIK_B) || Joypad(JOYPAD_BACK) || Joypad(JOYPAD_B);
		break;
	case KEY_SHIFT:
		return Keyboard(DIK_RSHIFT) || Keyboard(DIK_LSHIFT) || Joypad(JOYPAD_L1);
		break;
	case KEY_MOVE:
		return Standart(KEY_UP) || Standart(KEY_DOWN) || Standart(KEY_LEFT) || Standart(KEY_RIGHT);
		break;
	case KEY_PAUSE:
		return Keyboard(DIK_P) || Joypad(JOYPAD_START);
		break;
	case KEY_ALL:
		return m_pKeyboard->GetAllPress() || m_pJoyPad->GetPressAll();
		break;

	default:
		break;
	}

	// 予定されてないキーが呼ばれた
	assert(false);
	return false;
}

//*************************************************************************************
// 指定したデバイスの入力を確認
//*************************************************************************************
bool CInput::KeyChackNum(STAN_DART_INPUT_KEY key, int type, int nNum)
{
	// Standart
	auto Standart = [this, type](STAN_DART_INPUT_KEY key, int num)
	{
		switch (type)
		{
		case 1:
			return Press(key, num);
			break;
		case 2:
			return Trigger(key, num);
			break;
		case 3:
			return Release(key, num);
			break;
		default:
			break;
		}
		return false;
	};

	// keyboard
	auto Keyboard = [this, type](int key)
	{
		switch (type)
		{
		case 1:
			return Press(key);
			break;
		case 2:
			return Trigger(key);
			break;
		case 3:
			return Release(key);
			break;
		default:
			break;
		}
		return false;
	};

	// Joupad
	auto Joypad = [this, type](DirectJoypad key, int num)
	{
		switch (type)
		{
		case 1:
			return Press(key, num);
			break;
		case 2:
			return Trigger(key, num);
			break;
		case 3:
			return Release(key, num);
			break;
		default:
			break;
		}
		return false;
	};

	switch (key)
	{
	case KEY_UP:
		return nNum == -1 ? Keyboard(DIK_W) || Keyboard(DIK_UP) : Joypad(JOYPAD_UP, nNum);
		break;
	case KEY_UP_LEFT:
		return nNum == -1 ? (Keyboard(DIK_W) && Keyboard(DIK_A)) || (Keyboard(DIK_UP) && Keyboard(DIK_LEFT)) : Joypad(JOYPAD_UP_LEFT, nNum);
		break;
	case KEY_UP_RIGHT:
		return nNum == -1 ? (Keyboard(DIK_W) && Keyboard(DIK_D)) || (Keyboard(DIK_UP) && Keyboard(DIK_RIGHT)) : Joypad(JOYPAD_UP_RIGHT, nNum);
		break;
	case KEY_DOWN:
		return nNum == -1 ? Keyboard(DIK_S) || Keyboard(DIK_DOWN) : Joypad(JOYPAD_DOWN, nNum);
		break;
	case KEY_DOWN_LEFT:
		return nNum == -1 ? (Keyboard(DIK_S) && Keyboard(DIK_A)) || (Keyboard(DIK_DOWN) && Keyboard(DIK_LEFT)) : Joypad(JOYPAD_DOWN_LEFT, nNum);
		break;
	case KEY_DOWN_RIGHT:
		return nNum == -1 ? (Keyboard(DIK_S) && Keyboard(DIK_D)) || (Keyboard(DIK_DOWN) && Keyboard(DIK_RIGHT)) : Joypad(JOYPAD_DOWN_RIGHT, nNum);
		break;
	case KEY_LEFT:
		return nNum == -1 ? Keyboard(DIK_A) || Keyboard(DIK_LEFT) : Joypad(JOYPAD_LEFT, nNum);
		break;
	case KEY_RIGHT:
		return nNum == -1 ? Keyboard(DIK_D) || Keyboard(DIK_RIGHT) : Joypad(JOYPAD_RIGHT, nNum);
		break;
	case KEY_DECISION:
		return nNum == -1 ? Keyboard(DIK_RETURN) : Joypad(JOYPAD_A, nNum);
		break;
	case KEY_SHOT:
		return nNum == -1 ? Keyboard(DIK_SPACE) : Joypad(JOYPAD_R1, nNum);
		break;
	case KEY_BACK:
		return nNum == -1 ? Keyboard(DIK_BACKSPACE) || Keyboard(DIK_B) : Joypad(JOYPAD_BACK, nNum) || Joypad(JOYPAD_B, nNum);
		break;
	case KEY_MOVE:
		return Standart(KEY_UP, nNum) || Standart(KEY_DOWN, nNum) || Standart(KEY_LEFT, nNum) || Standart(KEY_RIGHT, nNum);
		break;
	case KEY_PAUSE:
		return nNum == -1 ? Keyboard(DIK_P) : Joypad(JOYPAD_START, nNum) || Joypad(JOYPAD_HOME, nNum);
		break;
	default:
		break;
	}

	// 予定されてないキーが呼ばれた
	assert(false);
	return false;
}

//*************************************************************************************
// 入力したデバイスの番号を取得 (Press)
//*************************************************************************************
std::vector<int> CInput::PressDevice(STAN_DART_INPUT_KEY key)
{
	std::vector<int> inputedDeviceIndex;

	// キーボード入力の調査
	if (Release(key, -1))
	{
		inputedDeviceIndex.push_back(-1);
	}

	// JoyPad入力の調査
	for (int i = 0;i < m_pJoyPad->GetJoyPadNumMax();i++)
	{
		if (Release(key, i))
		{
			inputedDeviceIndex.push_back(i);
		}
	}
	
	return inputedDeviceIndex;
}

//*************************************************************************************
// 入力したデバイスの番号を取得 (Trigger)
//*************************************************************************************
std::vector<int> CInput::TriggerDevice(STAN_DART_INPUT_KEY key)
{
	std::vector<int> inputedDeviceIndex;

	// キーボード入力の調査
	if (Trigger(key, -1))
	{
		inputedDeviceIndex.push_back(-1);
	}

	// JoyPad入力の調査
	for (int i = 0; i < m_pJoyPad->GetJoyPadNumMax(); i++)
	{
		if (Trigger(key, i))
		{
			inputedDeviceIndex.push_back(i);
		}
	}

	return inputedDeviceIndex;
}

//*************************************************************************************
// 入力したデバイスの番号を取得 (Release)
//*************************************************************************************
std::vector<int> CInput::ReleaseDevice(STAN_DART_INPUT_KEY key)
{
	std::vector<int> inputedDeviceIndex;

	// キーボード入力の調査
	if (Release(key, -1))
	{
		inputedDeviceIndex.push_back(-1);
	}

	// JoyPad入力の調査
	for (int i = 0; i < m_pJoyPad->GetJoyPadNumMax(); i++)
	{
		if (Release(key, i))
		{
			inputedDeviceIndex.push_back(i);
		}
	}

	return inputedDeviceIndex;
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

bool CInput::Press(DirectJoypad key)
{
	return m_pJoyPad->GetPressAll(key);
}

bool CInput::Trigger(DirectJoypad key)
{
	return m_pJoyPad->GetTriggerAll(key);
}

bool CInput::Release(DirectJoypad key)
{
	return m_pJoyPad->GetReleaseAll(key);
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
//リリース処理(ジョイパッド)
//*************************************************************************************
bool CInput::Release(DirectJoypad key, int nNum)
{
	return m_pJoyPad->GetRelease(key, nNum);
}

//*************************************************************************************
//十字キーのベクトル
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveKey()
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}
	int nCheck = m_pJoyPad->GetCross();

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
D3DXVECTOR3 CInput::VectorMoveJoyStick(bool bleftandright, int nNum)
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	VectorMove = m_pJoyPad->GetJoyStickData(bleftandright, nNum);
	//長さ１のベクトル変換
	D3DXVec3Normalize(&VectorMove, &VectorMove);
	return VectorMove;
}

int CInput::GetAcceptJoyPadCount()
{
	return m_pJoyPad->GetJoyPadNumMax();
}
