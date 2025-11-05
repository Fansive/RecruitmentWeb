// 发布新岗位页面功能

// 初始化页面
document.addEventListener("DOMContentLoaded", function () {
    setUserRole('company');
});

// 发布职位
function publishJob() {
    const form = document.getElementById("addJobForm");
    const formData = new FormData(form);

    const jobs = getCompanyJobs();

    const newJob = {
        id: jobs.length + 1,
        title: formData.get("jobTitle"),
        type: formData.get("jobType"),
        salary: formData.get("salary"),
        location: formData.get("location"),
        description: formData.get("description"),
        requirements: formData.get("requirements"),
        applications: []
    };

    jobs.push(newJob);
    setCompanyJobs(jobs); // 保存到本地存储

    alert("职位发布成功！");
    // 发布成功后跳转回公司管理页面
    goTo('company');
}

// 返回上一页
function goBack() {
    goTo('company');
}