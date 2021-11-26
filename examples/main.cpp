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
// #include <random>   /* 亂數函式庫 */
#include <ctime>
#include <cstdlib>

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
	sim->setTimeStep(0.01f);

	/* Specify the default parameters for agents that are subsequently added. */
	// setAgentDefaults(float neighborDist, size_t maxNeighbors, float timeHorizon, float radius, float maxSpeed, const Vector3 &velocity)
	// sim->setAgentDefaults(15.0f, 10, 1.0f, 1.5f, 2.0f);
	// sim->setAgentDefaultsCyl(15.0f, 10, 1.0f, 1.5f, 2.0f, true, 6.0f);
	// sim->setAgentDefaultsCyl(15.0f, 10, 1.0f, 1.5f, 2.0f, false, 3.5f);
	sim->setAgentDefaultsCyl(0.4f, 10, 1.0f, 0.15f, 1.5f, false, 3.5f);



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

	sim->addAgent(RVO::Vector3(0, 10, 10));
	sim->addAgent(RVO::Vector3(0, 5, 7));
	goals.push_back(RVO::Vector3(0, 0, 10));
	goals.push_back(RVO::Vector3(0, 5, 7));


	// sim->addAgent(RVO::Vector3(10, 10, 10));
	// sim->addAgent(RVO::Vector3(-10, 10, 10));
	// sim->addAgent(RVO::Vector3(-10, -10, 10));
	// sim->addAgent(RVO::Vector3(10, -10, 10));

	// sim->addAgentCyl(RVO::Vector3(10, 10, 10));
	// sim->addAgentCyl(RVO::Vector3(-10, 10, 10));
	// sim->addAgentCyl(RVO::Vector3(-10, -10, 10));
	// sim->addAgentCyl(RVO::Vector3(10, -10, 10));

	// goals.push_back(RVO::Vector3(-4.99, 0, 10));
	// goals.push_back(RVO::Vector3(0, 5, 10));
	// goals.push_back(RVO::Vector3(-5, -5, 10)); // x and y cannot be the same (why????)
	// goals.push_back(RVO::Vector3(5, 5, 10));

	// goals.push_back(RVO::Vector3(-10, -9.9, -5));
	// goals.push_back(RVO::Vector3(10, -10, -5.5));
	// goals.push_back(RVO::Vector3(10, 10, -5));
	// goals.push_back(RVO::Vector3(-10, 10, -5));

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
			RVO::Vector3 pos_i = sim->getAgentPosition(i);
			RVO::Vector3 pos_j = sim->getAgentPosition(j);
			// if (!sim->getsimMode(i) && RVO::abs(pos_i - pos_j) <= 2.0f * sim->getAgentRadius(i)) {
			// 	std::cout << RVO::abs(sim->getAgentPosition(i) - sim->getAgentPosition(j)) << std::endl;
			// 	std::cout << "sph height, " << std::abs(pos_i[2] - pos_j[2]) << ", " << sim->getAgentHeight(i) << std::endl;
			// 	return true;
			// } else if (	sim->getsimMode(i) && 
			// 			(std::sqrt((pos_i[0]-pos_j[0])*(pos_i[0]-pos_j[0]) + (pos_i[1]-pos_j[1])*(pos_i[1]-pos_j[1])) <= 2.0f * sim->getAgentRadius(i)) &&
			// 			(std::abs(pos_i[2] - pos_j[2]) < sim->getAgentHeight(i))) {
			// 	std::cout << "cyl height, " << std::abs(pos_i[2] - pos_j[2]) << ", " << sim->getAgentHeight(i) << std::endl;
			// 	// std::cout << std::sqrt((pos_i[0]-pos_j[0])*(pos_i[0]-pos_j[0]) + (pos_i[1]-pos_j[1])*(pos_i[1]-pos_j[1])) << std::endl;
			// 	return true;
			// }
			if (RVO::abs(pos_i - pos_j) <= 2.0f * sim->getAgentRadius(i)) {
				// std::cout << RVO::abs(sim->getAgentPosition(i) - sim->getAgentPosition(j)) << std::endl;
				// std::cout << std::abs(pos_i[2] - pos_j[2]) << ", " << sim->getAgentHeight(i) << std::endl;
				// std::cout << i << ", " << j << " collision" << std::endl;
				return true;
			}
		}
	}

	return false;
}

