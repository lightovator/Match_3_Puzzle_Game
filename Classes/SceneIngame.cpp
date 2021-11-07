#include "stdafx.h"
#include "SceneIngame.h"
//Precompiled Header�̹Ƿ� ��� cpp������ ù �ٿ� stdafx.h�� �������ݴϴ�.

void SceneIngame::createBlock(int x, int y, int type){
	auto cache = Director::getInstance()->getTextureCache();

	//Texture�� ���� ���� Rect�� �»���� �����Դϴ�. (Rect ���)���� ������ Ư���ϰ� ���ϴ��� �����Դϴ�.
	//(x ��ǥ, y��ǥ(�κ���), ����, ����)�� ���簢������ �߶� �����ɴϴ�.
	auto spr = Sprite::createWithTexture(
		cache->getTextureForKey("res/match3_tiles_px.png"),
		Rect(0 + (type * 40), 0, 40, 40));
	spr->setScale(2.0);
	addChild(spr);

	//��ǥ�� �򰥸��� �ʵ��� ���� �迭�� �����ϴ� ���� �ƴ϶� �Լ��� ���ؼ� �������ݴϴ�.
	setBlockData(x, y, type);
	setBlockSprite(x, y, spr);
}

int SceneIngame::getBlockData(int x, int y){
	return blockData[y][x];
}

void SceneIngame::setBlockData(int x, int y, int type){
	blockData[y][x] = type;
}

Sprite* SceneIngame::getBlockSprite(int x, int y){
	return blockSprite[y][x];
}

void SceneIngame::setBlockSprite(int x, int y, Sprite* s){
	blockSprite[y][x] = s;
}

void SceneIngame::destroyBlock(int x, int y){
	//blockData�� 0�̶�� Sprite�� ���� ������ removeFromParent()�� �Ұ����մϴ�.
	if (blockData[y][x] != 0) {
		blockSprite[y][x]->removeFromParent();
		blockSprite[y][x] = nullptr;
		blockData[y][x] = 0;
	}
}

Vec2 SceneIngame::convertGameCoordToBlockCoord(Vec2 gameCoord){
	//��� offset(�׸��� ���߾�)�� ��������, ��� ���ϴ��� ����(blockOrigin)���μ� �����մϴ�.
	Vec2 blockOrigin = BLOCK_OFFSET 
		- Vec2((BLOCK_HORIZONTAL * BLOCK_WIDTH) / 2, (BLOCK_VERTICAL * BLOCK_HEIGHT) / 2)
		+ Vec2(BLOCK_WIDTH / 2, BLOCK_HEIGHT / 2);
	Vec2 delta = gameCoord - blockOrigin;

	//0.5�� �ݿø��� ���� ��ġ�Դϴ�.
	//(�߿�) �Ϲ������� (int)(x+0.5)��� �������� ����� �ݿø��� ���� �����մϴ�.
	//(�߿�) int ����ȯ�� �����ϴ� ��� �Ҽ��� �Ʒ��� �����˴ϴ�.
	Vec2 pos = Vec2((int)(delta.x / BLOCK_WIDTH + 0.5), (int)(delta.y / BLOCK_HEIGHT + 0.5));

	return pos;
}

Vec2 SceneIngame::convertBlockCoordToGameCoord(Vec2 blockCoord){
	Vec2 blockOrigin = BLOCK_OFFSET 
		- Vec2((BLOCK_HORIZONTAL * BLOCK_WIDTH) / 2, (BLOCK_VERTICAL * BLOCK_HEIGHT) / 2)
		+ Vec2(BLOCK_WIDTH / 2, BLOCK_HEIGHT / 2);
	return blockOrigin + Vec2(blockCoord.x * BLOCK_WIDTH, blockCoord.y * BLOCK_HEIGHT);
}

int SceneIngame::findEmptyBlockYIndex(int x, int y){
	//y�� 0�� ������ �ش� ���� �ٴ� �κк��� ã�� �� �ֽ��ϴ�.
	for (int i = y; i < BLOCK_VERTICAL; i++) {
		if (getBlockData(x, i) == 0) return i;
	}
	return -1;
}

