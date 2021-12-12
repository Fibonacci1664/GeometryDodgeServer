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

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void createTCPListner();
	void acceptConnections();
	void send(Player_UI_Data_Packet playerUIpckt, Asteroids_Data_Packet asteroidsPckt, Projectiles_Data_Packet projcetilesPckt);

private:
	//sf::IpAddress SERVERIP = "127.0.0.1";
	unsigned short SERVERPORT = 5555;
	//std::queue<PlayerDataMsg> playerData;
	std::size_t received;
	sf::TcpListener tcpListenerSocket;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*> clientSockets;
};