float get_h_min(RVO::RVOSimulator *sim, float rh_min)
{
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		for (size_t j = i+1; j < sim->getNumAgents(); ++j) {
			RVO::Vector3 pos_i = sim->getAgentPosition(i);
			RVO::Vector3 pos_j = sim->getAgentPosition(j);
			// std::cout << "h_min, " << h_min << std::endl;
			// std::cout << "22, " << std::abs(pos_i[2] - pos_j[2]) << ", " << sim->getAgentHeight(i) << std::endl;
			// std::cout << std::sqrt((pos_i[0]-pos_j[0])*(pos_i[0]-pos_j[0]) + (pos_i[1]-pos_j[1])*(pos_i[1]-pos_j[1])) << std::endl;

			// if (std::sqrt((pos_i[0]-pos_j[0])*(pos_i[0]-pos_j[0]) + (pos_i[1]-pos_j[1])*(pos_i[1]-pos_j[1])) <= rh_min[0] 
			// 	&& std::abs(pos_i[2] - pos_j[2]) < sim->getAgentHeight(i)){
			// 	rh_min[0] = std::sqrt((pos_i[0]-pos_j[0])*(pos_i[0]-pos_j[0]) + (pos_i[1]-pos_j[1])*(pos_i[1]-pos_j[1]));
			// 	std::cout << "r_min: " << i << "," << j << " " << rh_min[0] << std::endl;
			// }
			if (std::sqrt((pos_i[0]-pos_j[0])*(pos_i[0]-pos_j[0]) + (pos_i[1]-pos_j[1])*(pos_i[1]-pos_j[1])) <= 2.0f * sim->getAgentRadius(i) && 
				std::abs(pos_i[2] - pos_j[2]) < rh_min){
				rh_min = std::abs(sim->getAgentPosition(i)[2] - sim->getAgentPosition(j)[2]);
				std::cout << "h_min: " << i << "," << j << " " << rh_min << std::endl;
			}
		}
	}
	return rh_min;
}

void downwash(RVO::RVOSimulator *sim){
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		for (size_t j = 0; j < sim->getNumAgents(); ++j) {
			if (i==j){continue;}
			RVO::Vector3 pos_i = sim->getAgentPosition(i);
			RVO::Vector3 pos_j = sim->getAgentPosition(j);
			float dxy = std::sqrt((pos_i[0]-pos_j[0])*(pos_i[0]-pos_j[0]) + (pos_i[1]-pos_j[1])*(pos_i[1]-pos_j[1]));
			float dz = pos_j[2]-pos_i[2];
			if ( dxy <= 2.0f * sim->getAgentRadius(i) && dz > 0){
				// std::cout << "Downwash force" << std::endl;
				float force = -25.0*1.28*sim->getAgentRadius(i)*sim->getAgentRadius(i)*3.1415*9.81/(2*3.1415*dz*dz);
				RVO::Vector3 vel = sim->getAgentVelocity(i);
				vel[2] += force*sim->getTimeStep();
				sim->setAgentVelocity(i, vel);
				RVO::Vector3 pos = sim->getAgentPosition(i);
				pos[2] += vel[2]*sim->getTimeStep();
				sim->setAgentPosition(i, pos);
				// continue;
			}
		}
	}
}


void cross(RVO::RVOSimulator *sim){
	int num = 5;
	float rad = 0.4;
	float width = 2.0;
    int count = 1;
	do{
		bool init_col = false;
		bool fina_col = false;
		RVO::Vector3 init;
		RVO::Vector3 fina;
		float pt_1 = std::rand()/float(RAND_MAX)/(-2)-2.5;
		float pt_2 = std::rand()/float(RAND_MAX)/2+width+2.5;
		// std::cout << pt_1 << ", " << pt_2 << std::endl;

		if(std::rand()/float(RAND_MAX)<0.5){
			float temp = pt_1;
			pt_1 = pt_2;
			pt_2 = temp;
		}
		if(std::rand()/float(RAND_MAX)<0.5){
			init = RVO::Vector3(pt_1, std::rand()/float(RAND_MAX)*width, std::rand()/float(RAND_MAX));
			fina = RVO::Vector3(pt_2, std::rand()/float(RAND_MAX)*width, std::rand()/float(RAND_MAX));
		} else{
			init = RVO::Vector3(std::rand()/float(RAND_MAX)*width, pt_1, std::rand()/float(RAND_MAX));
			fina = RVO::Vector3(std::rand()/float(RAND_MAX)*width, pt_2, std::rand()/float(RAND_MAX));
		}
		for (size_t i = 0; i < sim->getNumAgents(); ++i) {
			if(RVO::abs(init - sim->getAgentPosition(i))<2*rad){
				init_col = true;
				break;
			} 
			if(RVO::abs(fina - goals[i])<2*rad){
				fina_col = true;
				break;
			} 
		}
		if (!init_col && !fina_col){
			sim->addAgent(init);
			goals.push_back(fina);
			// std::cout << count << ": " << sim->getNumAgents() << ": " << goals.size() << ": " << init << ", " << fina << std::endl;
			count += 1;
		}
	}while(count<=num);
}

