
#include <cmath>
#include "raylib.h"
#include "ArrayQueue.h"


void MoveCameraEvent(Camera& camera){if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){UpdateCamera(&camera, CAMERA_FREE);}}

void CreatePointEvent(PointArrayQueue& ActivePoints){
	
	if (IsKeyPressed(KEY_P)){
				ControlPoint3D p = ControlPoint3D();
				p.ControlPointFromInput();
				ActivePoints.Append(p);
	}
}

void SelectPointEvent(PointArrayQueue& SelectedPoints, PointArrayQueue& ActivePoints, Camera& camera){

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
				if (PointHit && (tmpCollision.distance < minDist) && (!tmpPoint.IsSelected())){ clstIdx = k; minDist = tmpCollision.distance;}
				
			}

		}

		//if we ever found a point

		if (clstIdx >= 0){
			ControlPoint3D& selectedPoint = ActivePoints.getItem(clstIdx);
			selectedPoint.Select();

			//warning: Append copies the result!
			SelectedPoints.Append(selectedPoint);
		}

	}

	

}

