#include "includes.h"
#include "hook.h"
#include "mem.h"

bool Detour32(BYTE* src, BYTE* dst, const uintptr_t len) {
	if (len < 5) return false;

	DWORD curProtection;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

	uintptr_t relativeAddr = dst - src - 5;
	*src = 0xE9;
	*(uintptr_t*)(src + 1) = relativeAddr;
	VirtualProtect(src, len, curProtection, &curProtection);
	return true;
}

BYTE* TrampolineHook(BYTE* src, BYTE* dst, const uintptr_t len) {
	if (len < 5) return 0;

	BYTE* gateway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy_s(gateway, len, src, len);

	uintptr_t gatewayRelativeAddr = src - gateway - 5;

	*(gateway + len) = 0xE9;
	*(uintptr_t*)((uintptr_t)gateway + len + 1) = gatewayRelativeAddr;

	Detour32(src, dst, len);

	return gateway;
}


Hook::Hook(const char* moduleName, const char* procName, BYTE* dst, BYTE* ptrToGatewayFnPtr, const uintptr_t len) {
	HMODULE hmod = GetModuleHandleA(moduleName);
	if (hmod == NULL) return;
	this->src = (BYTE*)GetProcAddress(hmod, procName);
	this->dst = dst;
	this->len = len;
	this->ptrToGatewayFnPtr = ptrToGatewayFnPtr;
}


Hook::Hook(BYTE* src, BYTE* dst, BYTE* ptrToGatewayFnPtr, const uintptr_t len) {
	this->src = src;
	this->dst = dst;
	this->len = len;
	this->ptrToGatewayFnPtr = ptrToGatewayFnPtr;
}

void Hook::Enable() {
	if (hkStatus == true) return;
	memcpy_s(originalBytes, len, src, len);
	*(uintptr_t*)ptrToGatewayFnPtr = (uintptr_t)TrampolineHook(src, dst, len);
	hkStatus = true;
}

void Hook::Disable() {
	if (hkStatus == false) return;
	mem::Patch(src, originalBytes, len);
	hkStatus = false;
}