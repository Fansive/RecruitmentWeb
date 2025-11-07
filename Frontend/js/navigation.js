/**
 * 导航栏生成和管理
 */

/**
 * 根据角色生成导航菜单项
 */
function getNavMenuItems(role) {
    const items = [];
    
    switch(role) {
        case USER_ROLES.JOBHUNTER:
            items.push(
                { text: '个人信息', href: 'pages/candidate-edit.html' },
                { text: '投递记录', href: 'pages/candidate-applications.html' }
            );
            break;
        case USER_ROLES.COMPANY:
            items.push(
                { text: '公司信息', href: 'pages/company-edit.html' },
                { text: '发布岗位', href: 'pages/company-post-job.html' },
                { text: '岗位申请记录', href: 'pages/company-applications.html' }
            );
            break;
        case USER_ROLES.ADMIN:
            items.push(
                { text: '审核申请', href: 'pages/admin-review.html' }
            );
            break;
        default:
            // 未登录状态，只显示登录按钮
            items.push(
                { text: '登录/注册', href: 'login.html' }
            );
    }
    
    return items;
}

/**
 * 生成导航栏HTML
 */
function generateNavbar() {
    const role = getCurrentRole();
    const menuItems = getNavMenuItems(role);
    const currentPath = window.location.pathname;
    const currentFile = currentPath.split('/').pop() || 'index.html';
    
    // 判断当前页面是否在pages目录下
    const isInPages = currentPath.includes('/pages/');
    const basePath = isInPages ? '../' : '';
    
    let menuHTML = '';
    menuItems.forEach(item => {
        const isActive = currentFile === item.href.split('/').pop();
        // 根据当前页面位置调整路径
        let href = item.href;
        if (href.startsWith('pages/')) {
            // 如果在pages目录下，去掉pages/前缀；否则保持原样
            href = isInPages ? href.replace('pages/', '') : href;
        } else if (href === 'login.html') {
            // login.html始终在根目录，如果在pages目录下需要添加../
            href = isInPages ? '../login.html' : 'login.html';
        } else if (isInPages && !href.startsWith('http') && !href.startsWith('/')) {
            // 如果当前在pages目录下，且href不是pages/开头，需要添加../
            href = '../' + href;
        }
        menuHTML += `
            <li>
                <a href="${href}" class="${isActive ? 'active' : ''}">${item.text}</a>
            </li>
        `;
    });
    
    // 如果已登录，添加退出登录按钮
    if (role) {
        menuHTML += `
            <li>
                <a href="#" onclick="handleLogout(); return false;" style="color: #ef4444;">退出登录</a>
            </li>
        `;
    }
    
    return `
        <nav class="navbar">
            <div class="navbar-container">
                <a href="${basePath}index.html" class="logo">
                    <span>🏢</span>
                    <span>招聘网站</span>
                </a>
                <ul class="nav-menu">
                    ${menuHTML}
                </ul>
            </div>
        </nav>
    `;
}

/**
 * 初始化导航栏
 */
function initNavigation() {
    const navbarPlaceholder = document.getElementById('navbar-placeholder');
    if (navbarPlaceholder) {
        navbarPlaceholder.innerHTML = generateNavbar();
    }
}

/**
 * 处理退出登录
 */
function handleLogout() {
    if (confirm('确定要退出登录吗？')) {
        clearUser();
        const currentPath = window.location.pathname;
        const isInPages = currentPath.includes('/pages/');
        const basePath = isInPages ? '../' : '';
        window.location.href = basePath + 'index.html';
    }
}

// 页面加载时自动初始化导航栏
document.addEventListener('DOMContentLoaded', initNavigation);

