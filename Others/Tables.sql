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

-- 测试数据
-- 追加测试数据集

-- 公司数据
INSERT INTO Companies (userId, password, email, address, companyType, scale, contactInfo, description)
VALUES
('C001','pwd123','hr@fantasy.com','ff','IT','50-100','电话:123456','幻想 IT 公司'),
('C002','pwd234','contact@dream.com','梦境市','制造业','100-500','邮箱:contact@dream.com','梦境制造公司'),
('C003','pwd345','hr@techcorp.com','科技园','IT','200-500','电话:01088888888','TechCorp 软件开发公司'),
('C004','pwd456','hr@finbank.com','金融街','金融','500-1000','电话:01099999999','大型银行金融服务公司'),
('C005','pwd567','contact@medicare.com','健康区','医疗','100-300','邮箱:contact@medicare.com','医疗健康设备提供商');

-- 求职者数据
INSERT INTO JobHunters (userId, password, email, address, gender, phoneNumber, degree, graduationYear, school, major, resumeUrl)
VALUES
('J001','hunter123','hunter1@example.com','幻想乡','Male','13800138001','graduate',2023,'幻想大学','计算机科学','/resumes/j001.pdf'),
('J002','hunter234','hunter2@example.com','梦境市','Female','13800138002','postGraduate',2022,'梦境学院','机械工程','/resumes/j002.pdf'),
('J003','hunter345','hunter3@example.com','科技园','Female','13800138003','postGraduate',2021,'科技大学','软件工程','/resumes/j003.pdf'),
('J004','hunter456','hunter4@example.com','金融街','Male','13800138004','graduate',2023,'财经学院','金融学','/resumes/j004.pdf'),
('J005','hunter567','hunter5@example.com','健康区','Female','13800138005','graduate',2022,'医学院','护理','/resumes/j005.pdf'),
('J006','hunter678','hunter6@example.com','科技园','Male','13800138006','doctor',2020,'理工大学','计算机科学','/resumes/j006.pdf'),
('J007','hunter789','hunter7@example.com','金融街','Female','13800138007','postGraduate',2021,'财经学院','会计','/resumes/j007.pdf');

-- 岗位数据
INSERT INTO Jobs (companyId, title, description, location, salary, degreeRequirement)
VALUES
(1,'前端开发工程师','负责网页前端开发','幻想乡','10K-15K','graduate'),
(1,'后端开发工程师','负责服务器端开发与维护','幻想乡','12K-18K','postGraduate'),
(2,'金融分析师','分析投资项目和市场行情','梦境市','15K-25K','postGraduate'),
(2,'风险控制专员','评估金融风险与合规管理','梦境市','12K-20K','graduate'),
(3,'软件测试工程师','负责软件测试与质量保证','科技园','8K-12K','graduate'),
(3,'产品经理','负责产品规划与需求分析','科技园','12K-18K','postGraduate'),
(4,'客户经理','维护客户关系并开拓业务','金融街','10K-15K','graduate'),
(4,'财务会计','负责公司财务管理','金融街','8K-12K','graduate'),
(5,'护士','负责病人护理工作','健康区','6K-10K','graduate'),
(5,'医疗器械工程师','负责医疗设备研发与维护','健康区','10K-15K','postGraduate');

-- 求职者投递记录
INSERT INTO JobApplications (jobId, hunterId, status)
VALUES
(1,1,'pending'),
(2,1,'pending'),
(3,2,'pending'),
(4,2,'pending'),
(5,3,'pending'),
(6,3,'pending'),
(7,4,'pending'),
(8,4,'pending'),
(9,5,'pending'),
(10,5,'pending'),
(1,6,'pending'),
(2,6,'pending'),
(3,7,'pending');
