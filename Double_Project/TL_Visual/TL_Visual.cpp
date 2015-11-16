// TL_Visual.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

#include "CrowdDynamics.hpp"
using namespace gen;
EKeyCode quitKey = Key_Escape;
EKeyCode pauseKey = Key_P;

const int kNoAgents = 500;
const gen::CVector2 kWorldSize = CVector2(200.0f, 200.0f);
const float kTimeStep = 1.0f / 30.0f;
const int kXSubDiv = 20;
const int kYSubDiv = 20;

float frameTime = 0;

struct SAgent
{
	IModel* model;
	UID id;
};

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
	cam->SetRotationSpeed(180.0f);

	float frameRate;
	stringstream frameStream;
	frameStream.precision(4);

	gen::CMatrix3x3 tempAgentMat;
	string tempString;
	int tempInt, tempInt2;
	float tempModelMat[16];
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
		frameStream << frameRate;
		gameEngine->SetWindowCaption(frameStream.str());
		frameStream.str("");

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
	}

	// Delete the 3D engine now we are finished with it
	gameEngine->Delete();

	delete crowdEngine;
}
