//=============================================================================
//
// �v���C���[
// Author : �L�c����
//
//=============================================================================

//�C���N���[�h
#include"Effect.h"
#include"renderer.h"
#include"Application.h"

//�ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CEffect::m_apTexture[EFFECT_TEX] = {};
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffect::CEffect(int priorty) :CObject3D(priorty)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT  CEffect::Init()
{
	CObject3D::Init();
	return S_OK;
}

//=============================================================================
//�I��
//=============================================================================
void  CEffect::Uninit()
{
	CObject3D::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void  CEffect::Update()
{
	CObject3D::Update();

	//�傫����ω�������
	D3DXVECTOR3 Siz = GetSiz();
	D3DXCOLOR col = GetCol();

	Siz -= {1.0f, 0.0f, 1.0f};
	m_nLife--;
	col.a -= fAlphagain;

	//�F(�A���t�@�l��ω�������)
	if (col.a <= 0.0f || Siz.x <= 0.0f || Siz.y <= 0.0f || m_nLife <= 0)
	{//�A���t�@�l��0�ȉ��̏ꍇ�����I��
		Uninit();
		return;
	}
	else
	{
		SetSiz(Siz);
		SetCol(col);
	}

	//�ړ�����
	D3DXVECTOR3 pos = GetPos();
	pos += m_move;
	SetPos(pos);
}

//=============================================================================
// �`��
//=============================================================================
void  CEffect::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	
	D3DMATRIX mtxView;

	//�A���t�@�u�����f�B���O�����Z�����ɐݒ�
	if (m_bAlpha == true)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//�A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Z�o�b�t�@�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//���C�g����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	//�J�����̋t�s���ݒ�
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�J�����̋t�s���ݒ�
	mtx._11 = mtxView._11;
	mtx._12 = mtxView._21;
	mtx._13 = mtxView._31;
	mtx._21 = mtxView._12;
	mtx._22 = mtxView._22;
	mtx._23 = mtxView._32;
	mtx._31 = mtxView._13;
	mtx._32 = mtxView._23;
	mtx._33 = mtxView._33;

	//�ʒu�𔽉f
	D3DXVECTOR3 pos = GetPos();
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_apTexture[0]);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	CObject3D::Draw();

	pDevice->SetTexture(0, NULL);

	//���C�g�L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//�ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�A���t�@�e�X�g����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//Z�e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// ����
//=============================================================================
CEffect* CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, float lot, D3DXVECTOR3 move, int nLife, D3DXCOLOR col,int texnumber,bool alpha)
{
	CEffect*pEffect;
	pEffect = new CEffect(5);
	if (pEffect != nullptr)
	{// �|���S���̏���������
		pEffect->Init();
		pEffect->SetPos(pos);
		pEffect->SetRot(D3DXVECTOR3(lot, D3DX_PI*-0.5f, D3DX_PI*0.5f));
		pEffect->m_nLife = nLife;
		pEffect->SetCol(col);
		pEffect->m_move = move;
		pEffect->SetSiz(siz);
		pEffect->BindTexture(m_apTexture[texnumber]);	//�e�N�X�`���̐ݒ�
		pEffect->m_bAlpha = alpha;
		pEffect->fAlphagain = col.a / nLife;
	}
	return pEffect;
}

//=============================================================================
// �f�[�^��ǂݍ���
//=============================================================================
HRESULT CEffect::Load()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Splash01.png",
		&m_apTexture[0]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&m_apTexture[1]);

	return S_OK;
}

//=============================================================================
// �f�[�^��j��
//=============================================================================
void CEffect::Unload()
{
	for (int i = 0; i < EFFECT_TEX; i++)
	{
		if (m_apTexture[i] != nullptr)
		{//�e�N�X�`���̔j��
			m_apTexture[i]->Release();
			m_apTexture[i] = nullptr;
		}
	}
}
