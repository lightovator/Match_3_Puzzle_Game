#include "stdafx.h"
#include "SceneIngame.h"
//Precompiled Header이므로 모든 cpp파일의 첫 줄에 stdafx.h를 포함해줍니다.

void SceneIngame::createBlock(int x, int y, int type){
	auto cache = Director::getInstance()->getTextureCache();

	//Texture을 따올 때의 Rect는 좌상단이 기준입니다. (Rect 등에서)게임 엔진만 특이하게 좌하단이 기준입니다.
	//(x 좌표, y좌표(로부터), 가로, 세로)를 직사각형으로 잘라서 가져옵니다.
	auto spr = Sprite::createWithTexture(
		cache->getTextureForKey("res/match3_tiles_px.png"),
		Rect(0 + (type * 40), 0, 40, 40));
	spr->setScale(2.0);
	addChild(spr);

	//좌표를 헷갈리지 않도록 직접 배열에 접근하는 것이 아니라 함수를 통해서 지정해줍니다.
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
	//blockData가 0이라면 Sprite가 없기 때문에 removeFromParent()가 불가능합니다.
	if (blockData[y][x] != 0) {
		blockSprite[y][x]->removeFromParent();
		blockSprite[y][x] = nullptr;
		blockData[y][x] = 0;
	}
}

Vec2 SceneIngame::convertGameCoordToBlockCoord(Vec2 gameCoord){
	//블록 offset(그리드 정중앙)을 기준으로, 블록 좌하단을 원점(blockOrigin)으로서 설정합니다.
	Vec2 blockOrigin = BLOCK_OFFSET 
		- Vec2((BLOCK_HORIZONTAL * BLOCK_WIDTH) / 2, (BLOCK_VERTICAL * BLOCK_HEIGHT) / 2)
		+ Vec2(BLOCK_WIDTH / 2, BLOCK_HEIGHT / 2);
	Vec2 delta = gameCoord - blockOrigin;

	//0.5는 반올림을 위한 장치입니다.
	//(중요) 일반적으로 (int)(x+0.5)라는 형식으로 양수의 반올림을 자주 실행합니다.
	//(중요) int 형변환을 진행하는 경우 소수점 아래는 버림됩니다.
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
	//y에 0을 넣으면 해당 열의 바닥 부분부터 찾을 수 있습니다.
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
	//빈 블록, 그리고 빈 블록 위쪽에 가장 가까운 찬 블록을 교환해줍니다.
	//for 문으로 이것을 반복해주면 테트리스 게임처럼 블록을 떨어뜨릴 수 있습니다.

	for (int i = findEmptyBlockYIndex(x, 0); i < BLOCK_VERTICAL; i++) {
		int empty_y = findEmptyBlockYIndex(x, i);
		if (empty_y == -1) continue;
		int filled_y = findFilledBlockYIndex(x, empty_y + 1);
		if (filled_y == -1) continue;

		//변수 a, b를 계속 사용하기 위해 대괄호로 감쌉니다.
		//현재 BlockData와 BlockSprite는 전혀 별개로 관리되고 있으며 연결고리가 없습니다.
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

	//alignBlockSprite를 해주지 않으면 blockSprite의 데이터에는 들어있지만
	//실물로 나타나지는 않기 때문에 함수를 호출해주어야 합니다.
	alignBlockSprite();
}

//스택이 비어있는지 judgeData로 확인한 후 비어 있으면 값을 집어 넣습니다.
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
	//(주의) judgeStack은 아직 비우지 않았습니다.
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
		//아래의 for문으로 하면 judgeStackCount가 계속 변하기 때문에 의도한 결과를 얻을 수 없습니다.
		//for (int i = 0; i < judgeStackCount; i++) {
		while (judgeStackCount > 0){
			Vec2 p = stackPop();
			destroyBlock(p.x, p.y);
		}
	}
	stackEmpty();

	/* int blockData = getBlockData(x, y);

	if (blockData == 0) return;
	
	//현재 블록을 스택 안에 넣어줍니다.
	stackPush(Vec2(x, y));

	int push_cnt = 0;

	//동서남북 4방향을 계산하기 편하도록 루프를 씁니다.
	for (int i = 0; i < 4; i++) {
		int next_x = x; //x가 다음으로 가는 방향
		int next_y = y; //y가 다음으로 가는 방향
		int inc_x; //x의 증가분
		int inc_y; //y의 증가분

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
		//원래 블록은 스택 안에 내비둬야 하므로 stackEmpty 대신 stackPop을 사용합니다.
			for (int k = 0; k < push_cnt; k++) {
				stackPop();
			}
		}
	}
	if (judgeStackCount > 1) {
		while (judgeStackCount > 0) {

			//스택 자료형은 아무데서나 막 뺄 수 없고 끝에서부터 뺄 수 있으므로 직접 파라미터로 넣는 대신
			//아래처럼 변수로 받아서 넣습니다.
			Vec2 p = stackPop();
			destroyBlock(p.x, p.y);
		}
	}
	stackEmpty(); */
}

