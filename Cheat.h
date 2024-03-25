#pragma once
#include "samp.h"
#include "Hook.h"

#define MATRIX1 0x00B6F99C
#define MATRIX2 0x00B6F9E4 
#define MATRIX3 0x00B6FA2C 
#define MATRIX4 0x00B6FA74 

#define PI 3.1415926535


struct Vec2 {
	float x, y;
};

struct Vec3 {
	float x, y, z;
};

struct Vec4 {
	float x, y, z, w;
};

class Cheat {
public:
	stPlayerPool* playerPool;
	float ViewMatrix[16];
	byte* d3d9DeviceTable[119];
	D3DVIEWPORT9 Viewport;
	
public:
	Cheat(stPlayerPool* oplayerPool);
	void Init();
	bool GetD3dDevice();
	HWND GetWndHandle();
	void UpdateViewMatrix();
	bool WorldToScreen(LPDIRECT3DDEVICE9 pDevice, Vec3 position, Vec2& screen, int id);
	bool nWorldToScreen(Vec3 position, Vec3& screen);
	bool EnemyInCircle(Vec3 enemy2D, float radius);
	float VectorDistance(Vec3 a, Vec3 b);
	bool DrawEsp(Vec2 screen);
	bool AimAt(stPlayerPool* pool);
private:
};


