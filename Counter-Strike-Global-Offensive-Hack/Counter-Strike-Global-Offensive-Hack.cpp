#include <windows.h>
#include <string>

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void AddMenus(HWND);
void OnSize(HWND, UINT, int, int);

HMENU hMenu;

LPCWSTR menuTabNames[] = { L"File", L"Edit", L"View", L"Project", L"Build", L"Debug", L"Test" };

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
    WNDCLASS wc = { 0 };

    wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.hInstance = hInstance;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = &WindowProcedure;

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
 message - ex: The WM_SIZE message indicates the window was resized
 wp - contains additional data that pertains to the message
 lp - also contains additional data that pertains to the message
*/
LRESULT CALLBACK WindowProcedure(HWND hWindow, 
                                 UINT message, 
                                 WPARAM wp, 
                                 LPARAM lp) {
    switch (message) {
    case WM_COMMAND:
        for (int i = 0, n = sizeof(menuTabNames) / sizeof(menuTabNames[0]); i < n; i++) {
            // using wstring to concatenate our two LPCWSTRs
            std::wstring tabName(menuTabNames[i]);
            tabName += L" has been clicked!";

            // wParameter (i.e. wp) is the newItemID when the message is WM_COMMAND
            if (wp == i + 1) {
                MessageBox(hWindow, tabName.c_str(), L"Notification Window", MB_OK);
            }
        }
        break;
    case WM_CREATE:
        AddMenus(hWindow);
        break;
    case WM_SIZE:
        {
            int width = LOWORD(lp);
            int height = HIWORD(lp);

            OnSize(hWindow, (UINT)wp, width, height);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWindow, message, wp, lp);
    }

    return NULL;
}

void AddMenus(HWND hWindow) {
    hMenu = CreateMenu();

    for (int i = 0, n = sizeof(menuTabNames) / sizeof(menuTabNames[0]); i < n; i++) {
        AppendMenuW(hMenu, MF_STRING, i + 1, menuTabNames[i]);
    }

    SetMenu(hWindow, hMenu);
}

void OnSize(HWND hWindow, UINT flag, int width, int height) {
    // Handle resizing
}