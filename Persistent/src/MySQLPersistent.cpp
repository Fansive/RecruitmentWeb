#include "MySQLPersistent.h"
#include <DbConnector.h>
#include <sstream>
#include <algorithm>

std::string EscapeSQL(const std::string& str) {
    std::string result;
    result.reserve(str.length() * 2);
    for (char c : str) {
        if (c == '\'') {
            result += "''";
        } else if (c == '\\') {
            result += "\\\\";
        } else {
            result += c;
        }
    }
    return result;
}

int MySQLPersistent::CreateUser(const std::string& userId, const std::string& role)
{
    std::string sql;
    if (role == "JobHunter") {
        sql = "INSERT INTO JobHunters (userId, password) VALUES ('" + EscapeSQL(userId) + "', '')";
    } else if (role == "Company") {
        sql = "INSERT INTO Companies (userId, password) VALUES ('" + EscapeSQL(userId) + "', '')";
    } else {
        return 0;  
    }
    
    int rowsAffected = DbConnector::Instance.ExecuteUpdate(sql);
    if (rowsAffected <= 0) {
        return 0; 
    }
    
    auto result = DbConnector::Instance.ExecuteQuery("SELECT LAST_INSERT_ID() as id");
    if (!result.empty()) {
        return std::stoi(result[0].at("id"));
    }
    return 0;
}

int MySQLPersistent::GetUserIdByUserName(const std::string& userId)
{
     
    std::string sql = "SELECT id FROM JobHunters WHERE userId = '" + EscapeSQL(userId) + "'";
    auto result = DbConnector::Instance.ExecuteQuery(sql);
    if (!result.empty()) {
        return std::stoi(result[0].at("id"));
    }
    
     
    sql = "SELECT id FROM Companies WHERE userId = '" + EscapeSQL(userId) + "'";
    result = DbConnector::Instance.ExecuteQuery(sql);
    if (!result.empty()) {
        return std::stoi(result[0].at("id"));
    }
    
    return 0;
}

UserInfoDTO MySQLPersistent::GetUserInfo(const int id)
{
    UserInfoDTO dto;
    
     
    std::string sql = "SELECT id, userId, email FROM JobHunters WHERE id = " + std::to_string(id);
    auto result = DbConnector::Instance.ExecuteQuery(sql);
    if (!result.empty()) {
        dto.id = result[0].at("id");
        dto.userId = result[0].at("userId");
        dto.role = "JobHunter";
        dto.email = result[0].count("email") ? result[0].at("email") : "";
        return dto;
    }
    
     
    sql = "SELECT id, userId, email FROM Companies WHERE id = " + std::to_string(id);
    result = DbConnector::Instance.ExecuteQuery(sql);
    if (!result.empty()) {
        dto.id = result[0].at("id");
        dto.userId = result[0].at("userId");
        dto.role = "Company";
        dto.email = result[0].count("email") ? result[0].at("email") : "";
        return dto;
    }
    
    return dto;  
}

JobHunterDTO MySQLPersistent::GetJobHunterInfo(int id)
{
    JobHunterDTO dto;
    std::string sql = "SELECT * FROM JobHunters WHERE id = " + std::to_string(id);
    auto result = DbConnector::Instance.ExecuteQuery(sql);
    
    if (!result.empty()) {
        auto& row = result[0];
        dto.id = row.at("id");
        dto.userId = row.at("userId");
        dto.createdAt = row.count("createdAt") ? row.at("createdAt") : "";
        
        dto.info.userId = row.at("userId");
        dto.info.passwordHash = row.count("password") ? row.at("password") : "";
        dto.info.email = row.count("email") ? row.at("email") : "";
        dto.info.role = "JobHunter";
        dto.info.name = row.at("userId"); 
        dto.info.address = row.count("address") ? row.at("address") : "";
        dto.info.gender = row.count("gender") ? row.at("gender") : "";
        dto.info.phoneNumber = row.count("phoneNumber") ? row.at("phoneNumber") : "";
        dto.info.degree = row.count("degree") ? row.at("degree") : "";
        dto.info.graduationYear = row.count("graduationYear") && !row.at("graduationYear").empty() 
                                  ? std::stoi(row.at("graduationYear")) : 0;
        dto.info.school = row.count("school") ? row.at("school") : "";
        dto.info.major = row.count("major") ? row.at("major") : "";
        dto.info.resumeUrl = row.count("resumeUrl") ? row.at("resumeUrl") : "";
    }
    
    return dto;
}