SceneIngame* SceneIngame::create(){
	//static함수(주체 'this'가 없음)의 경우 아래 방식으로 autorelease를 해줍니다.
	auto ret = new SceneIngame();
	if (ret && ret->init())ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool SceneIngame::init(){
	//init은 콜백 함수입니다.
	//(중요)콜백 함수들은 반드시 앞쪽에서 부모의 콜백 함수를 먼저 불러주어야 합니다.
	//예를 들어 HelloWorld::init()의 경우에도 부모의 콜백 함수인 Scene::init()을 먼저 불러줍니다.
	if (!Scene::init()) return false;
	//SceneIngame::init이 실패하는 경우 SceneIngame::create()의 경우도 if 조건으로 인해서 autorelease되고 메모리에서 삭제됩니다.
	//또한, 오버라이드 된 함수는 부모의 함수(init 등)을 호출해주는 암묵적인 룰이 있습니다.
	//이곳에서는 Scene이 부모라고 할 수 있으며 Scene::init이 실패하는 경우 자동적으로 SceneIngame도 실행되지 못합니다.

	//Scene이 여기에서 상위 노드(상속 받은 부모 클래스)입니다.

	//랜덤한 블록 타입을 제공합니다.
	srand(time(0));

	//getInstance()로 가져올 수 있는 Director는 게임을 총괄하는 클래스입니다.
	//getTextureCache()로 이미지 파일에 대한 관리자를 불러온 후 addImage(...)로 이미지를 가져옵니다.
	//Sprite를 쓰는 경우 이미지 관리자를 통해 불러오는 과정이 자동으로 이루어지지만 
	//이미지를 잘라서 써야하는 경우 이렇게 수동으로 불러올 수도 있습니다.
	Director::getInstance()->getTextureCache()->addImage("res/match3_tiles_px.png");

	auto touch = EventListenerTouchOneByOne::create();
	//cocos2dx에서 eventListener는 람다 함수 또는 std::bind와 함께 사용됩니다.
	//eventDispatcher를 통해서 이벤트 지정 방식을 정해줍니다.
	touch->onTouchBegan = std::bind(&SceneIngame::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchMoved = std::bind(&SceneIngame::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchEnded = std::bind(&SceneIngame::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	//터치가 캔슬된 경우는 터치과 끝난 것과 똑같이 판단해줍니다.
	touch->onTouchCancelled = touch->onTouchEnded;

	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);

	return true;
}

void SceneIngame::onEnter(){
	//onEnter는 콜백 함수입니다.
	//(중요)콜백 함수들은 반드시 앞쪽에서 부모의 콜백 함수를 먼저 불러주어야 합니다.
	Scene::onEnter();

	this->initUI();
	this->initGame();
	this->startGame();
}

//게임의 상태들을 만들어주는 enum 클래스를 Environment 헤더 파일에 생성합니다.
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

//만들어진 블록 스프라이트를 화면에 정갈하게 맞춰주는 함수입니다.
void SceneIngame::alignBlockSprite(){
	for (int i = 0; i < BLOCK_HORIZONTAL; i++) {
		for (int k = 0; k < BLOCK_VERTICAL; k++) {
			auto s = getBlockSprite(i, k); //i와 k는 blockCoord로 보입니다.
			if (s != nullptr)s->setPosition(convertBlockCoordToGameCoord(Vec2(i, k)));
		}
	}
}

bool SceneIngame::onTouchBegan(Touch* t, Event* e){
	//Touch* t는 getLocation()라는 함수로 위치를 받아올 수 있습니다.
	Vec2 p = convertGameCoordToBlockCoord(t->getLocation());

	CCLOG("%f, %f", p.x, p.y);

	//룰: 터치하면 블록이 삭제되고 새로운 블록이 위에서 떨어집니다.
	destroyBlock(p.x, p.y);
	dropBlocks(p.x);

	for (int i = 0; i < BLOCK_VERTICAL; i++) {
		judgeMatch(p.x, i);
	}

	//연쇄적인 터치가 발생하는 것이 아닌 이상 return true를 해줍니다.
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
