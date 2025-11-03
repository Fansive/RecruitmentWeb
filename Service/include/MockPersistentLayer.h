#ifndef MOCK_PERSISTENT_LAYER_H
#define MOCK_PERSISTENT_LAYER_H

#include "DomainModels.h"
#include <optional>

/**
 * 模拟持久化层接口
 * 实际项目中，这会是 Persistent 层的公共接口
 */
class IMockPersistentLayer
{
public:
    virtual ~IMockPersistentLayer() = default;

    // --- User/Registration ---
    virtual bool saveNewUser(const UserBase &user) = 0;
    virtual std::optional<UserBase> getUserByEmail(const std::string &email) = 0;
    virtual bool updatePassword(const std::string &userId, const std::string &newHash) = 0;

    // --- Profile Management ---
    virtual std::optional<JobHunter> getJobHunterById(const std::string &userId) = 0;
    virtual bool updateJobHunterProfile(const JobHunter &profile) = 0;

    virtual std::optional<Company> getCompanyById(const std::string &userId) = 0;
    virtual bool updateCompanyProfile(const Company &profile) = 0;

    // --- Job Posting ---
    virtual std::vector<JobPosting> findJobs(const std::string &keywords, const std::string &location) = 0;
    virtual bool saveJobPosting(const JobPosting &post) = 0;
    virtual bool updateJobPosting(const JobPosting &post) = 0;

    // --- Application ---
    virtual bool saveApplication(const ApplicationRecord &record) = 0;
    virtual std::vector<ApplicationRecord> findApplicationsByJobHunter(const std::string &jobHunterId) = 0;
    virtual std::map<ApplicationRecord, JobHunter> findApplicationsByJobId(const std::string &jobId) = 0;
    virtual bool updateApplicationStatus(const std::string &applicationId, const std::string &newStatus) = 0;
};

// 实际使用时，Service层将通过依赖注入获取该接口的一个具体实现
class PersistentLayerImpl : public IMockPersistentLayer
{
public:
    bool saveNewUser(const UserBase &user) override;
    std::optional<UserBase> getUserByEmail(const std::string &email) override;
    bool updatePassword(const std::string &userId, const std::string &newHash) override;
    std::optional<JobHunter> getJobHunterById(const std::string &userId) override;
    bool updateJobHunterProfile(const JobHunter &profile) override;
    std::optional<Company> getCompanyById(const std::string &userId) override;
    bool updateCompanyProfile(const Company &profile) override;
    std::vector<JobPosting> findJobs(const std::string &keywords, const std::string &location) override;
    bool saveJobPosting(const JobPosting &post) override;
    bool updateJobPosting(const JobPosting &post) override;
    bool saveApplication(const ApplicationRecord &record) override;
    std::vector<ApplicationRecord> findApplicationsByJobHunter(const std::string &jobHunterId) override;
    std::map<ApplicationRecord, JobHunter> findApplicationsByJobId(const std::string &jobId) override;
    bool updateApplicationStatus(const std::string &applicationId, const std::string &newStatus) override;
};

#endif // MOCK_PERSISTENT_LAYER_H
