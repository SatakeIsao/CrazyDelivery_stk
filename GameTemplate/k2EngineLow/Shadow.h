#pragma once

namespace nsK2EngineLow {

	class Shadow
	{
	public:
		/// <summary>
		/// ������
		/// </summary>
		void Init();
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="renderObjects"></param>
		void Render(
		RenderContext& rc,
		std::vector<IRenderer* >& renderObjects
		);
		/// <summary>
		/// �e�̕`��
		/// </summary>
		/// <param name="rc"�����_�����O�R���e�L�X�g></param>
		void SpriteShadowDraw(RenderContext& rc);

		RenderTarget& GetRenderTarget()
		{
			return shadowMap;
		}

	private:
		/// <summary>
		/// �����_�[�^�[�Q�b�g�̏�����
		/// </summary>
		void InitRenderTarget();
		/// <summary>
		/// �e�`��p�̃J�����̏�����
		/// </summary>
		void InitLightCamera();

		
	private:
		Camera m_lightCamera;
		Vector3 m_lightCameraPos;
		RenderTarget shadowMap;
		Sprite sprite;
	};
}


