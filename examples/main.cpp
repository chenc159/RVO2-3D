/*
 * Sphere.cpp
 * RVO2-3D Library
 *
 * Copyright 2008 University of North Carolina at Chapel Hill
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Please send all bug reports to <geom@cs.unc.edu>.
 *
 * The authors may be contacted via:
 *
 * Jur van den Berg, Stephen J. Guy, Jamie Snape, Ming C. Lin, Dinesh Manocha
 * Dept. of Computer Science
 * 201 S. Columbia St.
 * Frederick P. Brooks, Jr. Computer Science Bldg.
 * Chapel Hill, N.C. 27599-3175
 * United States of America
 *
 * <https://gamma.cs.unc.edu/RVO2/>
 */

/* Example file showing a demo with 812 agents initially positioned evenly distributed on a sphere attempting to move to the antipodal position on the sphere. */

// #ifndef RVO3D_OUTPUT_TIME_AND_POSITIONS
// #define RVO3D_OUTPUT_TIME_AND_POSITIONS 1
// #endif

#include <cmath>
#include <cstddef>
#include <vector>

#include <iostream>
#include <fstream>
// #if RVO3D_OUTPUT_TIME_AND_POSITIONS
// #endif

#include <RVO.h>

#ifndef M_PI
const float M_PI = 3.14159265358979323846f;
#endif

/* Store the goals of the agents. */
std::vector<RVO::Vector3> goals;

void setupScenario(RVO::RVOSimulator *sim)
{
	/* Specify the global time step of the simulation. */
	sim->setTimeStep(0.125f);

	/* Specify the default parameters for agents that are subsequently added. */
	// setAgentDefaults(float neighborDist, size_t maxNeighbors, float timeHorizon, float radius, float maxSpeed, const Vector3 &velocity)
	sim->setAgentDefaults(15.0f, 10, 1.0f, 1.5f, 2.0f);



	// Set sphere or cylinder (bool + cylinder height/shpere radius)...



	/* Add agents, specifying their start position, and store their goals on the opposite side of the environment. */
	// for (float a = 0; a < M_PI; a += 0.1f) {
	// 	const float z = 100.0f * std::cos(a);
	// 	const float r = 100.0f * std::sin(a);

	// 	for (size_t i = 0; i < r / 2.5f; ++i) {
	// 		const float x = r * std::cos(i * 2.0f * M_PI / (r / 2.5f));
	// 		const float y = r * std::sin(i * 2.0f * M_PI / (r / 2.5f));

	// 		sim->addAgent(RVO::Vector3(x, y, z));
	// 		goals.push_back(-sim->getAgentPosition(sim->getNumAgents() - 1));
	// 	}
	// 	if (sim->getNumAgents() >= 5){break;}
	// }
	sim->addAgent(RVO::Vector3(10, 10, 10));
	sim->addAgent(RVO::Vector3(-10, 10, 10));
	sim->addAgent(RVO::Vector3(-10, -10, 10));
	sim->addAgent(RVO::Vector3(10, -10, 10));

	// goals.push_back(RVO::Vector3(-4.99, 0, 10));
	// goals.push_back(RVO::Vector3(0, 5, 10));
	// goals.push_back(RVO::Vector3(-5, -5, 10)); // x and y cannot be the same (why????)
	// goals.push_back(RVO::Vector3(5, 5, 10));

	goals.push_back(RVO::Vector3(-10, -9.9, -5));
	goals.push_back(RVO::Vector3(10, -10, -5.5));
	goals.push_back(RVO::Vector3(10, 10, -5));
	goals.push_back(RVO::Vector3(-10, 10, -5));

	// for (float i = 1; i < sim->getNumAgents(); ++i) {
	// 	goals.push_back(-sim->getAgentPosition(i));
	// 	std::cout << goals[i] << std::endl;
	// }

	// std::cout << "number of agent: " << sim->getNumAgents() << std::endl;
	// std::cout << "rad: " << sim->getAgentRadius(0) << std::endl;
	// std::cout << "neighbdist: " << sim->getAgentNeighborDist(0) << std::endl;
	// std::cout << "max speed: " << sim->getAgentMaxSpeed(0) << std::endl;
	// std::cout << "time horizon: " << sim->getAgentTimeHorizon(0) << std::endl;
}