int SceneIngame::findFilledBlockYIndex(int x, int y)
{
	for (int i = y; i < BLOCK_VERTICAL; i++) {
		if (getBlockData(x, i) != 0) return i;
	}
	return -1;
}

void SceneIngame::dropBlocks(int x){
	//�� ���, �׸��� �� ��� ���ʿ� ���� ����� �� ����� ��ȯ���ݴϴ�.
	//for ������ �̰��� �ݺ����ָ� ��Ʈ���� ����ó�� ����� ����߸� �� �ֽ��ϴ�.

	for (int i = findEmptyBlockYIndex(x, 0); i < BLOCK_VERTICAL; i++) {
		int empty_y = findEmptyBlockYIndex(x, i);
		if (empty_y == -1) continue;
		int filled_y = findFilledBlockYIndex(x, empty_y + 1);
		if (filled_y == -1) continue;

		//���� a, b�� ��� ����ϱ� ���� ���ȣ�� ���Դϴ�.
		//���� BlockData�� BlockSprite�� ���� ������ �����ǰ� ������ ������� �����ϴ�.
		{
			int a = getBlockData(x, empty_y);
			int b = getBlockData(x, filled_y);
			SWAP(int, a, b);
			setBlockData(x, empty_y, a);
			setBlockData(x, filled_y, b);
		} {
			Sprite* a = getBlockSprite(x, empty_y);
			Sprite* b = getBlockSprite(x, filled_y);
			SWAP(Sprite*, a, b);
			setBlockSprite(x, empty_y, a);
			setBlockSprite(x, filled_y, b);
		}
	}

	//alignBlockSprite�� ������ ������ blockSprite�� �����Ϳ��� ���������
	//�ǹ��� ��Ÿ������ �ʱ� ������ �Լ��� ȣ�����־�� �մϴ�.
	alignBlockSprite();
}

//������ ����ִ��� judgeData�� Ȯ���� �� ��� ������ ���� ���� �ֽ��ϴ�.
void SceneIngame::stackPush(Vec2 value) {
	if (judgeData[(int)value.y][(int)value.x] != 0) return;
	judgeStack[judgeStackCount++] = value;
	judgeData[(int)value.y][(int)value.x] = 1;
}

Vec2 SceneIngame::stackPop() {
	auto ret = judgeStack[--judgeStackCount];
	judgeData[(int)ret.y][(int)ret.x] = 0;
	return ret;
}

void SceneIngame::stackEmpty() {
	judgeStackCount = 0;
	for (int i = 0; i < BLOCK_HORIZONTAL; i++){
		for (int k = 0; k < BLOCK_VERTICAL; k++) {
			judgeData[k][i] = 0;
		}
	}
	//(����) judgeStack�� ���� ����� �ʾҽ��ϴ�.
}

bool SceneIngame::stackFind(Vec2 value){
	return judgeData[(int)value.y][(int)value.x] == 1;
}

