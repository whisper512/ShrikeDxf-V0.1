#pragma once

//样式表
#include <QPalette>

#define BACKGROUND_GRAY "background-color:#d0d0d0;"
#define BACKGROUND_GRAY2 "background-color:#f0f0f0;"
#define BACKGROUND_WHITE "background-color:#FFFFFF;"

enum eThremColor
{
	eThremColor_Light,
	eThremColor_Dark
};

namespace StyleSheet
{
    QPalette CreatePaletteLigth();
	QPalette CreatePaletteDark();
}


