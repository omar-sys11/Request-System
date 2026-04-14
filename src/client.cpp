#include <iostream>
#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;

#define PORT 12345
#define MAX_MSG 1024

int main() {

        boost::asio::io_context io;

	tcp::resolver resolver(io);
	tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");

	tcp::socket sock(io);
	boost::asio::connect(sock, endpoints);
	cout << "connected to server!" << endl;

	string msg = "Hello from client";
	boost::system::error_code err;
	boost::asio::write(sock, boost::asio::buffer(msg), err);

	if (err) {
		cout << "send error: " << err.message() << endl;
		return 1;
	}

	cout << "message sent, waiting for reply" <<endl;

	char buf[MAX_MSG];
	int bytesRead = sock.read_some(boost::asio::buffer(buf, MAX_MSG), err);

	if (err && err != boost::asio::error::eof) {
		cout<< "read error: " << err.message() << endl;
		return 1;
	}

	buf[bytesRead] = '\0';
	cout << "server says: " << buf << endl;

	return 0;
}
