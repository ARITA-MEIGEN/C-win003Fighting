//=============================================================================
//
// アプリケーション
// Author : 有田明玄
//
//=============================================================================

// インクルード
#include "Application.h"
#include"renderer.h"
#include"main.h"
#include"input.h"
#include"DebugProc.h"
#include"Object.h"
#include"Fade.h"
#include"sound.h"
#include"Title.h"
#include"Game.h"
#include"Result.h"

//====================================
//静的メンバ変数
//====================================
CRenderer*CApplication::m_pRenderer = nullptr;
CDebugProc*CApplication::m_pDebugProc = nullptr;
CFade* CApplication::m_pFade = nullptr;
int CApplication::m_nWinner = 0;

//コントローラー周り
CInput*CApplication::m_pInput = nullptr;

//画面遷移周り
CApplication::MODE CApplication::m_mode;
CTitle*CApplication::m_pTitle = nullptr;
CGame*CApplication::m_pGame = nullptr;
CResult*CApplication::m_pResult = nullptr;

//====================================
//コンストラクタ
//====================================
CApplication::CApplication()
{

}

//====================================
//デストラクタ
//====================================
CApplication::~CApplication()
{

}

//====================================
//初期化
//====================================
HRESULT CApplication::Init(HWND hWnd, bool bWindow, HINSTANCE hInstance)
{
	m_pRenderer = new CRenderer;

	//初期化処理
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{//初期化処理に失敗した場合
		return-1;
	}

	//-------------------------------
	//キーボードとジョイパッドの生成
	//-------------------------------
	m_pInput = CInput::Create();

	//入力処理の初期化
	if (FAILED(m_pInput->Init(hInstance, hWnd)))
	{
		return-1;
	}

	//----------------------------
	//サウンド初期化
	//----------------------------

	InitSound(hWnd);

	//----------------------------
	// モードの設定
	//----------------------------
	m_pFade = new CFade;
	SetMode(MODE_TITLE);
	m_pFade->Init(MODE_TITLE);

	//----------------------------
	// デバッグ用文字の生成
	//----------------------------
	m_pDebugProc = new CDebugProc;
	m_pDebugProc->Init();

	return S_OK;
}

//====================================
//終了
//====================================
void CApplication::Uninit()
{
	//----------------------------
	//レンダリングクラスの破壊
	//----------------------------
	if (m_pRenderer != nullptr)
	{
		// 終了処理
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	//----------------------------
	// タイトルの終了
	//----------------------------
	if (m_pTitle != nullptr)
	{
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = nullptr;
	}

	//----------------------------
	// ゲームの終了
	//----------------------------
	if (m_pGame != nullptr)
	{
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
	}

	//----------------------------
	// リザルトの終了
	//----------------------------
	if (m_pResult != nullptr)
	{
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = nullptr;
	}

	//----------------------------
	// フェードの終了
	//----------------------------
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	//----------------------------
	// デバッグ用文字の終了
	//----------------------------
	if (m_pDebugProc != nullptr)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}

	//----------------------------
	// 入力処理の終了
	//----------------------------
	if (m_pInput != nullptr)
	{
		m_pInput->Uninit();
	}

	//サウンド処理の終了
	UninitSound();
}

//====================================
//更新
//====================================
void CApplication::Update()
{
#ifdef _DEBUG
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスへのポインタ
																			//ワイヤーフレーム
	if (m_pInput->Trigger(DIK_1))
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	if (m_pInput->Trigger(DIK_2))
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
#endif // _DEBUG

	//レンダラー更新
	m_pRenderer->Update();

	//キーボードとジョイパッドの更新
	if (m_pInput != nullptr)
	{
		m_pInput->Update();
	}

	//モードごとの更新
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Update();
		break;

	case MODE_GAME:
		m_pGame->Update();
		break;

	case MODE_RESULT:
		m_pResult->Update();
		break;

	default:
		break;
	}

	//フェードの更新
	m_pFade->Update();
}

//====================================
//描画
//====================================
void CApplication::Draw()
{
	m_pRenderer->Draw();

	//モードごとの描画
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Draw();
		break;

	case MODE_GAME:
		m_pGame->Draw();
		break;

	case MODE_RESULT:
		m_pResult->Draw();
		break;

	default:
		break;
	}
}

//====================================
//レンダラーの取得
//====================================
CRenderer * CApplication::GetRenderer()
{
	return m_pRenderer;
}

//====================================
//デバッグ情報の取得
//====================================
CDebugProc * CApplication::GetDebugProc()
{
	return m_pDebugProc;
}

//====================================
//フェードの取得
//====================================
CFade * CApplication::GetFade()
{
	return m_pFade;
}

//======================================================
//モードの設定
//======================================================
void CApplication::SetMode(MODE mode)
{
	//新しい画面(モード)の初期化処理
	switch (m_mode)
	{
	case MODE_TITLE:	//タイトル画面
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle=nullptr;
		break;

	case MODE_GAME:		//ゲーム画面
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
		break;

	case MODE_RESULT:	//リザルト画面
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = nullptr;
		break;
	}
	m_mode = mode;	//現在の画面(モード)を切り替える
	CObject::ReleaseAll();

	//新しい画面(モード)の初期化処理
	switch (m_mode)
	{
	case MODE_TITLE:	//タイトル画面
		m_pTitle = nullptr;
		m_pTitle = new CTitle;
		m_pTitle->Init();
		break;

	case MODE_GAME:		//ゲーム画面
		m_pGame = nullptr;
		m_pGame = new CGame;
		m_pGame->Init();
		break;

	case MODE_RESULT:	//リザルト画面
		m_pResult = nullptr;
		m_pResult = new CResult;
		m_pResult->Init();
		break;
	}
}