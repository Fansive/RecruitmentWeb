/*Author: xzy*/
#include <Server.h>
using httplib::Request;
using httplib::Response;

void HttpHandler::RegisterPageRouting() {
	server.Get("/", [](const Request&, Response& res) {
		res.set_content(ReadFile("index.html"), "text/html");
	});
}

void HttpHandler::RegisterRoutingAndRun() {
	RegisterPageRouting();
	RegisterAccountManagementRouting();

	server.listen(IP, Port);
}



