//Here we define WIN32_LEAN_AND_MEAN. We do this to speed up the build process, it reduces the size of the Win32 header files by excluding some of the less used APIs.
#define WIN32_LEAN_AND_MEAN
#ifndef UNICODE
#define UNICODE
#endif 
#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//int WINAPI WinMain(
//    _In_ HINSTANCE hInstance,      // handle to current instance
//    _In_opt_ HINSTANCE hPrevInstance,  // handle to previous instance
//    _In_ LPSTR lpCmdLine,          // command line
//    _In_ int nCmdShow              // show state
//)
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nShowCmd*/)

_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Open a window
    HWND hwnd;
    WNDCLASSEXW winClass = {};

    winClass.lpfnWndProc = &WndProc;
    winClass.hInstance = hInstance;
    winClass.lpszClassName = L"MyWindowClass";

    winClass.cbSize = sizeof(WNDCLASSEXW);
    winClass.style = CS_HREDRAW | CS_VREDRAW;
    winClass.hIcon = LoadIconW(0, IDI_APPLICATION);
    winClass.hCursor = LoadCursorW(0, IDC_ARROW);
    winClass.hIconSm = LoadIconW(0, IDI_APPLICATION);

    if (!RegisterClassExW(&winClass)) {
        MessageBoxA(0, "RegisterClassEx failed", "Fatal Error", MB_OK);
        return GetLastError();
    }

    //RECT initialRect = { 0, 0, 1024, 768 };
    //AdjustWindowRectEx(&initialRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);
    //LONG initialWidth = initialRect.right - initialRect.left;
    //LONG initialHeight = initialRect.bottom - initialRect.top;

    hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW,
        winClass.lpszClassName,
        L"00. Opening a Win32 Window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0,
        1024,
        768,
        0, 0, hInstance, 0);

    if (!hwnd) {
        MessageBoxA(0, "CreateWindowEx failed", "Fatal Error", MB_OK);
        return GetLastError();
    }
    //ShowWindow(hwnd, 3);

    bool isRunning = true;
    while (isRunning)
    {
        MSG message = {};
        while (PeekMessageW(&message, 0, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT)
                isRunning = false;
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }

        Sleep(1);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
    switch (msg)
    {
    case WM_KEYDOWN:
    {
        if (wparam == VK_ESCAPE)
            DestroyWindow(hwnd);
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    default:
        result = DefWindowProcW(hwnd, msg, wparam, lparam);
    }
    return result;
}
