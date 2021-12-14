/*
 * This is the Network Manager class and handles
 *		- Creating a TCP listener.
 *		- Accepting connections.
 *		- Sending all data to all clients using socket selector.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2021.
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "PlayerDataMsg.h"
#include <queue>
#include <vector>
#include "AsteroidDataMsg.h"
#include "UIDataMsg.h"
#include "Asteroids_Data_Packet.h"
#include "Player_UI_Data_Packet.h"
#include "Projectiles_Data_Packet.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// GLOBALS
extern bool printDataToConsole;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void createTCPListner();
	void acceptConnections();
	void sendPlayer_UI_Packet(Player_UI_Data_Packet playerUIpckt);
	void sendAsteroidPacket(Asteroids_Data_Packet asteroidsPckt);
	void sendProjectilesPacket(Projectiles_Data_Packet projcetilesPckt);

	void receiveForEcho();
	void sendEcho();

	void sendGameState(int gameState);

private:
	//sf::IpAddress SERVERIP = "127.0.0.1";
	unsigned short SERVERPORT = 5555;
	std::size_t received;
	sf::TcpListener tcpListenerSocket;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*> clientSockets;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////