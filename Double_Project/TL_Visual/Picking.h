#pragma once

#include "MathsLib.h"
#include <TL-Engine.h>
using namespace tle;

const float gNearClip = 2.0f;
const float gFarClip = 30000.0f;

gen::CVector3 WorldPosFromPixel(I3DEngine* gameEngine, ICamera* cam)
{
	D3DXVECTOR4 q;
	D3DXVECTOR2 pixelPt;
	pixelPt.x = static_cast<float>(gameEngine->GetMouseX());
	pixelPt.y = static_cast<float>(gameEngine->GetMouseY());

	//Convert to a -1 to 1 coord system from pixel position
	q.x = pixelPt.x / (gameEngine->GetWidth() / 2) - 1;
	q.y = 1 - pixelPt.y / (gameEngine->GetHeight() / 2);

	q.w = gNearClip;	//Near clip distance
	q.z = 0;			//Closest depth buffer value

						//Undo perspective
	q.x *= q.w;
	q.y *= q.w;
	q.z *= q.w;

	D3DXMATRIX worldMatrix;
	cam->GetMatrix(worldMatrix);

	D3DXMATRIX viewMatrix;

	D3DXMatrixInverse(&viewMatrix, NULL, &worldMatrix);

	D3DXMATRIX projectionMatrix;
	float aspect = (float)gameEngine->GetWidth() / gameEngine->GetHeight();
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 3.4f, aspect, gNearClip, gFarClip);

	//Multiply by inverse viewprojmatrix to convert from camera space to world space
	D3DXMATRIX viewProj = viewMatrix * projectionMatrix;
	D3DXMatrixInverse(&viewProj, 0, &viewProj);	//Inverse the viewproj matrix
	D3DXVec4Transform(&q, &q, &viewProj);

	//Create floor plane at origin
	D3DXPLANE floor;
	D3DXPlaneFromPointNormal(&floor, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	//Calculate the position on the plane that the ray was cast to
	D3DXVECTOR3 rayPos;
	D3DXPlaneIntersectLine(&rayPos, &floor, &D3DXVECTOR3(q), &D3DXVECTOR3(cam->GetX(), cam->GetY(), cam->GetZ()));


	return gen::CVector3(rayPos.x, rayPos.y, rayPos.z); //Returns the position on the plane that the ray was cast to
}
