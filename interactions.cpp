
#include <cmath>
#include "raylib.h"
#include "raymath.h"
#include "ArrayQueue.h"
#include <iostream>
#include <vector>

struct MovingTracker{
	bool ComputeXY = false;
	bool ComputeXZ = false;
};

MovingTracker GLOBAL_MOVING_TRACKER;



void MoveCameraEvent(Camera& camera){if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){UpdateCamera(&camera, CAMERA_FREE);}}

void CreatePointEvent(PointArrayQueue& ActivePoints){
	
	if (IsKeyPressed(KEY_P)){
				ControlPoint3D p = ControlPoint3D();
				p.ControlPointFromInput();
				ActivePoints.Append(p);
	}
}

void SelectPointEvent(PointArrayQueue& ActivePoints, Camera& camera){

	if ( (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))){
		Vector2 mousePos = GetMousePosition();
		Ray mouseRay = GetScreenToWorldRay(mousePos, camera);

		int clstIdx = -1;
		float minDist = INFINITY;

		for (int k = 0; k < ActivePoints.getSize(); k++){

			ControlPoint3D& tmpPoint = ActivePoints.getItem(k);

			//filter for visible points

			if (tmpPoint.IsVisible()){

				//get collision data

				RayCollision tmpCollision = GetRayCollisionSphere(mouseRay, tmpPoint.getCoords(), 0.05f);

				bool PointHit = tmpCollision.hit;

				//if we get a hit and the distance is smaller than the one previously recorded
				if (PointHit && (tmpCollision.distance < minDist)){ clstIdx = k; minDist = tmpCollision.distance;}
				
			}

		}

		//if we ever found a point

		if (clstIdx >= 0){
			ControlPoint3D& selectedPoint = ActivePoints.getItem(clstIdx);
			
			if (selectedPoint.IsSelected()){selectedPoint.Unselect();}
			else{selectedPoint.Select();}		
			
		}

	}

	

}

void RemoveSelectedPoints(PointArrayQueue& ActivePoints){

	if (IsKeyPressed(KEY_DELETE)){
		
		std::vector<int> indexesToBeDeleted = ActivePoints.FindSelectedPoints();

		for (int k = 0; k < indexesToBeDeleted.size(); k++){
			ActivePoints.RemoveItem(indexesToBeDeleted[k]);
		}
	}

}

Vector3 ComputeMouseRayXZintersection(Ray MouseRay, ControlPoint3D p){

	Vector3 RaydDir = MouseRay.direction;
	Vector3 RayPos = MouseRay.position;
	Vector3 pCoords = p.getCoords();

	float pY = pCoords.y;
	float RayY = RayPos.y;
	float RayDirY =  RaydDir.y;

	if (fabsf(RayDirY) < 1e-6f){return pCoords;};

	float t = (pY - RayY) / RayDirY;

	if (t < 0.0f){return pCoords;};

	Vector3 interCoords = RayPos + t * RaydDir;

	//float interX = interCoords.x;
	//float interZ = interCoords.z;
	//float interY = interCoords.y;

	return interCoords;
}

Vector3 ComputeMouseRayXYintersection(Ray MouseRay, ControlPoint3D p){

	Vector3 RaydDir = MouseRay.direction;
	Vector3 RayPos = MouseRay.position;
	Vector3 pCoords = p.getCoords();

	float pZ = pCoords.z;
	float RayZ = RayPos.z;
	float RayDirZ =  RaydDir.z;

	if (fabsf(RayDirZ) < 1e-6f){return pCoords;};

	float t = (pZ - RayZ) / RayDirZ;

	if (t < 0.0f){return pCoords;};

	Vector3 interCoords = RayPos + t * RaydDir;

	//float interX = interCoords.x;
	//float interZ = interCoords.z;
	//float interY = interCoords.y;

	return interCoords;
}


void MoveSelectedPoints(PointArrayQueue& ActivePoints, Camera& camera){

	if (IsKeyDown(KEY_M)){

		std::vector<int> indexesToBeMoved = ActivePoints.FindSelectedPoints();

		if (indexesToBeMoved.size() < 1){std::cout << "no points to be moved!!!" << std::endl; return;}
		if (indexesToBeMoved.size() >= 2){std::cout << "you are trying to move too many points!!!" << std::endl; return;}

		ControlPoint3D& p = ActivePoints.getItem( indexesToBeMoved[0] );
		Vector2 mousePos = GetMousePosition();
		Ray mouseRay = GetScreenToWorldRay(mousePos, camera);

		Vector3 newPos;

		if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)){
			
			GLOBAL_MOVING_TRACKER.ComputeXY = false;
			GLOBAL_MOVING_TRACKER.ComputeXZ = true;
		}
		if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)){
			
			GLOBAL_MOVING_TRACKER.ComputeXY = true;
			GLOBAL_MOVING_TRACKER.ComputeXZ = false;
		}

		if (GLOBAL_MOVING_TRACKER.ComputeXY){newPos = ComputeMouseRayXYintersection(mouseRay, p);}
		if (GLOBAL_MOVING_TRACKER.ComputeXZ){newPos = ComputeMouseRayXZintersection(mouseRay, p);}

		if (GLOBAL_MOVING_TRACKER.ComputeXY || GLOBAL_MOVING_TRACKER.ComputeXZ){

			DrawSphere(newPos, 0.05f, (Color) RED);

			if (IsKeyPressed(KEY_ENTER)){
				p.Move(newPos);
			}
		}		

	}
	else{
		GLOBAL_MOVING_TRACKER.ComputeXY = false;
		GLOBAL_MOVING_TRACKER.ComputeXZ = false;
	}


}

