//=================================================
// Content     (�t�F�[�h�̐ݒ�)(fade.cpp)
// Author     : �L�c����
// Since      : (�쐬��2021/08/29)
//=================================================
#include "fade.h"

 //===========================
 // �R���X�g���N�^
 //===========================
 CFade::CFade()
 {

 }

 //===========================
 // �f�X�g���N�^
 //===========================
 CFade::~CFade()
 {

 }

 //===========================
 // ������
 //===========================
 HRESULT CFade::Init(CApplication::MODE modeNext)
 {
	 //�񋓌^�̏�����
	 m_fade = FADE_IN;		//�t�F�[�h�C�����
	 m_modeNext = modeNext;	//���̉��(���[�h)��ݒ�
	 m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//�����|���S��(�s����)�ɂ��Ă���

	 CreateFade();

	 return S_OK;
 }

 //===========================
 // �I��
 //===========================
 void CFade::Uninit()
 {
	
 }

 //===========================
 // �X�V
 //===========================
 void CFade::Update()
 {
	 if (m_fade != FADE_NONE)
	 {
		 if (m_fade == FADE_IN)
		 {//�t�F�[�h�C�����
			 m_col.a -= 0.02f;	//�|���S���𓧖��ɂ��Ă���

			 if (m_col.a <= 0.0f)
			 {
				 m_col.a = 0.0f;
				 m_fade = FADE_NONE;	//�������Ă��Ȃ���Ԃ�
			 }
		 }
		 else if (m_fade == FADE_OUT)
		 {//�t�F�[�h�A�E�g���
			 m_col.a += 0.02f;	//�|���S����s�����ɂ��Ă���

			 if (m_col.a >= 1.0f)
			 {
				 m_col.a = 1.0f;
				 m_fade = FADE_IN;	//�t�F�[�h�C����Ԃ�

				 CApplication::SetMode(m_modeNext);
				
				 CreateFade();
			 }
		 }
	 }
	 m_pObject->SetCol(m_col);
	 m_pObject->Update();		//���ɒu���Ȃ��Ɛ������ꂽ��u���������̂܂܂ɂȂ��Ă��܂�
 }

 //===========================
 // �`��
 //===========================
 void CFade::Draw()
 {
	 m_pObject->Draw();
 }

 //===========================
 // �ݒ�
 //===========================
 void CFade::SetFade(CApplication::MODE modeNext)
 {
	 if (m_fade != FADE_OUT)
	 {
		 m_fade = FADE_OUT;		//�t�F�[�h�A�E�g��Ԃ�
		 m_modeNext = modeNext;	//���̉�ʂ�ݒ�
		 m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	 }
 }

 void CFade::CreateFade()
 {
	 //
	 m_pObject = new CObject2D(CObject::OBJTYPE_FADE);

	 m_pObject->Init();

	 m_pObject->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	 m_pObject->SetSiz(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));

	 m_pObject->SetCol(m_col);

	 // m_pObject->SetTexture(CTexture::TEXTURE_NONE);	//�e�N�X�`���̐ݒ�
 }
