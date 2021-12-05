#include <csignal>
#include "Socket.hpp"

void signalHandler(int signum)
{
	std::cout << "\nInterrupt signal ( " << signum << " ) received.\n";
	g_exit = true;
}

void signalPipeHandler(int signum)
{
	std::cout << "\nInterrupt signal (SIGPIPE) received.\n";
	static_cast<void>(signum);
}
