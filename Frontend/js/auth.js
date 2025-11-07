/**
 * 用户认证和角色管理（依赖后端）
 */

/**
 * 检查页面访问权限
 */
function checkAuth(requiredRole = null) {
    const currentRole = getCurrentRole();
    
    // 如果页面需要登录但没有登录
    if (requiredRole && !isLoggedIn()) {
        window.location.href = 'login.html';
        return false;
    }
    
    // 如果页面需要特定角色但当前角色不匹配
    if (requiredRole && currentRole !== requiredRole) {
        window.location.href = 'index.html';
        return false;
    }
    
    return true;
}


