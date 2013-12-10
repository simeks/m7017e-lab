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

	int tcp_port = 25010;
	int udp_min = 25011; // Udp range: 25011 - 25111 (max 100 ports)
	int udp_max = 25111;
	if(!ParseCommandLine(argc, argv, tcp_port))
	{
		ShowHelp();
		return 1;
	}

	QCoreApplication app(argc, argv);

	Server server(tcp_port, udp_min, udp_max);

	return app.exec();
}