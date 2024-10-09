#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow {
	RenderingEngine::RenderingEngine()
	{
	
	}

	RenderingEngine::~RenderingEngine()
	{
		
	}

	void RenderingEngine::Init()
	{
		//�t���[���o�b�t�@�[�̉����A�������擾
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

		//MainRenderTarget�Z�b�g
		m_mainRenderingTarget.Create(
			frameBuffer_w,	//�e�N�X�`���̕�
			frameBuffer_h,	//�e�N�X�`���̍���
			1,											//Mipmap���x��
			1,											//�e�N�X�`���z��̃T�C�Y
			DXGI_FORMAT_R32G32B32A32_FLOAT,				//�J���[�o�b�t�@�̃t�H�[�}�b�g
			DXGI_FORMAT_D32_FLOAT						//�f�v�X�X�e���V���o�b�t�@�̃t�H�[�}�b�g
		);
		m_spriteInitData.m_textures[0] = &m_mainRenderingTarget.GetRenderTargetTexture();
		m_spriteInitData.m_width = frameBuffer_w;
		m_spriteInitData.m_height = frameBuffer_h;

		m_spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		m_copyToFrameBufferSprite.Init(m_spriteInitData);

		bloom.InitRenderTarget(m_mainRenderingTarget);


		//2D(�t�H���g��X�v���C�g)�p�̏�����
		//Init2DSprite();

		//�u���[���̏�����
		//�P�x���o
		InitBloomLumi();
		//�K�E�V�A���u���[
		InitBloomGauss();
		//�{�P�摜�����Z��������e�N�X�`��
		InitBloomBoke();

		//�V���h�E�̂��߂̏�����
		InitShadowMap();

		//�ŏI�I�ȃe�N�X�`����\��t���邽�߂̃X�v���C�g��������
		//InitFinalSprite();
	}


	void RenderingEngine::InitShadowMap()
	{
		shadow.Init();
	}

	void RenderingEngine::InitBloomLumi()
	{
		bloom.InitLumi(m_mainRenderingTarget);
	}

	void RenderingEngine::InitBloomGauss()
	{
		bloom.InitGaussBlur();
	}

	void RenderingEngine::InitBloomBoke()
	{
		bloom.InitBoke(m_mainRenderingTarget);
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//�e�̕`��
		shadow.Render(rc,m_renderObjects);


		//PreRender2D(rc);
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderingTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderingTarget);
		rc.ClearRenderTargetView(m_mainRenderingTarget);

		//���f���̕`��
		ModelDraw(rc);

		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderingTarget);

		// �u���[��
		//�P�x���o
		bloom.RenderLumi(rc);
		//�K�E�V�A���u���[�����s
		 bloom.RenderGauss(rc);
		//�{�P�摜�����Z����
		///// bloom.RenderBoke(rc, m_mainRenderingTarget);
		// �ŏI�`��
		
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderingTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderingTarget);
		bloom.FinalSpriteDraw(rc);
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderingTarget);
		
		//Render2D(rc);

		//shadowSP.Draw(renderContext);
		
		//����̃X�v���C�g
		// shadow.SpriteShadowDraw(rc);

		//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[
		CopyMainRenderTargetFrameBuffer(rc);
		
		Render2DSprite(rc);
		//�o�^����Ă���`��I�u�W�F�N�g���N���A
		m_renderObjects.clear();
	}

	

	void RenderingEngine::ModelDraw(RenderContext& rc)
	{
		for (auto& renderObj : m_renderObjects)
		{
			renderObj->OnRenderModel(rc);
		}

		////���C���̃^�[�Q�b�g���g����悤�ɂȂ�܂ő҂�
		//rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderingTarget);
		////�^�[�Q�b�g�Z�b�g
		//rc.SetRenderTargetAndViewport(m_mainRenderingTarget);
		////�^�[�Q�b�g�̃N���A
		//rc.ClearRenderTargetView(m_mainRenderingTarget);

		////�܂Ƃ߂ă��f�������_�[��`��
		//for (auto MobjData : ModelRenderObject) {
		//	MobjData->OnDraw(rc);
		//}

		////�`�悳���܂ő҂�
		//rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderingTarget);
	}

	void RenderingEngine::RenderShadowDraw(RenderContext& rc)
	{
		//�e�`��p�̃��C�g�J�������쐬����

		lightCamera.SetAspectOnrFlag(true);

		lightCamera.SetViewAngle(Math::DegToRad(80.0f));

		//�J�����̈ʒu��ݒ�A����̓��C�g�̈ʒu
		lightCamera.SetPosition(-2000, 2000, 2000);

		//�J�����̒����_��ݒ�A����̓��C�g���Ƃ炵�Ă���ꏊ
		lightCamera.SetTarget(0, 0, 0);

		//�������ݒ�A����̓��C�g���^���������Ă���̂ŁAX��������ɂ��Ă���
		lightCamera.SetUp(1, 0, 0);

		//���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���
		lightCamera.Update();

		for (auto& renderObj : m_renderObjects)
		{
			renderObj->OnRenderShadowMap(rc, lightCamera.GetProjectionMatrix());
		}

		////�^�[�Q�b�g���V���h�E�}�b�v�ɕύX
		//rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMapTarget);
		//rc.SetRenderTargetAndViewport(m_shadowMapTarget);
		//rc.ClearRenderTargetView(m_shadowMapTarget);

		////�܂Ƃ߂ĉe���f�������_�[��`��
		//for (auto MobjData : ModelRenderObject)
		//{
		//	//��l���Ȃ烉�C�g�J�������X�V
		//	if (MobjData->GetSyuzinkou() == true) {
		//		//���C�g�J�����̍X�V
		//		lightCamera.SetPosition(MobjData->GetPositionX(), MobjData->GetPositionY() + 5000.0f, MobjData->GetPositionZ());
		//		lightCamera.SetTarget(MobjData->GetPositionX(), MobjData->GetPositionY(), MobjData->GetPositionZ());
		//		lightCamera.Update();
		//	}
		//	//���C�g�r���[�Z�b�g
		//	SetLVP
		//	//MobjData->OnRenderShadowMap(rc, lightCamera, GetViewProjectionMatrix());
		//}
	}

	/*void RenderingEngine::InitFinalSprite()
	{
		m_spiteInitData.m_textures[0] = &m_mainRenderingTarget.GetRenderTargetTexture();
		m_spiteInitData.m_width = m_mainRenderingTarget.GetWidth();
		m_spiteInitData.m_height = m_mainRenderingTarget.GetHeight();
		m_spiteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		m_copyToframeBufferSprite.Init(m_spiteInitData);
	}*/

	void RenderingEngine::CopyMainRenderTargetFrameBuffer(RenderContext& rc)
	{
		//�t���[���o�b�t�@���Z�b�g
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
		//bloom.GetCopyToFrameBuffer().Draw(rc);
		m_copyToFrameBufferSprite.Draw(rc);
	}

	//2D�`�揈��
	void RenderingEngine::Render2DSprite(RenderContext& rc)
	{
		for (auto& renderObj : m_renderObjects)
		{
			renderObj->OnRender2D(rc);
		}
	}
}