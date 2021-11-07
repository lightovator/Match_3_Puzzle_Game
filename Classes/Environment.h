#ifndef __ENVIRONMENT_H__
#define __ENVIORNMENT_H__

// ���� �⺻ ȯ���� �����صα� ���� ����Դϴ�.

//������ ���µ��� ������ִ� enum Ŭ������ Environment ��� ���Ͽ� �����մϴ�.
enum class GameState {
	PLAYING, PAUSED, WIN, LOSE
};

//�׸��尡 ���η� 7ĭ(�⺻ ���簢 ��� 7��)�Դϴ�.
#define BLOCK_HORIZONTAL 7
//�׸��尡 ���η� 9(�⺻ ���簢 ��� 9��)ĭ�Դϴ�.
#define BLOCK_VERTICAL 9

//BLOCK_OFFSET�� ����� ��ġ�� ���� ������(�׸��� ���߾�)�� ���մϴ�.
#define BLOCK_OFFSET Vec2(720/2,1280/2)
//��� ���� �����Դϴ�.
#define BLOCK_WIDTH 80
//��� ���� �����Դϴ�.
#define BLOCK_HEIGHT 80

#define SWAP(TYPE, A, B){ TYPE t = A; A = B; B = t; }

#endif