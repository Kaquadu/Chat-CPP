#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace sf;

enum serverStatus { run, stop, preprocessing };

serverStatus status = preprocessing;

void ConsoleLaunch();
void ConnectionProcessing();
void DataProcessing();
vector <IpAddress> IPList;

int main()
{
	IPList.push_back("0.0.0.0");
	Thread console(&ConsoleLaunch);
	console.launch();
	Thread connectionProcessing(&ConnectionProcessing);
	connectionProcessing.launch();
	Thread dataProcessing(&DataProcessing);
	dataProcessing.launch();
	return 0;
}

void ConsoleLaunch()
{
	string command;
	cin >> command;
	if (command == "stop")
		status = stop;
}

void ConnectionProcessing()
{
	sf::TcpListener listener;
	if (listener.listen(53000) != sf::Socket::Done)
	{
		cout << "Cannot bind port to the server." << endl;
	}

	while (status != stop)
	{
		sf::TcpSocket client;
		Packet packet;
		string data;
		if (listener.accept(client) != sf::Socket::Done)
		{
			cout << "Cannot accept client to the server." << endl;
		}
		else
		{
			cout << "Connection established. Client IP: " << client.getRemoteAddress() << endl;
			if (client.receive(packet) != sf::Socket::Done)
			{
				cout << "Cannot recieve data from a client." << endl;
			}
			packet >> data;
			std::cout << "Sent: " << data << std::endl;

			bool onTheList = false;
			for (int i = 0; i < IPList.size(); i++)
			{
				cout << "Putting on the list: " << client.getRemoteAddress() << "   i: " << i << endl;
				if (IPList[i] == client.getRemoteAddress())
				{
					onTheList = true;
				}
			}
			if (onTheList == false)
			{
				cout << "There is no such IP address here!" << endl;
				IPList.push_back(client.getRemoteAddress());
			}

			for (int i = 0; i < IPList.size(); i++)
			{
				cout << "Sending respond to: " << IPList[i] << endl;
				TcpSocket respond;
				if (respond.connect(IPList[i], 53001) != Socket::Done);
				{
					//
				}
				if (respond.send(packet) != Socket::Done)
				{
					//
				}
				respond.disconnect();
			}

		}
	}
}

void DataProcessing()
{

}