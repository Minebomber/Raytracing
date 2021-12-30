#include "raytrace.h"
#include "resource.h"

#ifndef GWL_HINSTANCE
    #define GWL_HINSTANCE (-6)
#endif

#define MAX_LOADSTRING 100

#define INIT_WIDTH 800
#define INIT_HEIGHT 600

#define INIT_SAMPLES ID_SAMPLES_64
#define INIT_DEPTH ID_DEPTH_256

raytrace rt;

struct {
    int samples;
    int depth;
} options;

bool renderDone = true;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

void set_option_samples(HWND hWnd, UINT id);
void set_option_depth(HWND hWnd, UINT id);

void start_render(HWND hWnd);
void stop_render(HWND hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR  lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WCHAR szTitle[MAX_LOADSTRING];
    WCHAR szWindowClass[MAX_LOADSTRING];
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RAYTRACING, szWindowClass, MAX_LOADSTRING);
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAYTRACING));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_RAYTRACING);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    RECT wSize = { 0, 0, INIT_WIDTH, INIT_HEIGHT };
    AdjustWindowRect(&wSize, WS_OVERLAPPEDWINDOW, TRUE);
    int wWidth = wSize.right - wSize.left;
    int wHeight = wSize.bottom - wSize.top;
    HWND hWnd = CreateWindowW(
        szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       (GetSystemMetrics(SM_CXSCREEN) - wWidth) / 2,
       (GetSystemMetrics(SM_CYSCREEN) - wHeight) / 2,
       wWidth, wHeight, NULL, NULL, hInstance, NULL
    );
    if (!hWnd) {
        return FALSE;
    }
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    if (!raytrace_init(&rt, INIT_WIDTH, INIT_HEIGHT))
        MessageBox(hWnd, _TEXT("Could not init raytrace"), _TEXT("Error"), MB_ICONERROR | MB_OK);

    set_option_samples(hWnd, INIT_SAMPLES);
    set_option_depth(hWnd, INIT_DEPTH);
    EnableMenuItem(GetMenu(hWnd), ID_RENDER_STOP, MF_BYCOMMAND | MF_DISABLED);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAYTRACING));
    MSG msg;
    BOOL running = TRUE;
    while (running) {
        if (GetMessage(&msg, NULL, 0, 0)) {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        if (!renderDone && !rt.isRendering)
            stop_render(hWnd);
        if (msg.message == WM_QUIT)
            running = FALSE;
    }
    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_TIMER:
        RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId) {
            case IDM_ABOUT:
                DialogBox((HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_RENDER_START:
                start_render(hWnd);
                break;
            case ID_RENDER_STOP:
                stop_render(hWnd);
                break;
            case ID_SAMPLES_2:
            case ID_SAMPLES_4:
            case ID_SAMPLES_8:
            case ID_SAMPLES_16:
            case ID_SAMPLES_32:
            case ID_SAMPLES_64:
            case ID_SAMPLES_128:
            case ID_SAMPLES_256:
            case ID_SAMPLES_512:
            case ID_SAMPLES_1024:
            case ID_SAMPLES_2048:
                set_option_samples(hWnd, wmId);
                break;
            case ID_DEPTH_2:
            case ID_DEPTH_4:
            case ID_DEPTH_8:
            case ID_DEPTH_16:
            case ID_DEPTH_32:
            case ID_DEPTH_64:
            case ID_DEPTH_128:
            case ID_DEPTH_256:
            case ID_DEPTH_512:
            case ID_DEPTH_1024:
            case ID_DEPTH_2048:
                set_option_depth(hWnd, wmId);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            raytrace_paint(&rt, hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void start_render(HWND hWnd) {
    renderDone = false;
    rt.samples = options.samples;
    rt.depth = options.depth;
    EnableMenuItem(GetMenu(hWnd), ID_RENDER_START, MF_BYCOMMAND | MF_DISABLED);
    EnableMenuItem(GetMenu(hWnd), ID_RENDER_STOP, MF_BYCOMMAND | MF_ENABLED);
    SetTimer(hWnd, 1, 33, (TIMERPROC)NULL);
    if (!raytrace_start_render(&rt))
        MessageBox(hWnd, _TEXT("Could not start raytrace"), _TEXT("Error"), MB_ICONERROR | MB_OK);
}

void stop_render(HWND hWnd) {
    if (renderDone) return;
    rt.isRendering = false;
    renderDone = true;
    raytrace_stop_render(&rt);
    KillTimer(hWnd, 1);
    RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
    EnableMenuItem(GetMenu(hWnd), ID_RENDER_START, MF_BYCOMMAND | MF_ENABLED);
    EnableMenuItem(GetMenu(hWnd), ID_RENDER_STOP, MF_BYCOMMAND | MF_DISABLED);
}

void set_option_samples(HWND hWnd, UINT id) {
    CheckMenuRadioItem(
        GetMenu(hWnd),
        ID_SAMPLES_2,
        ID_SAMPLES_2048,
        id,
        MF_BYCOMMAND
    );
    options.samples = (int)pow(2, id - ID_SAMPLES_2 + 1);
}

void set_option_depth(HWND hWnd, UINT id) {
    CheckMenuRadioItem(
        GetMenu(hWnd),
        ID_DEPTH_2,
        ID_DEPTH_2048,
        id,
        MF_BYCOMMAND
    );
    options.depth = (int)pow(2, id - ID_DEPTH_2 + 1);
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
