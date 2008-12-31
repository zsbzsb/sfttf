#include "App.hpp"

int main(int argc, char* argv[])
{
	try
	{
		App theApp;
		theApp.Run(argc, argv);
	}
	catch (const std::exception& e)
	{
		printf("Exception: %s\n", e.what());
		return 1;
	}
	return 0;
}