#if RVO3D_OUTPUT_TIME_AND_POSITIONS
void updateVisualization(RVO::RVOSimulator *sim)
{
	/* Output the current global time. */
	std::cout << sim->getGlobalTime();

	/* Output the position for all the agents. */
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		std::cout << " " << sim->getAgentPosition(i);
	}

	std::cout << std::endl;
}
#endif

void setPreferredVelocities(RVO::RVOSimulator *sim)
{
	/* Set the preferred velocity to be a vector of unit magnitude (speed) in the direction of the goal. */
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		RVO::Vector3 goalVector = goals[i] - sim->getAgentPosition(i);

		if (RVO::absSq(goalVector) > 1.0f) {
			goalVector = RVO::normalize(goalVector)*sim->getAgentMaxSpeed(i);
		}
		// std::cout << i << goalVector << std::endl;
		sim->setAgentPrefVelocity(i, goalVector);
	}
}

bool reachedGoal(RVO::RVOSimulator *sim)
{
	/* Check if all agents have reached their goals. */
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		if (RVO::absSq(sim->getAgentPosition(i) - goals[i]) > 4.0f * sim->getAgentRadius(i) * sim->getAgentRadius(i)) {
			// std::cout << i << " " << RVO::absSq(sim->getAgentPosition(i) - goals[i]) << sim->getAgentPosition(i)<< std::endl;
			return false;
		}
	}

	return true;
}

bool collision(RVO::RVOSimulator *sim)
{
	/* Check if all agents have reached their goals. */
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		for (size_t j = i+1; j < sim->getNumAgents(); ++j) {
			if (RVO::abs(sim->getAgentPosition(i) - sim->getAgentPosition(j)) <= 2.0f * sim->getAgentRadius(i)) {
				// std::cout << RVO::abs(sim->getAgentPosition(i) - sim->getAgentPosition(j)) << std::endl;
				return true;
			}
		}
	}

	return false;
}

int main()
{
	clock_t tStart = clock();

	/* Create a new simulator instance. */
	RVO::RVOSimulator *sim = new RVO::RVOSimulator();

	/* Set up the scenario. */
	setupScenario(sim);

	
	/* Record data in .csv */
	std::ofstream myfile;
    myfile.open ("test.csv");
	myfile << "Trans_Time," << "Exe_Time," << "x," << "y," << "z," << "roll," << "pitch,"  << "vx," << "vy," << "vz \n";
	myfile << "Goals_x," << "Goals_y_z";
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		myfile << "," << goals[i][0] << "," << goals[i][1] << "," << goals[i][2] << "," << "0,0,0,0,0";
	}
	myfile << "\n";

	/* Perform (and manipulate) the simulation. */
	do {
		// #if RVO3D_OUTPUT_TIME_AND_POSITIONS
		// 		updateVisualization(sim);
		// #endif

		setPreferredVelocities(sim);
		// std::cout << sim->getAgentPosition(0) << std::endl;

		sim->doStep();

		myfile << sim->getGlobalTime() << ",0";
		for (size_t i = 0; i < sim->getNumAgents(); ++i) {
			myfile << "," << sim->getAgentPosition(i)[0] << "," << sim->getAgentPosition(i)[1] << "," << sim->getAgentPosition(i)[2] << "," << "0,0,0,0,0";
			// std::cout << sim->getAgentPosition(i) << std::endl;
			// std::cout << i << "vel" << sim->getAgentVelocity(i) << std::endl;
		}
		myfile << "\n";
		if (collision(sim)){ std::cout << sim->getGlobalTime() << ", Collide!!!" << std::endl; break; }
		if (sim->getGlobalTime() >= 100){ std::cout << sim->getGlobalTime() << ", Stuck!!!" << std::endl; break; }
		// do {
		// 	std::cout << '\n' << "Press a key to continue...";
		// } while (std::cin.get() != '\n');
	}
	while (!reachedGoal(sim));

	std::cout << sim->getGlobalTime() << ", End!!!" << std::endl;
	

	// for (size_t i = 0; i < sim->getNumAgents(); ++i) {
	// 	std::cout << sim->getAgentPosition(i)[1] << std::endl;
	// }

    myfile.close();
	delete sim;

    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);


	return 0;
}
