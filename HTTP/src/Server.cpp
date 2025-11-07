/*Author: xzy*/
#include <Server.h>
#include <SimpleFileLoader.h>

using httplib::Request;
using httplib::Response;
using string = std::string;

namespace
{
	class MockService : public IService {
	public:
		bool IsSessionExists(const string& sessionId) override {
			return true;
		}

		StatusDTO Login(const string& userId, const string& password, const string& role, string& sessionId) override
		{
			return StatusDTO();
		}

		SignupDTO Signup(const string& userId, const string& password, const string& role, bool&success) override
		{
			return SignupDTO();
		}

		UserInfoDTO GetMe(const string& sessionId) override
		{
			return UserInfoDTO();
		}

		JobHunterDTO GetJobHunterInfo(int id) override
		{
			return JobHunterDTO();
		}

		StatusDTO UpdateHunterInfo(int id, const JobHunter& newInfo) override
		{
			return StatusDTO();
		}

		std::vector<JobApplicationDTO> GetJobApplicationsByJobHunter(int id) override
		{
			return std::vector<JobApplicationDTO>();
		}

		CompanyDTO GetCompanyById(int id) override
		{
			return CompanyDTO();
		}

		StatusDTO UpdateCompanyInfo(int id, const Company& newInfo) override
		{
			return StatusDTO();
		}

		std::vector<JobDTO> SearchJobs(const string& keyword, const string& location) override
		{
			return std::vector<JobDTO>();
		}

		int CreateJob(const JobInfo& newJob) override
		{
			return 0;
		}

		JobApplicationDTO ApplyForJob(int id) override
		{
			return JobApplicationDTO();
		}

		std::vector<JobApplicationDTO> GetJobApplicationsByCompany(int id) override
		{
			return std::vector<JobApplicationDTO>();
		}

		StatusDTO UpdateJobApplicationStatus(int id) override
		{
			return StatusDTO();
		}

		std::vector<Company> GetPendingCompaniesEdits() override
		{
			return std::vector<Company>();
		}

		std::vector<JobInfo> GetPendingJobsEdits() override
		{
			return std::vector<JobInfo>();
		}

		StatusDTO CheckPending(const string& type, bool isApproved) override
		{
			return StatusDTO();
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
