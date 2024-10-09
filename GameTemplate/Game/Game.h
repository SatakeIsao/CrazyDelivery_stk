#pragma once

class BackGround;
class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void PlayerMove();
	void Update();
	void Render(RenderContext& rc);

	//テスト用座標
	Vector3 m_modelTestPos = Vector3::Zero;

	Vector3 m_pointLightTestPos = { 0.0f,20.0f,0.0f };

	Vector3 m_spotLightTestPos = { 0.0f,20.0f,0.0f };
	
	Vector3 m_spotLightTestDirection = { 1.0,-1.0f,1.0f };
	//テストカメラ
	//GameCamera* m_testCamera = nullptr;

	PointLight m_testPointLight;
private:
	ModelRender m_modelPlayer;

	ModelRender m_modelFloor;

	BackGround* m_bgModel = nullptr;

	Vector3 m_position;
};

