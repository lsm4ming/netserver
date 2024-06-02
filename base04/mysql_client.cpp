#include <iostream>
#include <mysql.h>
#include <chrono>
#include <cstring>

constexpr const char *MYSQL_USER = "root";
constexpr const char *MYSQL_HOST = "192.168.239.50";
constexpr const char *MYSQL_PASS = "123456";

int insertData(MYSQL *conn)
{
    /**
    CREATE TABLE users (
        user_id INT AUTO_INCREMENT PRIMARY KEY,
        username VARCHAR(50) NOT NULL UNIQUE,
        email VARCHAR(100) NOT NULL UNIQUE,
        password VARCHAR(100) NOT NULL,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );
     */
    auto stmt = mysql_stmt_init(conn);
    if (!stmt)
    {
        std::cerr << "Failed to initialize statement: " << mysql_error(conn) << std::endl;
        return 1;
    }
    const char *query = "INSERT INTO users (username, email, password,created_at) VALUES (?, ?, ?, ?)";
    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        std::cerr << "Failed to prepare statement: " << mysql_stmt_error(stmt) << std::endl;
        return 1;
    }
    // 绑定参数
    MYSQL_BIND bind[4];
    memset(bind, 0, sizeof(bind));

    std::string username = "lsm1998";
    std::string email = "487005831@qq.com";
    std::string password = "123456";
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_time = *std::localtime(&now_time_t);

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = const_cast<char *>(username.c_str());
    bind[0].buffer_length = username.length();
    bind[0].is_null = nullptr;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = const_cast<char *>(email.c_str());
    bind[1].buffer_length = email.length();
    bind[1].is_null = nullptr;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = const_cast<char *>(password.c_str());
    bind[2].buffer_length = password.length();
    bind[2].is_null = nullptr;

    MYSQL_TIME param{};

    param.year = tm_time.tm_year + 1900;
    param.month = tm_time.tm_mon + 1;
    param.day = tm_time.tm_mday;
    param.hour = tm_time.tm_hour;
    param.minute = tm_time.tm_min;
    param.second = tm_time.tm_sec;

    bind[3].buffer_type = MYSQL_TYPE_TIMESTAMP;
    bind[3].buffer = &param;
    bind[3].is_null = nullptr;

    if (mysql_stmt_bind_param(stmt, bind))
    {
        std::cerr << "Failed to bind parameters: " << mysql_stmt_error(stmt) << std::endl;
        return 1;
    }

    // 执行查询
    if (mysql_stmt_execute(stmt))
    {
        std::cerr << "Failed to execute statement: " << mysql_stmt_error(stmt) << std::endl;
        return 1;
    }

    return 0;
}

int main()
{
    // docker run --name mysql -p 3306:3306 -e MYSQL_ROOT_PASSWORD=123456 -d mysql
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    // 初始化连接
    conn = mysql_init(nullptr);

    // 连接数据库
    if (!mysql_real_connect(conn, MYSQL_HOST, MYSQL_USER, MYSQL_PASS, "demo", 0, nullptr, 0))
    {
        std::cerr << "Connection error: " << mysql_error(conn) << std::endl;
        return 1;
    }

    // insertData(conn);

    // 执行查询
    if (mysql_query(conn, "SELECT * FROM users"))
    {
        std::cerr << "Query error: " << mysql_error(conn) << std::endl;
        return 1;
    }

    // 获取查询结果
    res = mysql_use_result(conn);

    // 打印结果
    while ((row = mysql_fetch_row(res)) != nullptr)
    {
        std::cout << "Row: ";
        auto len = mysql_num_fields(res);
        for (int i = 0; i < len; i++)
        {
            std::cout << "[" << i << "]" << row[i] << " ";
        }
        std::cout << std::endl;
    }

    // 释放结果集
    mysql_free_result(res);

    // 关闭连接
    mysql_close(conn);
    return 0;
}