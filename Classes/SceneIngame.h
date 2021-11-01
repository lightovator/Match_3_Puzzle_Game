#ifndef __SCENE_INGAME_H__
#define __SCENE_INGAME_H__

#include "stdafx.h"

class SceneIngame : public Scene {
private:

public:
	static SceneIngame* create();
	virtual bool init() override;
	//'override'�� ���� �����ϸ� Scene�̶�� �θ� Ŭ������ init�̶�� �Լ��� �ִ� ����
	//�������̵� �ߴٴ� ���Դϴ�.

	virtual void onEnter() override;
	//��� cocos2d�� ������ onEnter, onExit �� �ݹ� �Լ��� �̸� ������ �ֽ��ϴ�.
	//'onEnter'�� ��尡 � ���� ��忡 �پ ������ �� �� �ڵ����� ������ �Ǵ� �ݹ� �Լ� �Դϴ�.
	//���� Scene ���� ���� ��ü�� �������ָ� ������ ������ �˾Ƽ� ȣ���� �ǰ� �˴ϴ�.

	void initUI();
	void initGame();
	void destroyUI();
	void destroyGame();
	//UI�� Sprite���� ����ų� �����ִ� �Լ����Դϴ�.

	void startGame();
	void pauseGame();
	void winGame();
	void loseGame();

};



#endif