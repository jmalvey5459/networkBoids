#pragma once

#include <vector>

#include "RakNet/DS_List.h"
#include "Boid.h"
#include "RakNet/BitStream.h"
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

	void writeToBitstream(RakNet::BitStream& bsOut, unsigned char typeID);
	void readFromBitstream(RakNet::Packet* packet);

	/*Flock operator=(const Flock b)
	{
		count = b.count;
		boids = b.boids;
		return this;
	}*/
};