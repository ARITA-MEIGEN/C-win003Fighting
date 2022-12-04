//=================================================
// Content     (ジョイパッドの設定)(InputJoyPad.cpp)
// Author     : 有田明玄
//=================================================
#include "InputJoyPad.h"


//==========================================
//ジョイパッドのコンストラクタ
//==========================================
CInputJoyPad::CInputJoyPad()
{

}

//==========================================
//ジョイパッドのデストラクタ
//==========================================
CInputJoyPad::~CInputJoyPad()
{

}
//=========================================
//ジョイパッドの初期化処理
//=========================================
HRESULT CInputJoyPad::Init(void)
{
	//メモリのクリア
	memset(&m_joyKeyState, 0, sizeof(XINPUT_STATE));
	memset(&m_joyKeyStateTrigger, 0, sizeof(XINPUT_STATE));

	//Xinputのステートの設定(有効にする)
	XInputEnable(true);

	//ジョイパッドの振動制御の0クリア
	ZeroMemory(&m_joyMoter, sizeof(XINPUT_VIBRATION));

	//振動制御の初期化
	m_nStrength = 0;
	m_nTime = 0;

	return S_OK;
}

//=========================================
//ジョイパッドの終了処理
//=========================================
void CInputJoyPad::Uninit(void)
{
	//Xinputのステートの設定(無効にする)
	XInputEnable(false);
}

//=========================================
//ジョイパッドの更新処理
//=========================================
void CInputJoyPad::Update(void)
{
	XINPUT_STATE joykeyState;	//ジョイパッドの入力情報
								//ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		m_joyKeyStateTrigger.Gamepad.wButtons =
			(m_joyKeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons)&joykeyState.Gamepad.wButtons;	//キーのトリガー情報保存
																											// キーのリリース情報保存
		m_joyKeyStateRelease.Gamepad.wButtons = (m_joyKeyState.Gamepad.wButtons^ joykeyState.Gamepad.wButtons)&m_joyKeyState.Gamepad.wButtons;
		m_joyKeyState = joykeyState;	//ジョイパッドのプレス情報を保存
	}

	//ジョイパッドの振動
	m_joyMoter.wLeftMotorSpeed = (WORD)m_nStrength;
	m_joyMoter.wRightMotorSpeed = (WORD)m_nStrength;
	XInputSetState(0, &m_joyMoter);

	if (m_nTime>0)
	{
		m_nTime--;
	}
	else
	{
		m_nStrength = 0;
		m_nTime = 0;
	}
}

//=========================================
//ジョイパッドのプレス情報を取得
//=========================================
bool CInputJoyPad::GetJoypadPress(JOYKEY key)
{
	return (m_joyKeyState.Gamepad.wButtons&(0x01 << key)) ? true : false;
}

//=========================================
//ジョイパッドのトリガー情報を取得
//=========================================
bool CInputJoyPad::GetJoypadTrigger(JOYKEY key)
{
	return (m_joyKeyStateTrigger.Gamepad.wButtons&(0x01 << key)) ? true : false;
}

//=========================================
//ジョイパッドのリリース情報を取得
//=========================================
bool CInputJoyPad::GetJoypadRelease(JOYKEY key)
{
	return (m_joyKeyStateRelease.Gamepad.wButtons&(0x01 << key)) ? true : false;;
}

//=========================================
//ジョイパッドのスティック情報を取得
//=========================================
D3DXVECTOR3 CInputJoyPad::GetJoypadStick(JOYKEY key)
{
	switch (key)
	{
	case JOYKEY_LSTICK:
		m_JoyStickPos = D3DXVECTOR3(m_joyKeyState.Gamepad.sThumbLX / DEADZONE, -m_joyKeyState.Gamepad.sThumbLY / DEADZONE, 0.0f);
		break;
	case JOYKEY_RSTICK:
		m_JoyStickPos = D3DXVECTOR3(m_joyKeyState.Gamepad.sThumbRX / DEADZONE, -m_joyKeyState.Gamepad.sThumbRY / DEADZONE, 0.0f);
		break;
	}
	return m_JoyStickPos;
}
//=========================================
//コントローラーの振動制御
//=========================================
void CInputJoyPad::Joypadvibration(int nTime, int nStrength)
{
	m_nTime = nTime;			//振動持続時間
	m_nStrength = nStrength;	//振動の強さ
}
