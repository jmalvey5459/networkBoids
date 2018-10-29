// Certificate of Authenticity
//
// EGP-405-01 Networking for Online Games
// Project 2 & Lab 4
// 10-30-2018
//
// Vedant Chaudhari, 1532077
// John Malvey, 1005854
//
// We certify that this work is entirely our own.The assessor of this project may reproduce this project 
// and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism 
// - checking service, which may retain a copy of the project on its database.

#include "Flock.h"
#include "ServerDefine.h"
Flock::Flock() {
	this->count = NUM_BOIDS;

	for (int iter = 0; iter < count; iter++) {
		//Boid tempBoid = Boid();	// Incase you need to set properties later on
		//boids.push_back(Boid());
		boidsList.Push(Boid(), 0, 0);
	}
}
Flock::Flock(int numOfBoids) {
	this->count = numOfBoids;

	for (int iter = 0; iter < count; iter++) {
		//Boid tempBoid = Boid();	// Incase you need to set properties later on
		//boids.push_back(Boid());
		boidsList.Push(Boid(), 0, 0);
	}
}

float Flock::getDistance(Boid boidA, Boid boidB) {
	return sqrt((boidB.pos[0] - boidA.pos[0]) * (boidB.pos[0] - boidA.pos[0])
		+ (boidB.pos[1] - boidA.pos[1]) * (boidB.pos[1] - boidA.pos[1]));
}

void Flock::update() {
	for (int iter = 0; iter < count; iter++)
		boidsList[iter].clearBuffers();

	// Cohesion
	for (int iter = 0; iter < count; iter++) {
		for (int iter2 = iter + 1; iter2 < count; iter2++) {
			if (getDistance(boidsList[iter], boidsList[iter2]) < BOID_RADIUS) {
				boidsList[iter].numOfNeighbors++;
				boidsList[iter2].numOfNeighbors++;
				boidsList[iter].addToTargetPosBuffer(boidsList[iter2].pos[0], boidsList[iter2].pos[1]);
				boidsList[iter2].addToTargetPosBuffer(boidsList[iter].pos[0], boidsList[iter].pos[1]);
			}
		}
	}
	for (int iter = 0; iter < count; iter++) {
		if (boidsList[iter].numOfNeighbors > 0) {
			boidsList[iter].cohesionTargetPos[0] = boidsList[iter].targetPosBuffer[0] / boidsList[iter].numOfNeighbors;
			boidsList[iter].cohesionTargetPos[1] = boidsList[iter].targetPosBuffer[1] / boidsList[iter].numOfNeighbors;
		}
		else {
			boidsList[iter].cohesionTargetPos[0] = boidsList[iter].pos[0];
			boidsList[iter].cohesionTargetPos[1] = boidsList[iter].pos[1];
		}
	}

	// Separation
	for (int iter = 0; iter < count; iter++)
		boidsList[iter].clearBuffers();

	for (int iter = 0; iter < count; iter++) {
		for (int iter2 = iter + 1; iter2 < count; iter2++) {
			if (getDistance(boidsList[iter], boidsList[iter2]) < SEPERATION_RADIUS) {
				boidsList[iter].numOfNeighbors++;
				boidsList[iter2].numOfNeighbors++;
				boidsList[iter].addToTargetPosBuffer(boidsList[iter2].pos[0], boidsList[iter2].pos[1]);
				boidsList[iter2].addToTargetPosBuffer(boidsList[iter].pos[0], boidsList[iter].pos[1]);
			}
		}
	}
	for (int iter = 0; iter < count; iter++) {
		if (boidsList[iter].numOfNeighbors > 0) {
			if (boidsList[iter].targetPosBuffer[0] / boidsList[iter].numOfNeighbors
				!= boidsList[iter].pos[0]) {
				boidsList[iter].separationTargetPos[0] = boidsList[iter].pos[0] - 1000
					/ (boidsList[iter].targetPosBuffer[0] / boidsList[iter].numOfNeighbors - boidsList[iter].pos[0]);
			}
			if (boidsList[iter].targetPosBuffer[1] / boidsList[iter].numOfNeighbors
				!= boidsList[iter].pos[1]) {
				boidsList[iter].separationTargetPos[1] = boidsList[iter].pos[0] - 1000
					/ (boidsList[iter].targetPosBuffer[1] / boidsList[iter].numOfNeighbors - boidsList[iter].pos[1]);
			}
		}
		else {
			boidsList[iter].separationTargetPos[0] = boidsList[iter].pos[0];
			boidsList[iter].separationTargetPos[1] = boidsList[iter].pos[1];
		}
	}

	// Alignment
	for (int iter = 0; iter < count; iter++) {
		boidsList[iter].clearBuffers();
		boidsList[iter].sumofNeighborsTheta = 0;
	}
	for (int iter = 0; iter < count; iter++) {
		for (int iter2 = iter + 1; iter2 < count; iter2++) {
			if (getDistance(boidsList[iter], boidsList[iter2]) < BOID_RADIUS) {
				boidsList[iter].numOfNeighbors++;
				boidsList[iter2].numOfNeighbors++;
				boidsList[iter].sumofNeighborsTheta += boidsList[iter].theta;
				boidsList[iter2].sumofNeighborsTheta += boidsList[iter2].theta;
			}
		}
	}
	for (int iter = 0; iter < count; iter++) {
		if (boidsList[iter].numOfNeighbors > 0) {
			boidsList[iter].alignmentTargetPos[0] = boidsList[iter].pos[0] + cos(boidsList[iter].sumofNeighborsTheta
				/ boidsList[iter].numOfNeighbors);
			boidsList[iter].alignmentTargetPos[1] = boidsList[iter].pos[1] + cos(boidsList[iter].sumofNeighborsTheta
				/ boidsList[iter].numOfNeighbors);
		}
		else {
			boidsList[iter].alignmentTargetPos[0] = boidsList[iter].pos[0];
			boidsList[iter].alignmentTargetPos[1] = boidsList[iter].pos[1];
		}
	}

	// Update Boids
	for (int iter = 0; iter < count; iter++)
		//boids[iter].update();
		boidsList[iter].update();
}

