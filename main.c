#include <windows.h>
#define g_hFrame_id 1000
#define g_hLabel_id 1001

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HBRUSH hBlackBrush = NULL;
    static HBRUSH hTransparentBrush = NULL;

    switch (uMsg) {
        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            HWND hControl = (HWND)lParam;

            if (GetDlgCtrlID(hControl) == g_hFrame_id) {
                if (!hBlackBrush) {
                    hBlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
                }
                SetBkColor(hdcStatic, RGB(0, 0, 0));
                return (LRESULT)hBlackBrush;
            }
            else if (GetDlgCtrlID(hControl) == g_hLabel_id) {
                if (!hTransparentBrush) {
                    hTransparentBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
                }
                SetBkMode(hdcStatic, TRANSPARENT);
                SetTextColor(hdcStatic, RGB(255, 255, 255));
                return (LRESULT)hTransparentBrush;
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND: {
            return 0;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSW wc = {0};

    // Fill in the window class structure
    wc.hInstance = hInstance;
    wc.hbrBackground = CreateSolidBrush(RGB(128, 128, 128));
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wc.lpszClassName = L"TetrisWindowClass";
    wc.lpfnWndProc = WindowProc;

    // Register the window class
    if (!RegisterClassW(&wc)) {
        MessageBoxW(NULL, L"Window class registration failed!", L"Error", MB_ICONEXCLAMATION);
        return 1;
    }

    HWND hwnd = CreateWindowExW(
        0,                              // Optional window styles
        L"TetrisWindowClass",            // Window class name
        L"Tetris 1.0",                   // Window title
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,   // Position
        750,
        1150,                       // Width and height
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                            // Additional application data
    );

    HWND game = CreateWindowEx(
        0,
        "STATIC",
        NULL,
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        5,
        150,
        735,
        965,
        hwnd,
        (HMENU)g_hFrame_id, //id for static text element
        hInstance,
        NULL
    );

    HWND label1 = CreateWindowEx(
        0,
        "STATIC",
        NULL,
        WS_CHILD | WS_VISIBLE | SS_CENTER | WS_TABSTOP,
        5,
        5,
        120,
        25,
        hwnd,
        (HMENU)g_hLabel_id, //id for static text element
        hInstance,
        NULL
    );

    SetWindowText( label1, "Next Block:" );

    HWND nextBlockWindow = CreateWindowEx(
        0,
        "STATIC",
        NULL,
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        5,
        25,
        120,
        120,
        hwnd,
        (HMENU)g_hFrame_id, //id for static text element
        hInstance,
        NULL
    );


    if (!hwnd) {
        MessageBoxW(NULL, L"Window creation failed!", L"Error", MB_ICONEXCLAMATION);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}