StatusDTO MySQLPersistent::UpdateHunterInfo(int id, const JobHunter& newInfo)
{
    StatusDTO dto;
    dto.success = false;
    
    std::ostringstream sql;
    sql << "UPDATE JobHunters SET ";
    sql << "email = " << (newInfo.email.empty() ? "NULL" : "'" + EscapeSQL(newInfo.email) + "'") << ", ";
    sql << "address = " << (newInfo.address.empty() ? "NULL" : "'" + EscapeSQL(newInfo.address) + "'") << ", ";
    sql << "gender = " << (newInfo.gender.empty() ? "NULL" : "'" + EscapeSQL(newInfo.gender) + "'") << ", ";
    sql << "phoneNumber = " << (newInfo.phoneNumber.empty() ? "NULL" : "'" + EscapeSQL(newInfo.phoneNumber) + "'") << ", ";
    sql << "degree = " << (newInfo.degree.empty() ? "NULL" : "'" + EscapeSQL(newInfo.degree) + "'") << ", ";
    sql << "graduationYear = " << (newInfo.graduationYear > 0 ? std::to_string(newInfo.graduationYear) : "NULL") << ", ";
    sql << "school = " << (newInfo.school.empty() ? "NULL" : "'" + EscapeSQL(newInfo.school) + "'") << ", ";
    sql << "major = " << (newInfo.major.empty() ? "NULL" : "'" + EscapeSQL(newInfo.major) + "'") << ", ";
    sql << "resumeUrl = " << (newInfo.resumeUrl.empty() ? "NULL" : "'" + EscapeSQL(newInfo.resumeUrl) + "'") << " ";
    sql << "WHERE id = " << id;
    
    int rows = DbConnector::Instance.ExecuteUpdate(sql.str());
    dto.success = (rows > 0);
    
    return dto;
}

std::vector<JobApplicationDTO> MySQLPersistent::GetJobApplicationsByJobHunter(int id)
{
    std::vector<JobApplicationDTO> resultList;
    
    std::string sql = "SELECT ja.id, ja.jobId, ja.appliedAt, ja.status, "
                     "j.title, j.description, j.location, j.salary, j.degreeRequirement, j.companyId, "
                     "c.userId as companyName "
                     "FROM JobApplications ja "
                     "JOIN Jobs j ON ja.jobId = j.id "
                     "JOIN Companies c ON j.companyId = c.id "
                     "WHERE ja.hunterId = " + std::to_string(id);
    
    auto result = DbConnector::Instance.ExecuteQuery(sql);
    
    for (auto& row : result) {
        JobApplicationDTO dto;
        dto.id = row.at("id");
        dto.jobId = row.at("jobId");
        dto.appliedAt = row.count("appliedAt") ? row.at("appliedAt") : "";
        dto.status = row.count("status") ? row.at("status") : "";
        
        dto.jobInfo.id = std::stoi(row.at("jobId"));
        dto.jobInfo.title = row.count("title") ? row.at("title") : "";
        dto.jobInfo.companyName = row.count("companyName") ? row.at("companyName") : "";
        dto.jobInfo.location = row.count("location") ? row.at("location") : "";
        dto.jobInfo.salary = row.count("salary") ? row.at("salary") : "";
        dto.jobInfo.description = row.count("description") ? row.at("description") : "";
        dto.jobInfo.degreeRequirement = row.count("degreeRequirement") ? row.at("degreeRequirement") : "";
        
        resultList.push_back(dto);
    }
    
    return resultList;
}

