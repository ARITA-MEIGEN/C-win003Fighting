//=============================================================================
//
// ���[�V�����ǂݍ���
// Author : �L�c����
//
//=============================================================================

#include"Motion.h"

CMotion::CMotion(int nPriority)
{
}

CMotion::~CMotion()
{
}

HRESULT CMotion::Init()
{
	return E_NOTIMPL;
}

void CMotion::Uninit()
{
}

void CMotion::Update()
{
	
}

void CMotion::Read(char* Filename, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int Frame;
	int DiffF;
	m_NowKey;
	m_MotionKey[m_NowKey];

	//�t�@�C���ǂݍ���

	for (int i = 0; i < MAX_PARTS; i++)
	{//���f���p�[�c�������[�v
		if (m_MotionKey[i].aKey != nullptr)
		{
			//���f���̈ʒu��ݒ�
			//���f���̌�����ݒ�
		}
	}

	{
		{//�p�[�c�����p�ӂ���
			DiffF = Frame / m_MotionKey[0].nFrame;	//���݂̃t���[��(�Đ�����)/�S�̃t���[��=����

			{
				D3DXVECTOR3 Diffpos = pos - m_MotionKey[m_NowKey].aKey[0].pos;//���ݒn�ƖړI�n�̍����𐶐�
				D3DXVECTOR3 Diffrot = rot - m_MotionKey[m_NowKey].aKey[0].rot;//���ݒn�ƖړI�n�̍����𐶐�

				Diffpos /= Frame;
				Diffrot /= Frame;
			}		//���ݒn
			{
				m_MotionKey[m_NowKey].aKey[0].pos;
				m_MotionKey[m_NowKey].aKey[0].rot;	//�ړI�n
			}
		}

		{
			//�t���[�����A
			{
				pos; rot;
			}		//�p�[�c�����p�ӂ���
		}


	}
}
