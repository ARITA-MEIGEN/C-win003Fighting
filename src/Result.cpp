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
#include"renderer.h"
#include"sound.h"

//静的メンバ変数
CObject2D*CResult::m_pBg = nullptr;
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
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	LPDIRECT3DTEXTURE9 tex[3];

	//背景の生成
	m_pBg = new CObject2D(CObject::OBJTYPE_UI);
	m_pBg->Init();
	m_pBg->SetPos(D3DXVECTOR3((float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2, 0.0f));
	m_pBg->SetSiz(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
	m_pBg->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Result000.png",
		&tex[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Result001.png",
		&tex[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Result002.png",
		&tex[2]);

	m_pBg->BindTexture(tex[CApplication::GetWinner()]);

	PlaySound(SOUND_LABEL_BGM_RESULT);

	return S_OK;
}

//====================================
//終了
//====================================
void CResult::Uninit()
{
	StopSound();

}

//====================================
//更新
//====================================
void CResult::Update()
{
	CInput* pInput = CApplication::GetInput();

	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		if ((pInput->Trigger(KEY_ALL)) == true)		//ENTERキー
		{//エンターでランキングに
		 //モード設定
			PlaySound(SOUND_LABEL_SE_START);
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
