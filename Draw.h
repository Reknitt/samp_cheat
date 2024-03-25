#pragma once
#include "includes.h"

#define WHITE            D3DCOLOR_ARGB(255, 255, 255, 255)
#define RED                D3DCOLOR_ARGB(255, 255, 000, 000)
#define GREEN            D3DCOLOR_ARGB(255, 000, 255, 000)
#define BLUE            D3DCOLOR_ARGB(255, 000, 000, 255) 
#define BLACK            D3DCOLOR_ARGB(150, 000, 000, 000)
#define mBLACK            D3DCOLOR_ARGB(100, 000, 000, 205)
#define PURPLE            D3DCOLOR_ARGB(255, 125, 000, 255) 
#define GREY            D3DCOLOR_ARGB(255, 128, 128, 128) 
#define YELLOW            D3DCOLOR_ARGB(255, 255, 255, 000) 
#define ORANGE            D3DCOLOR_ARGB(255, 255, 125, 000)

class Draw {
public:
	LPDIRECT3DDEVICE9 pDevice;
	D3DVIEWPORT9 viewport;

	void DrawFilledRect(float x, float y, float w, float h, D3DCOLOR color);
	void DrawLine(float x1, float y1, float x2, float y2, float thickness, D3DCOLOR color);
	void DrawCircle(float x, float y, float r, float thickness, D3DCOLOR color);
};


