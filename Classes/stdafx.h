#ifndef __STDAFX_H__
#define __STDAFX_H__

//증분 빌드를 위한 헤더입니다a. precompiled header라고 합니다.
//C++에서는 첫번째 빌드에서는 느리지만 두번째 빌드부터 속도가 더욱 빨라지는
//증분 빌드를 지원합니다.

//Properties -> precompiled header를 설정해주었기 때문에 '모든 cpp파일'의 첫번째 줄에 
//stdafx.h를 포함해주어야 정상적으로 작동합니다.

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;

#endif