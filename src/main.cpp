#include "tcp.hpp"
#include "database.hpp"
#include <pigpio.h>
#include <iostream>


int main()
{
    std::mutex mutx;
    tcp::http::server sr("192.168.1.4",8080,45);
    database mysql("tcp://127.0.0.1:3306", "user", "password","ControlSiteDB");

    sr.Get("/",[](const tcp::http::Request &req, tcp::http::Response &res){
        res.set_content(res.ReadFile("../WebPage/first.html"),"text/html");
    });

    sr.Get("/main.js",[](const tcp::http::Request &req, tcp::http::Response &res){
        res.set_content(res.ReadFile("../WebPage/main.js"),"application/javascript");
    });  

    sr.Get("/style.css",[](const tcp::http::Request &req, tcp::http::Response &res){
        res.set_content(res.ReadFile("../WebPage/style.css"),"text/css");
    });

    sr.Get("/padlock.png",[](const tcp::http::Request &req, tcp::http::Response &res){
        res.set_content(res.ReadFile("../WebPage/padlock.png"),"image/png");
    });

    sr.Get("/profile.png",[](const tcp::http::Request &req, tcp::http::Response &res){
        res.set_content(res.ReadFile("../WebPage/profile.png"),"image/png");
    });

    sr.Get("/favicon.ico",[](const tcp::http::Request &req, tcp::http::Response &res){
        res.set_content(res.ReadFile("../WebPage/profile.png"),"image/png");
    });

    sr.Get("/login",[&](const tcp::http::Request &req, tcp::http::Response &res){
        std::string password =  mysql.search("users",req.get_api_request().second[0]);
        std::cout << "password "<<password << '\n';
        if(password == req.get_api_request().second[1])
        {
            req.set_client_authorized();
            res.set_content(res.ReadFile("../WebPage/led.html"),"text/html");
        }
        else
        {
            res.set_content("wrong password or username or twice","text/plain");
        }
    });
    
    sr.Get("/control",[&](const tcp::http::Request &req, tcp::http::Response &res){
        if(req.is_client_authorized())
        {
            res.set_content(res.ReadFile("../WebPage/led.html"),"text/html");
        }
        else
        {
            res.set_content("<html><body>You need to login</body></html>","text/html");
        }
    });
    sr.Get("/led.css",[](const tcp::http::Request &req, tcp::http::Response &res){
        if(req.is_client_authorized())
        {
            res.set_content(res.ReadFile("../WebPage/led.css"),"text/css");
        }
        else
        {
            res.set_content("<html><body>You need to login</body></html>","text/html");
        }
    });

    sr.Get("/led.js",[](const tcp::http::Request &req, tcp::http::Response &res){
        if(req.is_client_authorized())
        {
             res.set_content(res.ReadFile("../WebPage/led.js"),"application/javascript");
        }
        else
        {
            res.set_content("<html><body>You need to login</body></html>","text/html");
        }
    }); 


    sr.Get("/led_on.jpeg",[](const tcp::http::Request &req, tcp::http::Response &res){
        if(req.is_client_authorized())
        {
            res.set_content(res.ReadFile("../WebPage/led_on.jpeg"),"image/jpeg");
        }
        else
        {
            res.set_content("<html><body>You need to login</body></html>","text/html");
        }
    });

    sr.Get("/led_off.jpeg",[](const tcp::http::Request &req, tcp::http::Response &res){
        if(req.is_client_authorized())
        {
            res.set_content(res.ReadFile("../WebPage/led_off.jpeg"),"image/jpeg");
        }
        else
        {
            res.set_content("<html><body>You need to login</body></html>","text/html");
        }
    });

    sr.Get("/ON",[&](const tcp::http::Request &req, tcp::http::Response &res){
        if(req.is_client_authorized())
        {
            res.set_content("done","text/plain");
            std::lock_guard<std::mutex> lock(mutx);
            gpioWrite(2, 1);
        }
        else
        {
            res.set_content("<html><body>You need to login</body></html>","text/html");
        }
    });
    
    sr.Get("/OFF",[&](const tcp::http::Request &req, tcp::http::Response &res){
        if(req.is_client_authorized())
        {
            res.set_content("done","text/plain");
            std::lock_guard<std::mutex> lock(mutx);
            gpioWrite(2, 0);
        }
        else
        {
            res.set_content("<html><body>You need to login</body></html>","text/html");
        }
    });
    gpioInitialise() ;
    gpioSetMode(2, PI_OUTPUT);
    sr.run();
    return 0;
}