#include <bits/stdc++.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/driver.h>

class database
{
private:
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
public:
    explicit database(std::string ip, std::string username, std::string password,std::string db);
    database(database &) = delete;
    database(database &&) = delete; 
    void operator =(database &)=  delete;
    void operator =(database &&)=  delete;
    std::string search(std::string table,std::string email);
    ~database();
};

