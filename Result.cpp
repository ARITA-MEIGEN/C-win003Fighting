//=============================================================================
//
// オブジェクト
// Author : 有田明玄
//
//=============================================================================
//インクルード
#include"main.h"
#include"Result.h"
#include"input.h"
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
	CInput* pInput = CApplication::GetInput();

	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		if (pInput->Trigger(DIK_RETURN) == true)		//ENTERキー
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
