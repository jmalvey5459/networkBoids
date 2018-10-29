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

#pragma once

#include <vector>

#include "Boid.h"

#include "RakNet/DS_List.h"
#include "RakNet/BitStream.h"

// Bitstream

class Flock {
private:
	int count;

public:
	//std::vector<Boid> boids;
	DataStructures::List<Boid> boidsList;
	Flock();

	Flock(int numOfBoids);

	// Steering behaviors

	float getDistance(Boid boidA, Boid boidB);

	void update();
	void render();

	void writeToBitstream(RakNet::BitStream& bsOut, unsigned char typeID);
	void readFromBitstream(RakNet::Packet* packet);

	/*Flock operator=(const Flock& b)
	{
		count = b.count;
		boidsList = b.boidsList;
		return *this;
	}*/
};