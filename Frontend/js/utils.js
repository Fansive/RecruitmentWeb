/**
 * 工具函数
 */

// 存储键名常量
const STORAGE_KEYS = {
    USER: 'currentUser',
    ROLE: 'userRole',
    TOKEN: 'authToken'
};

/**
 * 用户角色枚举
 */
const USER_ROLES = {
    JOBHUNTER: 'jobHunter',
    COMPANY: 'company',
    ADMIN: 'admin'
};

/**
 * 获取当前用户信息
 */
function getCurrentUser() {
    const userStr = localStorage.getItem(STORAGE_KEYS.USER);
    return userStr ? JSON.parse(userStr) : null;
}

/**
 * 获取当前用户角色
 */
function getCurrentRole() {
    return localStorage.getItem(STORAGE_KEYS.ROLE) || null;
}

/**
 * 设置用户信息
 */
function setCurrentUser(user, role) {
    localStorage.setItem(STORAGE_KEYS.USER, JSON.stringify(user));
    localStorage.setItem(STORAGE_KEYS.ROLE, role);
}

/**
 * 清除用户信息
 */
function clearUser() {
    localStorage.removeItem(STORAGE_KEYS.USER);
    localStorage.removeItem(STORAGE_KEYS.ROLE);
    localStorage.removeItem(STORAGE_KEYS.TOKEN);
}

/**
 * 检查用户是否已登录
 */
function isLoggedIn() {
    return getCurrentUser() !== null;
}

/**
 * 格式化日期
 */
function formatDate(dateString) {
    if (!dateString) return '';
    const date = new Date(dateString);
    return date.toLocaleDateString('zh-CN', {
        year: 'numeric',
        month: '2-digit',
        day: '2-digit',
        hour: '2-digit',
        minute: '2-digit'
    });
}

/**
 * 显示消息提示
 */
function showAlert(message, type = 'info') {
    const alertDiv = document.createElement('div');
    alertDiv.className = `alert alert-${type}`;
    alertDiv.textContent = message;
    
    const container = document.querySelector('.container');
    if (container) {
        container.insertBefore(alertDiv, container.firstChild);
        
        // 3秒后自动移除
        setTimeout(() => {
            alertDiv.remove();
        }, 3000);
    }
}

/**
 * 防抖函数
 */
function debounce(func, wait) {
    let timeout;
    return function executedFunction(...args) {
        const later = () => {
            clearTimeout(timeout);
            func(...args);
        };
        clearTimeout(timeout);
        timeout = setTimeout(later, wait);
    };
}

/**
 * 生成唯一ID
 */
function generateId() {
    return Date.now().toString(36) + Math.random().toString(36).substr(2);
}

/**
 * 验证邮箱格式
 */
function validateEmail(email) {
    const re = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    return re.test(email);
}

/**
 * 验证手机号格式
 */
function validatePhone(phone) {
    const re = /^1[3-9]\d{9}$/;
    return re.test(phone);
}