void SceneIngame::judgeMatch(int x, int y){
	int blockData = getBlockData(x, y);

	if (blockData == 0) return;

	stackPush(Vec2(x, y));

	int push_cnt = 0;

	for (int i = 0; i < 4; i++) {

		int next_x = x;
		int next_y = y;
		int inc_x;
		int inc_y;

		switch (i) {
		case 0: inc_x = 1; inc_y = 0; push_cnt = 0; break;
		case 1: inc_x = -1; inc_y = 0; break;
		case 2: inc_x = 0; inc_y = 1; push_cnt = 0; break;
		case 3: inc_x = 0; inc_y = -1; break;
		}

		while (true) {
			next_x += inc_x;
			next_y += inc_y;

			if (next_x < 0 || next_x >= BLOCK_HORIZONTAL) break;
			if (next_y < 0 || next_y >= BLOCK_VERTICAL) break;
			if (getBlockData(next_x, next_y) == blockData) {
				stackPush(Vec2(next_x, next_y));
				push_cnt++;
			} else break;
		}

		if (i % 2 == 0) continue;
		if (push_cnt < 2) {
			for (int k = 0; k < push_cnt; k++) {
				stackPop();
			}
		}
	}

	if (judgeStackCount > 1) {
		//�Ʒ��� for������ �ϸ� judgeStackCount�� ��� ���ϱ� ������ �ǵ��� ����� ���� �� �����ϴ�.
		//for (int i = 0; i < judgeStackCount; i++) {
		while (judgeStackCount > 0){
			Vec2 p = stackPop();
			destroyBlock(p.x, p.y);
		}
	}
	stackEmpty();

	/* int blockData = getBlockData(x, y);

	if (blockData == 0) return;
	
	//���� ����� ���� �ȿ� �־��ݴϴ�.
	stackPush(Vec2(x, y));

	int push_cnt = 0;

	//�������� 4������ ����ϱ� ���ϵ��� ������ ���ϴ�.
	for (int i = 0; i < 4; i++) {
		int next_x = x; //x�� �������� ���� ����
		int next_y = y; //y�� �������� ���� ����
		int inc_x; //x�� ������
		int inc_y; //y�� ������

		switch (i) {
		case 0:inc_x = 1; inc_y = 0; push_cnt = 0; break;
		case 1:inc_x = -1; inc_y = 0; break;
		case 2:inc_x = 0; inc_y = 1; push_cnt = 0; break;
		case 3:inc_x = 0; inc_y = -1; break;
		}

		while (true) {
			next_x += inc_x;
			next_y += inc_y;
			if (next_x < 0 || next_x >= BLOCK_HORIZONTAL)break;
			if (next_y < 0 || next_y >= BLOCK_VERTICAL)break;
			if (getBlockData(next_x, next_y) == blockData) {
				stackPush(Vec2(next_x, next_y));
				push_cnt++;
			}
			else break;
		}

		if (i % 2 == 0) continue;
		if (push_cnt < 2) {
		//���� ����� ���� �ȿ� ����־� �ϹǷ� stackEmpty ��� stackPop�� ����մϴ�.
			for (int k = 0; k < push_cnt; k++) {
				stackPop();
			}
		}
	}
	if (judgeStackCount > 1) {
		while (judgeStackCount > 0) {

			//���� �ڷ����� �ƹ������� �� �� �� ���� ���������� �� �� �����Ƿ� ���� �Ķ���ͷ� �ִ� ���
			//�Ʒ�ó�� ������ �޾Ƽ� �ֽ��ϴ�.
			Vec2 p = stackPop();
			destroyBlock(p.x, p.y);
		}
	}
	stackEmpty(); */
}

