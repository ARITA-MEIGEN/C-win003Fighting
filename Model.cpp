//=================================================
// Content     (�Q�[���̐ݒ�)(player.cpp)
// Author     : �L�c����
//=================================================

//�C���N���[�h
#include"Model.h"
#include"Game.h"
#include"renderer.h"
#include"Camera.h"
#include"InputKeyBoard.h"
#include"Shadow.h"
#include"Mesh.h"
#include"Light.h"

//�}�N����`
#define PLAYER_SPEED	(2.0f)	//�ړ����x
#define MIN_SPEED		(0.1f)	//���C�ɂ��Œᑬ�x
#define NUM_MODELPARTS	(1)		//���f���̃p�[�c��

//�ÓI�����o�ϐ��錾
LPD3DXBUFFER CModel::m_pPlayerBuffMat[NUM_PLAYERPARTS] = {};	//�}�e���A�����ւ̃|�C���^
DWORD		 CModel::m_nPlayerNumMat[NUM_PLAYERPARTS] = {};		//�}�e���A�����̐�
LPD3DXMESH	 CModel::m_pPlayerMesh[NUM_PLAYERPARTS] = {};		//���b�V���ւ̃|�C���^

//===========================
//�R���X�g���N�^
//===========================
CModel::CModel() 
{

}

//===========================
//�f�X�g���N�^
//===========================
CModel::~CModel()
{

}

//===========================
//����������
//===========================
HRESULT CModel::Init()
{
	LoadModel();
	return S_OK;
}

//===========================
//�I������
//===========================
void CModel::Uninit(void)
{

}

//===========================
//�X�V����
//===========================
void CModel::Update(void)
{

}

//===========================
//�`�揈��
//===========================
void CModel::Draw(D3DXMATRIX pMtx)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���̏��
	m_mtxParent = pMtx;

	//���݂̃}�e���A�����ێ�
	pDevice->GetMaterial(&matDef);

	//�p�[�c�̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�p�[�c�̃��f���̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�p�[�c�̃��f���̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//�e�̃}�g���b�N�X�Ɗ|�����킹��
	if (m_pParent != nullptr)
	{
		m_mtxParent = m_pParent->GetMtx();
	}
	else
	{//����(�ŐV)��Mtx���擾(Player��Mtx)�e�̂��Ȃ����f������������ꍇ�͕ʑΉ����K�v
		pDevice->GetTransform(D3DTS_WORLD, &m_mtxParent);
	}

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxParent);

	//�e�̐���
	//Shadow()

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pPlayerBuffMat[m_nNumber]->GetBufferPointer();

	//�}�e���A���̕`��
	for (int nCnt2 = 0; nCnt2 < (int)m_nPlayerNumMat[m_nNumber]; nCnt2++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCnt2].MatD3D);

		//�v���C���[�p�[�c�̕`��
		m_pPlayerMesh[m_nNumber]->DrawSubset(nCnt2);
	}
	//�ێ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//===========================
//����
//===========================
CModel * CModel::Create(D3DXVECTOR3 pos, int Number)
{
	CModel*pModel;
	pModel = new CModel();
	pModel->Init();
	pModel->m_nNumber = Number;
	pModel->SetPos(pos);
	return pModel;
}

//===========================
//���f���ǂݍ���
//===========================
void CModel::LoadModel()
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	pDevice = CApplication::GetRenderer()->GetDevice();

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\sphere.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[0],
		NULL,
		&m_nPlayerNumMat[0],
		&m_pPlayerMesh[0]);

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\LegBase.000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[1],
		NULL,
		&m_nPlayerNumMat[1],
		&m_pPlayerMesh[1]);

	//�x�[�X���e���L�[��2�Ƃ���1�̕���
	//���֐�
	D3DXLoadMeshFromX("data\\MODEL\\LegJoint1_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[2],
		NULL,
		&m_nPlayerNumMat[2],
		&m_pPlayerMesh[2]);
	//������
	D3DXLoadMeshFromX("data\\MODEL\\LegThigh1_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[3],
		NULL,
		&m_nPlayerNumMat[3],
		&m_pPlayerMesh[3]);
	//��
	D3DXLoadMeshFromX("data\\MODEL\\LegFoot1_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[4],
		NULL,
		&m_nPlayerNumMat[4],
		&m_pPlayerMesh[4]);

	//�x�[�X�̐��ʂ��e���L�[��2�Ƃ���9�̕���
	//���֐�
	D3DXLoadMeshFromX("data\\MODEL\\LegJoint2_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[5],
		NULL,
		&m_nPlayerNumMat[5],
		&m_pPlayerMesh[5]);
	//������
	D3DXLoadMeshFromX("data\\MODEL\\LegThigh2_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[6],
		NULL,
		&m_nPlayerNumMat[6],
		&m_pPlayerMesh[6]);

	//��
	D3DXLoadMeshFromX("data\\MODEL\\LegFoot2_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[7],
		NULL,
		&m_nPlayerNumMat[7],
		&m_pPlayerMesh[7]);

	//�x�[�X�̐��ʂ��e���L�[��2�Ƃ���3�̕���
	//���֐�
	D3DXLoadMeshFromX("data\\MODEL\\LegJoint3_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[8],
		NULL,
		&m_nPlayerNumMat[8],
		&m_pPlayerMesh[8]);
	//������
	D3DXLoadMeshFromX("data\\MODEL\\LegThigh3_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[9],
		NULL,
		&m_nPlayerNumMat[9],
		&m_pPlayerMesh[9]);

	//��
	D3DXLoadMeshFromX("data\\MODEL\\LegFoot3_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[10],
		NULL,
		&m_nPlayerNumMat[10],
		&m_pPlayerMesh[10]);

	//�x�[�X�̐��ʂ��e���L�[��2�Ƃ���7�̕���
	//���֐�
	D3DXLoadMeshFromX("data\\MODEL\\LegJoint4_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[11],
		NULL,
		&m_nPlayerNumMat[11],
		&m_pPlayerMesh[11]);
	//������
	D3DXLoadMeshFromX("data\\MODEL\\LegThigh4_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[12],
		NULL,
		&m_nPlayerNumMat[12],
		&m_pPlayerMesh[12]);

	//��
	D3DXLoadMeshFromX("data\\MODEL\\LegFoot4_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[13],
		NULL,
		&m_nPlayerNumMat[13],
		&m_pPlayerMesh[13]);
	Siz(m_nNumber);
}

