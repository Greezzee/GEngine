#pragma once
#include "GameManager.h"
#include "../Graphics/GraphicManager.h"
#include "../Time/TimeManager.h"
#include "../Control/InputManager.h"
#include <stdio.h>
#include "../SceneManagment/SceneManager.h"

bool GameManager::is_exit;

void GameManager::Init()
{
	GraphicManager::Init();
	TimeManager::Init();
	InputManager::Init();
}

void GameManager::Launch(Scene* start_scene)
{
	is_exit = false;

	// INIT
	//------------------------------
	SceneManager::OpenScene(start_scene);
	//------------------------------

	int frames = 0;
	unsigned sec = 0;
	while (!is_exit) {
		// UPDATE
		//------------------------------
		TimeManager::Update();
		InputManager::Update();
		//------------------------------

		is_exit = is_exit || SceneManager::UpdateScenes();
		is_exit = is_exit || GraphicManager::Update();
		sec += (unsigned)TimeManager::GetDeltaTime();
		frames++;
		if (sec >= 1000000) {
			sec = 0;
			//printf("%d\n", frames);
			frames = 0;
		}
	}

	// DESTROY
	//------------------------------
	SceneManager::Destroy();
	GraphicManager::Exit();
	//------------------------------
}