void Flock::render() {
	for (int iter = 0; iter < count; iter++)
		//boids[iter].render();
		boidsList[iter].render();
}

void Flock::writeToBitstream(RakNet::BitStream& bsOut, unsigned char typeID) {
	
	bsOut.Write(typeID);

	for (int iter = 0; iter < count; iter++) {
		bsOut.Write(boidsList[iter].numOfNeighbors);
		bsOut.Write(boidsList[iter].pos[0]);
		bsOut.Write(boidsList[iter].pos[1]);
		bsOut.Write(boidsList[iter].alignmentTargetPos[0]);
		bsOut.Write(boidsList[iter].alignmentTargetPos[1]);
		bsOut.Write(boidsList[iter].cohesionTargetPos[0]);
		bsOut.Write(boidsList[iter].cohesionTargetPos[1]);
		bsOut.Write(boidsList[iter].separationTargetPos[0]);
		bsOut.Write(boidsList[iter].separationTargetPos[1]);
		bsOut.Write(boidsList[iter].targetPosBuffer[0]);
		bsOut.Write(boidsList[iter].targetPosBuffer[1]);
		bsOut.Write(boidsList[iter].velocity[0]);
		bsOut.Write(boidsList[iter].velocity[1]);
		bsOut.Write(boidsList[iter].theta);
		bsOut.Write(boidsList[iter].sumofNeighborsTheta);
	}
}

void Flock::readFromBitstream(RakNet::Packet* packet) {
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	unsigned char typeID;
	bsIn.Read(typeID);
	for (int iter = 0; iter < count; iter++) {
		bsIn.Read(boidsList[iter].numOfNeighbors);
		bsIn.Read(boidsList[iter].pos[0]);
		bsIn.Read(boidsList[iter].pos[1]);
		bsIn.Read(boidsList[iter].alignmentTargetPos[0]);
		bsIn.Read(boidsList[iter].alignmentTargetPos[1]);
		bsIn.Read(boidsList[iter].cohesionTargetPos[0]);
		bsIn.Read(boidsList[iter].cohesionTargetPos[1]);
		bsIn.Read(boidsList[iter].separationTargetPos[0]);
		bsIn.Read(boidsList[iter].separationTargetPos[1]);
		bsIn.Read(boidsList[iter].targetPosBuffer[0]);
		bsIn.Read(boidsList[iter].targetPosBuffer[1]);
		bsIn.Read(boidsList[iter].velocity[0]);
		bsIn.Read(boidsList[iter].velocity[1]);
		bsIn.Read(boidsList[iter].theta);
		bsIn.Read(boidsList[iter].sumofNeighborsTheta);
	}
}