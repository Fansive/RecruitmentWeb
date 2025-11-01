 // 模拟职位数据
 const jobs = [
    { title: "前端开发工程师", company: "字节科技", desc: "负责Web前端开发,熟悉React/Vue。" },
    { title: "后端开发工程师", company: "阿里云计算", desc: "负责服务器端开发,熟悉C++/Java。" },
    { title: "数据分析师", company: "腾讯科技", desc: "负责数据建模与可视化,熟悉Python。" },
    { title: "UI设计师", company: "美团设计中心", desc: "负责界面设计与用户体验优化。" },
    { title: "测试工程师", company: "京东科技", desc: "负责自动化测试与性能分析。" },
  ];

  const jobList = document.getElementById("jobList");
  const modal = document.getElementById("jobModal");

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
    alert("已投递简历！");
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