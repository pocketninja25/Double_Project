// TL_Visual.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

#include "Picking.h"

#include "CrowdDynamics.hpp"
using namespace gen;
EKeyCode quitKey = Key_Escape;
EKeyCode pauseKey = Key_P;

const int kNoAgents = 50;
const gen::CVector2 kWorldSize = CVector2(200.0f, 200.0f);
const float kTimeStep = 1.0f / 10.0f;
const int kXSubDiv = 5;
const int kYSubDiv = 5;

float frameTime = 0;

struct SAgent
{
	IModel* model;
	UID id;
};

bool FindNearest(SAgent* agents, GSceneManager* manager, UID &nearestID, I3DEngine* gameEngine, ICamera* cam)
{
	CVector3 tempPos = WorldPosFromPixel(gameEngine, cam);
	CVector2 mousePosition = CVector2(tempPos.x, tempPos.z);

	CVector2 agentPosition;
	bool set = false;
	UID nearest;
	CVector2 nearestPosition;

	for (int i = 0; i < kNoAgents; i++)
	{
		if (manager->GetAgentPosition(agents[i].id, agentPosition))
		{
			if (!set)
			{
				nearest = agents[i].id;
				nearestPosition = agentPosition;
				set = true;
			}
			else if((agentPosition - mousePosition).Length() < (nearestPosition - mousePosition).Length())
			{
				nearest = agents[i].id;
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

	IMesh* floorMesh = gameEngine->LoadMesh("Floor.x");
	IModel* floorModel = floorMesh->CreateModel(kWorldSize.x / 2.0f, 0.0f, kWorldSize.y / 2.0f);
	IMesh* agentMesh = gameEngine->LoadMesh("Box.x");
	
	SAgent AGENTS[kNoAgents];
	
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
		AGENTS[i].model = agentMesh->CreateModel();
		AGENTS[i].id = IDs[i];
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
	for (int i = 0; i < kNoAgents; i++)
	{
		if (crowdEngine->GetAgentMatrix(AGENTS[i].id, tempAgentMat))
		{
			//Convert agent matrix to model matrix
			AGENTS[i].model->GetMatrix(tempModelMat);
			tempModelMat[0] = tempAgentMat.e00;
			tempModelMat[2] = tempAgentMat.e01;
			tempModelMat[8] = tempAgentMat.e10;
			tempModelMat[10] = tempAgentMat.e11;
			tempModelMat[12] = tempAgentMat.e20;
			tempModelMat[14] = tempAgentMat.e21;
			AGENTS[i].model->SetMatrix(tempModelMat);
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
		for (int i = 0; i < kNoAgents; i++)
		{
			if (crowdEngine->GetAgentMatrix(AGENTS[i].id, tempAgentMat))
			{
				//Convert agent matrix to model matrix
				AGENTS[i].model->GetMatrix(tempModelMat);
				tempModelMat[0] = tempAgentMat.e00;
				tempModelMat[2] = tempAgentMat.e01;
				tempModelMat[8] = tempAgentMat.e10;
				tempModelMat[10] = tempAgentMat.e11;
				tempModelMat[12] = tempAgentMat.e20;
				tempModelMat[14] = tempAgentMat.e21;
				AGENTS[i].model->SetMatrix(tempModelMat);
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
			for (int i = 0; i < kNoAgents; i++)
			{
				if (crowdEngine->GetAgentMatrix(AGENTS[i].id, tempAgentMat))
				{
					//Convert agent matrix to model matrix
					AGENTS[i].model->GetMatrix(tempModelMat);
					tempModelMat[0] = tempAgentMat.e00;
					tempModelMat[2] = tempAgentMat.e01;
					tempModelMat[8] = tempAgentMat.e10;
					tempModelMat[10] = tempAgentMat.e11;
					tempModelMat[12] = tempAgentMat.e20;
					tempModelMat[14] = tempAgentMat.e21;
					AGENTS[i].model->SetMatrix(tempModelMat);
				}
			}
		}
		if (gameEngine->KeyHit(Key_M) && crowdEngine->GetIsPaused())
		{
			cout << "Enter a number between 0 and " << kNoAgents << endl;
			cin >> tempInt;	//TODO: add validation
			
			cout << "Agent " << tempInt << " details: " << endl;
			if (crowdEngine->GetAgentString(AGENTS[tempInt].id, tempString))
			{
				cout << tempString << endl;
			}
			else
			{
				cout << "Error - Agent Not Found" << endl;
			}
		}
		if (gameEngine->KeyHit(Key_N) && crowdEngine->GetIsPaused())
		{
			cout << "Enter a number between 0 and " << kXSubDiv << endl;
			cin >> tempInt;
			cout << "Enter a number between 0 and " << kYSubDiv << endl;
			cin >> tempInt2;
			cout << "Square " << tempInt << ", " << tempInt2 << " details: " << endl;
			if (crowdEngine->GetSquareString(tempInt, tempInt2, tempString))
			{
				cout << tempString << endl;
			}
			else
			{
				cout << "Error - Square Not Found" << endl;
			}
		}
		if (gameEngine->KeyHeld(Mouse_LButton))
		{	
			if (holding == -1)	//If not holding anything, pick the nearest item up
			{
				UID tempHold;
				if (FindNearest(AGENTS, crowdEngine, tempHold, gameEngine, cam))
				{
					holding = tempHold;
					string agentString;
					if (crowdEngine->GetAgentString(holding, agentString))
					{
						cout << "Picked up: " << endl;
						cout << agentString << endl;
					}
					for (int i = 0; i < kNoAgents; i++)
					{
						if (AGENTS[i].id == holding)
						{
							AGENTS[i].model->SetSkin("tiles3.jpg");
						}
					}
				}
			}
			
			if (holding != -1)
			{
				CVector3 tempPos = WorldPosFromPixel(gameEngine, cam);
				crowdEngine->SetAgentPosition(holding, CVector2(tempPos.x, tempPos.z));

				if (crowdEngine->GetAgentMatrix(holding, tempAgentMat))
				{
					for (int i = 0; i < kNoAgents; i++)
					{
						if (AGENTS[i].id == holding)
						{
							AGENTS[i].model->GetMatrix(tempModelMat);
							tempModelMat[0] = tempAgentMat.e00;
							tempModelMat[2] = tempAgentMat.e01;
							tempModelMat[8] = tempAgentMat.e10;
							tempModelMat[10] = tempAgentMat.e11;
							tempModelMat[12] = tempAgentMat.e20;
							tempModelMat[14] = tempAgentMat.e21;
							AGENTS[i].model->SetMatrix(tempModelMat);
						}
					}//Convert agent matrix to model matrix
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
				for (int i = 0; i < kNoAgents; i++)
				{
					if (AGENTS[i].id == holding)
					{
						if (crowdEngine->GetAgentWatched(holding))
						{
							AGENTS[i].model->SetSkin("tiles2.jpg");
						}
						else
						{
							AGENTS[i].model->SetSkin("tiles1.jpg");
						}
					}
				}

				holding = -1;
			}
		}
		if (gameEngine->KeyHit(Mouse_RButton))
		{
			UID foundNearest;
			if (FindNearest(AGENTS, crowdEngine, foundNearest, gameEngine, cam))
			{
				bool isWatched = crowdEngine->GetAgentWatched(foundNearest);
				if (crowdEngine->SetAgentWatched(foundNearest, !crowdEngine->GetAgentWatched(foundNearest)))
				{
					cout << "Agent: " << foundNearest << " Is ";
					if (isWatched)	//Was being watched before change, is now not being watched
					{
						for (int i = 0; i < kNoAgents; i++)
						{
							if (AGENTS[i].id == foundNearest)
							{
								AGENTS[i].model->SetSkin("tiles1.jpg");
							}
						}
						cout << "Not ";
					}
					else
					{
						for (int i = 0; i < kNoAgents; i++)
						{
							if (AGENTS[i].id == foundNearest)
							{
								AGENTS[i].model->SetSkin("tiles2.jpg");
							}
						}
					}
					cout << "Being Watched" << endl;
				}
			}
		}
	}

	// Delete the 3D engine now we are finished with it
	gameEngine->Delete();

	delete crowdEngine;
}
