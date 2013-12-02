#include "shared/common.h"


#if defined(_WIN32) && !defined(DEBUG)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main(int argc, char *argv[])
#endif
{

#if defined(_WIN32) && !defined(DEBUG)
    char* argv[] = { "Main.exe" };
    int argc = 1;
#endif

	debug::Printf("Hello world!");


	return 0;
}
