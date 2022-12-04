//=============================================================================
//
// オブジェクト
// Author : 有田明玄
//
//=============================================================================
//インクルード
#include"main.h"
#include"Application.h"
#include"Game.h"
#include"Player.h"
#include"InputKeyBoard.h"
#include"InputJoyPad.h"
#include"Floor.h"
#include"Player.h"
#include"Mesh.h"
#include"Camera.h"
#include"Light.h"
#include"Shadow.h"
#include"Fade.h"


//静的メンバ変数
CPlayer*CGame::m_pPlayer = nullptr;
CExplosion*CGame::m_pExplosion = nullptr;
CEnemy*CGame::m_pEnemy = nullptr;
CBg*CGame::m_pBg = nullptr;
CScore*CGame::m_pScore = nullptr;
CLife*CGame::m_Life = nullptr;				//体力ゲージ
CMapdata* CGame::m_pMap = nullptr;
CSpecial*CGame::m_pSpecial = nullptr;
CGame::GAME CGame::m_gamestate;


CCamera*CGame::m_pCamera = nullptr;
CLight*CGame::m_pLight = nullptr;
CFloor*CGame::m_pFloor = nullptr;
CMesh*CGame::m_pMesh = nullptr;
//====================================
//コンストラクタ
//====================================
CGame::CGame()
{
}

//====================================
//デストラクタ
//====================================
CGame::~CGame()
{

}

//====================================
//初期化
//====================================
HRESULT CGame::Init()
{
	//テクスチャの読み込み
	CShadow::Load();
	CFloor::Load();

	//マップデータ読み込み
	//プレイヤーの生成
	CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);

	//背景の生成
	//メッシュフィールドの設定
	m_pMesh = CMesh::Create(20, 20);

	//カメラの設定
	m_pCamera = CCamera::Create();

	//ライトの設定
	m_pLight = new CLight;
	m_pLight->Init();

	//CFloor::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f,500.0f));

	return S_OK;
}

//====================================
//終了
//====================================
void CGame::Uninit()
{
	//テクスチャの破棄
	CShadow::Unload();
	CFloor::Unload();

	//カメラの設定
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
	}

	//ライトの設定
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
	}
}

//====================================
//更新
//====================================
void CGame::Update()
{
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();
	CInputJoyPad*pJoypad = CApplication::GetInputJoypad();
#ifdef _DEBUG
	//指定のキーが押されたかどうか
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		if (pKeyboard->GetTrigger(DIK_RETURN)
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_Y)
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_X)
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_A)
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_B)
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_START))
		{
			CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
		}
	}
#endif // !_DEBUG

	m_pCamera->Update();
	m_pLight->Update();
}

//====================================
//描画
//====================================
void CGame::Draw()
{
	m_pCamera->Set();
}

//====================================
//プレイヤーの取得
//====================================
CPlayer * CGame::GetPlayer()
{
	return m_pPlayer;
}

//====================================
//ゲームの状態設定
//====================================
void CGame::SetGame(GAME gamestate)
{
	m_gamestate = gamestate;
}

//====================================
//ゲームの状態獲得
//====================================
CGame::GAME CGame::GetGame()
{
	return m_gamestate;
}

//====================================
//カメラの取得
//====================================
CCamera * CGame::GetCamera()
{
	return m_pCamera;
}

//====================================
//床の情報
//====================================
CFloor * CGame::GetFloor()
{
	return m_pFloor;
}

//====================================
//床の情報
//====================================
CMesh * CGame::GetMesh()
{
	return m_pMesh;
}

//====================================
//ライトの情報
//====================================
CLight * CGame::GetLight()
{
	return m_pLight;
}