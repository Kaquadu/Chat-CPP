#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <conio.h>
#include <iostream>
#include <thread>
#include <string>

using namespace std;
using namespace sf;

enum clientStatus {go, eend};
clientStatus Cstatus = go;

void MessageSender();

int main()
{
	Thread sending(&MessageSender);
	sending.launch();


	sf::TcpListener listener;
	if (listener.listen(53001) != sf::Socket::Done)
	{
		cout << "Cannot bind port to the server." << endl;
	}
	while (Cstatus != eend)
	{
		TcpSocket recieve;
		Packet packet;
		string data;
		if (listener.accept(recieve) != sf::Socket::Done)
		{
			//cout << "Cannot accept client to the server." << endl;
		}
		else
		{
			if (recieve.receive(packet) != sf::Socket::Done)
			{
				cout << "Cannot recieve data from a client." << endl;
			}
			packet >> data;
			std::cout << "Sent: " << data << std::endl;
		}
	}
	return 0;
}

void MessageSender()
{
	TcpSocket socket;

	while (Cstatus != eend)
	{

		string data;
		getline(cin, data);
		//cout << data << endl;
		Packet packet;
		packet << data;

		while (socket.connect("25.1.53.224", 53000) != Socket::Done)
		{
			//cout << "Cannot connect to the server." << endl;
		}

		while (socket.send(packet) != sf::Socket::Done && data != "end")
		{
			cout << "Cannot send data to the server." << endl;
		}
		if (data == "end")
		{
			Cstatus = eend;
		}
		socket.disconnect();
	}
}