#pragma once

class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	bool Start();
	void Init();

	void Update();
	void Render(RenderContext& rc);

	
private:
	ModelRender m_bgModel;	//”wŒiƒ‚ƒfƒ‹
	Vector3 m_position;		//À•W

};

