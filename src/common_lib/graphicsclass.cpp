#include<learnd3d11/graphicsclass.h>

GraphicsClass::GraphicsClass(Renderer* render) : m_Renderer(render)
{
	m_Direct3D = 0;
	m_show_demo_window = false;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::InitImGui(HWND hwnd) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // 允许键盘控制
	io.ConfigWindowsMoveFromTitleBarOnly = true;              // 仅允许标题拖动

	// 设置Dear ImGui风格
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());

	return true;
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	
	// Create and initialize the Direct3D object.
	m_Direct3D = new D3DClass;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	result = m_Renderer->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Renderer", L"Error", MB_OK);
		return false;
	}

	result = InitImGui(hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize ImGui", L"Error", MB_OK);
		return false;
	}
	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
	//Release own Renderer
	if (m_Renderer)
	{
		m_Renderer->Shutdown();
		delete m_Renderer;
		m_Renderer = 0;
	}
	
	if(ImGui::GetCurrentContext())
		ImGui::DestroyContext();
}

bool GraphicsClass::Frame()
{
	bool result;

	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}
	return true;
}


bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (m_show_demo_window) {
		ImGui::ShowDemoWindow(&m_show_demo_window);
	}


	ImGui::Begin("Test Window");  
	ImGui::Text("Hello DirectX 11!");
	if (ImGui::Button("Click"))
		m_show_demo_window = !m_show_demo_window;
	ImGui::End();


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//m_Renderer->getCamera()->Render();
	//m_Direct3D->GetWorldMatrix(worldMatrix);
	//m_Renderer->getCamera()->GetViewMatrix(viewMatrix);
	//m_Direct3D->GetProjectionMatrix(projectionMatrix);
	ImGui::Render();
	result = m_Renderer->Render(m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();
	return true;
}
