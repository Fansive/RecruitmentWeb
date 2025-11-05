// 模拟职位数据
const jobs = [
  {
    id: 1,
    title: "前端开发工程师",
    company: "字节科技",
    desc: "负责Web前端开发,熟悉React/Vue。",
    salary: "12k-20k",
    location: "北京"
  },
  {
    id: 2,
    title: "后端开发工程师",
    company: "阿里云计算",
    desc: "负责服务器端开发,熟悉C++/Java。",
    salary: "15k-25k",
    location: "杭州"
  },
  {
    id: 3,
    title: "数据分析师",
    company: "腾讯科技",
    desc: "负责数据建模与可视化,熟悉Python。",
    salary: "10k-18k",
    location: "深圳"
  },
  {
    id: 4,
    title: "UI设计师",
    company: "美团设计中心",
    desc: "负责界面设计与用户体验优化。",
    salary: "10k-16k",
    location: "北京"
  },
  {
    id: 5,
    title: "测试工程师",
    company: "京东科技",
    desc: "负责自动化测试与性能分析。",
    salary: "9k-15k",
    location: "上海"
  },
];

const jobList = document.getElementById("jobList");
const modal = document.getElementById("jobModal");
let currentJobIndex = -1;

function renderJobs() {
  jobList.innerHTML = jobs
    .map(
      (job, i) => `
        <div class="job-card" onclick="showDetail(${i})">
          <h3>${job.title}</h3>
          <p>${job.company}</p>
          <p>${job.desc.slice(0, 20)}...</p>
        </div>
      `
    )
    .join("");
}

function showDetail(index) {
  currentJobIndex = index;
  const job = jobs[index];
  document.getElementById("jobTitle").textContent = job.title;
  document.getElementById("jobCompany").textContent = "公司：" + job.company;
  document.getElementById("jobDesc").textContent = job.desc;
  modal.style.display = "flex";
}

function closeModal() {
  modal.style.display = "none";
}

function applyJob() {
  if (currentJobIndex === -1) return;

  const job = jobs[currentJobIndex];

  // 检查投递记录功能是否可用
  if (typeof addApplicationRecord === 'function') {
    // 调用投递记录系统
    const record = addApplicationRecord(job, job.company);
    if (record) {
      alert("已投递简历！记录已保存到投递记录中。");
    } else {
      alert("投递失败，可能已投递过该职位");
    }
  } else {
    // 如果投递记录系统不可用，使用默认提示
    alert("已投递简历！");
  }

  closeModal();
}

// function search() {
//   const keyword = document.getElementById("searchInput").value.trim();
//   alert(`搜索 "${keyword}" 的职位或公司`);
// }

// function goTo(page) {
//   alert("跳转到：" + page);
// }

renderJobs();