#include<learnd3d11/systemclass.h>


SystemClass::SystemClass() {
	m_Graphics = 0;
}

SystemClass::~SystemClass() {

}

bool SystemClass::Initialize(Renderer* renderer) {
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;
	
	InitializeWindows(screenWidth, screenHeight);

	m_Graphics = new GraphicsClass(renderer);

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}


	return true;
}

void SystemClass::ShutDown() {
	// Release the graphics object.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	ShutdownWindows();
}


void SystemClass::Run() {
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));
	done = false;
	while (!done) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {
			done = true;
		}
		else {
			result = Frame();
			if (!result) {
				done = false;
			}
		}

	}

}

bool SystemClass::Frame() {
	bool result;
	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}
	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg)
	{
	case WM_KEYDOWN:
		return 0;
	case WM_KEYUP:
		return 0;
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight) {
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;

	int posX, posY;
	
	ApplicationHandle = this;

	m_hinstance = GetModuleHandle(NULL);

	m_applicationName = L"Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = m_applicationName;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (false) {

	}
	else {
		screenWidth = 1200;
		screenHeight = 700;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		m_applicationName,
		L"simple framework",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);


	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//ShowCursor(false);
}

void SystemClass::ShutdownWindows() {

	ShowCursor(true);

	if (false)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL;


}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam))
		return true;

	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