CompanyDTO MySQLPersistent::GetCompanyById(int id)
{
    CompanyDTO dto;
    std::string sql = "SELECT * FROM Companies WHERE id = " + std::to_string(id);
    auto result = DbConnector::Instance.ExecuteQuery(sql);
    
    if (!result.empty()) {
        auto& row = result[0];
        dto.id = row.at("id");
        dto.userId = row.at("userId");
        dto.createdAt = row.count("createdAt") ? row.at("createdAt") : "";
        
        dto.companyInfo.id = std::stoi(row.at("id"));
        dto.companyInfo.userId = row.at("userId");
        dto.companyInfo.passwordHash = row.count("password") ? row.at("password") : "";
        dto.companyInfo.email = row.count("email") ? row.at("email") : "";
        dto.companyInfo.role = "Company";
        dto.companyInfo.name = row.at("userId"); // Use userId as name
        dto.companyInfo.address = row.count("address") ? row.at("address") : "";
        dto.companyInfo.companyType = row.count("companyType") ? row.at("companyType") : "";
        dto.companyInfo.scale = row.count("scale") ? row.at("scale") : "";
        dto.companyInfo.contactInfo = row.count("contactInfo") ? row.at("contactInfo") : "";
        dto.companyInfo.description = row.count("description") ? row.at("description") : "";
    }
    
    return dto;
}

StatusDTO MySQLPersistent::UpdateCompanyInfo(int id, const Company& newInfo)
{
    StatusDTO dto;
    dto.success = false;
    
    std::ostringstream sql;
    sql << "UPDATE Companies SET ";
    sql << "email = " << (newInfo.email.empty() ? "NULL" : "'" + EscapeSQL(newInfo.email) + "'") << ", ";
    sql << "address = " << (newInfo.address.empty() ? "NULL" : "'" + EscapeSQL(newInfo.address) + "'") << ", ";
    sql << "companyType = " << (newInfo.companyType.empty() ? "NULL" : "'" + EscapeSQL(newInfo.companyType) + "'") << ", ";
    sql << "scale = " << (newInfo.scale.empty() ? "NULL" : "'" + EscapeSQL(newInfo.scale) + "'") << ", ";
    sql << "contactInfo = " << (newInfo.contactInfo.empty() ? "NULL" : "'" + EscapeSQL(newInfo.contactInfo) + "'") << ", ";
    sql << "description = " << (newInfo.description.empty() ? "NULL" : "'" + EscapeSQL(newInfo.description) + "'") << " ";
    sql << "WHERE id = " << id;
    
    int rows = DbConnector::Instance.ExecuteUpdate(sql.str());
    dto.success = (rows > 0);
    
    return dto;
}

std::vector<JobDTO> MySQLPersistent::GetAllJobs()
{
    std::vector<JobDTO> resultList;
    
    std::string sql = "SELECT j.*, c.userId as companyName "
                     "FROM Jobs j "
                     "JOIN Companies c ON j.companyId = c.id";
    
    auto result = DbConnector::Instance.ExecuteQuery(sql);
    
    for (auto& row : result) {
        JobDTO dto;
        dto.companyId = std::stoi(row.at("companyId"));
        dto.createdAt = row.count("createdAt") ? row.at("createdAt") : "";
        
        dto.info.id = std::stoi(row.at("id"));
        dto.info.title = row.count("title") ? row.at("title") : "";
        dto.info.companyName = row.count("companyName") ? row.at("companyName") : "";
        dto.info.location = row.count("location") ? row.at("location") : "";
        dto.info.salary = row.count("salary") ? row.at("salary") : "";
        dto.info.description = row.count("description") ? row.at("description") : "";
        dto.info.degreeRequirement = row.count("degreeRequirement") ? row.at("degreeRequirement") : "";
        
        resultList.push_back(dto);
    }
    
    return resultList;
}

