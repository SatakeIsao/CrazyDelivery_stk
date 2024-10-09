#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{

}

BackGround::~BackGround()
{

}

bool BackGround::Start()
{
	Init();

	return true;
}

void BackGround::Init()
{
	m_position.Set(0.0f,0.0f,0.0f);
	m_bgModel.Init("Assets/modelData/bg.tkm",0,0,enModelUpAxisY,false);
	m_bgModel.SetPosition(m_position);
	m_bgModel.Update();
}


void BackGround::Update()
{
	m_bgModel.Update();
}

void BackGround::Render(RenderContext& rc)
{
	//m_bgModel.Draw(rc);
}
