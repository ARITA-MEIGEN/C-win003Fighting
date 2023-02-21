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
#include"Floor.h"
#include"Player.h"
#include"Mesh.h"
#include"Camera.h"
#include"Light.h"
#include"Shadow.h"
#include"Fade.h"
#include"Life.h"
#include"sound.h"
#include"input.h"
#include"effect.h"

//静的メンバ変数
CPlayer*CGame::m_pPlayer[2] = {};
CLife*CGame::m_Life = nullptr;				//体力ゲージ
CGame::GAME CGame::m_gamestate;
bool CGame::bDebugCamera = nullptr;

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
	CLife::Load();
	CEffect::Load();

	//プレイヤーの生成
	m_pPlayer[0] = CPlayer::Create(D3DXVECTOR3(50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	m_pPlayer[1] = CPlayer::Create(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI*0.5f, 0.0f));
	m_pPlayer[0]->SetEnemy(m_pPlayer[1]);
	m_pPlayer[1]->SetEnemy(m_pPlayer[0]);

	//体力ゲージ
	m_Life = CLife::Create();

	//カメラの設定
	m_pCamera = CCamera::Create();

	//ライトの設定
	m_pLight = new CLight;
	m_pLight->Init();

	//床
	CFloor::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f),D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	CFloor::Create(D3DXVECTOR3(200.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	CFloor::Create(D3DXVECTOR3(-200.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));

	//奥の壁
	CFloor::Create(D3DXVECTOR3(0.0f, 100.0f, 300.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f), D3DXVECTOR3(D3DX_PI*-0.5f,0.0f , 0.0f));	
	CFloor::Create(D3DXVECTOR3(200.0f, 100.0f, 300.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f), D3DXVECTOR3(D3DX_PI*-0.5f, 0.0f, 0.0f));
	CFloor::Create(D3DXVECTOR3(-200.0f, 100.0f, 300.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f), D3DXVECTOR3(D3DX_PI*-0.5f, 0.0f, 0.0f));

	//横の壁
	CFloor::Create(D3DXVECTOR3(STAGE_WIDTH, 100.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f), D3DXVECTOR3(D3DX_PI*-0.5f, D3DX_PI*0.5f, 0.0f));
	CFloor::Create(D3DXVECTOR3(-STAGE_WIDTH, 100.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f), D3DXVECTOR3(D3DX_PI*-0.5f, D3DX_PI*-0.5f, 0.0f));

	//デバッグ用カメラ操作モード
	bDebugCamera = false;

	PlaySound(SOUND_LABEL_BGM_BATTLE001);

	m_Timer = 0;

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
	CLife::Unload();
	CEffect::Unload();

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

	if (m_Life != nullptr)
	{
		m_Life->Uninit();
		delete m_Life;
	}

	StopSound();
}

//====================================
//更新
//====================================
void CGame::Update()
{
	CInput* pInput = CApplication::GetInput();
#ifdef _DEBUG
	//指定のキーが押されたかどうか
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		if (pInput->Trigger(DIK_RETURN))
		{
			CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
		}
	}
	if (pInput->Trigger(DIK_F2))
	{//カメラのON/OFF
		bDebugCamera = !bDebugCamera;
	}
	if (bDebugCamera == true)
	{
		CDebugProc::Print("F2:カメラモード");
	}
	else
	{
		CDebugProc::Print("F2:プレイヤーモード");
	}
#endif // !_DEBUG

	m_pCamera->Update();
	m_pLight->Update();
	m_Life->Update();

	//片方死んだ場合タイマー加算
	if (m_pPlayer[0]->GetLife() <= 0)
	{
		CApplication::SetWinner(0);
		m_Timer++;
	}
	else if (m_pPlayer[1]->GetLife() <= 0)
	{
		CApplication::SetWinner(1);
		m_Timer++;
	}

	//タイマーが一定値以上でリザルトへ移行
	if (m_Timer >= END_TIMER&&CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
	}
}

//====================================
//描画
//====================================
void CGame::Draw()
{
	m_pCamera->Set();
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