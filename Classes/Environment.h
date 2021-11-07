#ifndef __ENVIRONMENT_H__
#define __ENVIORNMENT_H__

// 각종 기본 환경을 설정해두기 위한 헤더입니다.

//게임의 상태들을 만들어주는 enum 클래스를 Environment 헤더 파일에 생성합니다.
enum class GameState {
	PLAYING, PAUSED, WIN, LOSE
};

//그리드가 가로로 7칸(기본 정사각 블록 7개)입니다.
#define BLOCK_HORIZONTAL 7
//그리드가 세로로 9(기본 정사각 블록 9개)칸입니다.
#define BLOCK_VERTICAL 9

//BLOCK_OFFSET은 블록을 배치할 때의 기준점(그리드 정중앙)을 말합니다.
#define BLOCK_OFFSET Vec2(720/2,1280/2)
//블록 가로 길이입니다.
#define BLOCK_WIDTH 80
//블록 세로 길이입니다.
#define BLOCK_HEIGHT 80

#define SWAP(TYPE, A, B){ TYPE t = A; A = B; B = t; }

#endif