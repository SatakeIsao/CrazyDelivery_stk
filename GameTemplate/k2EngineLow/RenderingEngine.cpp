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
		//フレームバッファーの横幅、高さを取得
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

		//MainRenderTargetセット
		m_mainRenderingTarget.Create(
			frameBuffer_w,	//テクスチャの幅
			frameBuffer_h,	//テクスチャの高さ
			1,											//Mipmapレベル
			1,											//テクスチャ配列のサイズ
			DXGI_FORMAT_R32G32B32A32_FLOAT,				//カラーバッファのフォーマット
			DXGI_FORMAT_D32_FLOAT						//デプスステンシルバッファのフォーマット
		);
		m_spriteInitData.m_textures[0] = &m_mainRenderingTarget.GetRenderTargetTexture();
		m_spriteInitData.m_width = frameBuffer_w;
		m_spriteInitData.m_height = frameBuffer_h;

		m_spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		m_copyToFrameBufferSprite.Init(m_spriteInitData);

		bloom.InitRenderTarget(m_mainRenderingTarget);


		//2D(フォントやスプライト)用の初期化
		//Init2DSprite();

		//ブルームの初期化
		//輝度抽出
		InitBloomLumi();
		//ガウシアンブラー
		InitBloomGauss();
		//ボケ画像を加算合成するテクスチャ
		InitBloomBoke();

		//シャドウのための初期化
		InitShadowMap();

		//最終的なテクスチャを貼り付けるためのスプライトを初期化
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
		//影の描画
		shadow.Render(rc,m_renderObjects);


		//PreRender2D(rc);
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderingTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderingTarget);
		rc.ClearRenderTargetView(m_mainRenderingTarget);

		//モデルの描画
		ModelDraw(rc);

		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderingTarget);

		// ブルーム
		//輝度抽出
		bloom.RenderLumi(rc);
		//ガウシアンブラーを実行
		 bloom.RenderGauss(rc);
		//ボケ画像を加算合成
		///// bloom.RenderBoke(rc, m_mainRenderingTarget);
		// 最終描画
		
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderingTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderingTarget);
		bloom.FinalSpriteDraw(rc);
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderingTarget);
		
		//Render2D(rc);

		//shadowSP.Draw(renderContext);
		
		//左上のスプライト
		// shadow.SpriteShadowDraw(rc);

		//メインレンダリングターゲットの絵をフレームバッファにコピー
		CopyMainRenderTargetFrameBuffer(rc);
		
		Render2DSprite(rc);
		//登録されている描画オブジェクトをクリア
		m_renderObjects.clear();
	}

	

	void RenderingEngine::ModelDraw(RenderContext& rc)
	{
		for (auto& renderObj : m_renderObjects)
		{
			renderObj->OnRenderModel(rc);
		}

		////メインのターゲットが使えるようになるまで待つ
		//rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderingTarget);
		////ターゲットセット
		//rc.SetRenderTargetAndViewport(m_mainRenderingTarget);
		////ターゲットのクリア
		//rc.ClearRenderTargetView(m_mainRenderingTarget);

		////まとめてモデルレンダーを描画
		//for (auto MobjData : ModelRenderObject) {
		//	MobjData->OnDraw(rc);
		//}

		////描画されるまで待つ
		//rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderingTarget);
	}

	void RenderingEngine::RenderShadowDraw(RenderContext& rc)
	{
		//影描画用のライトカメラを作成する

		lightCamera.SetAspectOnrFlag(true);

		lightCamera.SetViewAngle(Math::DegToRad(80.0f));

		//カメラの位置を設定、これはライトの位置
		lightCamera.SetPosition(-2000, 2000, 2000);

		//カメラの注視点を設定、これはライトが照らしている場所
		lightCamera.SetTarget(0, 0, 0);

		//上方向を設定、今回はライトが真下を向いているので、X方向を上にしている
		lightCamera.SetUp(1, 0, 0);

		//ライトビュープロジェクション行列を計算している
		lightCamera.Update();

		for (auto& renderObj : m_renderObjects)
		{
			renderObj->OnRenderShadowMap(rc, lightCamera.GetProjectionMatrix());
		}

		////ターゲットをシャドウマップに変更
		//rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMapTarget);
		//rc.SetRenderTargetAndViewport(m_shadowMapTarget);
		//rc.ClearRenderTargetView(m_shadowMapTarget);

		////まとめて影モデルレンダーを描画
		//for (auto MobjData : ModelRenderObject)
		//{
		//	//主人公ならライトカメラを更新
		//	if (MobjData->GetSyuzinkou() == true) {
		//		//ライトカメラの更新
		//		lightCamera.SetPosition(MobjData->GetPositionX(), MobjData->GetPositionY() + 5000.0f, MobjData->GetPositionZ());
		//		lightCamera.SetTarget(MobjData->GetPositionX(), MobjData->GetPositionY(), MobjData->GetPositionZ());
		//		lightCamera.Update();
		//	}
		//	//ライトビューセット
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
		//フレームバッファもセット
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
		//bloom.GetCopyToFrameBuffer().Draw(rc);
		m_copyToFrameBufferSprite.Draw(rc);
	}

	//2D描画処理
	void RenderingEngine::Render2DSprite(RenderContext& rc)
	{
		for (auto& renderObj : m_renderObjects)
		{
			renderObj->OnRender2D(rc);
		}
	}
}