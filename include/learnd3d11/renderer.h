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
	CameraClass* getCamera();

private:

	bool Init(ID3D11Device*, HWND);
	void ShutdownBuffers();
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

private:
	UINT m_numVerts;
	UINT m_stride;
	UINT m_offset;
	CameraClass* m_Camera;

	//
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
	//
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;


private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
};

#endif _RENDERER_H_
