// 初始化页面
document.addEventListener("DOMContentLoaded", function () {
    setUserRole('company');
    // 初始化数据
    loadCompanyJobsFromStorage();
    renderCompanyJobs();
});

// 渲染公司发布的职位
function renderCompanyJobs() {
    const jobList = document.getElementById("companyJobList");
    const jobs = getCompanyJobs();

    if (jobs.length === 0) {
        jobList.innerHTML = '<p class="no-jobs-message">暂无发布的职位</p>';
        return;
    }

    jobList.innerHTML = jobs.map(job => `
        <div class="job-management-card">
            <div class="job-header">
                <div class="job-info">
                    <h3 class="job-title">${job.title}</h3>
                    <p class="job-meta"><strong>类型:</strong> ${job.type} | <strong>薪资:</strong> ${job.salary}</p>
                    <p class="job-meta"><strong>地点:</strong> ${job.location}</p>
                    <p class="job-description">${job.description}</p>
                    <p class="job-requirements"><strong>要求:</strong> ${job.requirements}</p>
                </div>
                <div class="application-count">
                    <div class="count-number">${job.applications.length}</div>
                    <div class="count-label">申请数</div>
                </div>
            </div>
            <div class="job-actions">
                <button class="btn-view-applications" onclick="viewApplications(${job.id})">查看申请</button>
                <button class="btn-edit-job" onclick="editJob(${job.id})">编辑</button>
                <button class="btn-delete-job" onclick="deleteJob(${job.id})">删除</button>
            </div>
        </div>
    `).join("");
}

// 查看职位申请
function viewApplications(jobId) {
    const jobs = getCompanyJobs();
    const job = jobs.find(j => j.id === jobId);
    if (!job) return;

    const applicationsList = document.getElementById("applicationsList");
    applicationsList.innerHTML = `
        <h3 class="applications-title">${job.title} - 申请列表</h3>
        ${job.applications.length === 0 ?
            '<p class="no-applications-message">暂无申请</p>' :
            job.applications.map(app => `
                <div class="application-item">
                    <div class="application-header">
                        <span class="applicant-name">${app.name}</span>
                        <span class="application-status ${getStatusClass(app.status)}">${app.status}</span>
                    </div>
                    <div class="application-actions">
                        <button class="btn-agree" onclick="updateApplicationStatus(${jobId}, ${app.id}, '已同意')">同意</button>
                        <button class="btn-reject" onclick="updateApplicationStatus(${jobId}, ${app.id}, '已拒绝')">拒绝</button>
                    </div>
                </div>
            `).join("")
        }
    `;

    document.getElementById("applicationsModal").style.display = "flex";
}

// 获取状态对应的CSS类
function getStatusClass(status) {
    const statusMap = {
        '待处理': 'status-pending',
        '已同意': 'status-agreed',
        '已拒绝': 'status-rejected'
    };
    return statusMap[status] || '';
}

// 关闭申请管理模态框
function closeApplicationsModal() {
    document.getElementById("applicationsModal").style.display = "none";
}

// 更新申请状态
function updateApplicationStatus(jobId, applicationId, status) {
    const jobs = getCompanyJobs();
    const job = jobs.find(j => j.id === jobId);
    if (job) {
        const application = job.applications.find(app => app.id === applicationId);
        if (application) {
            application.status = status;
            setCompanyJobs(jobs); // 保存到本地存储
            renderCompanyJobs();
            viewApplications(jobId);
            alert(`已将 ${application.name} 的申请${status === '已同意' ? '同意' : '拒绝'}`);
        }
    }
}

// 编辑职位
function editJob(jobId) {
    const jobs = getCompanyJobs();
    const job = jobs.find(j => j.id === jobId);
    if (job) {
        // 跳转到发布页面并填充数据（这里需要实现数据传递）
        alert("编辑功能需要跳转到发布页面，待实现");
    }
}

// 删除职位
function deleteJob(jobId) {
    if (confirm("确定要删除这个职位吗？")) {
        const jobs = getCompanyJobs();
        const updatedJobs = jobs.filter(job => job.id !== jobId);
        setCompanyJobs(updatedJobs); // 保存到本地存储
        renderCompanyJobs();
        alert("职位删除成功！");
    }
}

// 发布新岗位（使用goTo函数跳转）
function showAddJobModal() {
    goTo('jobPublish');
}