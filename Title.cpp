//=============================================================================
//
// オブジェクト
// Author : 有田明玄
//
//=============================================================================
//インクルード
#include"main.h"
#include"Title.h"
#include"Application.h"
#include"InputJoyPad.h"
#include"InputKeyBoard.h"
#include"Fade.h"
#include"Object2D.h"

//静的メンバ変数
CObject2D*CTitle::m_pBg = nullptr;
//====================================
//コンストラクタ
//====================================
CTitle::CTitle()
{
}

//====================================
//デストラクタ
//====================================
CTitle::~CTitle()
{

}

//====================================
//初期化
//====================================
HRESULT CTitle::Init()
{
	//テクスチャの読み込み
	//背景の生成
	m_pBg = new CObject2D(CObject::OBJTYPE_UI);
	m_pBg->Init();
	m_pBg->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0.0f));
	m_pBg->SetSiz(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	m_pBg->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	return S_OK;
}

//====================================
//終了
//====================================
void CTitle::Uninit()
{
	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}
	//テクスチャの破棄
}

//====================================
//更新
//====================================
void CTitle::Update()
{
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();
	CInputJoyPad*pJoypad = CApplication::GetInputJoypad();

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
			CApplication::GetFade()->SetFade(CApplication::MODE_GAME);
		}
	}
}

//====================================
//描画
//====================================
void CTitle::Draw()
{

}

//====================================
//背景の取得
//====================================
CObject2D* CTitle::GetBg()
{
	return m_pBg;
}