#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <arpa/inet.h>
#include <sys/types.h>

namespace tcp::http
{
    class Request
    {
        private:
            friend class server;
            Request& set_buffer(std::string buffer);
            std::string request;
            std::vector<std::string>params;
            std::string clientIP;
        public:
           explicit Request(std::string clientip): clientIP(clientip){};
           std::pair<std::string,std::vector<std::string>> get_api_request() const;
           void set_client_authorized() const;
           bool is_client_authorized() const;
    };

    class Response
    {
        std::string response;
        std::string get_response_error_404();
        std::string get_response_OK_200();
        friend class server;
        public:
           explicit Response(){};
           void set_content(std::string content, std::string type);
           std::string ReadFile(std::string path);
    };

    class  server final
    { 
        private:
            using HandlerParams = std::map<std::string,std::function<void(const tcp::http::Request&, 
                                                                    tcp::http::Response &)>>;
            sockaddr_in sereverIPAddress ;
            int serverSocket;
            std::string ip;
            int port;
            static void handler(int csocket 
                        ,const std::weak_ptr<const HandlerParams>& p, std::string client_ip);
            int timeOut;

            std::shared_ptr<HandlerParams> gets = std::make_shared<HandlerParams>();
            friend class Request;
            inline static std::map<std::string,std::pair<int , bool>> authorized;
            inline static std::mutex mtx;
        public:
            explicit server(std::string IP , int Port, int  TimeOut = 72) : ip(IP) , port(Port) , timeOut(TimeOut) {}
            server(server &&) = delete;
            void operator =(server&) = delete;
            server(server &) = delete;
            ~server() = default;
            void Get(std::string request ,std::function<void (const tcp::http::Request &,tcp::http::Response &)> callback);
            int run();

    };
}