#pragma once

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include<learnd3d11/cameraclass.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;
class Renderer
{
public:
	Renderer();
	Renderer(const Renderer&);
	~Renderer();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*);
	//bool createVsPsAndLayout(ID3D11Device*, HWND, LPCWSTR, D3D11_INPUT_ELEMENT_DESC*);
	CameraClass* getCamera();

private:

	bool Init(ID3D11Device*, HWND);
	void ShutdownBuffers();
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

private:
	CameraClass* m_Camera;
	//
	UINT m_numVerts;
	UINT m_stride;
	UINT m_offset;
	//
	ID3D11Buffer* m_vertexBuffer;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;

};

#endif _RENDERER_H_
