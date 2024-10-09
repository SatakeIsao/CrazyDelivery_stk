#pragma once
#include "Shadow.h"
#include "Bloom.h"
namespace nsK2EngineLow {

	class Bloom;

	class RenderingEngine : public Noncopyable
	{
	public:
		RenderingEngine();
		~RenderingEngine();

		void Init();
		void InitShadowMap();
		void InitBloomLumi();
		void InitBloomGauss();
		void InitBloomBoke();
		
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.emplace_back(renderObject);
		}

		//void InitFinalSprite();
		//モデルの描画
		void ModelDraw(RenderContext& rc);
		//2Dモデルの描画
		void Render2DSprite(RenderContext& rc);
		//事前2Dモデルの描画
		//void PreRender2D(RenderContext& rc);
		//シャドウマップ描画処理
		void RenderShadowDraw(RenderContext& rc);
		//実行
		void Execute(RenderContext& rc);

		void CopyMainRenderTargetFrameBuffer(RenderContext& rc);

		////ライトビュースクリーンの設定
		//void SetLVP(Matrix mat)
		//{
		//	m_sceneLight.SetLVP(mat);
		//}
		//取得系の関数
		SceneLight& GetLightingCB()
		{
			return m_sceneLight;
		}


		RenderTarget& GetShadowMap()
		{
			return shadow.GetRenderTarget();
		}

		RenderTarget& GetBloom()
		{
			return bloom.GetRenderTarget();
		}

	private:
		SceneLight m_sceneLight;
		RenderTarget m_mainRenderingTarget;
		//シャドウ用
		//RenderTarget m_shadowMapTarget;
		Camera lightCamera;
		Shadow shadow;
		//ブルーム用
		Bloom bloom;
		//RenderTarget* luminanceRenderTarget;
		Sprite m_copyToFrameBufferSprite;
		SpriteInitData m_spriteInitData;

		std::vector<ModelRender*> ModelRenderObject;
		std::vector<IRenderer* > m_renderObjects;	//描画オブジェクトのリスト
	};

}

