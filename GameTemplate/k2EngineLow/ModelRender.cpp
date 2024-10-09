#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {

	ModelRender::ModelRender()
	{
		//コンストラクタの処理
	}

	ModelRender::~ModelRender()
	{
		// デストラクタの処理
	}

	void ModelRender::Init(
		const char* tkmfilePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModeluUpAcxis,
		bool isShadowCaster,
		bool isShadowReceiver)
	{
		ModelInitData initData;

		initData.m_tkmFilePath = tkmfilePath;

		initData.m_fxFilePath = "Assets/shader/testModel.fx";
		//initData.m_fxFilePath = "Assets/shader/shadowReceiverModel.fx";


		initData.m_expandConstantBuffer = &g_sceneLight->GetLightData();

		initData.m_expandConstantBufferSize = sizeof(g_sceneLight->GetLightData());
	
		//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する
		initData.m_vsEntryPointFunc = "VSMain";
	
		//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
		initData.m_vsSkinEntryPointFunc = "VSSkinMain";
		
		if (isShadowCaster == true)
		{
			InitShadowModel(tkmfilePath, enModeluUpAcxis);
		}

		if (isShadowReceiver == true)
		{
			initData.m_psEntryPointFunc = "PSShadowReceverMain";

			initData.m_expandShaderResoruceView[0] =
				&(g_renderingEngine->GetShadowMap().GetRenderTargetTexture());
		}
		else
		{
			initData.m_psEntryPointFunc = "PSNormalMain";
		}

		m_model.Init(initData);
	}

	void ModelRender::InitShadowModel(const char* tkmFilePath, EnModelUpAxis modelUpAxis)
	{
		ModelInitData shadowInitData;
		shadowInitData.m_tkmFilePath = tkmFilePath;
		shadowInitData.m_modelUpAxis = modelUpAxis;
		shadowInitData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";
		shadowInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
	
		//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する
		shadowInitData.m_vsEntryPointFunc = "VSMain";
		
		//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
		shadowInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		m_shadowModel.Init(shadowInitData);
	}

	void ModelRender::Update()
	{
		ModelMove();
		/*if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_moveState++;
			m_moveState %= 4;
		}*/

		/*switch (m_moveState)
		{
		case 0:
			ModelMove();
			break;
		case 1:
			break;

		}*/
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		g_renderingEngine->AddRenderObject(this);
	}

	void ModelRender::OnRenderShadowMap(RenderContext& rc, const Matrix& lvpMatrix)
	{
		m_shadowModel.Draw(
			rc,
			g_matIdentity,
			lvpMatrix,
			1
		);
	}

	void ModelRender::OnRenderModel(RenderContext& rc)
	{
		m_model.Draw(rc);
		
	}

	void ModelRender::ModelMove()
	{
		//カメラ基準の処理にしてない

	//モデルの移動
		m_position.y += g_pad[0]->GetLStickXF();
		m_position.x -= g_pad[0]->GetLStickYF();
		//モデルの回転
		m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
		m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);

		if (g_pad[0]->IsPress(enButtonUp)) {
			m_position.y += 0.02f;
		}
		if (g_pad[0]->IsPress(enButtonDown)) {
			m_position.y -= 0.02f;
		}
		if (g_pad[0]->IsPress(enButtonRight)) {
			m_position.x += 0.02f;
		}
		if (g_pad[0]->IsPress(enButtonLeft)) {
			m_position.x -= 0.02f;
		}

		//モデルのワールド行列更新
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_shadowModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	}

}