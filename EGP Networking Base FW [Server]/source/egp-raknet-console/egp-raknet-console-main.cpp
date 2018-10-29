/*
	RakNet Console Application
	Prepared by Dan Buckstein
	September 5, 2018

	This file was modified by Jack Malvey with permission from author.”
	"I certify that this work is
	entirely my own. The assessor of this project may reproduce this project
	and provide copies to other academic staff, and/or communicate a copy of
	this project to a plagiarism-checking service, which may retain a copy of the
	project on its database"
	-John Malvey 1005854
*/
#include "../headers/CommonInclude.h"


// entry function
int main(int const argc, char const *const *const argv)
{
	//Initializations
	unsigned short serverPort = 60000;
	const unsigned int MAX_CLIENTS = 3;


	GameMessageData serverName;

	//Begin Networking
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();

	
	ClientData clients[2];
	Flock coupledFlock((int)NUM_BOIDS * 2);

	RakNet::SocketDescriptor sd(serverPort, 0);
	peer->Startup(MAX_CLIENTS, &sd, 1);

	//connect client and server
	printf("Starting the server @ %s.\n", peer->GetLocalIP(2));
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);

	peer->SetTimeoutTime(999999, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	std::string placeHolder = "";
	RakNet::Packet *packet;
	while (1)
	{

		for (
			packet = peer->Receive();
			packet;
			peer->DeallocatePacket(packet), packet = peer->Receive()
			)
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_NEW_INCOMING_CONNECTION:
			{
				printf("A client has connected.\n");
				GameMessageData gameMessage;
				gameMessage.ID = INCOMING_CLIENTDATA;
				peer->Send((char*)&gameMessage, sizeof(gameMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);


			}
			break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;

			case ID_DISCONNECTION_NOTIFICATION:
				printf("A client has disconnected.\n");
				break;

			case ID_CONNECTION_LOST:
				printf("A client lost the connection.\n");
				break;
			
			break;

			//Jack
			case INCOMING_CLIENTDATA:
			{
				//add client data to client list
				printf("incoming client data: \n");
				
				ClientData* gameMessage = (ClientData*)packet->data;
				if (clients[0].instantiated == false)
				{
					//clients[0] = gameMessage;
					clients[0].clientFlock.readFromBitstream(packet);
					clients[0].clientIP = packet->systemAddress;
					clients[0].instantiated = true;
					printf("Client 1 connected \n");
					ClientData temp;
					temp.ID = GET_ORDER;
					peer->Send((char*)&temp, sizeof(temp), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

				}
				else if (clients[1].instantiated == false)
				{
					//clients[1] = *gameMessage;
					clients[1].clientFlock.readFromBitstream(packet);
					clients[1].clientIP = packet->systemAddress;
					clients[1].instantiated = true;
					printf("Client 2 connected \n");
					ClientData temp;
					temp.clientIP = clients[0].clientIP;
					temp.ID = GET_CLIENT_IP;
					peer->Send((char*)&temp, sizeof(temp), HIGH_PRIORITY, RELIABLE_ORDERED, 0, clients[1].clientIP, false);

					temp.clientIP = clients[1].clientIP;
					peer->Send((char*)&temp, sizeof(temp), HIGH_PRIORITY, RELIABLE_ORDERED, 0, clients[0].clientIP, false);

				}

				// ****TODO: Default to data push, simulate both ClientData flocks on server and send to peers for rendering
				if (packet->systemAddress == clients[0].clientIP)
				{
					//update flocks from incoming data
				}
				else if (packet->systemAddress == clients[1].clientIP)
				{
					//update flocks from incoming data
				}

				//send a request to the clients to send their data
				//ClientData getData;
				//getData.ID = INCOMING_CLIENTDATA;
				//peer->Send((char*)&getData, sizeof(getData), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

			}
			break;
			//jack Malvey
			case DATA_PUSH:
			{
				if (packet->systemAddress == clients[0].clientIP)
				{
					clients[0].clientFlock.update();

					RakNet::BitStream client0Flock;
					clients[0].clientFlock.writeToBitstream(client0Flock, RECIEVE_FLOCK_DATA);
					peer->Send(&client0Flock, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clients[0].clientIP, false);

					if (clients[1].instantiated == true)
					{
						clients[1].clientFlock.update();

						RakNet::BitStream client1Flock;
						clients[1].clientFlock.writeToBitstream(client1Flock, RECIEVE_FLOCK2_DATA);
						peer->Send(&client1Flock, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clients[0].clientIP, false);
					}
				}
				else if (packet->systemAddress == clients[1].clientIP)
				{
					if (clients[0].instantiated == true)
					{
						clients[0].clientFlock.update();

						RakNet::BitStream client0Flock;
						clients[0].clientFlock.writeToBitstream(client0Flock, RECIEVE_FLOCK2_DATA);
						peer->Send(&client0Flock, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clients[1].clientIP, false);
					}
					clients[1].clientFlock.update();

					RakNet::BitStream client1Flock;
					clients[1].clientFlock.writeToBitstream(client1Flock, RECIEVE_FLOCK_DATA);
					peer->Send(&client1Flock, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clients[1].clientIP, false);
				}
			}
			break;
			//Jack
			case DATA_SHARE:
			{
				//take in flock and send it to the other client
				if (packet->systemAddress == clients[0].clientIP)
				{

					if (clients[1].instantiated == true)
					{
						RakNet::BitStream client1Flock;
						clients[0].clientFlock.readFromBitstream(packet);
						clients[0].clientFlock.writeToBitstream(client1Flock, RECIEVE_FLOCK2_DATA);
						peer->Send(&client1Flock, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clients[1].clientIP, false);
					}
				}
				else if (packet->systemAddress == clients[1].clientIP)
				{

					if (clients[0].instantiated == true)
					{
						RakNet::BitStream client0Flock;
						clients[1].clientFlock.readFromBitstream(packet);
						clients[1].clientFlock.writeToBitstream(client0Flock, RECIEVE_FLOCK2_DATA);
						peer->Send(&client0Flock, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clients[0].clientIP, false);
					}
				}
			}
			break;
			//Jack
			case DATA_COUPLED:
			{
				//std::cout << "data coupled called\n";

				//read coupled flock into bigger flock
				coupledFlock.readFromBitstream(packet);
				//coupledFlock.update();
				if (packet->systemAddress == clients[0].clientIP)
				{
					if (clients[1].instantiated == true)
					{

						RakNet::BitStream client1Flock;
						coupledFlock.writeToBitstream(client1Flock, RECIEVE_COUPLEDFLOCK_DATA);
						peer->Send(&client1Flock, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clients[1].clientIP, false);
					}
				}
				else if (packet->systemAddress == clients[1].clientIP)
				{
					if (clients[0].instantiated == true)
					{

						RakNet::BitStream client0Flock;
						coupledFlock.writeToBitstream(client0Flock, RECIEVE_COUPLEDFLOCK_DATA);
						peer->Send(&client0Flock, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clients[0].clientIP, false);
					}
					
				}
			}
			break; 
			case RECIEVE_FLOCK2_DATA:
			{
				RakNet::BitStream sendData;
				clients[0].clientFlock.readFromBitstream(packet);
				clients[0].clientFlock.writeToBitstream(sendData, RECIEVE_FLOCK2_DATA);
				if (packet->systemAddress == clients[0].clientIP)
				{
					peer->Send(&sendData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clients[1].clientIP, false);

				}
				else if (packet->systemAddress == clients[1].clientIP)
				{
					
					peer->Send(&sendData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clients[0].clientIP, false);
				}
			}
				break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
	}
		// shut down networking by destroying peer interface instance
		RakNet::RakPeerInterface::DestroyInstance(peer);

		// exit
		printf("\n\n");
		system("pause");
		return 0;
	}
	
