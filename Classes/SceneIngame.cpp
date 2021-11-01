#include "stdafx.h"
#include "SceneIngame.h"
//Precompiled Header이므로 모든 cpp파일의 첫 줄에 stdafx.h를 포함해줍니다.

SceneIngame* SceneIngame::create(){
	//static함수의 경우 아래 방식으로 autorelease를 해줍니다.
	auto ret = new SceneIngame();
	if (ret && ret->init())ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;

}

bool SceneIngame::init(){
	//init은 콜백 함수입니다.
	if (!Scene::init()) return false;
	//SceneIngame::init이 실패하는 경우 SceneIngame::create()의 경우도 if 조건으로 인해서 autorelease되고 메모리에서 삭제됩니다.
	//또한, 오버라이드 된 함수는 부모의 함수(init)을 호출해주는 암묵적인 룰이 있습니다.
	//이곳에서는 Scene이 부모라고 할 수 있으며 Scene::init이 실패하는 경우 자동적으로 SceneIngame도 실행되지 못합니다.

	return true;
}

void SceneIngame::onEnter(){
	//onEnter는 콜백 함수입니다.
}

void SceneIngame::initUI(){
}

void SceneIngame::initGame(){
}

void SceneIngame::destroyUI(){
}

void SceneIngame::destroyGame(){
}

void SceneIngame::startGame(){
}

void SceneIngame::pauseGame()
{
}

void SceneIngame::winGame(){
}

void SceneIngame::loseGame(){
}
