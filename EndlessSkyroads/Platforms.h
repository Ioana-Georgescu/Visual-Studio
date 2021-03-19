#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace SkyroadsAux
{
	// color codes
	#define PURPLE 0 // player has landed on it
	#define BLUE 1 // normal
	#define GREEN 2 // gain fuel
	#define ORANGE 3 // stuck on max speed
	#define YELLOW 4 // lose fuel
	#define RED 5 // game end
	#define MINUSLIFE 6 // lose a life
	#define PLUSLIFE 7 // add a life
	#define GHOST 8 // pass through walls

	#define LANEWIDTH 2
	#define PLATFORMTHICKNESS 0.5
	#define PLATFORMTBOTTOM -0.5
	#define FIELDLENGHT 50
	#define WALLTHICKNESS 0.25 // thinner than player sphere
	#define SPRINGDISTANCE 8
	#define WALLRISESPEED 2.5
	#define DISTFORJUMP 3 // player must be able to start jump on platform and have room to land

	#define LEFTLANE -2
	#define CENTER 0
	#define RIGHTLANE 2

	struct PlarformLog
	{
		PlarformLog() : x(0), startZ(0), endZ(1), colorCode(0) {}
		PlarformLog(float x, float startZ, float endZ, int colorCode)
			: x(x), startZ(startZ), endZ(endZ), colorCode(colorCode) {}
		float x;
		float startZ;
		float endZ;
		int colorCode;

		bool operator < (const PlarformLog& platform) const
		{
			if (x < platform.x)
				return true;
			else if (x > platform.x)
				return false;
			else
				return (endZ > platform.endZ);
		}
	};

	struct WallLog
	{
		WallLog() : x(0), startZ(0), hight(0), spring(false), finalHight(0) {}
		WallLog(float x, float startZ, float hight, bool spring, float finalHight)
			: x(x), startZ(startZ), hight(hight) , spring(spring), finalHight(finalHight) {}
		float x;
		float startZ;
		float hight;
		bool spring;
		float finalHight;
	};

	class Platforms
	{
	public:
		Platforms()
		{
			farthestZ = 0;
			farthestAuxZ = 0;

			wallLimit = 30;	// initial wall chance: [0, 30) = 30%
			springLimit = 30;	// initial chance for a wall to spring up from the platform: [0, 30) = 30%
		}
		~Platforms() {}

		std::vector<PlarformLog> logList;
		std::vector<WallLog> walls;
		std::vector<int> mainLanes = { LEFTLANE, CENTER, RIGHTLANE };
		std::vector<int> auxLanes = { -8, -4, 4, 8 };

		// separate so that player always has platform to jump on on main lanes
		float farthestZ; // largest endZ for main lanes
		float farthestAuxZ; // largest endZ for aux lanes

		// modified in time to increase difficulty
		float wallLimit;
		float springLimit;


	protected:
		const int minLength = 4;
		const int minLengthWithWall = 7; // big enough to fit wall + space for jumping and landing
		const int maxLength = 10;
		const int minDist = 1; // bigger than player width
		const int maxDist = 3.25;
		const int minHight = 1;
		const int maxHightSafe = 2.25;
		const int maxHight = 5.5;

		// generating tiles
		// code = left|center|right
		// 1 = tile on this lane / safe color
		// 0 = no tile / any color
		// at least 1 must be generated to ensure endless running

		// 3 main tiles to generate / color type for each if 3 lanes must be generated
		std::vector<int> mainLanesCode = { 111, 110, 101, 011, 001, 010, 010 };

		// color type for lanes if 2 must be generated
		std::vector<int> twoLanesCode = { 11, 10, 01 };

		// which aux lanes to be generated
		std::vector<int> auxLanesCode = { 1111, 1001, 1010, 1100, 0011, 0101, 0110 };


		// gauges for color generator funcions
		const int blueLimitSafe = 75; // blue chance for safe color: [0, 75) = 75%
									  // green chance for safe color: [75, 100] = 25%

		const int blueLimit = 35;		// blue chance: [0, 35) = 35%
		const int greenLimit = 45;		// green chance: [35, 45) = 10%
		const int orangeLimit = 60;		// orange chance: [45, 60) = 15%
		const int yellowLimit = 75;		// yellow chance: [60, 75) = 15%
		const int redLimit = 85;		// red chance: [75, 85] = 10%
		const int minusLifeLimit = 90;  // minusLife chance: [85, 90) = 5%
		const int plusLifeLimit = 95;	// plusLife chance: [90, 95) = 5%
										// ghost chance: [95, 100) = 5%

		const int auxLimit = 20; // aux lane chance: [0, 20) = 20%

	public:
		float ReachableDist()
		{
			return ((float)(rand() % (maxDist * 100)) / 100 + minDist);
		}

		float NewLength()
		{
			return ((float)(rand() % (maxLength * 100)) / 100 + minLength);
		}

		float NewLengthWithWall()
		{
			return ((float)(rand() % (maxLength * 100)) / 100 + minLengthWithWall);
		}

		void InitNewPlatform(float lane, int colorCode, float lastRowEnd)
		{
			float startZ, endZ;
			startZ = lastRowEnd - ReachableDist();
			endZ = startZ - NewLength();
			logList.push_back(PlarformLog(lane, startZ, endZ, colorCode));
		}

		void InitNewPlatformWithWall(float lane, int colorCode, float lastRowEnd)
		{
			float startZ, endZ;
			startZ = lastRowEnd - ReachableDist();
			endZ = startZ - NewLengthWithWall();
			logList.push_back(PlarformLog(lane, startZ, endZ, colorCode));
		}

		void InitNewWall(float lane, float platformStartZ, float platformEndZ, float hight)
		{
			float startZ;
			startZ = -((float)(rand() % (int)(platformStartZ - platformEndZ - DISTFORJUMP)) - platformStartZ + DISTFORJUMP);

			float springChance = rand() % 100;
			if (springChance < springLimit)
			{
				walls.push_back(WallLog(lane, startZ, 0, true, hight));
			}
			else
			{
				walls.push_back(WallLog(lane, startZ, hight, false, hight));
			}
		}

		int GetPlatformByX(float x)
		{
			std::vector<PlarformLog>::iterator it = std::find_if(logList.begin(), logList.end(), [x](const PlarformLog& val) {
				return (val.x == x);
				}
			);

			return std::distance(logList.begin(), it);
		}

		int SafeColor() // no plus life color to not make game too easy
		{
			int code = rand() % 100;
			if (code < blueLimitSafe)
			{
				return BLUE;
			}
			return GREEN;
		}

		int AnyColor()
		{
			int code = rand() % 100;
			if (code < blueLimit)
			{
				return BLUE;
			}
			else if (code < greenLimit)
			{
				return GREEN;
			}
			else if (code < orangeLimit)
			{
				return ORANGE;
			}
			else if (code < yellowLimit)
			{
				return YELLOW;
			}
			else if (code < redLimit)
			{
				return RED;
			}
			else if (code < minusLifeLimit)
			{
				return MINUSLIFE;
			}
			else if (code < plusLifeLimit)
			{
				return PLUSLIFE;
			}
			return GHOST;
		}

		float SafeHight()
		{
			return ((float)(rand() % (maxHightSafe * 100)) / 100 + minHight);
		}

		float AnyHight()
		{
			return ((float)(rand() % (maxHight * 100)) / 100 + minHight);
		}

		void AddPlatforms(int lives)
		{
			int codeIndex; // what lane configuration will be chosen
			int laneCode; // the cofiguration code from lanes code vector
			int safeCode; // which main lanes will guaranteed to be safe colors
			int wallChance; // to determine if wall on platform

			// intermediar new farthest distances
			float newRowFarthestZ = farthestZ;
			float newRowFarthestAuxZ = farthestAuxZ;

			// if the longest tile is red the next row might be accessible only from the red tile
			//		=> endless running = impossible
			// additional tiles must be added on the lanes next to it till its length is covered
			int blockedLane = -1; // no lane has this index
			std::vector<float> newEndZs = { farthestZ, farthestZ, farthestZ }; // for LEFTLANE, CENTER, RIGHTLANE

			// add aux lanes
			int aux = rand() % 100;
			if (aux < auxLimit)
			{
				codeIndex = rand() % auxLanesCode.size();
				laneCode = auxLanesCode[codeIndex];

				for (int i = 0; i < auxLanes.size(); i++)
				{
					if (laneCode % 10 == 1) // platform on this lane
					{
						wallChance = rand() % 100;
						if (wallChance < wallLimit)
						{
							InitNewPlatformWithWall(auxLanes[i], AnyColor(), farthestAuxZ);
							InitNewWall(auxLanes[i], logList[logList.size() - 1].startZ, logList[logList.size() - 1].endZ, AnyHight());
						}
						else
						{
							InitNewPlatform(auxLanes[i], AnyColor(), farthestAuxZ);
						}
						newRowFarthestAuxZ = std::min(newRowFarthestAuxZ, logList[logList.size() - 1].endZ);
					}
					laneCode = laneCode / 10; // move to code for next lane
				}
			}

			// add main lanes
			codeIndex = rand() % mainLanesCode.size(); // at least 1 reachable will be generated
			laneCode = mainLanesCode[codeIndex];

			if (codeIndex == 0) // 111 = all 3 main lanes
			{
				safeCode = mainLanesCode[rand() % mainLanesCode.size()]; // 1, 2 or 3 will be safe
			}
			else if (codeIndex < 4) // 2 lanes
			{
				safeCode = twoLanesCode[rand() % twoLanesCode.size()]; // 1 or both 3 will be safe
			}
			else // 1 lane
			{
				safeCode = 1; // must be safe
			}

			for (int i = 0; i < mainLanes.size(); i++)
			{
				if (laneCode % 10 == 1)
				{
					if (safeCode % 10 == 1)
					{
						wallChance = rand() % 100;
						if (wallChance < wallLimit)
						{
							InitNewPlatformWithWall(mainLanes[i], SafeColor(), farthestZ);
							InitNewWall(mainLanes[i], logList[logList.size() - 1].startZ, logList[logList.size() - 1].endZ, SafeHight());
						}
						else
						{
							InitNewPlatform(mainLanes[i], SafeColor(), farthestZ);
						}
					}
					else
					{
						wallChance = rand() % 100;
						if (wallChance < wallLimit)
						{
							InitNewPlatformWithWall(mainLanes[i], AnyColor(), farthestZ);
							InitNewWall(mainLanes[i], logList[logList.size() - 1].startZ, logList[logList.size() - 1].endZ, AnyHight());
						}
						else
						{
							InitNewPlatform(mainLanes[i], AnyColor(), farthestZ);
						}
					}
					safeCode = safeCode / 10;

					newEndZs[i] = logList[logList.size() - 1].endZ;

					// falling on RED or MINUSLIFE with only 1 life == game end
					if ((logList[logList.size() - 1].colorCode == RED)
						|| ((logList[logList.size() - 1].colorCode == MINUSLIFE) && (lives == 1)))
					{
						blockedLane = i;
					}

					// pick the smallest number
					// smallest = farthest away from camera in player movement direction
					// all platforms from a row must use the same farthestZ as a reference when initialised => update intermediar value
					newRowFarthestZ = std::min(newRowFarthestZ, newEndZs[i]);
				}
				laneCode = laneCode / 10;
			}

			// all platforms from a row must use the same farthestZ as a reference when initialised
			farthestZ = std::min(farthestZ, newRowFarthestZ);

			// game end platform was generated and it is the longest
			if ((blockedLane > 0) && (farthestZ == newEndZs[blockedLane]))
			{
				// farthest z between the other 2 lanes
				switch (blockedLane)
				{
				case 0:
					newRowFarthestZ = std::min(newEndZs[1], newEndZs[2]);
					break;
				case 1:
					newRowFarthestZ = std::min(newEndZs[0], newEndZs[2]);
					break;
				case 2:
					newRowFarthestZ = std::min(newEndZs[0], newEndZs[1]);
					break;
				default:
					break;
				}

				// generate safe platforms to pass the RED one
				while (newRowFarthestZ > farthestZ)
				{
					for (int i = 0; i < mainLanes.size(); i++)
					{
						if (i != blockedLane)
						{
							// new safe platform accesible from the last one on this lane
							InitNewPlatform(mainLanes[i], SafeColor(), newEndZs[i]);
							newEndZs[i] = logList[logList.size() - 1].endZ;

							newRowFarthestZ = std::min(newRowFarthestZ, newEndZs[i]);
						}
					}
				}

				farthestZ = newRowFarthestZ;
			}
			farthestAuxZ = std::min(farthestZ, newRowFarthestAuxZ);

			std::sort(logList.begin(), logList.end());
		}
	};
}