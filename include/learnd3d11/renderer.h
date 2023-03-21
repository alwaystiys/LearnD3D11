#pragma once

#ifndef _RENDERER_H_
#define _RENDERER_H_

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

	virtual bool Initialize(ID3D11Device*, HWND);
	virtual void Shutdown();
	virtual bool Render(ID3D11DeviceContext*);
	virtual bool Init(ID3D11Device*, HWND);
	virtual void alertError(HRESULT, ID3DBlob*);

//private:
protected:
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
