#include "thpc_net.h"
using namespace thpc;

/*
PacketHandler::PacketHandler()
{
	// NetConnection is going to send/receive packets and sort the data
	
}

NetClient::NetClient()
{
	thpc_Debug_PrintToDebugger("Connecting...");
	int timeOut = 5; // feel free to change this number
	int port = 7777; // feel free to change this number
	sf::IpAddress ipString = sf::IpAddress::getLocalAddress();
	sf::Socket::Status status = socket.connect(ipString, port, sf::seconds(timeOut));
	if (status != sf::Socket::Done)
	{
		// error
		thpc_Debug_PrintToDebugger("Error!");
	}
	else
	{
		// wonderful
		thpc_Debug_PrintToDebugger("Success!");
	}
}

NetHost::NetHost()
{
	// This class is going to be threaded as it'l be the one handling the connections between client and host
	// The code below *should* accept an incoming connection from client
	listener.listen(port);
	selector.add(listener);
	while (listen) // currently set to false in header file
	{
		thpc_Debug_PrintToDebugger("Listening");
		if (selector.wait())
		{
			if (selector.isReady(listener))
			{
				thpc_Debug_PrintToDebugger("Listener ready for connection");
				sf::TcpSocket *socket = new sf::TcpSocket;
				listener.accept(*socket);
				// std::cout << "\nConnection from: " << stocket->getRemoteAddress();
				sockets.push_back(socket);
				selector.add(*socket);
			}
		}
	}
}
*/