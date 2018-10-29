// Certificate of Authenticity
//
// EGP-405-01 Networking for Online Games
// Project 2 & Lab 4
// 10-30-2018
//
// Vedant Chaudhari, 1532077
// John Malvey
//
// We certify that this work is entirely our own.The assessor of this project may reproduce this project 
// and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism 
// - checking service, which may retain a copy of the project on its database.

// Based on https://github.com/forsythe/boids

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <vector>

#include "Define.h" 
#include "Flock.h"
#include "SDLInterface.h"
#include "InputInterface.h"

#include "../../DevSDKs/include/RakNet/RakPeerInterface.h"
#include "../../DevSDKs/include/RakNet/MessageIdentifiers.h"
#include "../../DevSDKs/include/RakNet/BitStream.h"
#include "../../DevSDKs/include/RakNet/RakNetTypes.h"
#include <stdio.h>

#include "ServerDefine.h"

void update() {
	SDLInterface::getInstance()->render();
	SDLInterface::getInstance()->background();
	InputInterface::getInstance()->update();
	SDLInterface::getInstance()->setColor(255, 255, 255, 255);
}

int main(int argc, char *argv[]) {
	SDLInterface::getInstance()->init(WIDTH, HEIGHT, TITLE);
	Flock flock = Flock(NUM_BOIDS);
	Flock flock2 = Flock(NUM_BOIDS);
	Flock flockCouple = Flock(NUM_BOIDS * 2);

	Uint32 iTime = SDL_GetTicks();
	Uint32 iTime2 = 0;

	//networking vars
	RakNet::Packet *packet;
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	const short SERVERPORT = 60000;
	char str[512];
	RakNet::SocketDescriptor sd;
	bool twoFlocks = false;
	bool coupleReadyToSend = false;
	RakNet::SystemAddress otherClientAddress = RakNet::UNASSIGNED_SYSTEM_ADDRESS;

	//std::cout << "Flock Size: " << sizeof(Flock) << "\n";

	//network modes
	int dataMode = COUPLED_MODE;

	peer->Startup(1, &sd, 1);

	//set up client connection to server
	printf("Enter server IP\n");
	fgets(str, 512, stdin);
	if (str[0] == 10) {
		strcpy(str, "127.0.0.1");
	}

	system("cls");
	peer->Connect(str, SERVERPORT, 0, 0);

	//big timeout timer
	peer->SetTimeoutTime(999999, RakNet::UNASSIGNED_SYSTEM_ADDRESS);


	
	while (SDLInterface::getInstance()->isExit == false) {
		
		//recieve network packets
		for (
			packet = peer->Receive();
			packet; 
			peer->DeallocatePacket(packet), packet = peer->Receive()
			)
		{
			switch (packet->data[0])
			{
			case SETMODE_PUSH:
			{
				dataMode = PUSH_MODE;
			}
			break;
			case SETMODE_SHARE:
			{
				dataMode = PUSH_MODE;
			}
			break;
			case SETMODE_COUPLED:
			{
				dataMode = COUPLED_MODE;
			}
			break;
			case SEND_CLIENTDATA:
			{
				std::cout << "Sent client flocking data to server" << std::endl;
				//send out client data to server
				// ****TODO Probably load the flock up with boids before it's connected to the server
				RakNet::BitStream sendData;
				flock.update();
				flock.writeToBitstream(sendData,SEND_CLIENTDATA);
				//sendData.clientFlock.boids = flock.boids;
				
				peer->Send(&sendData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

			}
			break;
			case RECIEVE_FLOCK_DATA:
			{
				//std::cout << "New Flock Data Recieved\n";
				flock.readFromBitstream(packet);
			}
			break;
			case RECIEVE_FLOCK2_DATA:
			{
				std::cout << "New Flock Data Recieved\n";
				twoFlocks = true;
				flock2.readFromBitstream(packet);
				if (dataMode == COUPLED_MODE)
				{
					for (int i = NUM_BOIDS; i < ((int)NUM_BOIDS *2); i++)
					{
						//fill the second half of the coupled flock with the boids list
						flockCouple.boidsList[i] = flock2.boidsList[i - NUM_BOIDS];
					}
					//now that the full coupled Boids list is available, we can send it!
					coupleReadyToSend = true;
				}
			}
			break; 
			case RECIEVE_COUPLEDFLOCK_DATA:
			{
				//std::cout << "New Flock Data Recieved\n";
				flockCouple.readFromBitstream(packet);
			}
			break;
			case GET_CLIENT_IP:
			{
				std::cout << "client IP recieved\n";
				ClientData *temp = ((ClientData*)packet->data);
				otherClientAddress = temp->clientIP;
			}
			break;
			default:
				//printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
			iTime2 = SDL_GetTicks();

			// send data to server
			switch (dataMode)
			{
			//Jack
			case PUSH_MODE:
			{
				if (iTime2 - iTime >= TICK) {
					iTime = SDL_GetTicks();
					
					ClientData temp;
					temp.ID = DATA_PUSH;
					//printf("sending push request\n");
					peer->Send((char*)&temp, sizeof(temp), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
				}
			}
			break;
			//Jack
			case SHARE_MODE:
			{
				if (iTime2 - iTime >= TICK) {
					iTime = SDL_GetTicks();

					RakNet::BitStream sendData;
					flock.update();
					flock.writeToBitstream(sendData, DATA_SHARE);
					peer->Send(&sendData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
				}

			}
			break;
			//Jack
			case COUPLED_MODE:
			{
				if (iTime2 - iTime >= TICK) {
					iTime = SDL_GetTicks();

					RakNet::BitStream sendData;
					//flock.update();
					//send state to other client after updating
					flock.writeToBitstream(sendData, RECIEVE_FLOCK2_DATA);
					peer->Send(&sendData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, otherClientAddress, false);
					for (int i = 0; i < NUM_BOIDS; i++)
					{
						//fill the first half of the coupled flock with the boids list
						flockCouple.boidsList[i] = flock.boidsList[i];
					}
				}
				if (coupleReadyToSend)
				{
					coupleReadyToSend = false;
					RakNet::BitStream sendData;
					flockCouple.writeToBitstream(sendData, DATA_COUPLED);
					peer->Send(&sendData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
				}
			}
			break;
			}
			if (dataMode == COUPLED_MODE)
			{
				flockCouple.render();
			}
			else {
			
				flock.render();
				if (twoFlocks)
					flock2.render();
			}
			update();
		}
	
	SDLInterface::getInstance()->exit();
	return 0;
}