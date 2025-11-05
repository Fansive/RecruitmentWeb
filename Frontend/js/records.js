// 投递记录数据管理
let applicationRecords = null;

// 初始化投递记录数据
function initApplicationRecords() {
    if (applicationRecords === null) {
        const savedRecords = localStorage.getItem('applicationRecords');

        if (savedRecords) {
            applicationRecords = JSON.parse(savedRecords);
        } else {
            // 使用示例数据确保有内容显示
            applicationRecords = [
                {
                    id: 1,
                    jobId: 1,
                    jobTitle: "前端开发工程师",
                    company: "字节科技",
                    salary: "12k-20k",
                    location: "北京",
                    status: '已投递',
                    statusClass: 'status-submitted'
                },
                {
                    id: 2,
                    jobId: 2,
                    jobTitle: "UI设计师",
                    company: "腾讯科技",
                    salary: "10k-16k",
                    location: "上海",
                    status: '已录用',
                    statusClass: 'status-hired'
                },
                {
                    id: 3,
                    jobId: 3,
                    jobTitle: "后端开发工程师",
                    company: "阿里云计算",
                    salary: "15k-25k",
                    location: "杭州",
                    status: '已拒绝',
                    statusClass: 'status-rejected'
                }
            ];
            // 保存示例数据
            localStorage.setItem('applicationRecords', JSON.stringify(applicationRecords));
        }
    }
    return applicationRecords;
}

// 初始化页面
document.addEventListener("DOMContentLoaded", function () {
    // 设置用户角色
    if (typeof setUserRole === 'function') {
        setUserRole('jobSeeker');
    }

    // 渲染投递记录
    renderApplicationRecords();
});

// 渲染投递记录
function renderApplicationRecords() {
    const recordsContainer = document.getElementById("applicationRecords");

    if (!recordsContainer) {
        return;
    }

    // 获取数据
    const records = initApplicationRecords();

    if (records.length === 0) {
        recordsContainer.innerHTML = `
            <div class="no-records-message">
                <p>暂无投递记录</p>
                <p class="hint">快去首页发现心仪的职位吧！</p>
                <div class="buttons" style="margin-top: 30px; display: flex; justify-content: center;">
                    <button class="btn-go-home" onclick="goTo('home')" style="background: #0078d7; color: white; border: none; padding: 12px 24px; border-radius: 8px; cursor: pointer; font-size: 14px; font-weight: 500; transition: background 0.2s;">
                        ← 返回首页
                    </button>
                </div>
            </div>
        `;
        return;
    }

    recordsContainer.innerHTML = `
        ${records.map(record => `
            <div class="application-record-card">
                <div class="record-header">
                    <div class="record-info">
                        <h3 class="job-title">${record.jobTitle}</h3>
                        <p class="company-info"><strong>公司:</strong> ${record.company}</p>
                        <p class="job-meta"><strong>薪资:</strong> ${record.salary} | <strong>地点:</strong> ${record.location}</p>
                    </div>
                    <div class="status-info">
                        <span class="application-status ${record.statusClass}">${record.status}</span>
                    </div>
                </div>
                <div class="record-actions">
                    <button class="btn-view-details" onclick="viewJobDetails(${record.jobId})">查看职位</button>
                </div>
            </div>
        `).join('')}
        <div style="position: fixed; bottom: 30px; right: 30px; z-index: 1000;">
            <button class="btn-back-home" onclick="goTo('home')" style="background: #0078d7; color: white; border: none; padding: 12px 20px; border-radius: 25px; cursor: pointer; font-size: 14px; font-weight: 500; box-shadow: 0 2px 8px rgba(0,0,0,0.15); transition: all 0.2s; display: flex; align-items: center; gap: 8px;">
                <span>←</span>
                <span>返回首页</span>
            </button>
        </div>
    `;
}

// 添加新的投递记录
function addApplicationRecord(job, company) {
    const records = initApplicationRecords();

    // 检查是否已经投递过这个职位
    const existingRecord = records.find(record => record.jobId === job.id);
    if (existingRecord) {
        console.log("已经投递过该职位:", job.title);
        return null;
    }

    const newRecord = {
        id: records.length + 1,
        jobId: job.id,
        jobTitle: job.title,
        company: company,
        salary: job.salary || "面议",
        location: job.location || "未指定",
        status: '已投递',
        statusClass: 'status-submitted'
    };

    records.push(newRecord);
    applicationRecords = records;
    localStorage.setItem('applicationRecords', JSON.stringify(records));

    return newRecord;
}

// 更新投递状态
function updateApplicationStatus(recordId, status) {
    const records = initApplicationRecords();
    const record = records.find(r => r.id === recordId);

    if (record) {
        record.status = status;

        // 设置状态对应的CSS类 - 只有三种状态
        const statusMap = {
            '已投递': 'status-submitted',
            '已录用': 'status-hired',
            '已拒绝': 'status-rejected'
        };
        record.statusClass = statusMap[status] || 'status-submitted';

        localStorage.setItem('applicationRecords', JSON.stringify(records));
        return true;
    }
    return false;
}

// 查看职位详情
function viewJobDetails(jobId) {
    alert(`查看职位ID: ${jobId} 的详情`);
}