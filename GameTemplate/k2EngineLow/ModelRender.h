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
		//ディレクションライトの構造体
		struct DirectionLight
		{
			Vector3 direction;
			float pad0;
			//int castShadow = true;
			Vector3 color;
			float pad1;
		};

		//ポイントライトの構造体
		struct PointLight
		{
			Vector3 ptPosition;			// 位置
			float pd2;
			Vector3 ptColor;			// カラー
			float ptRange;				// 影響範囲

		};

		//ライトの構造体
		struct Light
		{
			DirectionLight directionalLight[4];	//ディレクションライトの配列
			PointLight pointLight[4];	//ポイントライト
			Vector3 eyePos;		//視点の位置
			float pad1;
			Vector3 ambientLight;	//環境光
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
		//描画処理
		void Draw(RenderContext& rc);
		
		//シャドウマップへの描画パスから呼ばれる処理
		void OnRenderShadowMap(RenderContext& rc, const Matrix& lvpMatrix)override;
		/// <summary>
		/// モデルの描画
		/// </summary>
		/// <param name="rc">RenderContext</param>
		void OnRenderModel(RenderContext& rc)override;
		void ModelMove();
		//void DirLigMove();
		//void PtLigMove();
		//座標を更新
		void SetPosition(Vector3 pos)
		{
			m_position = pos;
		}
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x,y,z });
		}

		//モデルを取得
		Model& GetModel()
		{
			return m_model;
		}

		//モデルの座標を取得
		Vector3& GetPosition()
		{
			return m_position;
		}

		//モデルのX座標を取得
		float& GetPositionX()
		{
			return m_position.x;
		}

		//モデルのY座標を取得
		float& GetPositionY()
		{
			return m_position.y;
		}

		//モデルのZ座標を取得
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
		Model m_model;					//モデル
		Model m_bgModel;
		Model m_shadowModel;			//シャドウマップ
		Light m_light;					//シェーダーに送るライトの情報
		Vector3 m_position = Vector3::Zero;		//座標
		Quaternion m_rotation;			//回転
		Vector3 m_scale = g_vec3One;	//拡大率
		Shadow m_shadow;
		//RenderTarget m_shadowMap;
		int m_moveState = 0;

		//bool m_isShadowCaster = false;
		/*bool m_syuzinkou = false;
		bool m_flashFlag = false;
		bool m_UVScrollFlag = false;*/
	};
}

