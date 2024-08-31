#include "database.hpp"

database::database(std::string ip, std::string username, std::string password,std::string db)
{
    try
    {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(ip, username, password);
        con->setSchema(db);
        stmt = con->createStatement();
        
    }
    catch (sql::SQLException &e) {
        std::cout << "SQLException: " << e.what() << std::endl;
        std::cout << "Error code: " << e.getErrorCode() << std::endl;
        std::cout << "SQLState: " << e.getSQLState() << std::endl;
    }
}

std::string database::search(std::string table,std::string email)
{    
    std::string query = "SELECT email, password FROM users WHERE email = '" + email + "'";
    res = stmt->executeQuery(query);
    while (res->next()) {
        return res->getString("password") ;
    }
    return "";
}

database::~database()
{
    delete res;
    delete stmt;
    delete con;
}
