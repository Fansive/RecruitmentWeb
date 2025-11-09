#include <Server.h>
#include <DbConnector.h>
#include <SimpleFileLoader.h>
#include <Logger.h>

int main()
{
	DbConnector::Instance.Connect();
	std::cout << HttpHandler::GetURL() << std::endl;
	HttpHandler::Instance.RegisterRoutingAndRun();
	return 0;
}
