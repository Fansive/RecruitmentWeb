// 初始化页面
document.addEventListener("DOMContentLoaded", function () {
    setUserRole('company');
    loadCompanyInfo();
});

// 加载公司信息
function loadCompanyInfo() {
    const savedCompanyInfo = localStorage.getItem('companyInfo');
    if (savedCompanyInfo) {
        const companyInfo = JSON.parse(savedCompanyInfo);
        const form = document.getElementById("companyInfoForm");

        Object.keys(companyInfo).forEach(key => {
            const input = form.querySelector(`[name="${key}"]`);
            if (input) {
                input.value = companyInfo[key];
            }
        });
    }
}

// 保存公司信息
function saveCompanyInfo() {
    const form = document.getElementById("companyInfoForm");
    const formData = new FormData(form);

    const companyInfo = {};
    formData.forEach((value, key) => {
        companyInfo[key] = value;
    });

    localStorage.setItem('companyInfo', JSON.stringify(companyInfo));
    alert("公司信息已保存");
}

// 返回上一页
function goBack() {
    goTo('company');
}


// 发布新岗位（使用goTo函数跳转）
function showAddJobModal() {
    goTo('jobPublish');
}