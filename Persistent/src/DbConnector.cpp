#include <DbConnector.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/statement.h>
#include <Logger.h>

DbConnector DbConnector::Instance;

bool DbConnector::Connect(const string& host, const string& userName, const string& password, const string& database) {
    try {
        driver = get_driver_instance();
        connection.reset(driver->connect(host, userName, password));
        connection->setSchema(database);
        Execute(database);
        return true;
    }
    catch (const sql::SQLException& e) {
        LogError(e);
        return false;
    }
}
bool DbConnector::Execute(const string& sql) {
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        return stmt->execute(sql);
    }
    catch (const sql::SQLException& e) {
        LogError(e);
        return false;
    }
}
DbConnector::Entries DbConnector::ExecuteQuery(const string& sql) {
    Entries rows;
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(sql));
        auto meta = res->getMetaData();
        int cols = meta->getColumnCount();

        while (res->next()) {
            std::map<std::string, std::string> row;
            for (int i = 1; i <= cols; i++)
                row[meta->getColumnName(i)] = res->getString(i);
            rows.push_back(std::move(row));
        }
        return rows;
    }
    catch (const sql::SQLException& e) {
        LogError(e);
        return {};
    }
}
int DbConnector::ExecuteUpdate(const string& sql) {
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        return stmt->executeUpdate(sql);
    }
    catch (const sql::SQLException& e) {
        LogError(e);
        return -1;
    }
}
void DbConnector::Disconnect() {
    connection->close();
}