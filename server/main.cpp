#include <asio.hpp>
#include <iostream>
#include <unordered_map>
#include <thread>

using asio::ip::tcp;

std::unordered_map<std::string, std::string> storage;

std::string make_hash(const std::string& s) {
    return std::to_string(std::hash<std::string>{}(s));
}

void handle(tcp::socket socket) {
    try {
        char data[1024];
        int len = socket.read_some(asio::buffer(data));

        std::string req(data, len);
        std::cout << "REQ:\n" << req << "\n";

        if (req.find("POST /files") != std::string::npos) {
            std::string body = req.substr(req.find("\r\n\r\n") + 4);
            std::string hash = make_hash(body);
            storage[hash] = body;
            asio::write(socket, asio::buffer("HTTP/1.1 200 OK\r\n\r\nHASH:" + hash));
        }
        else if (req.find("GET /files ") != std::string::npos) {
            std::string list;
            for (auto& [h,_] : storage)
                list += h + "\n";
            asio::write(socket, asio::buffer("HTTP/1.1 200 OK\r\n\r\n" + list));
        }
        else if (req.find("GET /files/") != std::string::npos) {
            int s = req.find("GET /files/") + 11;
            int e = req.find(" ", s);
            std::string hash = req.substr(s, e - s);
            if (storage.count(hash))
                asio::write(socket, asio::buffer("HTTP/1.1 200 OK\r\n\r\n" + storage[hash]));
            else
                asio::write(socket, asio::buffer("HTTP/1.1 404 Not Found\r\n\r\n"));
        }
    } catch(...) {}
}

int main() {
    asio::io_context io;
    tcp::acceptor acc(io, tcp::endpoint(tcp::v4(), 8080));
    std::cout << "Server running on 8080\n";
    while (true) {
        tcp::socket socket(io);
        acc.accept(socket);
        std::thread(handle, std::move(socket)).detach();
    }
}
