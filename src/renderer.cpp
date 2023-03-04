//=============================================================================
//
// レンダー
// Author : 有田明玄
//
//=============================================================================

//インクルード
#include"renderer.h"
#include"Object2D.h"
#include"Application.h"
#include"DebugProc.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CRenderer::CRenderer()
{
	//メンバ変数の初期化
	g_pD3D = nullptr;							// Direct3Dオブジェクト
	g_pD3DDevice = nullptr;						// Deviceオブジェクト
	g_pFont = nullptr;							// フォント
}

//=============================================================================
//デストラクタ
//=============================================================================
CRenderer::~CRenderer()
{
	assert(g_pD3D == nullptr);					// Direct3Dオブジェクト
	assert(g_pD3DDevice == nullptr);			// Deviceオブジェクト
	assert(g_pFont == nullptr);					//Crendererがnullか確認
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;
	g_pD3D = nullptr;
	g_pD3DDevice = nullptr;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == nullptr)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if ((FAILED(CRenderer::g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice))) &&
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		(FAILED(CRenderer::g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice))) &&
		// 上記の設定が失敗したら
		// 描画と頂点処理をCPUで行なう
		(FAILED(CRenderer::g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice))))
	{
		// 生成失敗
		return E_FAIL;
	}
	////CRendererの初期化処理
	//d3dpp.EnableAutoDepthStencil = TRUE;
	//d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//ポリゴンの大きさに併せてきれいにテクスチャをはる
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	//テクスチャ座標Uが1.0を超えた場合繰り返すかどうかを決める
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	//テクスチャ座標Vが1.0を超えた場合繰り返すかどうかを決める

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	//OPはオペレーションの意 ポリゴンとテクスチャの色を混ぜる
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	//テクスチャのアルファ値を参照する
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	//ポリゴンのアルファ値を参照する

#ifdef _DEBUG
	//// デバッグ情報表示用フォントの生成
	//D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
	//	OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &g_pFont);
#endif

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void  CRenderer::Uninit()
{
	//オブジェクトの破棄
	CObject::ReleaseAll();

#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (g_pFont != nullptr)
	{
		g_pFont->Release();
		g_pFont = nullptr;
	}
#endif // _DEBUG

	// デバイスの破棄
	if (g_pD3DDevice != nullptr)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != nullptr)
	{
		g_pD3D->Release();
		g_pD3D = nullptr;
	}
}

//=============================================================================
// 更新
//=============================================================================
void  CRenderer::Update()
{
	// ポリゴンの更新処理
	CObject::UpdateAll();
}

//=============================================================================
// 描画
//=============================================================================
void  CRenderer::Draw()
{
	//Zバッファ&ステンシルバッファのクリア
	//g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_STENCIL | D3DCLEAR_TARGET));

	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//// 平行投影行列の作成
		//D3DXMATRIX matProjection;
		//D3DXMatrixOrthoLH(&matProjection, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f);
		//g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProjection);

		// ポリゴンの描画処理
		CObject::DrawAll();

		//デバッグ情報の描画
		CApplication::GetDebugProc()->Draw();

		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);

}

//======================================================
//デバイスの取得
//======================================================
LPDIRECT3DDEVICE9  CRenderer::GetDevice(void)
{
	return g_pD3DDevice;
}

//=============================================================================
// FPS表示
//=============================================================================
#ifdef _DEBUG

void  CRenderer::DrawFPS()
{
	int nCntFPS = GetFPS();
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	TCHAR str[256];

	wsprintf(str, _T("FPS : %d\n"), nCntFPS);

	// テキスト描画
	g_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif // _DEBUG
