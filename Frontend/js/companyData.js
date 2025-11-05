// 公司职位数据管理
let companyJobs = null;

// 初始化职位数据
function initCompanyJobs() {
    if (companyJobs === null) {
        const savedJobs = localStorage.getItem('companyJobs');

        console.log("从本地存储加载的数据:", savedJobs);

        if (savedJobs) {
            // 如果本地存储中有数据，使用保存的数据
            companyJobs = JSON.parse(savedJobs);
            console.log("使用保存的数据，职位数量:", companyJobs.length);
        } else {
            // 如果没有保存的数据，使用默认数据（两个示例职位）
            console.log("没有保存的数据，创建示例数据");
            companyJobs = [
                {
                    id: 1,
                    title: "前端开发工程师",
                    type: "全职",
                    salary: "12k-20k",
                    location: "北京",
                    description: "负责公司Web前端开发工作，使用React/Vue等技术栈。",
                    requirements: "3年以上前端开发经验，熟悉React/Vue框架",
                    applications: [
                        { id: 1, name: "张三", status: "待处理" },
                        { id: 2, name: "李四", status: "待处理" }
                    ]
                },
                {
                    id: 2,
                    title: "UI设计师",
                    type: "全职",
                    salary: "10k-16k",
                    location: "上海",
                    description: "负责公司产品界面设计和用户体验优化。",
                    requirements: "2年以上UI设计经验，熟练使用Sketch/Figma",
                    applications: [
                        { id: 3, name: "王五", status: "待处理" }
                    ]
                }
            ];
            // 保存示例数据到本地存储
            saveCompanyJobsToStorage();
            console.log("示例数据已保存到本地存储");
        }
    }
    return companyJobs;
}

// 从本地存储加载职位数据
function loadCompanyJobsFromStorage() {
    const jobs = initCompanyJobs();
    console.log("加载的职位数据:", jobs);
    return jobs;
}

// 保存职位数据到本地存储
function saveCompanyJobsToStorage() {
    if (companyJobs !== null) {
        localStorage.setItem('companyJobs', JSON.stringify(companyJobs));
        console.log("数据已保存到本地存储:", companyJobs);
    }
}

// 获取职位数据
function getCompanyJobs() {
    return loadCompanyJobsFromStorage();
}

// 设置职位数据
function setCompanyJobs(jobs) {
    companyJobs = jobs;
    saveCompanyJobsToStorage();
}


