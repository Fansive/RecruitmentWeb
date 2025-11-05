document.addEventListener("DOMContentLoaded", () => {
  // 插入统一导航栏到 body 最上方
  const currentPage = window.location.pathname.split('/').pop();
  const isCompanyPage = currentPage === 'company.html';
  const isAdminPage = currentPage === 'admin.html';
  const isJobSeekerPage = currentPage === 'index.html';

  // 获取用户角色（这里可以从登录信息或本地存储中获取）
  const userRole = getUserRole(); // 默认返回 'jobSeeker'

  document.body.insertAdjacentHTML("afterbegin", `
    <header>
      <a class="logo" href="index.html">🏢 招聘网</a>

      <div class="search-box">
        <input type="text" id="searchInput" placeholder="搜索公司 / 职位..." />
        <button onclick="search()">搜索</button>
      </div>

      <div class="menu">
        ${getMenuButtons(userRole, isCompanyPage, isAdminPage, isJobSeekerPage)}
      </div>
    </header>
  `);
});

// 获取用户角色（模拟函数，实际应该从登录状态获取）
function getUserRole() {
  // 这里可以从 localStorage 或登录状态获取用户角色
  const role = localStorage.getItem('userRole') || '';
  return role;
}

// 根据用户角色生成菜单按钮
function getMenuButtons(userRole, isCompanyPage, isAdminPage, isJobSeekerPage) {
  switch (userRole) {
    case 'company':
      // 招聘方（公司）
      return `
        <button onclick="goTo('companyInfo')">公司信息</button>
        <button onclick="showAddJobModal()">发布新岗位</button>
        <button onclick="logout()">退出</button>
      `;

    case 'admin':
      // 管理员
      return `
        <button onclick="goTo('userManagement')">用户管理</button>
        <button onclick="goTo('jobManagement')">职位管理</button>
        <button onclick="goTo('systemSettings')">系统设置</button>
        <button onclick="logout()">退出</button>
      `;

    case 'jobSeeker':
      // 求职者
      return `
        <button onclick="goTo('profile')">我的</button>
        <button onclick="goTo('records')">投递记录</button>
        <button onclick="logout()">退出</button>
      `;
    case 'registering':
    default:
      // 注册状态和其他情况都返回空
      return '';
  }
}

/* 通用搜索功能 */
function search() {
  const keyword = document.getElementById("searchInput")?.value.trim();
  if (!keyword) return alert("请输入搜索关键词");
  alert(`搜索 "${keyword}" 的职位或公司`);
}

/* 通用页面跳转功能 */
function goTo(page) {
  switch (page) {
    case "profile":
      location.href = "profile.html";
      break;
    case "records":
      location.href = "records.html";
      break;
    case "home":
      location.href = "index.html";
      break;
    case "company":
      location.href = "company.html";
      break;
    case "companyInfo":
      location.href = "companyInfo.html";
      break;
    case "userManagement":
      alert("跳转到用户管理页面（待实现）");
      break;
    case "jobPublish":
      location.href = "jobPublish.html";
      break;
    case "systemSettings":
      alert("跳转到系统设置页面（待实现）");
      break;
    default:
      location.href = "index.html";
  }
}

/* 退出登录功能 */
function logout() {
  if (confirm("确定要退出登录吗？")) {
    // 清除登录状态
    localStorage.removeItem('userRole');
    localStorage.removeItem('userInfo');
    // 跳转到登录页面
    location.href = "login.html";
  }
}

/* 设置用户角色（用于登录后设置） */
function setUserRole(role) {
  localStorage.setItem('userRole', role);
}