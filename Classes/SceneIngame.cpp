#include "stdafx.h"
#include "SceneIngame.h"
//Precompiled Header�̹Ƿ� ��� cpp������ ù �ٿ� stdafx.h�� �������ݴϴ�.

SceneIngame* SceneIngame::create(){
	//static�Լ��� ��� �Ʒ� ������� autorelease�� ���ݴϴ�.
	auto ret = new SceneIngame();
	if (ret && ret->init())ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;

}

bool SceneIngame::init(){
	//init�� �ݹ� �Լ��Դϴ�.
	if (!Scene::init()) return false;
	//SceneIngame::init�� �����ϴ� ��� SceneIngame::create()�� ��쵵 if �������� ���ؼ� autorelease�ǰ� �޸𸮿��� �����˴ϴ�.
	//����, �������̵� �� �Լ��� �θ��� �Լ�(init)�� ȣ�����ִ� �Ϲ����� ���� �ֽ��ϴ�.
	//�̰������� Scene�� �θ��� �� �� ������ Scene::init�� �����ϴ� ��� �ڵ������� SceneIngame�� ������� ���մϴ�.

	return true;
}

void SceneIngame::onEnter(){
	//onEnter�� �ݹ� �Լ��Դϴ�.
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