int MySQLPersistent::CreateJob(const JobInfo& newJob)
{
    std::ostringstream sql;
    sql << "INSERT INTO Jobs (companyId, title, location, salary, description, degreeRequirement) VALUES ("
        << newJob.companyId << ", "
        << "'" << EscapeSQL(newJob.title) << "', "
        << "'" << EscapeSQL(newJob.location) << "', "
        << "'" << EscapeSQL(newJob.salary) << "', "
        << "'" << EscapeSQL(newJob.description) << "', "
        << "'" << EscapeSQL(newJob.degreeRequirement) << "')";

    int rowsAffected = DbConnector::Instance.ExecuteUpdate(sql.str());
    if (rowsAffected <= 0) return 0;

    auto result = DbConnector::Instance.ExecuteQuery("SELECT LAST_INSERT_ID() as id");
    if (!result.empty()) {
        return std::stoi(result[0].at("id"));
    }
    return 0;
}


JobApplicationDTO MySQLPersistent::ApplyForJob(int jobId, int jobHunterId)
{
    JobApplicationDTO dto;

    if (jobId <= 0 || jobHunterId <= 0) return dto;

    std::ostringstream sql;
    sql << "INSERT INTO JobApplications (hunterId, jobId, appliedAt, status) VALUES ("
        << jobHunterId << ", "
        << jobId << ", "
        << "NOW(), 'pending')";

    int rows = DbConnector::Instance.ExecuteUpdate(sql.str());
    if (rows <= 0) return dto;

    auto result = DbConnector::Instance.ExecuteQuery("SELECT LAST_INSERT_ID() as id");
    if (result.empty()) return dto;

    dto.id = result[0].at("id");
    dto.jobId = std::to_string(jobId);
    dto.status = "pending";
    dto.appliedAt = "";  

    std::string jobSql = "SELECT j.*, c.userId as companyName FROM Jobs j JOIN Companies c ON j.companyId = c.id WHERE j.id = " + std::to_string(jobId);
    auto jobResult = DbConnector::Instance.ExecuteQuery(jobSql);
    if (!jobResult.empty()) {
        auto& row = jobResult[0];
        dto.jobInfo.id = std::stoi(row.at("id"));
        dto.jobInfo.title = row.count("title") ? row.at("title") : "";
        dto.jobInfo.companyName = row.count("companyName") ? row.at("companyName") : "";
        dto.jobInfo.location = row.count("location") ? row.at("location") : "";
        dto.jobInfo.salary = row.count("salary") ? row.at("salary") : "";
        dto.jobInfo.description = row.count("description") ? row.at("description") : "";
        dto.jobInfo.degreeRequirement = row.count("degreeRequirement") ? row.at("degreeRequirement") : "";
    }

    return dto;
}


std::vector<JobApplicationDTO> MySQLPersistent::GetJobApplicationsByCompany(int id)
{
    std::vector<JobApplicationDTO> resultList;
    
    std::string sql = "SELECT ja.id, ja.jobId, ja.appliedAt, ja.status, "
                     "j.title, j.description, j.location, j.salary, j.degreeRequirement, j.companyId, "
                     "c.userId as companyName "
                     "FROM JobApplications ja "
                     "JOIN Jobs j ON ja.jobId = j.id "
                     "JOIN Companies c ON j.companyId = c.id "
                     "WHERE j.companyId = " + std::to_string(id);
    
    auto result = DbConnector::Instance.ExecuteQuery(sql);
    
    for (auto& row : result) {
        JobApplicationDTO dto;
        dto.id = row.at("id");
        dto.jobId = row.at("jobId");
        dto.appliedAt = row.count("appliedAt") ? row.at("appliedAt") : "";
        dto.status = row.count("status") ? row.at("status") : "";
        
        dto.jobInfo.id = std::stoi(row.at("jobId"));
        dto.jobInfo.title = row.count("title") ? row.at("title") : "";
        dto.jobInfo.companyName = row.count("companyName") ? row.at("companyName") : "";
        dto.jobInfo.location = row.count("location") ? row.at("location") : "";
        dto.jobInfo.salary = row.count("salary") ? row.at("salary") : "";
        dto.jobInfo.description = row.count("description") ? row.at("description") : "";
        dto.jobInfo.degreeRequirement = row.count("degreeRequirement") ? row.at("degreeRequirement") : "";
        
        resultList.push_back(dto);
    }
    
    return resultList;
}

