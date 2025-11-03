/*Author: xzy*/
#include <Server.h>
#include <SimpleFileLoader.h>

using httplib::Request;
using httplib::Response;
using string = std::string;

namespace
{
	class MockService : public IService
	{
	public:
		RetType Login(const string &id, const string &password, string &sessionId) override
		{
			printf("[received login]:id=%s; password=%s\n", id.c_str(), password.c_str());
			sessionId = "42";
			return RetType::Success;
		}
		RetType Signup(const string &id, const string &password, const string &role) override
		{
			printf("[received signup]:id=%s; password=%s\n", id.c_str(), password.c_str());
			return RetType::Success;
		}
		RetType IsSessionExists(const string &sessionId) override
		{
			return RetType::Session_Existed;
		}
	};
}
HttpHandler HttpHandler::Instance(std::unique_ptr<IService>(new MockService));

void HttpHandler::RegisterRoutingAndRun()
{
	server.set_mount_point("/", "./Frontend");
	server.set_pre_routing_handler(PreRoutingHandler);
	server.Post("/api/signup", SignupHandler);
	server.Post("/api/login", LoginHandler);
	server.listen(IP, Port);
}
