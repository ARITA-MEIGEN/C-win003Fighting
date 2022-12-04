//=============================================================================
//
// オブジェクト
// Author : 有田明玄
//
//=============================================================================
//インクルード
#include"main.h"
#include"Result.h"
#include"InputJoyPad.h"
#include"InputKeyBoard.h"
#include"Fade.h"
#include"Application.h"

//静的メンバ変数
CBg*CResult::m_pBg = nullptr;
CScore*CResult::m_pScore=nullptr;

//====================================
//コンストラクタ
//====================================
CResult::CResult()
{
}

//====================================
//デストラクタ
//====================================
CResult::~CResult()
{

}

//====================================
//初期化
//====================================
HRESULT CResult::Init()
{
	//テクスチャの読み込み

	//背景の生成


	return S_OK;
}

//====================================
//終了
//====================================
void CResult::Uninit()
{
	//テクスチャの破棄
}

//====================================
//更新
//====================================
void CResult::Update()
{
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();
	CInputJoyPad*pJoypad = CApplication::GetInputJoypad();

	////文字のサイズが想定よりデカくなるバグに対しての応急処置
	//m_pScore->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2 - 70.0f, SCREEN_HEIGHT / 2 + 30.5f, 0.0f), D3DXVECTOR2(RESULT_SCORE_WIDTH, RESULT_SCORE_HEIGHT));

	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		if ((pKeyboard->GetTrigger(DIK_RETURN) == true ||
			pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_START) == true)
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_Y) == true
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_X) == true
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_A) == true
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_B) == true)		//ENTERキー
		{//エンターでランキングに
		 //モード設定
			//PlaySound(SOUND_LABEL_SE_GAMESTART);
			CApplication::GetFade()->SetFade(CApplication::MODE_TITLE);
		}
	}
}

//====================================
//描画
//====================================
void CResult::Draw()
{

}

//====================================
//背景の取得
//====================================
CBg * CResult::GetBg()
{
	return m_pBg;
}

//====================================
//スコアの取得
//====================================
CScore*CResult::GetScore()
{
	return m_pScore;
}
