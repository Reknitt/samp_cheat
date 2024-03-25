#pragma once
#pragma once
#include "includes.h"

namespace mem {
	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
}
