#pragma once

#include "IUserService.h"
#include "DomainModels.h"
#include <vector>
#include <memory>

using string = std::string;

class IAdminService : public IUserService
{
public:
    virtual ~IAdminService() = default;
    // 岗位管理
    virtual bool ReviewJobPosting(const string &jobId, bool approve) = 0;
};
