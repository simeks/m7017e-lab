#include "qt/playerwindow.h"
#include "player.h"

#include <QApplication>


#if defined(_WIN32) && !defined(DEBUG)
int WINAPI WinMain(HINSTANCE hInstanceIn, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{

#if defined(_WIN32) && !defined(DEBUG)
    char* argv[] = { "Main.exe" };
    int argc = 1;
#endif

	Player player;

    QApplication a(argc, argv);
    PlayerWindow w;
    w.show();



    return a.exec();
}
