// login.js

function showRegister() {
    document.getElementById('loginForm').style.display = 'none';
    document.getElementById('registerForm').style.display = 'flex';
  }
  
  function showLogin() {
    document.getElementById('registerForm').style.display = 'none';
    document.getElementById('loginForm').style.display = 'flex';
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

    const response = await fetch("/api/login", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({ id, password })
    });

    const data = await response.json();
    console.log(data);
  
    // 登录成功后跳转示例
    if (role === 'jobSeeker') window.location.href = 'index.html';
    else if (role === 'recruiter') window.location.href = 'index.html';
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
    alert(`模拟注册成功！身份：${role}，账号：${username}`);
  
    // 注册完成后自动切回登录
    showLogin();
  }
  