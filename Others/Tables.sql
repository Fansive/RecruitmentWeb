SET NAMES utf8mb4;
-- 创建(若没有)并进入数据库
DROP DATABASE IF EXISTS recruitment_web;
CREATE DATABASE recruitment_web CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
USE recruitment_web;

CREATE TABLE JobHunters (
   id INT AUTO_INCREMENT PRIMARY KEY,
   userId VARCHAR(50) NOT NULL UNIQUE,
   password VARCHAR(50) NOT NULL,
   email VARCHAR(50),
   address VARCHAR(200),
   createdAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    -- JobHunter 特有字段
    gender ENUM('Male', 'Female'),
    phoneNumber VARCHAR(20),
    degree ENUM('senior', 'junior', 'graduate', 'postGraduate', 'doctor'),
    graduationYear INT,
    school VARCHAR(100),
    major VARCHAR(100),
    resumeUrl VARCHAR(255)
);

CREATE TABLE Companies (
   id INT AUTO_INCREMENT PRIMARY KEY,
   userId VARCHAR(50) NOT NULL UNIQUE,
   password VARCHAR(50) NOT NULL,
   email VARCHAR(50),
   address VARCHAR(200),
   createdAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    -- Company 特有字段
    companyType VARCHAR(100),
    scale VARCHAR(50),
    contactInfo VARCHAR(255),
    description TEXT
);

CREATE TABLE Jobs (
    id INT AUTO_INCREMENT PRIMARY KEY,
    companyId INT NOT NULL,       -- 外键引用 Companies.id
    title VARCHAR(100) NOT NULL,
    description TEXT,
    location VARCHAR(255),
    salary VARCHAR(50),
    degreeRequirement ENUM('senior', 'junior', 'graduate', 'postGraduate', 'doctor'),
    createdAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (companyId) REFERENCES Companies(id) ON DELETE CASCADE
);

CREATE TABLE JobApplications (
    id INT AUTO_INCREMENT PRIMARY KEY,
    jobId INT NOT NULL, -- 外键引用 Jobs.id
    hunterId INT NOT NULL,       -- 外键引用 JobHunters.id
    appliedAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    status ENUM('pending','company accepted','company rejected', 'jobhunter accepted', 'jobhunter rejected') DEFAULT 'pending',
    FOREIGN KEY (jobId) REFERENCES Jobs(id) ON DELETE CASCADE,
    FOREIGN KEY (hunterId) REFERENCES JobHunters(id) ON DELETE CASCADE,
    UNIQUE(jobId, hunterId)      -- 防止重复投递
);

CREATE TABLE PendingReviews (
    id INT AUTO_INCREMENT PRIMARY KEY,
    type ENUM('company_edit', 'job_post') NOT NULL,  -- 审核类型
    companyId INT NOT NULL,                          -- 外键引用 Companies.id
    status ENUM('pending', 'approved', 'rejected') DEFAULT 'pending',  -- 审核状态
    reviewData JSON NOT NULL,                        -- 待审核的数据（JSON格式存储）
    createdAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    reviewedAt TIMESTAMP NULL,                       -- 审核时间
    reviewedBy INT NULL,                             -- 审核人ID（如果需要记录审核人）
    FOREIGN KEY (companyId) REFERENCES Companies(id) ON DELETE CASCADE,
    INDEX idx_type_status (type, status),            -- 索引优化查询
    INDEX idx_companyId (companyId)
);
