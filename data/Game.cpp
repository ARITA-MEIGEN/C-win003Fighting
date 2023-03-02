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
#include"Time.h"
#include"UI.h"

//静的メンバ変数
CPlayer*CGame::m_pPlayer[2] = {};
CLife*CGame::m_Life = nullptr;				//体力ゲージ
CGame::GAME CGame::m_gamestate;
bool CGame::bDebugCamera = nullptr;

CCamera*CGame::m_pCamera = nullptr;
CLight*CGame::m_pLight = nullptr;
CFloor*CGame::m_pFloor = nullptr;
CTimer*CGame::m_pTimer = nullptr;
CUI*CGame::m_pUI = nullptr;
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
	CTimer::Load();
	CShadow::Load();

	//プレイヤーの生成
	m_pPlayer[0] = CPlayer::Create(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI*0.5f, 0.0f));
	m_pPlayer[1] = CPlayer::Create(D3DXVECTOR3(50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
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

	m_pTimer = CTimer::Create();
	m_Timer = 0;

	//UI生成
	m_pUI = CUI::Create();

	m_gamestate = GAME_START;

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
	CTimer::Unload();
	CShadow::Unload();

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

	if (m_pUI != nullptr)
	{
		m_pUI->Uninit();
		delete m_pUI;
	}
	
	if (m_pTimer != nullptr)
	{
		m_pTimer->Uninit();
		delete m_pTimer;
	}

	StopSound();
}

//====================================
//更新
//====================================
void CGame::Update()
{
	CInput* pInput = CApplication::GetInput();
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		m_pUI->Update();	//UI
		m_pTimer->Update();

#ifdef _DEBUG
		//指定のキーが押されたかどうか
		if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			if (pInput->Trigger(DIK_RETURN))
			{
				CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
			}
		}
#endif // !_DEBUG
	}
	m_pCamera->Update();
	m_pLight->Update();
	m_Life->Update();

	//片方死んだ場合タイマー加算
	if (m_pPlayer[0]->GetLife() <= 0 || m_pPlayer[1]->GetLife() <= 0 || m_pTimer->GetTimer() <= 0)
	{
		if (m_pPlayer[0]->GetNowMotion() == CPlayer::PM_DOWN|| m_pPlayer[1]->GetNowMotion() == CPlayer::PM_DOWN|| m_pTimer->GetTimer() <= 0)
		{
			CGame::SetGame(CGame::GAME_END);
		}

		m_Timer++;
		if (m_pPlayer[1]->GetLife() == m_pPlayer[0]->GetLife())
		{
			CApplication::SetWinner(2);
		}
		else if (m_pPlayer[0]->GetLife() < m_pPlayer[1]->GetLife())
		{
			CApplication::SetWinner(1);
		}
		else if (m_pPlayer[1]->GetLife() < m_pPlayer[0]->GetLife())
		{
			CApplication::SetWinner(0);
		}
	}

	//タイマーが一定値以上でリザルトへ移行
	if (m_Timer >= END_TIMER&&CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
	}

	if (CGame::GetTimer()->GetTimer() == DEFAULT_TIME&& CGame::GetTimer()->GetTimer() < 0)
	{
		m_gamestate = GAME_END;
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
//ライトの情報
//====================================
CLight * CGame::GetLight()
{
	return m_pLight;
}