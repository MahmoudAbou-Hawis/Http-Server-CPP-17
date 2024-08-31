#include "tcp.hpp"
#include <time.h>
#include <errno.h>

int tcp::http::server::run()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0)
    {
        return -1;
    }
    sereverIPAddress.sin_family = AF_INET;
    sereverIPAddress.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &sereverIPAddress.sin_addr) <= 0) {
        return -1;
    }
    bind(serverSocket, (sockaddr *)&sereverIPAddress , sizeof(sereverIPAddress));

    listen(serverSocket, 5);
    struct sockaddr_in client_address;
    socklen_t client_addrlen = sizeof(client_address);
    struct timeval timeout;
    timeout.tv_sec = timeOut; 
    timeout.tv_usec = 0; 
    char clientIp[INET_ADDRSTRLEN];
    int clientSocket ;
    std::string client_ip;
    while(true)
    {
        clientSocket = accept(serverSocket, (struct sockaddr *)&client_address, &client_addrlen);
        inet_ntop(AF_INET, &client_address.sin_addr, clientIp, sizeof(clientIp));
        
        if (setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) 
        {
            close(clientSocket);
            continue;
        }
        client_ip = std::string(clientIp);
        std::cout << "client IP: " << client_ip << '\n';
        mtx.lock();
        authorized[client_ip].first++;
        mtx.unlock();
        std::thread(handler,clientSocket,gets,client_ip).detach();
    }
    close(serverSocket);
    
}

void tcp::http::server::Get(std::string request ,std::function<void (const tcp::http::Request &,
                                                                           tcp::http::Response &)> callback)
{
    (*gets)[request] = callback;
}



void tcp::http::server::handler(int csocket
                               ,const std::weak_ptr<const HandlerParams>& p,std::string client_ip)
{

    char buffer[1024];
    std::cout << "Enter Thread" <<'\n';
    Request req(client_ip);
    Response res;
    ssize_t s ;
    std::string client_request;
    auto AllAPIs = p.lock();
    std::string response;
    while(true)
    {
        memset(buffer, 0, sizeof(buffer));
        s = recv(csocket, buffer, sizeof(buffer), 0);
        if (s < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                std::cout << "TimeOut";
                break;
            } else {
                 std::cout << "failed\n";
                break;
            }
        } else if (s == 0) {
            std::cout<<"connection closed\n";
            break;
        } else {
            client_request = req.set_buffer(std::string(buffer)).request;
            auto it = (*AllAPIs).find(client_request) ;
            if(it != (*AllAPIs).end())
            {
                it->second(req,res);
                response = res.get_response_OK_200();
            }
            else
            {
                
               response = res.get_response_error_404();
            }
            send(csocket, response.c_str(), response.size(), 0);
        }
    }

    std::lock_guard<std::mutex> lock(tcp::http::server::mtx);
    tcp::http::server::authorized[client_ip].first--;
    if(tcp::http::server::authorized[client_ip].first == 0)
    {
        tcp::http::server::authorized[client_ip].second = false;
    }
    std::cout << "Thread OUT" <<'\n';
    close(csocket);
}

std::pair<std::string,std::vector<std::string>> tcp::http::Request::get_api_request() const
{
    return std::make_pair(request,params);
}

void tcp::http::Request::set_client_authorized() const
{
    std::lock_guard<std::mutex> lock(tcp::http::server::mtx);
    tcp::http::server::authorized[clientIP].second = true;
    
}

bool tcp::http::Request::is_client_authorized() const
{
    std::lock_guard<std::mutex> lock(tcp::http::server::mtx);
    return tcp::http::server::authorized[clientIP].second ;
}

tcp::http::Request & tcp::http::Request::set_buffer(std::string buffer)
{
    params.clear();
    request = "";
    std::string buffer2;
    for(int i = 4 ; buffer[i] != ' ' ;i++)
    {
        buffer2 += buffer[i];
    }
    buffer = buffer2;
    int i;
    for( i = 0 ; i < buffer.size(); i++)
    {
        if(buffer[i] == '?') break;
        request += buffer[i];
    }
    std::string value;
    for(i ; i < buffer.size();i++)
    {
        if(buffer[i - 1] == '=' && buffer[i] != '&' && buffer[i] != ' ')
        {
            value += buffer[i];
            buffer[i] = '=';
        }
        else if(buffer[i] == ' ')
        {
            break;
        }
        else
        {
            if(buffer[i] == '&'){
                params.push_back(value);
            }
            value = "";
        }
    }
    params.push_back(value);
    return *this;
}


std::string tcp::http::Response::get_response_error_404()
{
    return "HTTP/1.1 404 Not Found\r\n"
                                    "Content-Type: text/html\r\n"
                                    "Content-Length: 103\r\n"
                                    "Connection: keep-alive\r\n"
                                    "\r\n"
                                    "<!DOCTYPE html>"
                                    "<html>"
                                    "<head><title>404 Not Found</title></head>"
                                    "<body><h1>404 Not Found</h1>"
                                    "<p>The requested resource was not found on this server.</p>"
                                    "</body>"
                                    "</html>";
    
}


std::string tcp::http::Response::get_response_OK_200()
{
    return response;
}

void tcp::http::Response::set_content(std::string content, std::string type)
{
    response = "HTTP/1.1 200 OK\r\n" 
               "Content-Type: "+type +"\r\n"
                "Content-Length: " + std::to_string(content.size()) + "\r\n"
                "Connection: keep-alive\r\n\r\n"
                 + content;
}

std::string tcp::http::Response::ReadFile(std::string path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return ""; 
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string buffer(size, '\0');
    if (!file.read(&buffer[0], size)) {
        return ""; 
    }
    return buffer;
}
