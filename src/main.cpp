#include "qt/playerwindow.h"
#include <QApplication>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstanceIn, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{

#ifdef _WIN32
        char* argv[] = { "Main.exe" };
        int argc = 1;
#endif

    QApplication a(argc, argv);
    PlayerWindow w;
    w.show();

    return a.exec();
}
