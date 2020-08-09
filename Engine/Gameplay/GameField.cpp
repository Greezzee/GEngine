#include "GameField.h"

#include <stdlib.h>
#include <stdio.h>
using namespace ge;
using namespace tge;
void GameField::Init()
{
	_objects = new ObjectsArray;

	_objects->top = 0;
	_objects->to_kill.top = 0;
	for (unsigned i = 0; i < MAX_OBJECTS; i++) {
		_objects->obj[i].obj = nullptr;
		for (int j = 0; j < LAYERS_COUNT; j++)
			_objects->obj[i].layer_pos[j] = -1;
		_objects->to_kill.obj_id[i] = -1;
	}

	for (unsigned i = 0; i < LAYERS_COUNT; i++) {
		_objects->layers[i].top = 0;
		for (unsigned j = 0; j < MAX_OBJECTS; j++)
			_objects->layers[i].obj_id[j] = -1;
	}
}

void GameField::Update()
{
	for (int i = 0; i < _objects->top; i++)
		_objects->obj[i].obj->Update();

	for (int i = 0; i < _objects->top; i++)
		_objects->obj[i].obj->Draw();
	
	ClearKilledObjects();
}

void GameField::Destroy()
{
	for (unsigned i = 0; i < MAX_OBJECTS; i++)
		if (_objects->obj[i].obj != nullptr) {
			KillObject(i);
		}
	ClearKilledObjects();
	delete _objects;
}

void GameField::SpawnObject(const GameobjectSpawnData& new_object)
{
	if (_objects->top < MAX_OBJECTS) {
		_objects->obj[_objects->top].obj = new_object.obj;
		new_object.obj->SetID(_objects->top);
		new_object.obj->SetGamefield(this);
		for (int i = 0; i < LAYERS_COUNT; i++) {
			if (new_object.is_on_layer[i]) {
				_objects->layers[i].obj_id[_objects->layers[i].top] = _objects->top;
				_objects->obj[_objects->top].layer_pos[i] = _objects->layers[i].top;
				_objects->layers[i].top++;
			}
		}
		_objects->top++;
	}
	else
		throw;
}

void GameField::KillObject(const unsigned ID)
{
	if (_objects->to_kill.top < MAX_OBJECTS) {
		_objects->to_kill.obj_id[_objects->to_kill.top] = ID;
		_objects->to_kill.top++;
	}
}

void GameField::KillObject(const GameObject* object)
{
	for (unsigned i = 0; i < _objects->top; i++) {
		if (_objects->obj[i].obj == object) {
			KillObject(i);
			break;
		}
	}
}

void GameField::ClearKilledObjects()
{
	for (unsigned to_delete = 0; to_delete < _objects->to_kill.top; to_delete++) {
		int delete_id = _objects->to_kill.obj_id[to_delete];
		if (delete_id == -1)
			continue;
		for (int i = 0; i < LAYERS_COUNT; i++) //стираем объект со всех слоёв
			if (_objects->obj[delete_id].layer_pos[i] != -1) {
				if (_objects->obj[delete_id].layer_pos[i] == _objects->layers[i].top - 1) {
					_objects->layers[i].top--;
					_objects->layers[i].obj_id[_objects->layers[i].top] = -1;
				}
				else {
					_objects->layers[i].top--;
					unsigned moved_id = _objects->layers[i].obj_id[_objects->layers[i].top];
					_objects->layers[i].obj_id[_objects->obj[delete_id].layer_pos[i]] = moved_id;
					_objects->layers[i].obj_id[_objects->obj[moved_id].layer_pos[i]] = -1;
					_objects->obj[moved_id].layer_pos[i] = _objects->obj[delete_id].layer_pos[i];
				}
			}
		//стираем сам объект
		_objects->obj[delete_id].obj->Destroy();
		delete _objects->obj[delete_id].obj;
		_objects->top--;
		if (delete_id == _objects->top) {
			_objects->obj[delete_id].obj = nullptr;
			for (int i = 0; i < LAYERS_COUNT; i++)
				_objects->obj[delete_id].layer_pos[i] = -1;
		}
		else {
			unsigned moved_id = _objects->top;
			_objects->obj[delete_id] = _objects->obj[moved_id];
			_objects->obj[delete_id].obj->SetID(delete_id);
			for (int i = 0; i < LAYERS_COUNT; i++)
				if (_objects->obj[moved_id].layer_pos[i] != -1)
					_objects->layers[i].obj_id[_objects->obj[moved_id].layer_pos[i]] = delete_id;
			_objects->obj[moved_id].obj = nullptr;
			for (int i = 0; i < LAYERS_COUNT; i++)
				_objects->obj[moved_id].layer_pos[i] = -1;
			for (unsigned i = to_delete + 1; i < _objects->to_kill.top; i++) {
				if (_objects->to_kill.obj_id[i] == moved_id)
					_objects->to_kill.obj_id[i] = delete_id;
				else if (_objects->to_kill.obj_id[i] == delete_id)
					_objects->to_kill.obj_id[i] = -1;
			}
				
		}
	}
	for (unsigned i = 0; i < _objects->to_kill.top; i++)
		_objects->to_kill.obj_id[i] = -1;
	_objects->to_kill.top = 0;

}
