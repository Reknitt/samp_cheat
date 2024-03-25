#include "Cheat.h"
#include "Draw.h"

void Cheat::UpdateViewMatrix()
{
	memcpy_s(this->ViewMatrix, 16, (void*)MATRIX1, 16);
}


float* M1 = (float*)0x00B6F99C;
float* M2 = (float*)0x00B6F9E4;
float* M3 = (float*)0x00B6FA2C;
float* M4 = (float*)0x00B6FA74; 
float* M5 = (float*)0xB6FA2C;
float* M6 = (float*)0x00B6F99C;
bool Cheat::WorldToScreen(LPDIRECT3DDEVICE9 pDevice, Vec3 pos, Vec2& screen, int id)
{
	float* matrix = M1;
	if (id == 1)
		matrix = M1;
	if (id == 2)
		matrix = M2;
	if (id == 3)
		matrix = M3;
	if (id == 4)
		matrix = M4;
	matrix = M5;
	memcpy_s(ViewMatrix, sizeof(ViewMatrix), (void*)matrix, sizeof(ViewMatrix));
	HRESULT hr = pDevice->GetViewport(&this->Viewport);
	

	if (FAILED(hr))
		return false;

	Vec4 clipCoords;
	clipCoords.x = pos.x * ViewMatrix[0] + pos.y * ViewMatrix[1] + pos.z * ViewMatrix[2] + ViewMatrix[3];
	clipCoords.y = pos.x * ViewMatrix[4] + pos.y * ViewMatrix[5] + pos.z * ViewMatrix[6] + ViewMatrix[7];
	clipCoords.z = pos.x * ViewMatrix[8] + pos.y * ViewMatrix[9] + pos.z * ViewMatrix[10] + ViewMatrix[11];
	clipCoords.w = pos.x * ViewMatrix[12] + pos.y * ViewMatrix[13] + pos.z * ViewMatrix[14] + ViewMatrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (Viewport.Width / 2 * NDC.x) + (NDC.x + Viewport.Width / 2);
	screen.y = -(Viewport.Height / 2 * NDC.y) + (NDC.y + Viewport.Height / 2);
	return true;
}

bool Cheat::nWorldToScreen(Vec3 vecWorld, Vec3& vecScreen)
{
	/** C++-ifyed function 0x71DA00, formerly called by CHudSA::CalcScreenCoors **/

	// Get the static view matrix as D3DXMATRIX
	D3DXMATRIX m((float*)(0xB6FA2C));

	// Get the static virtual screen (x,y)-sizes
	DWORD* dwLenX = (DWORD*)(0xC17044);
	DWORD* dwLenY = (DWORD*)(0xC17048);

	// Do a transformation
	vecScreen.x = vecWorld.z * m._31 + vecWorld.y * m._21 + vecWorld.x * m._11 + m._41;
	vecScreen.y = vecWorld.z * m._32 + vecWorld.y * m._22 + vecWorld.x * m._12 + m._42;
	vecScreen.z = vecWorld.z * m._33 + vecWorld.y * m._23 + vecWorld.x * m._13 + m._43;

	// Get the correct screen coordinates

	float fRecip = 1.0f / vecScreen.z;
	vecScreen.x *= fRecip * (*dwLenX);
	vecScreen.y *= fRecip * (*dwLenY);
	return true;
}

bool Cheat::EnemyInCircle(Vec3 enemy2D, float radius)
{
	DWORD* dwLenX = (DWORD*)(0xC17044);
	DWORD* dwLenY = (DWORD*)(0xC17048);
	float result = powf(enemy2D.x - *dwLenX / 2, 2) + powf(enemy2D.y - *dwLenY / 2, 2);
	if (result < (radius * radius))
		return true;
	return false;
}



void Cheat::Init()
{
	//oEndScene = (tEndScene)TrampolineHook((BYTE*)d3d9DeviceTable[42], (BYTE*)hkEndScene, 7);
	if (!GetD3dDevice())
		return;
}

bool Cheat::GetD3dDevice()
{
	IDirect3D9* d3dSys = Direct3DCreate9(D3D_SDK_VERSION);
	IDirect3DDevice9* dummyDev = NULL;

	// Options to create dummy device
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetWndHandle();

	HRESULT dummyDeviceCreated = d3dSys->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &dummyDev);

	if (dummyDeviceCreated != S_OK) {
		d3dpp.Windowed = !d3dpp.Windowed;
		HRESULT dummyDeviceCreated = d3dSys->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &dummyDev);
		
		if (dummyDeviceCreated != S_OK) {
			d3dSys->Release();
			return false;
		}
	}

	memcpy_s(d3d9DeviceTable, sizeof(d3d9DeviceTable), *reinterpret_cast<void***>(dummyDev), sizeof(d3d9DeviceTable));
	dummyDev->Release();
	d3dSys->Release();
	return TRUE;
}

HWND window;
BOOL CALLBACK EnumWins(HWND hwnd, LPARAM lParam) {
	DWORD procID;
	GetWindowThreadProcessId(hwnd, &procID);
	if (procID != GetCurrentProcessId())
		return TRUE;

	window = hwnd;
	return FALSE;
}


HWND Cheat::GetWndHandle()
{
	window = NULL;
	EnumWindows(EnumWins, NULL);
	return window;
}
bool Cheat::DrawEsp(Vec2 screen)
{
	return false;
}

bool Cheat::AimAt(stPlayerPool* pool)
{	//https://www.blast.hk/threads/32774/
	stLocalPlayer* player = pool->pLocalPlayer;
	int enid = pool->pLocalPlayer->sAimingAtPid;
	if (enid == 65535)
		return false;
	stRemotePlayer* enemy = pool->pRemotePlayer[enid];

	Vec3 enPos;
	enPos.x = enemy->pPlayerData->fOnFootPos[0];
	enPos.y = enemy->pPlayerData->fOnFootPos[1];
	enPos.z = enemy->pPlayerData->fOnFootPos[2];
	Vec3 enSpeed;
	enSpeed.x = enemy->pPlayerData->fOnFootMoveSpeed[0];
	enSpeed.y = enemy->pPlayerData->fOnFootMoveSpeed[1];
	enSpeed.z = enemy->pPlayerData->fOnFootMoveSpeed[2];

	Vec3 plPos;
	plPos.x = player->onFootData.fPosition[0];
	plPos.y = player->onFootData.fPosition[1];
	plPos.z = player->onFootData.fPosition[2];


	Vec3 vec;
	vec.x = plPos.x - (enPos.x + enSpeed.x / 60);
	vec.y = plPos.y - (enPos.y + enSpeed.y / 60);
	vec.z = plPos.z - (enPos.z + enSpeed.z / 60);

	float angleX = atan2f(vec.y, -vec.x) - PI / 2;
	*(float*)0xB6F258 = -(angleX - PI / 2);

	return true;
}

float Cheat::VectorDistance(Vec3 a, Vec3 b)
{
	Vec3 c;
	c.x = b.x - a.x;
	c.y = b.y - a.y;
	c.z = b.z - a.z;
	return sqrtf(c.x * c.x + c.y * c.y + c.z * c.z);
}

Cheat::Cheat(stPlayerPool* oplayerPool)
{
	this->playerPool = oplayerPool;
	UpdateViewMatrix();
}
