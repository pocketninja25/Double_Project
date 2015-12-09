// TL_Visual.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;
#include <map>
using std::map;
#include "Picking.h"

#include "CrowdDynamics.hpp"
using namespace gen;
EKeyCode quitKey = Key_Escape;
EKeyCode pauseKey = Key_P;

const int kNoAgents = 50;
const gen::CVector2 kWorldSize = CVector2(200.0f, 200.0f);
const float kTimeStep = 1.0f / 10.0f;
const int kXSubDiv = 20;
const int kYSubDiv = 20;

float frameTime = 0;

struct SAgent
{
	IModel* model;
	UID id;
};

bool FindNearestAgent(map<UID, IModel*> agents, GSceneManager* manager, UID &nearestID, I3DEngine* gameEngine, ICamera* cam)
{
	CVector3 tempPos = WorldPosFromPixel(gameEngine, cam);
	CVector2 mousePosition = CVector2(tempPos.x, tempPos.z);

	CVector2 agentPosition;
	bool set = false;
	UID nearest;
	CVector2 nearestPosition;

	for (auto agent : agents)
	{
		if (manager->GetAgentPosition(agent.first, agentPosition))
		{
			if (!set)
			{
				nearest = agent.first;
				nearestPosition = agentPosition;
				set = true;
			}
			else if ((agentPosition - mousePosition).Length() < (nearestPosition - mousePosition).Length())
			{
				nearest = agent.first;
				nearestPosition = agentPosition;
			}
		}
	}

	nearestID = nearest;

	return set;
}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* gameEngine = New3DEngine(kTLX);
	gameEngine->StartWindowed();

	// Add default folder for meshes and other media
	gameEngine->AddMediaFolder( ".\\Media" );

	//IMesh* floorMesh = gameEngine->LoadMesh("Floor.x");
	//IModel* floorModel = floorMesh->CreateModel(kWorldSize.x / 2.0f, 0.0f, kWorldSize.y / 2.0f);
	IMesh* agentMesh = gameEngine->LoadMesh("Box.x");
	IMesh* floorTileMesh = gameEngine->LoadMesh("FloorTile.x");

	map<UID, IModel*> AGENTS;
	IModel** FloorTiles;
	
	FloorTiles = new IModel*[kXSubDiv * kYSubDiv];
	for (int i = 0; i < kXSubDiv; i++)
	{
		for (int j = 0; j < kYSubDiv; j++)
		{
			FloorTiles[i * kXSubDiv + j] = floorTileMesh->CreateModel(i * (kWorldSize.x/kXSubDiv), 0.0f, j * (kWorldSize.y / kYSubDiv));
			FloorTiles[i * kXSubDiv + j]->ScaleX(kWorldSize.x / kXSubDiv);
			FloorTiles[i * kXSubDiv + j]->ScaleZ(kWorldSize.y / kYSubDiv);
		}
	}


	//--------------------------
	// CrowdDynamics Setup
	//--------------------------
	SWorldInfo worldData;
	worldData.TimeStep = kTimeStep;
	worldData.WorldSize = kWorldSize;
	worldData.xSubdivisions = kXSubDiv;
	worldData.ySubdivisions = kYSubDiv;
	GSceneManager* crowdEngine = GSceneManager::GetInstance(&worldData);
	vector<UID> IDs = crowdEngine->AddXAgents(kNoAgents);
	for (int i = 0; i < kNoAgents; i++)
	{
		//Use the Y Column as the height (0 column), CrowdDynamics uses X and Y as this will use X and Z
		//Where the model spawns is irrelevant as it's matrix is built from the CrowdDynamics matrix
		AGENTS.insert(make_pair(IDs[i], agentMesh->CreateModel()));
	}
	
	/**** Set up your scene here ****/
	ICamera* cam = gameEngine->CreateCamera(kManual, kWorldSize.x/2.0f, 220.0f, kWorldSize.y/2.0f);
	cam->RotateX(90.0f);
	cam->SetMovementSpeed(400.0f);
	cam->SetRotationSpeed(0.0f);// 180.0f);

	float frameRate;
	stringstream frameStream;
	
	gen::CMatrix3x3 tempAgentMat;
	string tempString;
	int tempInt, tempInt2;
	float tempModelMat[16];

	//Assign the simulation matrices to the model matrices - first time only
	for (auto agent : AGENTS)
	{
		if (crowdEngine->GetAgentMatrix(agent.first, tempAgentMat))
		{
			agent.second->GetMatrix(tempModelMat);
			tempModelMat[0] = tempAgentMat.e00;
			tempModelMat[2] = tempAgentMat.e01;
			tempModelMat[8] = tempAgentMat.e10;
			tempModelMat[10] = tempAgentMat.e11;
			tempModelMat[12] = tempAgentMat.e20;
			tempModelMat[14] = tempAgentMat.e21;
			agent.second->SetMatrix(tempModelMat);
		}
	}

	UID holding = -1;

	// The main game loop, repeat until engine is stopped
	while (gameEngine->IsRunning())
	{
		if (gameEngine->KeyHit(Key_Escape))
		{
			gameEngine->Stop();
		}
		frameTime = gameEngine->Timer();
		// Draw the scene
		gameEngine->DrawScene();
		
		frameRate = 1 / frameTime;
		frameStream << static_cast<int>(frameRate);
		gameEngine->SetWindowCaption(frameStream.str());
		frameStream.str("");

		if (gameEngine->KeyHeld(Key_Numpad4))
		{
			cam->MoveX(-80.0f * frameTime);
		}
		if (gameEngine->KeyHeld(Key_Numpad6))
		{
			cam->MoveX(80.0f * frameTime);
		}
		if (gameEngine->KeyHeld(Key_Numpad2))
		{
			cam->MoveZ(-80.0f * frameTime);
		}
		if (gameEngine->KeyHeld(Key_Numpad8))
		{
			cam->MoveZ(80.0f * frameTime);
		}
		if (gameEngine->KeyHeld(Key_Numpad7))
		{
			cam->MoveY(-80.0f * frameTime);
		}
		if (gameEngine->KeyHeld(Key_Numpad9))
		{
			cam->MoveY(80.0f * frameTime);
		}

		/**** Update your scene each frame here ****/
		crowdEngine->Update(frameTime);	//Update the CrowdDynamics simulation

		//Assign the simulation matrices to the model matrices
		for (auto agent : AGENTS)
		{
			if (crowdEngine->GetAgentMatrix(agent.first, tempAgentMat))
			{
				agent.second->GetMatrix(tempModelMat);
				tempModelMat[0] = tempAgentMat.e00;
				tempModelMat[2] = tempAgentMat.e01;
				tempModelMat[8] = tempAgentMat.e10;
				tempModelMat[10] = tempAgentMat.e11;
				tempModelMat[12] = tempAgentMat.e20;
				tempModelMat[14] = tempAgentMat.e21;
				agent.second->SetMatrix(tempModelMat);
			}
		}
		
		if (gameEngine->KeyHit(pauseKey))
		{
			crowdEngine->SetPaused(!crowdEngine->GetIsPaused());	//Toggle paused
		}
		if (gameEngine->KeyHeld(Key_T) && crowdEngine->GetIsPaused())
		{
			crowdEngine->PerformOneTimeStep();	//Update the CrowdDynamics simulation

			//Assign the simulation matrices to the model matrices
			for (auto agent : AGENTS)
			{
				if (crowdEngine->GetAgentMatrix(agent.first, tempAgentMat))
				{
					agent.second->GetMatrix(tempModelMat);
					tempModelMat[0] = tempAgentMat.e00;
					tempModelMat[2] = tempAgentMat.e01;
					tempModelMat[8] = tempAgentMat.e10;
					tempModelMat[10] = tempAgentMat.e11;
					tempModelMat[12] = tempAgentMat.e20;
					tempModelMat[14] = tempAgentMat.e21;
					agent.second->SetMatrix(tempModelMat);
				}
			}
		}
		if (gameEngine->KeyHit(Key_N))
		{
			CVector3 tempPos = WorldPosFromPixel(gameEngine, cam);
			CVector2 mousePosition = CVector2(tempPos.x, tempPos.z);

			crowdEngine->GetSquareString(mousePosition, tempString);
			cout << tempString;
		}
		if (gameEngine->KeyHeld(Mouse_LButton))
		{	
			if (holding == -1)	//If not holding anything, pick the nearest item up
			{
				UID tempHold;
				if (FindNearestAgent(AGENTS, crowdEngine, tempHold, gameEngine, cam))
				{
					holding = tempHold;
					string agentString;
					if (crowdEngine->GetAgentString(holding, agentString))
					{
						cout << "Picked up: " << endl;
						cout << agentString << endl;
					}
					try		//Find the agent 'holding' and set the holding skin
					{
						AGENTS.at(holding)->SetSkin("tiles3.jpg");

					}
					catch (std::out_of_range err)
					{
						cout << err.what();
					}
				}
			}
			if (holding != -1)
			{
				CVector3 tempPos = WorldPosFromPixel(gameEngine, cam);
				crowdEngine->SetAgentPosition(holding, CVector2(tempPos.x, tempPos.z));

				if (crowdEngine->GetAgentMatrix(holding, tempAgentMat))
				{
					try		//Find the agent 'holding' and set the holding skin
					{
						AGENTS.at(holding)->GetMatrix(tempModelMat);
						tempModelMat[0] = tempAgentMat.e00;
						tempModelMat[2] = tempAgentMat.e01;
						tempModelMat[8] = tempAgentMat.e10;
						tempModelMat[10] = tempAgentMat.e11;
						tempModelMat[12] = tempAgentMat.e20;
						tempModelMat[14] = tempAgentMat.e21;
						AGENTS.at(holding)->SetMatrix(tempModelMat);
					}
					catch (std::out_of_range err)
					{
						cout << err.what();
					}
				}
			}
		}
		else
		{
			if (holding != -1)
			{
				string agentString;
				if (crowdEngine->GetAgentString(holding, agentString))
				{
					cout << "Dropped: " << endl;
					cout << agentString << endl;
				}
				try
				{
					if (crowdEngine->GetAgentWatched(holding))
					{
						AGENTS.at(holding)->SetSkin("tiles2.jpg");
					}
					else
					{
						AGENTS.at(holding)->SetSkin("tiles1.jpg");
					}
				}
				catch (std::out_of_range err)
				{
					cout << err.what();
				}
				
				holding = -1;
			}
		}
		if (gameEngine->KeyHit(Mouse_RButton))
		{
			UID foundNearest;
			if (FindNearestAgent(AGENTS, crowdEngine, foundNearest, gameEngine, cam))
			{
				bool isWatched = crowdEngine->GetAgentWatched(foundNearest);
				if (crowdEngine->SetAgentWatched(foundNearest, !crowdEngine->GetAgentWatched(foundNearest)))
				{
					cout << "Agent: " << foundNearest << " Is ";
					if (isWatched)	//Was being watched before change, is now not being watched
					{
						try
						{
							AGENTS.at(foundNearest)->SetSkin("tiles1.jpg");
						}
						catch (std::out_of_range err)
						{
							cout << endl << err.what() << endl;
						}
						cout << "Not ";
					}
					else
					{
						try
						{
							AGENTS.at(foundNearest)->SetSkin("tiles2.jpg");
						}
						catch (std::out_of_range err)
						{
							cout << endl << err.what() << endl;
						}
					}
					cout << "Being Watched" << endl;
				}
			}
		}
	}

	// Delete the 3D engine now we are finished with it
	gameEngine->Delete();
	delete[] FloorTiles;
	delete crowdEngine;
}
