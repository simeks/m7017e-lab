#include <QApplication>
#include "qt/mainwindow.h"

#include "shared/common.h"


#if defined(_WIN32) && !defined(DEBUG)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main(int argc, char *argv[])
#endif
{
	gst_init(&argc, &argv);

#if defined(_WIN32) && !defined(DEBUG)
    char** argv = __argv;
    int argc = __argc;
#endif

    QApplication app(argc, argv);
    MainWindow w;
    w.show();
	
	return app.exec();
}
