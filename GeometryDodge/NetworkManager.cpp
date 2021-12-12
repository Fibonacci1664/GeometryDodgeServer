#include "NetworkManager.h"

NetworkManager::NetworkManager()
{
	
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::createTCPListner()
{
	// Bind the listener to a port
	if (tcpListenerSocket.listen(SERVERPORT) != sf::Socket::Done)
	{
		std::cout << "Bind failed!\n";
	}
	else
	{
		std::cout << "Bind worked\n";
		std::cout << "Listening for connections on TCP, port number " << SERVERPORT << "\n";
	}
	
	// Add the listener to the selector
	selector.add(tcpListenerSocket);
}

void NetworkManager::acceptConnections()
{
	if (selector.wait(sf::seconds(0.001f)))
	{
		// Test the listener
		if (selector.isReady(tcpListenerSocket))
		{
			// The listener is ready: there is a pending connection
			sf::TcpSocket* client = new sf::TcpSocket;

			if (tcpListenerSocket.accept(*client) == sf::Socket::Done)
			{
				// Add the new client to the clients list
				clientSockets.push_back(client);

				// Add the new client to the selector so that we will
				// be notified when they send something
				selector.add(*client);

				std::cout << "A new observer has joined!\n";
			}
			else
			{
				// Error, we won't get a new connection, delete the socket
				delete client;
			}
		}
	}
}

void NetworkManager::send(Player_UI_Data_Packet playerUIpckt, Asteroids_Data_Packet asteroidsPckt, Projectiles_Data_Packet projectilesPckt)
{
	for (auto it = clientSockets.begin(); it != clientSockets.end(); ++it)
	{
		sf::TcpSocket& client = **it;

		//std::cout << "\n############### PLAYER AND UI DATA ###############\n\n";

		// Send UI and Player data
		if (client.send(&playerUIpckt, sizeof(Player_UI_Data_Packet)) != sf::Socket::Done)
		{
			std::cout << "Error sending UI and Player data by TCP!\n";
			break;
		}
		else
		{
			/*std::cout << "Score: " << playerUIpckt.uiData.score << '\n';

			std::cout << "Player " << playerUIpckt.playerData.playerID << '\n'
				<< "Msg time sent: " << playerUIpckt.playerData.timeSent << '\n'
				<< "Player X: " << playerUIpckt.playerData.x << '\n'
				<< "Player Y: " << playerUIpckt.playerData.y << '\n';*/
		}

		//std::cout << "\n############### PLAYER AND UI DATA END ###############\n";

		//std::cout << "\n############### ASTEROID VECTOR SIZE DATA ###############\n\n";

		// Send the size of the astroid data msg vector
		if (client.send(&asteroidsPckt.asteroidDataMsgSize, sizeof(int)) != sf::Socket::Done)
		{
			std::cout << "Error sending asteroid data msg size by TCP!\n";
			break;
		}
		else
		{
			//std::cout << "Asteroid data msg vector size: " << asteroidsPckt.asteroidDataMsgSize << '\n';// " data sent from server to client port: " << client.getRemotePort() << "\n";
		}

		//std::cout << "\n############### ASTEROID VECTOR SIZE DATA END ###############\n";

		//std::cout << "\n############### ALL ASTEROID DATA ###############\n";

		if (asteroidsPckt.asteroidDataMsgs.size() == 0)
		{
			//std::cout << "\n############### NO ASTEROID DATA TO SEND ###############\n";
		}

		// Send ALL Asteroid data
		for (int i = 0; i < asteroidsPckt.asteroidDataMsgs.size(); ++i)
		{
			std::size_t bytesSent = 0.0f;

			AsteroidDataMsg* asteroidMsg = asteroidsPckt.asteroidDataMsgs[i];

			//std::cout << "\n############### ASTEROID " << asteroidsPckt.asteroidDataMsgs[i]->asteroidID << " MSG DATA ###############\n\n";

			if (client.send(asteroidMsg, sizeof(AsteroidDataMsg), bytesSent) != sf::Socket::Done)
			{
				std::cout << "Error sending Asteroid Data from server by TCP!\n";
				break;
			}
			else
			{
				/*std::cout << "Asteroid ID: " << asteroidsPckt.asteroidDataMsgs[i]->asteroidID << '\n'
						  << "Msg time sent: " << asteroidsPckt.asteroidDataMsgs[i]->timeSent << '\n'
						  << "Asteroid X: " << asteroidsPckt.asteroidDataMsgs[i]->x << '\n'
						  << "Asteroid Y: " << asteroidsPckt.asteroidDataMsgs[i]->y << '\n'
						  << "Sent to remote port: " << client.getRemotePort() << "\n"
						  << "Bytes sent " << bytesSent << "\n";*/
			}

			//std::cout << "\n############### ASTEROID " << asteroidsPckt.asteroidDataMsgs[i]->asteroidID << " MSG DATA END ###############\n";
		}



		///////////////////// PROJECTILE STUFF /////////////////////

		//std::cout << "\n############### PROJECTILE VECTOR SIZE DATA ###############\n\n";

		// Send the size of the astroid data msg vector
		if (client.send(&projectilesPckt.projectileDataMsgSize, sizeof(int)) != sf::Socket::Done)
		{
			std::cout << "Error sending projectile data msg size by TCP!\n";
			break;
		}
		else
		{
			//std::cout << "Projectile data msg vector size: " << projectilesPckt.projectileDataMsgSize << '\n';// " data sent from server to client port: " << client.getRemotePort() << "\n";
		}

		//std::cout << "\n############### PROJECTILE VECTOR SIZE DATA END ###############\n";

		//std::cout << "\n############### ALL PROJECTILE DATA ###############\n";

		if (projectilesPckt.projectileDataMsgs.size() == 0)
		{
			//std::cout << "\n############### NO PROJECTILE DATA TO SEND ###############\n";
		}

		// Send ALL Projectile data
		for (int i = 0; i < projectilesPckt.projectileDataMsgs.size(); ++i)
		{
			std::size_t bytesSent = 0.0f;

			ProjectileDataMsg* projectileMsg = projectilesPckt.projectileDataMsgs[i];

			//std::cout << "\n############### PROJECTILE " << projectilesPckt.projectileDataMsgs[i]->projectileID << " MSG DATA ###############\n\n";

			if (client.send(projectileMsg, sizeof(ProjectileDataMsg), bytesSent) != sf::Socket::Done)
			{
				std::cout << "Error sending Projectile Data from server by TCP!\n";
				break;
			}
			else
			{
				/*std::cout << "Projectile ID: " << projectilesPckt.projectileDataMsgs[i]->projectileID << '\n'
					<< "Msg time sent: " << projectilesPckt.projectileDataMsgs[i]->timeSent << '\n'
					<< "Projectile X: " << projectilesPckt.projectileDataMsgs[i]->x << '\n'
					<< "Projectile Y: " << projectilesPckt.projectileDataMsgs[i]->y << '\n'
					<< "Sent to remote port: " << client.getRemotePort() << "\n"
					<< "Bytes sent " << bytesSent << "\n";*/
			}

			//std::cout << "\n############### PROJECTILE " << projectilesPckt.projectileDataMsgs[i]->projectileID << " MSG DATA END ###############\n";
		}
	}
}