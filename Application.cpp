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
#include"InputKeyBoard.h"
#include"InputJoyPad.h"
#include"DebugProc.h"
#include"Object.h"
#include"Fade.h"

#include"Title.h"
#include"Game.h"
#include"Result.h"

//====================================
//静的メンバ変数
//====================================
CRenderer*CApplication::m_pRenderer = nullptr;
CInputKeyboard*CApplication::m_pInputKeyboard = nullptr;
CInputJoyPad*CApplication::m_pInputJoyPad = nullptr;
CDebugProc*CApplication::m_pDebugProc = nullptr;
CFade* CApplication::m_pFade = nullptr;

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
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputJoyPad = new CInputJoyPad;

	//初期化処理
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{//初期化処理に失敗した場合
		return-1;
	}

	//キーボードの生成
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return-1;
	}

	//ジョイパッドの生成(XInput)
	if (FAILED(m_pInputJoyPad->Init()))
	{
		return-1;
	}

	//----------------------------
	// モードの設定
	//----------------------------
	m_pFade = new CFade;
	SetMode(MODE_GAME);
	m_pFade->Init(MODE_GAME);

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
	//キーボードの破棄
	//----------------------------
	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	//----------------------------
	//ジョイパッドの破棄
	//----------------------------
	if (m_pInputJoyPad != nullptr)
	{
		m_pInputJoyPad->Uninit();
		delete m_pInputJoyPad;
		m_pInputJoyPad = nullptr;
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
}

//====================================
//更新
//====================================
void CApplication::Update()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスへのポインタ
	//ワイヤーフレーム
	if (m_pInputKeyboard->GetTrigger(DIK_1))
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	if (m_pInputKeyboard->GetTrigger(DIK_2))
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	//レンダラー更新
	m_pRenderer->Update();
	
	//キーボードの更新
	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Update();
	}

	//ジョイパッドの更新
	if (m_pInputJoyPad!=nullptr)
	{
		m_pInputJoyPad->Update();
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
//キーボードの取得
//====================================
CInputKeyboard * CApplication::GetInputKeyboard()
{
	return m_pInputKeyboard;
}

//====================================
//ジョイパッドの取得
//====================================
CInputJoyPad * CApplication::GetInputJoypad()
{
	return m_pInputJoyPad;
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


////======================================================
////ステータスの表示
////======================================================
//void CApplication::DrawStatas(void)
//{
//	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
//	char aStr[512];
//
//	//文字列に代入
//	//wsprintf(&aStr[0], "FPS:%3d\n", g_nCountFPS);
//
//	//モデルの操作キー
//	strcat(aStr, "モデルの操作:↑/↓/←/→\n");
//
//	//視点の旋回キー
//	strcat(aStr, "視点の旋回:Q/E/R/F\n");
//
//	//注視点の旋回キー
//	strcat(aStr, "注視点の旋回:J/L/I/K\n");
//
//	//カメラの位置操作キー
//	strcat(aStr, "カメラの位置操作:W/A/S/D\n");
//
//	////視点の座標
//	//char aStr2[400];
//	//sprintf(aStr2, "視点の位置:%3f/%3f/%3f\n", m_pCamera->posV.x, m_pCamera->posV.y, m_pCamera->posV.z);
//	//strcat(aStr, aStr2);
//
//	////注視点の座標
//	//char aStr3[400];
//	//sprintf(aStr3, "注視点の位置:%3f/%3f/%3f\n", m_pCamera->posR.x, m_pCamera->posR.y, m_pCamera->posR.z);
//	//strcat(aStr, aStr3);
//
//	////ワイヤーフレーム
//	//strcat(aStr, "ワイヤーフレームON/OFF:1/2\n");
//
//	////モデルの操作キー
//	//strcat(aStr, "発射ァ！:SPACEを押すんだ！\n");
//
//	//テキストの描画
//	m_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
//}

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
		break;

	case MODE_GAME:		//ゲーム画面
		m_pGame->Uninit();
		break;

	case MODE_RESULT:	//リザルト画面
		m_pResult->Uninit();
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