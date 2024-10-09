#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"

Game::Game()
{
	
}

Game::~Game()
{

}

bool Game::Start()
{
	m_modelPlayer.Init("Assets/modelData/unityChan.tkm",
		0, 0, enModelUpAxisY, true,false);

	m_bgModel = NewGO<BackGround>(0, "background");
	Vector3 testpos = { 0.0f,0.0f,0.0f };
	m_modelPlayer.SetPosition(Vector3::Zero);
	//m_modelFloor.SetPosition(Vector3::Zero);
	return true;
}

void Game::PlayerMove()
{
	m_position.x = g_pad[0]->GetLStickXF();
	m_position.y = g_pad[0]->GetLStickYF();

	m_modelPlayer.SetPosition(m_position);

	m_modelPlayer.Update();
}

void Game::Update()
{
	m_modelTestPos = Vector3::Zero;
	PlayerMove();
	m_modelPlayer.Update();
	//m_modelFloor.Update();
	//m_testPointLight.SetAffectPowParam(0.5f);
	//Rotation();

}

//void Game::Rotation()
//{
//	Quaternion qRot;
//	if (g_pad[0]->IsPress(enButtonRight))
//	{
//		qRot.SetRotationDegY(1.0f);
//	}
//
//	else if (g_pad[0]->IsPress(enButtonLeft))
//	{
//		qRot.SetRotationDegY(-1.0f);
//	}
//
//	//qRot.Apply(light.direction);
//}


void Game::Render(RenderContext& rc)
{
	m_modelPlayer.Draw(rc);
}
