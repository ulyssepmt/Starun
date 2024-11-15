#include "utils.h"

void CleanUpGameScreen(HWND gameInterface, int xPos, int yPos, int wRectangle, int hRectangle) {

        PatBlt(GetDC(gameInterface), xPos, yPos, wRectangle, hRectangle, BLACKNESS);
}

void CleanUPResources(HWND hWND, HDC hDC, HDC hDCMem, HBITMAP hObject) {

    if (hDCMem) {
        // Sélectionne un objet stocké précédemment dans le DC
        SelectObject(hDCMem, GetStockObject(NULL_BRUSH));

        DeleteDC(hDCMem);
    }

    if (hObject) {
        DeleteObject(hObject);
    }

    if (hDC) {
        ReleaseDC(hWND, hDC);
    }

    if(hWND) {
        DestroyWindow(hWND);
    }
}

void ShowGameError(LPCSTR message) {

    MessageBox(0, message, "Starun", MB_OK | MB_ICONERROR);

    ExitProcess(1);

}

/* debugging
void checkALError(const char* msg) {
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        printf("OpenAL error (%s): %s\n", msg, alGetString(error));
    }
}
*/