//===========================
//�e�̍쐬
//===========================
void CModel::Shadow()
{
	D3DXMATERIAL *pMat;			//�}�e���A���̂Ȃ�
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXCOLOR col[2];			//�F�Ɣ����̐ݒ�

	//�e��`�悷�鏈��
	D3DXPLANE planeField;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, normal;
	//�V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxShadow);

	//���C�g�̋t�����Ō�̐��l�̓f�B���N�V���i�����C�g�ȊO�̏ꍇ�������炵���H
	D3DXVECTOR3 lightvec = CGame::GetLight()->GetVec(2);		//���C�g�̎擾
	vecLight = D3DXVECTOR4(-lightvec.x, -lightvec.y, -lightvec.z, 0.0f);

	pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);
	D3DXMatrixShadow(&m_mtxShadow, &vecLight, &planeField);

	//���f���̃}�g���b�N�X�Ƃ̊|���Z
	D3DXMatrixMultiply(&m_mtxShadow, &m_mtxShadow, &m_mtxWorld);

	//�e�̕`��
	//�}�e���A���̕`��
	pMat = (D3DXMATERIAL*)m_pPlayerBuffMat[m_nNumber]->GetBufferPointer();

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxShadow);

	for (int i = 0; i < (int)m_nPlayerNumMat[m_nNumber]; i++)
	{
		col[0] = pMat[i].MatD3D.Diffuse;			//�F�̕ۑ�
		col[1] = pMat[i].MatD3D.Emissive;			//�����̕ۑ�

		pMat[i].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pMat[i].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[i].MatD3D);

		//�v���C���[�p�[�c�̕`��
		m_pPlayerMesh[m_nNumber]->DrawSubset(i);

		pMat[i].MatD3D.Diffuse = col[0];
		pMat[i].MatD3D.Emissive = col[1];
	}
}

//===========================
//�e���f���̐ݒ�
//===========================
void CModel::SetParent(CModel * pModel)
{
	m_pParent = pModel;
}

//===========================
//�e���f���̎擾
//===========================
CModel * CModel::GetParent()
{
	return m_pParent;
}

//===========================
//�}�g���b�N�X�̎擾
//===========================
D3DXMATRIX CModel::GetMtx()
{
	return m_mtxWorld;
}

//===========================
//�����̐ݒ�
//===========================
void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//===========================
//�����̉��Z
//===========================
D3DXVECTOR3 CModel::GetRot()
{
	return m_rot;
}

//===========================
//�ʒu�̐ݒ�
//===========================
void CModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
//�ʒu�̎擾
//===========================
D3DXVECTOR3 CModel::GetPos()
{
	return m_pos;
}

float CModel::GetWidth()
{
	 return m_vtxMax.x - m_vtxMin.x ;
}

//===========================
//�ʒu�̎擾
//===========================
void CModel::Siz(int number)
{
	BYTE*pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y

	//���_�o�b�t�@�̃��b�N
	m_pPlayerMesh[number]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//���_���̎擾
	nNumVtx = m_pPlayerMesh[number]->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pPlayerMesh[number]->GetFVF());

	//���_���W�̑��
	//���ׂĂ̒��_��pos���擾����
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);	//�ő�l�̕ۑ��p
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);	//�ŏ��l�̕ۑ��p
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
		//���_���W���r���ă��f���̍ŏ��l�ő�l���擾
		if (vtx.x > vtxMax.x)
		{//X�̍ő�l���擾
			vtxMax.x = vtx.x;
		}
		if (vtx.x < vtxMin.x)
		{//X�̍ŏ��l���擾
			vtxMin.x = vtx.x;
		}
		if (vtx.y > vtxMax.y)
		{//Y�̍ő�l���擾
			vtxMax.y = vtx.y;
		}
		if (vtx.y < vtxMin.y)
		{//Y�̍ŏ��l���擾
			vtxMin.y = vtx.y;
		}
		if (vtx.z > vtxMax.z)
		{//Z�̍ő�l���擾
			vtxMax.z = vtx.z;
		}
		if (vtx.z < vtxMin.z)
		{//Z�̍ŏ��l���擾
			vtxMin.z = vtx.z;
		}

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}
	//���_�o�b�t�@�̃A�����b�N
	m_pPlayerMesh[number]->UnlockVertexBuffer();

	m_vtxMax = vtxMax;							//���_���W�̍ő�l
	m_vtxMin = vtxMin;							//���_���W�̍ŏ��l
}
