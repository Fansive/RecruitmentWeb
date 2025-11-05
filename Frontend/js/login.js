// login.js

function showRegister() {
  document.getElementById('loginForm').style.display = 'none';
  document.getElementById('registerForm').style.display = 'flex';

  // 注册页面时设置一个特殊状态
  setUserRole('registering');
}

function showLogin() {
  document.getElementById('registerForm').style.display = 'none';
  document.getElementById('loginForm').style.display = 'flex';

  // 返回登录页面时清除特殊状态
  localStorage.removeItem('userRole');
}

async function login() {
  const form = document.getElementById('loginForm');
  const role = form.role.value;
  const id = form.username.value.trim();
  const password = form.password.value.trim();

  if (!id || !password) {
    alert('请填写账号和密码');
    return;
  }

  console.log('登录信息:', { role, id, password });

  // 设置用户角色
  setUserRole(role);

  // 登录成功后跳转
  if (role === 'jobSeeker') window.location.href = 'index.html';
  else if (role === 'company') window.location.href = 'company.html';
  else if (role === 'admin') window.location.href = 'admin.html';
}

function register() {
  const form = document.getElementById('registerForm');
  const role = form.role.value;
  const username = form.username.value.trim();
  const password = form.password.value.trim();
  const confirmPassword = form.confirmPassword.value.trim();

  if (!username || !password || !confirmPassword) {
    alert('请完整填写账号和密码');
    return;
  }

  if (password !== confirmPassword) {
    alert('两次输入的密码不一致');
    return;
  }

  console.log('注册信息:', { role, username, password });

  // 设置用户角色
  setUserRole(role);

  alert(`注册成功！身份：${getRoleDisplayName(role)}，账号：${username}`);

  // 注册完成后自动跳转到对应页面
  if (role === 'jobSeeker') window.location.href = 'index.html';
  else if (role === 'company') window.location.href = 'company.html';
  else if (role === 'admin') window.location.href = 'admin.html';
}

// 获取角色显示名称
function getRoleDisplayName(role) {
  const roleMap = {
    'jobSeeker': '求职者',
    'company': '招聘方',
    'admin': '管理员'
  };
  return roleMap[role] || role;
}