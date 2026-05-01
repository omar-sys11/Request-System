#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;

#define PORT 12345
#define MAX_MSG 4096

vector<tcp::socket*> clients;
mutex clientsMutex;

void broadcast(const string& msg, tcp::socket* sender) {
    lock_guard<mutex> lock(clientsMutex);
    for (int i = 0; i < clients.size(); i++) {
        if (clients[i] != sender) {
            boost::system::error_code err;
            boost::asio::write(*clients[i], boost::asio::buffer(msg), err);
            if (err) {
                cout << "broadcast error to a client: " << err.message() << endl;
            }
        }
    }
}

void removeClient(tcp::socket* sock) {
    lock_guard<mutex> lock(clientsMutex);
    for (int i = 0; i < clients.size(); i++) {
        if (clients[i] == sock) {
            clients.erase(clients.begin() + i);
            break;
        }
    }
}

void handleClient(tcp::socket* sock) {
    char buf[MAX_MSG];

    while (true) {
        boost::system::error_code err;
        int bytesRead = sock->read_some(boost::asio::buffer(buf, MAX_MSG), err);

        if (err) {
            cout << "client disconnected: " << err.message() << endl;
            break;
        }

        buf[bytesRead] = '\0';
        string msg(buf);
        cout << "received: " << msg << endl;

        broadcast(msg, sock);
    }

    removeClient(sock);
    delete sock;
}

int main() {
    boost::asio::io_context io;

    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), PORT));
    cout << "server listening on port " << PORT << endl;

    while (true) {
        tcp::socket* sock = new tcp::socket(io);
        acceptor.accept(*sock);

        cout << "new client connected!" << endl;

       {
            lock_guard<mutex> lock(clientsMutex);
            clients.push_back(sock);
        }

        thread t(handleClient, sock);
        t.detach();
    }

    return 0;
}