StatusDTO MySQLPersistent::UpdateJobApplicationStatus(int id, const string& status)
{
    StatusDTO dto;
    dto.success = false;
    
    std::string sql = "UPDATE JobApplications SET status = '" + EscapeSQL(status) + "' WHERE id = " + std::to_string(id);
    
    int rows = DbConnector::Instance.ExecuteUpdate(sql);
    dto.success = (rows > 0);
    
    return dto;
}

std::vector<PendingReview> MySQLPersistent::GetPendingCompaniesEdits()
{
    std::vector<PendingReview> resultList;
    
    std::string sql = "SELECT pr.*, c.userId as companyName "
                     "FROM PendingReviews pr "
                     "JOIN Companies c ON pr.companyId = c.id "
                     "WHERE pr.type = 'company_edit' AND pr.status = 'pending'";
    
    auto result = DbConnector::Instance.ExecuteQuery(sql);
    
    for (auto& row : result) {
        PendingReview review;
        review.id = std::stoi(row.at("id"));
        review.type = row.count("type") ? row.at("type") : "";
        review.companyId = row.count("companyId") ? row.at("companyId") : "";
        review.companyName = row.count("companyName") ? row.at("companyName") : "";
        review.status = row.count("status") ? row.at("status") : "";
        review.reviewDataJson = row.count("reviewData") ? row.at("reviewData") : "";
        review.createdAt = row.count("createdAt") ? row.at("createdAt") : "";
        review.reviewedAt = row.count("reviewedAt") && !row.at("reviewedAt").empty() ? row.at("reviewedAt") : "";
        review.reviewedBy = row.count("reviewedBy") && !row.at("reviewedBy").empty() ? row.at("reviewedBy") : "";
        
        resultList.push_back(review);
    }
    
    return resultList;
}

std::vector<PendingReview> MySQLPersistent::GetPendingJobsEdits()
{
    std::vector<PendingReview> resultList;
    
    std::string sql = "SELECT pr.*, c.userId as companyName "
                     "FROM PendingReviews pr "
                     "JOIN Companies c ON pr.companyId = c.id "
                     "WHERE pr.type = 'job_post' AND pr.status = 'pending'";
    
    auto result = DbConnector::Instance.ExecuteQuery(sql);
    
    for (auto& row : result) {
        PendingReview review;
        review.id = std::stoi(row.at("id"));
        review.type = row.count("type") ? row.at("type") : "";
        review.companyId = row.count("companyId") ? row.at("companyId") : "";
        review.companyName = row.count("companyName") ? row.at("companyName") : "";
        review.status = row.count("status") ? row.at("status") : "";
        review.reviewDataJson = row.count("reviewData") ? row.at("reviewData") : "";
        review.createdAt = row.count("createdAt") ? row.at("createdAt") : "";
        review.reviewedAt = row.count("reviewedAt") && !row.at("reviewedAt").empty() ? row.at("reviewedAt") : "";
        review.reviewedBy = row.count("reviewedBy") && !row.at("reviewedBy").empty() ? row.at("reviewedBy") : "";
        
        resultList.push_back(review);
    }
    
    return resultList;
}

StatusDTO MySQLPersistent::CheckPending(const std::string& type, bool isApproved, int pendingReviewId)
{
    StatusDTO dto;
    dto.success = false;

    if (pendingReviewId <= 0) return dto;

    std::string status = isApproved ? "approved" : "rejected";

    std::ostringstream sql;
    sql << "UPDATE PendingReviews SET "
        << "status = '" << status << "', "
        << "reviewedAt = NOW() "
        << "WHERE id = " << pendingReviewId
        << " AND type = '" << EscapeSQL(type) << "'";

    int rows = DbConnector::Instance.ExecuteUpdate(sql.str());
    dto.success = (rows > 0);
    return dto;
}

