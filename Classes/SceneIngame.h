#ifndef __SCENE_INGAME_H__
#define __SCENE_INGAME_H__

#include "stdafx.h"

class SceneIngame : public Scene {
private:

public:
	static SceneIngame* create();
	virtual bool init() override;
	//'override'는 생략 가능하며 Scene이라는 부모 클래스에 init이라는 함수가 있는 것을
	//오버라이드 했다는 뜻입니다.

	virtual void onEnter() override;
	//모든 cocos2d의 노드들은 onEnter, onExit 등 콜백 함수가 미리 정해져 있습니다.
	//'onEnter'는 노드가 어떤 상위 노드에 붙어서 실행이 될 때 자동으로 실행이 되는 콜백 함수 입니다.
	//따라서 Scene 노드는 이제 몸체만 구현해주면 적정한 시점에 알아서 호출이 되게 됩니다.

	void initUI();
	void initGame();
	void destroyUI();
	void destroyGame();
	//UI와 Sprite등을 만들거나 없애주는 함수들입니다.

	void startGame();
	void pauseGame();
	void winGame();
	void loseGame();

};



#endif