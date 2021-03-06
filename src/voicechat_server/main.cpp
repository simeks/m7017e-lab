#include "shared/common.h"

#include "server.h"

#include <QCoreApplication>
#include <gst/gst.h>

void ShowHelp()
{
	std::cout	<< "Command-line arguments:" << std::endl
				<< "-p <port>\tSpecifies which port (udp/tcp) to listen on." << std::endl
				<< "--port <port>\tSpecifies which port (udp/tcp) to listen on." << std::endl;

}

bool ParseCommandLine(int argc, char* argv[], int& port)
{
	int i = 1;
	while(i < argc)
	{
		const char* str = argv[i];
		if(str[0] != '-')
			break;

		if(str[1] == '-') // '--'
		{
			if((strcmp(str+2, "port") == 0) && (i+1 < argc)) // --port <port>
			{
				port = atoi(argv[++i]);
			}
			else
				return false; // Invalid usage
		}
		else // Single '-'
		{
			if(str[1] == 'p' && (i+1 < argc)) // -p <port>
			{
				port = atoi(argv[++i]);
			}
			else
				return false; // Invalid usage
		}

		++i;
	}
	return true;
}

int main(int argc, char* argv[])
{
	// initialize gstreamer
	gst_init(&argc, &argv);

	int port = 25010;
	if(!ParseCommandLine(argc, argv, port))
	{
		ShowHelp();
		return 1;
	}

	QCoreApplication app(argc, argv);

	Server server(port, port);

	return app.exec();
}