#include <iostream>
#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;

#define PORT 12345
#define MAX_MSG 1024

void handleClient(tcp::socket &sock) {
    char buf[MAX_MSG];
    boost::system::error_code err;

    int bytesRead = sock.read_some(boost::asio::buffer(buf, MAX_MSG), err);

    if (err) {
        cout << "Read error: " << err.message() << endl;
        return;
    }

    buf[bytesRead] = '\0';
    cout << "Client says: " << buf << endl;

    string response = "Hello from server! Got your message. ";
    boost::asio::write(sock, boost::asio::buffer(response), err);

    if (err) {
        cout << "write error: " << err.message() << endl;
    }
}

int main() {
    boost::asio::io_context io;

    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), PORT));
    cout << "server listening on port " << PORT << endl;

    while (true) {
        tcp::socket sock(io);
        acceptor.accept(sock);

        cout << "client connected" << endl;
        handleClient(sock);
        cout << "Complete, waiting for the next client" << endl;
    }

    return 0;
}
