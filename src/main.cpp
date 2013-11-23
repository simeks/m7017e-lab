#include "common.h"

#include "player.h"

#include <QApplication>
#include "qt/playerwindow.h"


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

    QApplication app(argc, argv);
	PlayerWindow window;

	window.show();

	return app.exec();


}
