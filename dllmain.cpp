// dllmain.cpp : Определяет точку входа для приложения DLL.

#include "includes.h"
#include "Cheat.h"
#include <stdio.h>
#include "Draw.h"

HANDLE hConsole;

void CreateConsole() {
	AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

int MATRIX = 1;
float distance = 100;
typedef HRESULT(APIENTRY* tEndScene) (LPDIRECT3DDEVICE9 pDevice);
Cheat cheat{ 0 };
Draw d3d;
/*hook directx EndScene*/
tEndScene oEndScene = nullptr;
HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 pDevice) {
	//std::cout << "[Hooked]" << std::endl;
	if (!d3d.pDevice)
		d3d.pDevice = pDevice;
	stLocalPlayer* pPlayer = cheat.playerPool->pLocalPlayer;
	Vec3 playerPos;
	playerPos.x = pPlayer->onFootData.fPosition[0];
	playerPos.y = pPlayer->onFootData.fPosition[1];
	playerPos.z = pPlayer->onFootData.fPosition[2];
	
	Vec3 player2D;
	D3DVIEWPORT9 viewport;
	stRemotePlayer* pEnemy;

	Vec3 enemyPos;
	Vec3 enemy2D;
	for (int i = 0; i < SAMP_MAX_PLAYERS; i++) {
		cheat.nWorldToScreen(playerPos, player2D);
		pEnemy = cheat.playerPool->pRemotePlayer[i];
		if (pEnemy == nullptr) {
			continue;
		}
		if (pEnemy->pPlayerData->onFootData.byteHealth < 0 || pEnemy->pPlayerData->onFootData.byteHealth > 100) {
			continue;
		}
		enemyPos.x = pEnemy->pPlayerData->fOnFootPos[0];
		enemyPos.y = pEnemy->pPlayerData->fOnFootPos[1];
		enemyPos.z = pEnemy->pPlayerData->fOnFootPos[2];
		if (cheat.VectorDistance(playerPos, enemyPos) < distance) {
			if (cheat.nWorldToScreen(enemyPos, enemy2D))
			{
				viewport = cheat.Viewport;
				d3d.DrawLine(enemy2D.x, enemy2D.y, player2D.x, player2D.y, 1.5f, PURPLE);
				cheat.AimAt(cheat.playerPool);
				std::cout << "aim" << std::endl;
				
			}
		}
	}

	return oEndScene(pDevice);
}

DWORD WINAPI HackThread(HMODULE hModule) {
	CreateConsole();
	uintptr_t* sampDll = (uintptr_t*)GetModuleHandle("samp.dll");
	if (sampDll == NULL)
		return 1;
	uintptr_t* offset1 = (uintptr_t*)*(sampDll + 0x0010D894 / sizeof(uintptr_t*));
	uintptr_t* offset2 = (uintptr_t*)*(offset1);
	uintptr_t* offset3 = (uintptr_t*)*(offset2 + 0xA0 / sizeof(uintptr_t*));
	stPlayerPool* oPlayerPool = (stPlayerPool*)offset3;
	cheat = Cheat(oPlayerPool);

	cheat.Init();
	//Hook EndSceneHook((BYTE*)cheat.d3d9DeviceTable[42], (BYTE*)&hkEndScene, (BYTE*)&oEndScene, 7);
	//EndSceneHook.Enable();
	oEndScene = (tEndScene)TrampolineHook((BYTE*)cheat.d3d9DeviceTable[42], (BYTE*)hkEndScene, 7);

	bool end = false;
	while (!end) {
		if (GetAsyncKeyState(VK_END) & 1)
			end = true;

		if (GetAsyncKeyState(VK_INSERT) & 1)
			distance += 10.0f;
		if (GetAsyncKeyState(VK_DELETE) & 1)
			distance -= 10.0f;
	}

	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
		case DLL_PROCESS_ATTACH:
			DWORD threadID;
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&HackThread, 0, 0, &threadID);
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