SceneIngame* SceneIngame::create(){
	//static�Լ�(��ü 'this'�� ����)�� ��� �Ʒ� ������� autorelease�� ���ݴϴ�.
	auto ret = new SceneIngame();
	if (ret && ret->init())ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool SceneIngame::init(){
	//init�� �ݹ� �Լ��Դϴ�.
	//(�߿�)�ݹ� �Լ����� �ݵ�� ���ʿ��� �θ��� �ݹ� �Լ��� ���� �ҷ��־�� �մϴ�.
	//���� ��� HelloWorld::init()�� ��쿡�� �θ��� �ݹ� �Լ��� Scene::init()�� ���� �ҷ��ݴϴ�.
	if (!Scene::init()) return false;
	//SceneIngame::init�� �����ϴ� ��� SceneIngame::create()�� ��쵵 if �������� ���ؼ� autorelease�ǰ� �޸𸮿��� �����˴ϴ�.
	//����, �������̵� �� �Լ��� �θ��� �Լ�(init ��)�� ȣ�����ִ� �Ϲ����� ���� �ֽ��ϴ�.
	//�̰������� Scene�� �θ��� �� �� ������ Scene::init�� �����ϴ� ��� �ڵ������� SceneIngame�� ������� ���մϴ�.

	//Scene�� ���⿡�� ���� ���(��� ���� �θ� Ŭ����)�Դϴ�.

	//������ ��� Ÿ���� �����մϴ�.
	srand(time(0));

	//getInstance()�� ������ �� �ִ� Director�� ������ �Ѱ��ϴ� Ŭ�����Դϴ�.
	//getTextureCache()�� �̹��� ���Ͽ� ���� �����ڸ� �ҷ��� �� addImage(...)�� �̹����� �����ɴϴ�.
	//Sprite�� ���� ��� �̹��� �����ڸ� ���� �ҷ����� ������ �ڵ����� �̷�������� 
	//�̹����� �߶� ����ϴ� ��� �̷��� �������� �ҷ��� ���� �ֽ��ϴ�.
	Director::getInstance()->getTextureCache()->addImage("res/match3_tiles_px.png");

	auto touch = EventListenerTouchOneByOne::create();
	//cocos2dx���� eventListener�� ���� �Լ� �Ǵ� std::bind�� �Բ� ���˴ϴ�.
	//eventDispatcher�� ���ؼ� �̺�Ʈ ���� ����� �����ݴϴ�.
	touch->onTouchBegan = std::bind(&SceneIngame::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchMoved = std::bind(&SceneIngame::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchEnded = std::bind(&SceneIngame::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	//��ġ�� ĵ���� ���� ��ġ�� ���� �Ͱ� �Ȱ��� �Ǵ����ݴϴ�.
	touch->onTouchCancelled = touch->onTouchEnded;

	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);

	return true;
}

void SceneIngame::onEnter(){
	//onEnter�� �ݹ� �Լ��Դϴ�.
	//(�߿�)�ݹ� �Լ����� �ݵ�� ���ʿ��� �θ��� �ݹ� �Լ��� ���� �ҷ��־�� �մϴ�.
	Scene::onEnter();

	this->initUI();
	this->initGame();
	this->startGame();
}

//������ ���µ��� ������ִ� enum Ŭ������ Environment ��� ���Ͽ� �����մϴ�.
void SceneIngame::initUI(){
}

void SceneIngame::initGame(){

	for (int i = 0; i < BLOCK_HORIZONTAL; i++) {
		for (int k = 0; k < BLOCK_VERTICAL; k++) {
			createBlock(i, k, rand() % 4 + 1);
		}
	}

	this->alignBlockSprite();
}

void SceneIngame::destroyUI(){
}

void SceneIngame::destroyGame(){
}

//������� ��� ��������Ʈ�� ȭ�鿡 �����ϰ� �����ִ� �Լ��Դϴ�.
void SceneIngame::alignBlockSprite(){
	for (int i = 0; i < BLOCK_HORIZONTAL; i++) {
		for (int k = 0; k < BLOCK_VERTICAL; k++) {
			auto s = getBlockSprite(i, k); //i�� k�� blockCoord�� ���Դϴ�.
			if (s != nullptr)s->setPosition(convertBlockCoordToGameCoord(Vec2(i, k)));
		}
	}
}

bool SceneIngame::onTouchBegan(Touch* t, Event* e){
	//Touch* t�� getLocation()��� �Լ��� ��ġ�� �޾ƿ� �� �ֽ��ϴ�.
	Vec2 p = convertGameCoordToBlockCoord(t->getLocation());

	CCLOG("%f, %f", p.x, p.y);

	//��: ��ġ�ϸ� ����� �����ǰ� ���ο� ����� ������ �������ϴ�.
	destroyBlock(p.x, p.y);
	dropBlocks(p.x);

	for (int i = 0; i < BLOCK_VERTICAL; i++) {
		judgeMatch(p.x, i);
	}

	//�������� ��ġ�� �߻��ϴ� ���� �ƴ� �̻� return true�� ���ݴϴ�.
	return true;
}

void SceneIngame::onTouchMoved(Touch* t, Event* e){
}

void SceneIngame::onTouchEnded(Touch* t, Event* e){
}

void SceneIngame::startGame(){
}

void SceneIngame::pauseGame(){
}

void SceneIngame::winGame(){
}

void SceneIngame::loseGame(){
}
