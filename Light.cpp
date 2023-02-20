//=================================================
// Content     (ゲームの設定)(light.cpp)
// Author     : 有田明玄
//=================================================
#include"light.h"
#include"Application.h"
#include"renderer.h"

//===========================
//ライトの初期化処理
//===========================
void CLight::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir;	//ライトの方向ベクトル

	//ライトをクリアする
	ZeroMemory(&m_alight[0], sizeof(D3DLIGHT9));

	//ライトの種類を設定
	m_alight[0].Type = D3DLIGHT_DIRECTIONAL;
	m_alight[1].Type = D3DLIGHT_DIRECTIONAL;
	m_alight[2].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	m_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_alight[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_alight[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライト１
	//ライトの方向を設定
	vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.4f);

	//正規化する
	D3DXVec3Normalize(&vecDir, &vecDir);
	m_alight[0].Direction = vecDir;

	//ライト2
	//ライトの方向を設定
	vecDir = D3DXVECTOR3(-0.5f, -0.8f, -0.4f);

	//正規化する
	D3DXVec3Normalize(&vecDir, &vecDir);
	m_alight[1].Direction = vecDir;

	//ライト3
	//ライトの方向を設定
	vecDir = D3DXVECTOR3(-0.4f, -0.8f, 0.2f);

	//正規化する
	D3DXVec3Normalize(&vecDir, &vecDir);
	m_alight[2].Direction = vecDir;

	//ライトを設定する
	pDevice->SetLight(0, &m_alight[0]);
	pDevice->SetLight(1, &m_alight[1]);
	pDevice->SetLight(2, &m_alight[2]);

	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);
	//ライトを有効にする
	pDevice->LightEnable(1, TRUE);
	//ライトを有効にする
	pDevice->LightEnable(2, TRUE);
}

//===========================
//ライトの終了処理
//===========================
void  CLight::Uninit(void)
{

}

//===========================
//ライトの更新処理
//===========================
void  CLight::Update(void)
{

}

//===========================
//ライトのベクトルの取得
//===========================
D3DXVECTOR3 CLight::GetVec(int number)
{
	return m_alight[number].Direction;
}