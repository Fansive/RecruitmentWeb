document.addEventListener("DOMContentLoaded", () => {
    // 插入统一导航栏到 body 最上方
    document.body.insertAdjacentHTML("afterbegin", `
    <header>
      <a class="logo" href="index.html">🏢 招聘网</a>

      <div class="search-box">
        <input type="text" id="searchInput" placeholder="搜索公司 / 职位..." />
        <button onclick="search()">搜索</button>
      </div>

      <div class="menu">
        <button onclick="goTo('profile')">我的</button>
        <button onclick="goTo('records')">投递记录</button>
      </div>
    </header>
  `);
});

/* 通用搜索功能 */
function search() {
    const keyword = document.getElementById("searchInput")?.value.trim();
    if (!keyword) return alert("请输入搜索关键词");
    alert(`搜索 "${keyword}" 的职位或公司`);
}

/* 通用页面跳转功能 */
function goTo(page) {
    if (page === "profile") location.href = "profile.html";
    else if (page === "records") alert("跳转到投递记录页面（待实现）");
    else if (page === "home") location.href = "index.html";
}
