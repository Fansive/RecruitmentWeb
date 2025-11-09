/**
 * 统一 API 客户端
 */

const API_BASE = '/api';

async function apiRequest(path, { method = 'GET', headers = {}, body = null } = {}) {
    const res = await fetch(`${API_BASE}${path}`, {
        method,
        headers: {
            'Content-Type': 'application/json',
            ...headers
        },
        body: body ? JSON.stringify(body) : null,
        credentials: 'include', // 使用 cookie 携带 sessionId
    });

    let data;
    const text = await res.text();
    try { data = text ? JSON.parse(text) : null; } catch (_) { data = text; }

    if (!res.ok) {
        const message = data && data.message ? data.message : `HTTP ${res.status}`;
        throw new Error(message);
    }
    return data;
}

// Auth (基于 sessionId cookie)
async function apiLogin({ userId, password, role }) {
    // 后端通过 Set-Cookie 设置 sessionId，返回简单的成功响应
    await apiRequest('/login', { method: 'POST', body: { userId, password, role } });
    // 登录成功后，获取当前用户信息
    return await apiGetCurrentUser();
}

// 获取当前登录用户信息（通过 sessionId）
async function apiGetCurrentUser() {
    return await apiRequest('/me', { method: 'GET' });
}

async function apiRegister({ userId, password, role }) {
    // 与后端接口 Signup(userId,password,role) 对齐
    return await apiRequest('/signup', { method: 'POST', body: { userId, password, role } });
}

// Jobhunters
async function apiGetJobhunter(id) { return await apiRequest(`/jobhunters/${id}`, { method: 'GET' }); }
async function apiUpdateJobhunter(id, payload) { return await apiRequest(`/jobhunters/${id}`, { method: 'PUT', body: payload }); }
async function apiGetJobhunterApplications(id) { return await apiRequest(`/jobhunters/${id}/applications`, { method: 'GET' }); }

// Companies
async function apiGetCompany(id) { return await apiRequest(`/companies/${id}`, { method: 'GET' }); }
async function apiUpdateCompany(id, payload) { return await apiRequest(`/companies/${id}`, { method: 'PUT', body: payload }); }
async function apiGetCompanyApplications(id) { return await apiRequest(`/companies/${id}/applications`, { method: 'GET' }); }

// Jobs
async function apiListJobs(params = {}) {
    const query = new URLSearchParams(params).toString();
    const q = query ? `?${query}` : '';
    return await apiRequest(`/jobs${q}`, { method: 'GET' });
}
async function apiCreateJob(payload) { return await apiRequest('/jobs', { method: 'POST', body: payload }); }
async function apiApplyJob(jobId) {
    // jobId 是数据库主键，hunterId 从当前登录用户获取
    return await apiRequest(`/jobs/${jobId}/applications`, { method: 'POST' });
}

// Applications
async function apiUpdateApplicationStatus(appId, status) {
    return await apiRequest(`/applications/${appId}`, { method: 'PATCH', body: { status } });
}

async function apiListPendingCompanyEdits() { return await apiRequest('/pending/company-edits', { method: 'GET' }); }
async function apiListPendingJobPosts() { return await apiRequest('/pending/job-posts', { method: 'GET' }); }
async function apiReviewPending(id, payload) { return await apiRequest(`/pending/${id}/review`, { method: 'POST', body: payload }); }

// 暴露到全局
window.Api = {
    apiRequest,
    apiLogin,
    apiRegister,
    apiGetCurrentUser,
    apiGetJobhunter,
    apiUpdateJobhunter,
    apiGetJobhunterApplications,
    apiGetCompany,
    apiUpdateCompany,
    apiGetCompanyApplications,
    apiListJobs,
    apiCreateJob,
    apiApplyJob,
    apiUpdateApplicationStatus,
    apiListPendingCompanyEdits,
    apiListPendingJobPosts,
    apiReviewPending,
};


