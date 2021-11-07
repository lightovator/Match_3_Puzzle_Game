#ifndef __SCENE_INGAME_H__
#define __SCENE_INGAME_H__

#include "stdafx.h"
#include "Environment.h"

class SceneIngame : public Scene {
private:
	GameState state; //enum class�Դϴ�.

	//��ǻ�� �󿡼��� ������ Z, Y, X ������ �δ� ���� �����ϴ�.

	//��ǻ�Ϳ��� �����Ǵ� ��� �������Դϴ�.
	//blockData�� 0���� ��� ����ִ� ���, 0�� �ƴ� ������� ��� ����� �����մϴ�.
	int blockData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	//��� �����͸� ������� ȭ�鿡 �ѷ����� ��������Ʈ�Դϴ�.
	//blockSprite�� nullptr�� ��� ����ֽ��ϴ�.
	Sprite* blockSprite[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	//(���ο� ����)Stack (First In Last Out ��Ģ�� ������) �ڷ� ������ ����ϴ�.
	//����� �ı��Ǵ� �˰����� �����ϱ� ���� �ʿ��մϴ�.

	//���� �������� ���� �� �ֽ��ϴ�. 
	//judgeData�� �ڷᰡ �� �ִ��� ������ Ȯ�θ� ���ִ� �迭�̹Ƿ� �򰥸��� �ʵ��� �����մϴ�.
	Vec2 judgeStack[128];

	//���ÿ� �ִ� �ڷ��� �� �Դϴ�.
	int judgeStackCount = 0;

	//0�̶�� ���ÿ� �ڷᰡ ����, 0�� �ƴ϶�� �ڷᰡ �ֽ��ϴ�.
	int judgeData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];
	// ==

	//����� ����� �Լ��Դϴ�.
	//x, y�� ��ǥ�Դϴ�.
	void createBlock(int x, int y, int type);

	//����� �����͸� �������ִ� �Լ��Դϴ�. x, y�� ������ �򰥸��� �ʵ��� ����� ����� �Լ��� ���� �����մϴ�.
	int getBlockData(int x, int y);

	//����� �����͸� �����Ϳ� ����־� �ݴϴ�.
	void setBlockData(int x, int y, int type);

	//�ش� ��ǥ ���� �ִ� ��������Ʈ(��Ʈ��)�� �����ɴϴ�.
	Sprite* getBlockSprite(int x, int y);

	//blockSprite ��ġ�� �Ķ������ Sprite* s�� ���� �־��ݴϴ�.
	void setBlockSprite(int x, int y, Sprite* s);

	//�ش� ��ġ�� ����� �����ݴϴ�. blockData�� ���� ���� blockSprite�� �����ݴϴ�.
	void destroyBlock(int x, int y);

	//(ȭ���� Ŭ������ ��) ������ ��ǥ�� ����� ��ǥ�� ��ȯ���ְų� �� �ݴ��� �Լ��Դϴ�.
	Vec2 convertGameCoordToBlockCoord(Vec2 gameCoord);
	Vec2 convertBlockCoordToGameCoord(Vec2 blockCoord);

	//x��ǥ�� �����Ͽ� �ش� ���� y��ǥ�������� �� ĭ�� ã�� �ε����� ��ȯ�մϴ�.
	//�� ĭ�� ������ ��Ÿ���� ������ -1�� �����մϴ�.
	int findEmptyBlockYIndex(int x, int y);

	//�� ĭ�� �߰ߵ� �κ�(y)���� �������� ������ �ٽ� ä���� ĭ�� ã���ϴ�.
	//���ִ� ĭ�� ������ ��Ÿ���� ������ -1�� �����մϴ�.
	int findFilledBlockYIndex(int x, int y);

	//����� ����߸��� �Լ��Դϴ�.
	//���� �ϴ��� ����ִ� ĭ�� �������� ����� �����͸� ��ȯ�մϴ�.
	void dropBlocks(int x);

	//-----���� ���� �Լ�-----

	//���ÿ� �ڷᰡ �ִ��� �Ǵ��ϰ� ������ ����ִ� �Լ��Դϴ�.
	void stackPush(Vec2 value);

	//���ÿ��� �ڷḦ ������ �Լ��Դϴ�.
	Vec2 stackPop();

	//������ ����ִ� �Լ��Դϴ�.
	void stackEmpty();

	//���ÿ��� �ڷᰡ �ִ��� ������ �Ǵ��ϴ� �Լ��Դϴ�.
	bool stackFind(Vec2 value);

	//(�߿�)Ư�� ��ǥ���� ��� �ı� ����(3 ��ġ)�� Ȯ�����ִ� �Լ��Դϴ�.
	void judgeMatch(int x, int y);
	
	//-----���� ���� �Լ�-----

public:
	static SceneIngame* create();
	virtual bool init() override;
	//'override'�� ���� �����ϸ� Scene�̶�� �θ� Ŭ������ init�̶�� �Լ��� �ִ� ����
	//�������̵� �ߴٴ� ���Դϴ�.
	//override�� �θ�κ��� ��� ���� �޼ҵ带 ������ �ϴ� ���� ���մϴ�.

	virtual void onEnter() override;
	//��� cocos2d�� ������ onEnter, onExit �� �ݹ� �Լ��� �̸� ������ �ֽ��ϴ�.
	//'onEnter'�� ��尡 � ���� ��忡 �پ ������ �� �� �ڵ����� ������ �Ǵ� �ݹ� �Լ� �Դϴ�.
	//���� Scene ���� ���� ��ü�� �������ָ� ������ ������ �˾Ƽ� ȣ���� �ǰ� �˴ϴ�.

	//UI�� Sprite���� ����ų� �����ִ� �Լ����Դϴ�.
	void initUI();
	void initGame();
	void destroyUI();
	void destroyGame();

	//���߾� ��ġ�� ���� �Լ��Դϴ�.
	void alignBlockSprite();

	//��ġ�� ���۵Ǿ��� ��, ��ġ�� �̵��� ��, ��ġ�� ������ �� ���� ȣ��Ǵ� �Լ��Դϴ�.
	//eventListener�� ����Ͽ� ����մϴ�.
	bool onTouchBegan(Touch* t, Event* e);
	void onTouchMoved(Touch* t, Event* e);
	void onTouchEnded(Touch* t, Event* e);

	void startGame();
	void pauseGame();
	void winGame();
	void loseGame();

};



#endif