//=============================================================================
//
// オブジェクト
// Author : 有田明玄
//
//=============================================================================
//インクルード
#include"main.h"
#include"UI.h"
#include"input.h"
#include"Fade.h"
#include"Application.h"
#include"renderer.h"
#include"sound.h"
#include"Game.h"


//静的メンバ変数
CObject2D*CUI::m_pStart[2] = {};

//====================================
//コンストラクタ
//====================================
CUI::CUI(int nPriority)
{
}

//====================================
//デストラクタ
//====================================
CUI::~CUI()
{

}

//====================================
//初期化
//====================================
HRESULT CUI::Init()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	LPDIRECT3DTEXTURE9 tex[2];
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\READY.png",
		&tex[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FIGHT.png",
		&tex[1]);

	//背景の生成
	for (int i = 0; i < 2; i++)
	{
		m_pStart[i] = new CObject2D(CObject::OBJTYPE_UI);
		m_pStart[i]->Init();
		m_pStart[i]->SetPos(D3DXVECTOR3((float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2, 0.0f));
		m_pStart[i]->SetSiz(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
		m_pStart[i]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - i));
		m_pStart[i]->BindTexture(tex[i]);
	}

	m_timer = 0;
	return S_OK;
}

//====================================
//終了
//====================================
void CUI::Uninit()
{
}

//====================================
//更新
//====================================
void CUI::Update()
{
	m_timer++;
	if (m_timer >= 120)
	{
		m_pStart[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		CGame::SetGame(CGame::GAME_ROUND1);
	}
	else  if (m_timer >= 60)
	{
		m_pStart[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pStart[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else  if (m_timer >= 30)
	{
		m_pStart[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_pStart[0]->GetCol().a - 0.05f));
	}
}

//====================================
//生成
//====================================
CUI * CUI::Create()
{
	CUI* pUI;
	pUI = new CUI;
	pUI->Init();
	return pUI;
}
