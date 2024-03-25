#pragma once

bool Detour32(BYTE* src, BYTE* dst, const uintptr_t len);
BYTE* TrampolineHook(BYTE* src, BYTE* dst, const uintptr_t len);

struct Hook {

	BYTE* src{ nullptr };
	BYTE* dst{ nullptr };
	uintptr_t len{ 0 };
	BYTE originalBytes[10]{ 0 };
	BYTE* ptrToGatewayFnPtr{ nullptr };
	bool hkStatus{ false };

	void Enable();
	void Disable();
	Hook(BYTE* src, BYTE* dst, BYTE* ptrToGatewayFnPtr, const uintptr_t len);
	Hook(const char* moduleName, const char* procName, BYTE* dst, BYTE* ptrToGatewayFnPtr, const uintptr_t len);
};