void hover(RVO::RVOSimulator *sim){
	int num = 10;
	float rad = 0.4;
	float width = 2.0;
	int count = 1;
	do{
		bool initfina_col = false;
		// bool fina_col = false;
		float pt_1 = std::rand()/float(RAND_MAX)*width;
		float pt_2 = std::rand()/float(RAND_MAX)*width;
		RVO::Vector3 init_fina = RVO::Vector3(pt_1, pt_2, std::rand()/float(RAND_MAX));
		init_fina = RVO::Vector3(pt_1, pt_2, std::rand()/float(RAND_MAX));

		for (size_t i = 0; i < sim->getNumAgents(); ++i) {
			if(RVO::abs(init_fina - sim->getAgentPosition(i))<2*rad || RVO::abs(init_fina - goals[i])<2*rad){
				initfina_col = true;
				break;
			}
		}
		if (!initfina_col){
			sim->addAgent(init_fina);
			goals.push_back(init_fina);
			// std::cout << count << ": " << sim->getNumAgents() << ": " << goals.size() << ": " << init << ", " << fina << std::endl;
			count += 1;
		}
	}while(count<=num);

}



int main()
{
	int col = 0;
	int stuck = 0;
	int succ = 0;
	clock_t tStart = clock(); // start sim time
	// seed(1)
	float rh_min = 999.99;
	for (size_t i = 0; i < 10; ++i) {
		// std::cout << i << " iteration: " << std::endl;
		// float rand = std::rand()/float(RAND_MAX)*2;
		// std::cout << RAND_MAX << " rand: " << rand << std::endl;


	// }


		/* Create a new simulator instance. */
		RVO::RVOSimulator *sim = new RVO::RVOSimulator();

		/* Set up the scenario. */
		// setupScenario(sim);
		sim->setTimeStep(0.01f);
		sim->setAgentDefaultsCyl(0.4f, 10, 1.0f, 0.15f, 1.5f, false, 3.5f);
		cross(sim);
		hover(sim);

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
			downwash(sim);

			myfile << sim->getGlobalTime() << ",0";
			for (size_t i = 0; i < sim->getNumAgents(); ++i) {
				myfile << "," << sim->getAgentPosition(i)[0] << "," << sim->getAgentPosition(i)[1] << "," << sim->getAgentPosition(i)[2] << "," << "0,0,0,0,0";
				// std::cout << sim->getAgentPosition(i) << std::endl;
				// std::cout << i << "vel" << sim->getAgentVelocity(i) << std::endl;
			}
			myfile << "\n";
			// rh_min = get_h_min(sim, rh_min);
			if (collision(sim)){ std::cout << i << " iteration: " << sim->getGlobalTime() << "s, Collide!!!" << std::endl; col += 1; break; }
			if (sim->getGlobalTime() >= 100){ std::cout << i << " iteration: " << sim->getGlobalTime() << "s, Stuck!!!" << std::endl; stuck += 1; break; }
			// do {
			// 	std::cout << '\n' << "Press a key to continue...";
			// } while (std::cin.get() != '\n');
		}
		while (!reachedGoal(sim));
		if (reachedGoal(sim)){ std::cout << i << " iteration: " << sim->getGlobalTime() << "s, Success!!!" << std::endl; succ += 1;}
		// std::cout << sim->getGlobalTime() << ", End!!!" << std::endl;
		

		// for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		// 	std::cout << sim->getAgentPosition(i)[1] << std::endl;
		// }

		myfile.close();
		delete sim;
		goals.clear();
	}

    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    // printf("Minimum range difference: %.2f\n", rh_min[0]);
    printf("Minimum altitude difference: %.2f\n", rh_min);


	return 0;
}
