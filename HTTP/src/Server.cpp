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

		JobApplicationDTO ApplyForJob(int id, int jobHunterId) override
		{
			return JobApplicationDTO();
		}

		std::vector<JobApplicationDTO> GetJobApplicationsByCompany(int id) override
		{
			return std::vector<JobApplicationDTO>();
		}

		StatusDTO UpdateJobApplicationStatus(int id, const string& status) override
		{
			return StatusDTO();
		}

		std::vector<PendingReview> GetPendingCompaniesEdits() override
		{
			return std::vector<PendingReview>();
		}

		std::vector<PendingReview> GetPendingJobsEdits() override
		{
			return std::vector<PendingReview>();
		}

		StatusDTO CheckPending(const string& type, bool isApproved, int pendingReviewId) override
		{
			return StatusDTO();
		}

	};
}
std::unique_ptr<IService> GetServiceImpl();
HttpHandler HttpHandler::Instance(GetServiceImpl());

void HttpHandler::RegisterRoutingAndRun() {
	server.set_mount_point("/", "./Frontend");
	server.set_pre_routing_handler(PreRoutingHandler);
	server.Post("/api/signup", SignupHandler);
	server.Post("/api/login", LoginHandler);
	server.Get("/api/me", GetMeHandler);
	server.Get(R"(/api/jobhunters/(\d+))", GetJobHunterInfoHandler);
	server.Put(R"(/api/jobhunters/(\d+))", UpdateHunterInfoHandler);
	server.Get(R"(/api/jobhunters/(\d+)/applications)", GetJobApplicationsByJobHunterHandler);
	server.Get(R"(/api/companies/(\d+))", GetCompanyByIdHandler);
	server.Put(R"(/api/companies/(\d+))", UpdateCompanyInfoHandler);
	server.Get(R"(/api/jobs?keyword=(\d+)&location=(\d+))", SearchJobsHandler);
	server.Post("/api/jobs", CreateJobHandler);
	server.Post(R"(/api/jobs/(\d+)/applications)", ApplyForJobHandler);
	server.Get(R"(/api/companies/(\d+)/applications)", GetJobApplicationsByCompanyHandler);
	server.Patch(R"(api/applications/(\d+))", UpdateJobApplicationStatusHandler);
	server.Get("/api/pending/company-edits", GetPendingCompaniesEditsHandler);
	server.Get("/api/pending/job-posts", GetPendingJobsEditsHandler);
	server.Get(R"(/api/pending/(\d+)/review)", CheckPendingHandler);
	server.listen(IP, Port);
}
