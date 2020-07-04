#include <windows.h>

LRESULT CALLBACK windowProcedure(HWND, UINT, WPARAM, LPARAM);

/*
 hInstance - base address of the instance of our application
 hPrevInstance -
 pCmdLine - command line arguments
 nCmdShow - information about how the window should be displayed

 Additional Note: WINAPI is a macro that evaluates to a Microsoft-specific
                  keyword that specifies a calling convention.
*/
int WINAPI wWinMain(HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    PWSTR pCmdLine,
                    int nCmdShow) {
    WNDCLASSW wc = { 0 };

    wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.hInstance = hInstance;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = &windowProcedure;

    if (!RegisterClass(&wc)) {
        return -1;
    }

    CreateWindow(L"myWindowClass", 
                 L"Hack for \"Counter-Strike: Global Offensive\"",
                 WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                 100, 100, 500, 500,
                 NULL, NULL, NULL, NULL);

    MSG message = { 0 };

    while (GetMessage(&message, NULL, NULL, NULL)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return 0;
}

/*
 hWindow - window handle
 message -
 wp -
 lp -
*/
LRESULT CALLBACK windowProcedure(HWND hWindow, 
                                 UINT message, 
                                 WPARAM wp, 
                                 LPARAM lp) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWindow, message, wp, lp);
    }

    return NULL;
}