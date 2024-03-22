#include <windows.h>
#include <stdbool.h>
#include <stdio.h>

#include "game.h"

//https://www.youtube.com/watch?v=PeelMbdDxgk <3 while coding

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg) {

    case WM_CREATE:


    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HBRUSH noir = CreateSolidBrush(RGB(0, 0, 0)); // black background
        SelectObject(hdc, noir);
        PatBlt(hdc, 0, 0, 500, 600, PATCOPY);
        EndPaint(hWnd, &ps);
        DeleteObject(noir);
        break;
    }

    case WM_SYSCOMMAND:
    {
        if ((wParam & 0xfff0) == SC_CLOSE) {
            DestroyWindow(hWnd);
            return 0;
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

    struct GAME game = {0};

    struct  GAME *pGame = &game;

    pGame->ui.w = 500;

    pGame->ui.h = 600;

    pGame->ui.gameName = "Starun";

    WNDCLASSEXA c;
    c.cbSize = sizeof(WNDCLASSEX);
    c.style = CS_DBLCLKS;
    c.lpfnWndProc = WndProc;
    c.cbClsExtra = 0;
    c.cbWndExtra = 0;
    c.hInstance = hInstance;
    c.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    c.hCursor = LoadCursor(NULL, IDC_ARROW);
    c.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    c.lpszMenuName = NULL;
    c.lpszClassName = pGame->ui.gameName;
    c.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&c)) {

        ShowGameError("[FATAL ERROR] Can't register class !");
    }

   HWND win = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, pGame->ui.gameName, pGame->ui.gameName,
                              WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
                              0, 0, pGame->ui.w, pGame->ui.h, NULL, NULL, hInstance, NULL);

    if (!win) {

        ShowGameError("[FATAL ERROR] Can't create game window!");
    }

    ShowWindow(win, nCmdShow);

    UpdateWindow(win);

    SetWindowPos(win, HWND_TOPMOST, ((pGame->ui.w + 800) / 2) - (pGame->ui.w / 2),
                 ((pGame->ui.h + 200) / 2) - (pGame->ui.h / 2), pGame->ui.w, pGame->ui.h, SWP_SHOWWINDOW);


    int libInput = MessageBoxA(0, "[WARNING] Before playing this game, you need to download the OPENAL library for Windows. Do you want to download it ?", "Starun",
               MB_YESNO | MB_ICONWARNING | MB_TOPMOST);

    if (libInput == IDYES) {

        ShellExecuteA(NULL,"open", "https://www.openal.org/downloads/" ,NULL, NULL, SW_SHOWNORMAL);

        MessageBoxA(0, "After that, restart the game and press no button to play !", "Starun", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);

        ExitProcess(0);
    }
    else {
        HANDLE hGame = CreateThread(0, 0, RunGame, (LPVOID)pGame, 0, 0);
    }



    //WaitForSingleObject(hGame, INFINITE);


    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
