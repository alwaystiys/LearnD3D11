#include<learnd3d11/renderer.h>

Renderer::Renderer()
{
	m_Camera = 0;
	// 
	m_vertexBuffer = 0;
	//
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
}

Renderer::Renderer(const Renderer&)
{
}

Renderer::~Renderer()
{
}


bool Renderer::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	result = Init(device, hwnd);
	if (!result)
	{
		return false;
	}
	return true;
}



bool Renderer::Init(ID3D11Device* device, HWND hwnd)
{
	// Create Vertex Shader
	HRESULT hResult = 0;
	ID3DBlob* vsBlob = 0;
	ID3DBlob* psBlob = 0;
	ID3DBlob* shaderCompileErrorsBlob = 0;
	
	hResult = D3DCompileFromFile(L"../../res/shaders/4.1.color.hlsl", nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vsBlob, &shaderCompileErrorsBlob);
	if (FAILED(hResult))
	{
		const char* errorString = NULL;
		if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
			errorString = "Could not compile shader; file not found";
		else if (shaderCompileErrorsBlob) {
			errorString = (const char*)shaderCompileErrorsBlob->GetBufferPointer();
			shaderCompileErrorsBlob->Release();
			shaderCompileErrorsBlob = 0;
		}
		MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
		return false;
	}
	hResult = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader);
	if (FAILED(hResult))
	{
		return false;
	}

	// Create Input Layout
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	

	hResult = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_layout);
	if (FAILED(hResult))
	{
		return false;
	}
	vsBlob->Release();
	vsBlob = 0;

	// Create Pixel Shader
	hResult = D3DCompileFromFile(L"../../res/shaders/4.1.color.hlsl", nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &psBlob, &shaderCompileErrorsBlob);
	if (FAILED(hResult))
	{
		const char* errorString = NULL;
		if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
			errorString = "Could not compile shader; file not found";
		else if (shaderCompileErrorsBlob) {
			errorString = (const char*)shaderCompileErrorsBlob->GetBufferPointer();
			shaderCompileErrorsBlob->Release();
			shaderCompileErrorsBlob = 0;
		}
		MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
		return false;
	}
	hResult = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pixelShader);
	if (FAILED(hResult))
	{
		return false;
	}
	psBlob->Release();
	psBlob = 0;

	// Create Vertex Buffer
	float vertexData[] = { // x, y, r, g, b, a
		0.0f,  0.5f, 0.f, 1.f, 0.f, 1.f,
		0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,
		-0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f
	};
	m_stride = 6 * sizeof(float);
	m_numVerts = sizeof(vertexData) / m_stride;
	m_offset = 0;

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = sizeof(vertexData);
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubresourceData = { vertexData };

	hResult = device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &m_vertexBuffer);
	if (FAILED(hResult))
	{
		return false;
	}
	return true;
}

void Renderer::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

CameraClass* Renderer::getCamera() {
	return m_Camera;
}

bool Renderer::Render(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(m_layout);

	deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
	deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_stride, &m_offset);

	deviceContext->Draw(m_numVerts, 0);

	return true;
}


void Renderer::ShutdownBuffers()
{
	// Release the index buffer.
	//if (m_indexBuffer)
	//{
	//	m_indexBuffer->Release();
	//	m_indexBuffer = 0;
	//}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void Renderer::ShutdownShader()
{
	// Release the matrix constant buffer.
	//if (m_matrixBuffer)
	//{
	//	m_matrixBuffer->Release();
	//	m_matrixBuffer = 0;
	//}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}

void Renderer::Shutdown()
{
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
	ShutdownShader();

	return;
}