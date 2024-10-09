#pragma once

#include "geometry/AABB.h"
#include "SceneLight.h"
#include "IRenderer.h"
#include "Shadow.h"
namespace nsK2EngineLow
{
	class ModelRender : public IRenderer
	{
	public:
		//�f�B���N�V�������C�g�̍\����
		struct DirectionLight
		{
			Vector3 direction;
			float pad0;
			//int castShadow = true;
			Vector3 color;
			float pad1;
		};

		//�|�C���g���C�g�̍\����
		struct PointLight
		{
			Vector3 ptPosition;			// �ʒu
			float pd2;
			Vector3 ptColor;			// �J���[
			float ptRange;				// �e���͈�

		};

		//���C�g�̍\����
		struct Light
		{
			DirectionLight directionalLight[4];	//�f�B���N�V�������C�g�̔z��
			PointLight pointLight[4];	//�|�C���g���C�g
			Vector3 eyePos;		//���_�̈ʒu
			float pad1;
			Vector3 ambientLight;	//����
		};

		ModelRender();
		~ModelRender();

		
		void Init(
			const char* tkmfilePath,
			AnimationClip* animationClip = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool m_isShadowCaster = true,
			bool m_isShadowReceiver = true
			);

		void InitShadowModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis
		);

		//void InitInstancingShadowDrawModel(
		//	const char* tkmFilePath,
		//	EnModelUpAxis enModelUpAxis);
		void Update();
		//�`�揈��
		void Draw(RenderContext& rc);
		
		//�V���h�E�}�b�v�ւ̕`��p�X����Ă΂�鏈��
		void OnRenderShadowMap(RenderContext& rc, const Matrix& lvpMatrix)override;
		/// <summary>
		/// ���f���̕`��
		/// </summary>
		/// <param name="rc">RenderContext</param>
		void OnRenderModel(RenderContext& rc)override;
		void ModelMove();
		//void DirLigMove();
		//void PtLigMove();
		//���W���X�V
		void SetPosition(Vector3 pos)
		{
			m_position = pos;
		}
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x,y,z });
		}

		//���f�����擾
		Model& GetModel()
		{
			return m_model;
		}

		//���f���̍��W���擾
		Vector3& GetPosition()
		{
			return m_position;
		}

		//���f����X���W���擾
		float& GetPositionX()
		{
			return m_position.x;
		}

		//���f����Y���W���擾
		float& GetPositionY()
		{
			return m_position.y;
		}

		//���f����Z���W���擾
		float& GetPositionZ()
		{
			return m_position.z;
		}

		//bool& GetSyuzinkou()
		//{
		//	return m_syuzinkou;
		//}

		//void OnDraw(RenderContext& rc)
		//{
		//	m_model.Draw(rc, 1, m_flashFlag, m_UVScrollFlag);
		//}

		//void PlayFlash()
		//{
		//	m_flashFlag = true;
		//}

		//void PlayerUVScroll()
		//{
		//	m_UVScrollFlag = true;
		//}
	private:
		Model m_model;					//���f��
		Model m_bgModel;
		Model m_shadowModel;			//�V���h�E�}�b�v
		Light m_light;					//�V�F�[�_�[�ɑ��郉�C�g�̏��
		Vector3 m_position = Vector3::Zero;		//���W
		Quaternion m_rotation;			//��]
		Vector3 m_scale = g_vec3One;	//�g�嗦
		Shadow m_shadow;
		//RenderTarget m_shadowMap;
		int m_moveState = 0;

		//bool m_isShadowCaster = false;
		/*bool m_syuzinkou = false;
		bool m_flashFlag = false;
		bool m_UVScrollFlag = false;*/
	};
}

