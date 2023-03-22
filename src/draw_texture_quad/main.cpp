#include<learnd3d11/systemclass.h>
#include<learnd3d11/renderer.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class TextureQuadRenderer : public Renderer {
public:
	TextureQuadRenderer() {
		m_indexBuffer = 0;
		m_textureView = 0;
		m_samplerState = 0;
	};
	~TextureQuadRenderer() {};

	bool Init(ID3D11Device*, HWND);
	bool Render(ID3D11DeviceContext*);
	void Shutdown();
private:
	ID3D11Buffer* m_indexBuffer;
	ID3D11ShaderResourceView* m_textureView;
	ID3D11SamplerState* m_samplerState;
};

bool TextureQuadRenderer::Init(ID3D11Device* device, HWND hwnd)
{
	LPCWSTR filePath = L"../../res/shaders/texture_quad.hlsl";
	LPCSTR vs_entty_point = "vs_main";
	LPCSTR ps_entty_point = "ps_main";
	LPCSTR vs_target = "vs_5_0";
	LPCSTR ps_target = "ps_5_0";

	// Create Vertex Shader
	HRESULT hResult = 0;
	ID3DBlob* vsBlob = 0;
	ID3DBlob* psBlob = 0;
	ID3DBlob* shaderCompileErrorsBlob = 0;

	hResult = D3DCompileFromFile(filePath, nullptr, nullptr, vs_entty_point, vs_target, 0, 0, &vsBlob, &shaderCompileErrorsBlob);
	if (FAILED(hResult)) {
		alertError(hResult, shaderCompileErrorsBlob);
		return false;
	}
	hResult = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader);
	if (FAILED(hResult))
		return false;

	// Create Input Layout
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hResult = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_layout);
	if (FAILED(hResult)) {
		MessageBoxA(0, "init input fail", "Shader CreateInputLayout Error", MB_ICONERROR | MB_OK);
		return false;
	}
	vsBlob->Release();
	vsBlob = 0;

	// Create Pixel Shader
	hResult = D3DCompileFromFile(filePath, nullptr, nullptr, ps_entty_point, ps_target, 0, 0, &psBlob, &shaderCompileErrorsBlob);
	if (FAILED(hResult)) {
		alertError(hResult, shaderCompileErrorsBlob);
		return false;
	}
	hResult = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pixelShader);
	if (FAILED(hResult))
		return false;
	psBlob->Release();
	psBlob = 0;

	// Create Vertex Buffer
	float vertexData[] = { // x, y, r, g, b, a, u, v
		-0.5f,  0.5f, 0.f,   1.f, 0.f, 1.f,    0.f, 0.f,
		0.5f, -0.5f, 1.f,    0.f, 0.f, 1.f,    1.f, 1.f,
		-0.5f, -0.5f, 0.f,   0.f, 1.f, 1.f,    0.f, 1.f,
		0.5f, 0.5f, 0.f,     0.f, 1.f, 1.f,    1.f, 0.f,
	};
	m_stride = 8 * sizeof(float);
	m_numVerts = sizeof(vertexData) / m_stride;
	m_offset = 0;

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = sizeof(vertexData);
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
	vertexSubresourceData.pSysMem = vertexData;

	hResult = device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &m_vertexBuffer);
	if (FAILED(hResult))
		return false;

	UINT indices[] = {
		0, 1, 2,
		0, 3, 1
	};
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
	indexSubresourceData.pSysMem = indices;

	hResult = device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &m_indexBuffer);
	if (FAILED(hResult))
		return false;


	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	device->CreateSamplerState(&samplerDesc, &m_samplerState);

	int texWidth, texHeight, texNumChannels;
	int texForceNumChannels = 4;
	unsigned char* textureByte = stbi_load("../../res/textures/bricks2.jpg", &texWidth, &texHeight, &texNumChannels, texForceNumChannels);

	assert(textureByte);
	int texBytesPerRow = 4 * texWidth;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = texWidth;
	textureDesc.Height = texHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA textureSubresourceData = {};
	textureSubresourceData.pSysMem = textureByte;
	textureSubresourceData.SysMemPitch = texBytesPerRow;

	ID3D11Texture2D* texture = 0;
	hResult = device->CreateTexture2D(&textureDesc, &textureSubresourceData, &texture);
	if (FAILED(hResult))
		return false;

	hResult = device->CreateShaderResourceView(texture, nullptr, &m_textureView);
	if (FAILED(hResult))
		return false;

	free(textureByte);

	return true;
}

bool TextureQuadRenderer::Render(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(m_layout);

	deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
	deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

	deviceContext->PSSetShaderResources(0, 1, &m_textureView);
	deviceContext->PSSetSamplers(0, 1, &m_samplerState);

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_stride, &m_offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//deviceContext->Draw(m_numVerts, 0);
	deviceContext->DrawIndexed(6, 0, 0);

	return true;
}

void TextureQuadRenderer::Shutdown()
{
	if (m_indexBuffer) {
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
	Renderer::Shutdown();
}

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	Renderer* myRenderer;
	bool result;

	System = new SystemClass;
	myRenderer = new TextureQuadRenderer;

	result = System->Initialize(myRenderer);
	if (result) {
		System->Run();
	}

	System->ShutDown();
	delete System;
	System = 0;

	return 0;
}