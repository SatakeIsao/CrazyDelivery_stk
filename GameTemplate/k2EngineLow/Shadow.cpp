#include "k2EngineLowPreCompile.h"
#include "Shadow.h"


void nsK2EngineLow::Shadow::Init()
{
	InitRenderTarget();
	InitLightCamera();
	SpriteInitData spriteInitData;
	spriteInitData.m_textures[0] = &shadowMap.GetRenderTargetTexture();
	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	spriteInitData.m_width = 256;
	spriteInitData.m_height = 256;
	sprite.Init(spriteInitData);
}

void nsK2EngineLow::Shadow::Render(RenderContext& rc, std::vector<IRenderer*>& renderObjects)
{
	rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
	rc.SetRenderTargetAndViewport(shadowMap);
	rc.ClearRenderTargetView(shadowMap);
	for (auto& renderer : renderObjects)
	{
		renderer->OnRenderShadowMap(rc, g_sceneLight->GetLightData().mLVP);//m_lightCamera.GetViewProjectionMatrix());
	}
	rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);


	

}

void nsK2EngineLow::Shadow::InitRenderTarget()
{
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	shadowMap.Create(
		1024,
		1024,
		1,
		1,
		//DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);
}

void nsK2EngineLow::Shadow::InitLightCamera()
{
	m_lightCamera.SetPosition(0, 600, 0);
	m_lightCamera.SetTarget(0, 0, 0);
	m_lightCamera.SetUp(1, 0, 0);
	m_lightCamera.SetViewAngle(Math::DegToRad(20.0f));
	m_lightCamera.Update();
}

void nsK2EngineLow::Shadow::SpriteShadowDraw(RenderContext& rc)
{
	sprite.Update({ FRAME_BUFFER_W / -2.0f, FRAME_BUFFER_H / 2.0f,  0.0f }, g_quatIdentity, g_vec3One, { 0.0f, 1.0f });
	sprite.Draw(rc);
}
