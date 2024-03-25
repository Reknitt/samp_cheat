#include "Draw.h"

void Draw::DrawFilledRect(float x, float y, float w, float h, D3DCOLOR color)
{
	D3DRECT rect = { x, y, x + w, y + h };
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void Draw::DrawLine(float x1, float y1, float x2, float y2, float thickness, D3DCOLOR color)
{
	ID3DXLine* LineL;
	D3DXCreateLine(pDevice, &LineL);

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	LineL->SetWidth(thickness);
	LineL->Draw(Line, 2, color);
	LineL->Release();
}

void Draw::DrawCircle(float x, float y, float r, float thickness, D3DCOLOR color)
{
}
