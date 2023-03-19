#pragma once
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <windows.h>
#include<learnd3d11/d3dclass.h>
#include<learnd3d11/renderer.h>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class GraphicsClass
{
public:
	GraphicsClass(Renderer*);
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_Direct3D;
	Renderer* m_Renderer;
};



#endif _GRAPHICSCLASS_H_

