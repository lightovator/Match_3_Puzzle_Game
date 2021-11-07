#ifndef __SCENE_INGAME_H__
#define __SCENE_INGAME_H__

#include "stdafx.h"
#include "Environment.h"

class SceneIngame : public Scene {
private:
	GameState state; //enum class입니다.

	//컴퓨터 상에서는 순서를 Z, Y, X 순으로 두는 것이 좋습니다.

	//컴퓨터에서 관리되는 블록 데이터입니다.
	//blockData는 0값일 경우 비어있는 블록, 0이 아닌 양수값일 경우 블록이 존재합니다.
	int blockData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	//블록 데이터를 기반으로 화면에 뿌려지는 스프라이트입니다.
	//blockSprite는 nullptr일 경우 비어있습니다.
	Sprite* blockSprite[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	//(새로운 개념)Stack (First In Last Out 규칙을 따르는) 자료 구조를 만듭니다.
	//블록이 파괴되는 알고리즘을 관리하기 위해 필요합니다.

	//실제 스택으로 값이 들어가 있습니다. 
	//judgeData는 자료가 들어가 있는지 스택을 확인만 해주는 배열이므로 헷갈리지 않도록 주의합니다.
	Vec2 judgeStack[128];

	//스택에 있는 자료의 수 입니다.
	int judgeStackCount = 0;

	//0이라면 스택에 자료가 없고, 0이 아니라면 자료가 있습니다.
	int judgeData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];
	// ==

	//블록을 만드는 함수입니다.
	//x, y는 좌표입니다.
	void createBlock(int x, int y, int type);

	//블록의 데이터를 가져와주는 함수입니다. x, y의 순서가 헷갈리지 않도록 블록을 만드는 함수와 따로 구분합니다.
	int getBlockData(int x, int y);

	//블록의 데이터를 데이터에 집어넣어 줍니다.
	void setBlockData(int x, int y, int type);

	//해당 좌표 위에 있는 스프라이트(비트맵)을 가져옵니다.
	Sprite* getBlockSprite(int x, int y);

	//blockSprite 위치에 파라미터의 Sprite* s를 직접 넣어줍니다.
	void setBlockSprite(int x, int y, Sprite* s);

	//해당 위치의 블록을 지워줍니다. blockData의 값을 쓰고 blockSprite를 없애줍니다.
	void destroyBlock(int x, int y);

	//(화면을 클릭했을 때) 게임의 좌표를 블록의 좌표로 변환해주거나 그 반대인 함수입니다.
	Vec2 convertGameCoordToBlockCoord(Vec2 gameCoord);
	Vec2 convertBlockCoordToGameCoord(Vec2 blockCoord);

	//x좌표를 제공하여 해당 열의 y좌표에서부터 빈 칸을 찾아 인덱스를 반환합니다.
	//빈 칸이 끝까지 나타나지 않으면 -1을 리턴합니다.
	int findEmptyBlockYIndex(int x, int y);

	//빈 칸이 발견된 부분(y)부터 위쪽으로 훑으면 다시 채워진 칸을 찾습니다.
	//차있는 칸이 끝까지 나타나지 않으면 -1을 리턴합니다.
	int findFilledBlockYIndex(int x, int y);

	//블록을 떨어뜨리는 함수입니다.
	//가장 하단의 비어있는 칸과 떨어지는 블록의 데이터를 교환합니다.
	void dropBlocks(int x);

	//-----스택 관련 함수-----

	//스택에 자료가 있는지 판단하고 없으면 집어넣는 함수입니다.
	void stackPush(Vec2 value);

	//스택에서 자료를 빼내는 함수입니다.
	Vec2 stackPop();

	//스택을 비워주는 함수입니다.
	void stackEmpty();

	//스택에서 자료가 있는지 없는지 판단하는 함수입니다.
	bool stackFind(Vec2 value);

	//(중요)특정 좌표에서 블록 파괴 조건(3 매치)을 확인해주는 함수입니다.
	void judgeMatch(int x, int y);
	
	//-----스택 관련 함수-----

public:
	static SceneIngame* create();
	virtual bool init() override;
	//'override'는 생략 가능하며 Scene이라는 부모 클래스에 init이라는 함수가 있는 것을
	//오버라이드 했다는 뜻입니다.
	//override는 부모로부터 상속 받은 메소드를 재정의 하는 것을 말합니다.

	virtual void onEnter() override;
	//모든 cocos2d의 노드들은 onEnter, onExit 등 콜백 함수가 미리 정해져 있습니다.
	//'onEnter'는 노드가 어떤 상위 노드에 붙어서 실행이 될 때 자동으로 실행이 되는 콜백 함수 입니다.
	//따라서 Scene 노드는 이제 몸체만 구현해주면 적정한 시점에 알아서 호출이 되게 됩니다.

	//UI와 Sprite등을 만들거나 없애주는 함수들입니다.
	void initUI();
	void initGame();
	void destroyUI();
	void destroyGame();

	//정중앙 배치를 위한 함수입니다.
	void alignBlockSprite();

	//터치가 시작되었을 때, 터치가 이동할 때, 터치가 끝났을 때 각각 호출되는 함수입니다.
	//eventListener에 등록하여 사용합니다.
	bool onTouchBegan(Touch* t, Event* e);
	void onTouchMoved(Touch* t, Event* e);
	void onTouchEnded(Touch* t, Event* e);

	void startGame();
	void pauseGame();
	void winGame();
	void loseGame();

};



#endif