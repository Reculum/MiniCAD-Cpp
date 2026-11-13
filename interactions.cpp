
#include <cmath>
#include <iostream>
#include <vector>
#include <random>

#include "raylib.h"
#include "raymath.h"

#include "ArrayQueue.h"
#include "curveGenerator.h"


std::random_device rd;
std::mt19937 gen(rd());
std::normal_distribution<float> StdNormDist(0.0f, 1.0f);

struct MovingTracker{
	bool ComputeXY = false;
	bool ComputeXZ = false;
	Vector3 A;
	Vector3 B;
	Vector3 C;
	Vector3 D;
};

MovingTracker GLOBAL_MOVING_TRACKER;



void MoveCameraEvent(Camera& camera){if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){UpdateCamera(&camera, CAMERA_FREE);}}

void CreatePointEvent(PointArrayQueue<ControlPoint3D>& ActivePoints){
	
	if (IsKeyPressed(KEY_P) && !(IsKeyDown(KEY_LEFT_SHIFT))){
				ControlPoint3D* p = new ControlPoint3D();
				p -> ControlPointFromInput();
				ActivePoints.Append(*p);
	}

	if (IsKeyPressed(KEY_P) && (IsKeyDown(KEY_LEFT_SHIFT))){
				ControlPoint3D* p =  new ControlPoint3D(StdNormDist(gen), StdNormDist(gen), StdNormDist(gen));
				ActivePoints.Append(*p);
	}

}

void SelectPointEvent(PointArrayQueue<ControlPoint3D>& ActivePoints, Camera& camera){

	if ( (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))){
		Vector2 mousePos = GetMousePosition();
		Ray mouseRay = GetScreenToWorldRay(mousePos, camera);

		int clstIdx = -1;
		float minDist = INFINITY;

		for (int k = 0; k < ActivePoints.getSize(); k++){

			ControlPoint3D& tmpPoint = ActivePoints.getItem(k);

			//filter for visible points

			if ( (&tmpPoint != nullptr) && (tmpPoint.IsVisible()) ){

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

void SelectCurveEvent(PointArrayQueue<NURBS_Curve>& ActiveNurbs, Camera& camera){

	if ( (IsKeyDown(KEY_LEFT_SHIFT)) && (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))){

		Vector2 mousePos = GetMousePosition();
		Ray mouseRay = GetScreenToWorldRay(mousePos, camera);

		int clstIdx = -1;
		float minDist = INFINITY;

		for (int k = 0; k < ActiveNurbs.getSize(); k++){

			NURBS_Curve& tmpNurbs = ActiveNurbs.getItem(k);
			NURBS_Curve* tmpNurbPtr = &tmpNurbs;

			if (tmpNurbPtr != nullptr){

				std::vector<Vector3> tmpTessellation = tmpNurbs.getTessellation();

				//filter for visible points

				float minTmpNurbsDist = INFINITY;

				for (int j = 0; j < tmpTessellation.size(); j++){

					Vector3 tmpPoint = tmpTessellation[j];

					float t = Vector3DotProduct(tmpPoint - mouseRay.position, mouseRay.direction);
					float ClstDistance = INFINITY;

					if (t >= 0){ClstDistance = Vector3Length(tmpPoint - mouseRay.position - mouseRay.direction * t);}

					//if we get a hit and the distance is smaller than the one previously recorded
					if (ClstDistance < minTmpNurbsDist){ minTmpNurbsDist = ClstDistance;}

				}

				if (minTmpNurbsDist < minDist){clstIdx = k; minDist = minTmpNurbsDist;}


			}
			

		}

		//if we ever found a point

		if (clstIdx >= 0){
			NURBS_Curve& selectedNurbs = ActiveNurbs.getItem(clstIdx);
			
			if (selectedNurbs.IsSelected()){selectedNurbs.Unselect();}
			else{selectedNurbs.Select();}		
			
		}

	}

	

}

void RemoveSelectedPoints(PointArrayQueue<ControlPoint3D>& ActivePoints){

	if (IsKeyPressed(KEY_DELETE)){
		
		std::vector<int> indexesToBeDeleted = ActivePoints.FindSelectedPoints();

		for (int k = 0; k < indexesToBeDeleted.size(); k++){

			if (!ActivePoints.getItem(indexesToBeDeleted[k]).IsItBelongingToCurve()){
				ActivePoints.RemoveItem(indexesToBeDeleted[k]);
			}
			else{std::cout << "The point can't be deleted! It belongs to a Curve!!!" << std::endl;}			
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


void MoveSelectedPoints(PointArrayQueue<ControlPoint3D>& ActivePoints, Camera& camera){

	if (IsKeyDown(KEY_M)){

		float halfSize = 5.0f;

		std::vector<int> indexesToBeMoved = ActivePoints.FindSelectedPoints();

		if (indexesToBeMoved.size() < 1){std::cout << "no points to be moved!!!" << std::endl; return;}
		if (indexesToBeMoved.size() >= 2){std::cout << "you are trying to move too many points!!!" << std::endl; return;}

		ControlPoint3D& p = ActivePoints.getItem( indexesToBeMoved[0] );
		Vector2 mousePos = GetMousePosition();
		Ray mouseRay = GetScreenToWorldRay(mousePos, camera);

		Vector3 newPos;
		float PlaneCenterX = p.getCoords().x;
		float PlaneCenterZ = p.getCoords().z;
		float PlaneCenterY = p.getCoords().y;

		if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)){
			
			GLOBAL_MOVING_TRACKER.ComputeXY = false;
			GLOBAL_MOVING_TRACKER.ComputeXZ = true;

			GLOBAL_MOVING_TRACKER.A = {PlaneCenterX + halfSize, PlaneCenterY, PlaneCenterZ - halfSize};
			GLOBAL_MOVING_TRACKER.B = {PlaneCenterX + halfSize, PlaneCenterY, PlaneCenterZ + halfSize};
			GLOBAL_MOVING_TRACKER.C = {PlaneCenterX - halfSize, PlaneCenterY, PlaneCenterZ + halfSize};
			GLOBAL_MOVING_TRACKER.D = {PlaneCenterX - halfSize, PlaneCenterY, PlaneCenterZ - halfSize};
		}


		if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)){
			
			GLOBAL_MOVING_TRACKER.ComputeXY = true;
			GLOBAL_MOVING_TRACKER.ComputeXZ = false;

			GLOBAL_MOVING_TRACKER.A = {PlaneCenterX + halfSize, PlaneCenterY + halfSize, PlaneCenterZ};
			GLOBAL_MOVING_TRACKER.B = {PlaneCenterX + halfSize, PlaneCenterY - halfSize, PlaneCenterZ};
			GLOBAL_MOVING_TRACKER.C = {PlaneCenterX - halfSize, PlaneCenterY - halfSize, PlaneCenterZ};
			GLOBAL_MOVING_TRACKER.D = {PlaneCenterX - halfSize, PlaneCenterY + halfSize, PlaneCenterZ};
		}

		if (GLOBAL_MOVING_TRACKER.ComputeXY){newPos = ComputeMouseRayXYintersection(mouseRay, p);}
		if (GLOBAL_MOVING_TRACKER.ComputeXZ){newPos = ComputeMouseRayXZintersection(mouseRay, p);}

		if (GLOBAL_MOVING_TRACKER.ComputeXZ || GLOBAL_MOVING_TRACKER.ComputeXY){

			DrawTriangle3D(GLOBAL_MOVING_TRACKER.C, GLOBAL_MOVING_TRACKER.B, GLOBAL_MOVING_TRACKER.D, Fade(SKYBLUE, 0.25f));
			DrawTriangle3D(GLOBAL_MOVING_TRACKER.A, GLOBAL_MOVING_TRACKER.D, GLOBAL_MOVING_TRACKER.B, Fade(SKYBLUE, 0.25f));

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


void CreateNurbsCurve(PointArrayQueue<NURBS_Curve>& ActiveNurbs, PointArrayQueue<ControlPoint3D>& ActivePoints, int degree){

	if (IsKeyPressed(KEY_C)){

		NURBS_Curve* tmpCurve = new NURBS_Curve(ActivePoints, degree);
		ActiveNurbs.Append(*tmpCurve);
	}

}

void ChangePointWeightOnCurve(PointArrayQueue<NURBS_Curve>& ActiveNurbs){

	if ( (IsKeyDown(KEY_LEFT_SHIFT)) && (IsKeyPressed(KEY_W)) ){

		std::vector<int> CurvesSelected = ActiveNurbs.FindSelectedPoints();

		if (CurvesSelected.size() != 1){std::cout << "you have either selected too many curves or no curve!!!" << std::endl; return;}

		NURBS_Curve& selectedNurbsCurve = ActiveNurbs.getItem(CurvesSelected[0]);
		std::vector<ControlPoint3D*> CtrlPoints = selectedNurbsCurve.getCtrlPoints();

		int SelectedCtrlPointIndex = -1;
		int nCtrlPointsSelected = 0;

		for (int k = 0; k < CtrlPoints.size(); k++){

			if (CtrlPoints[k] -> IsSelected()){SelectedCtrlPointIndex = k; nCtrlPointsSelected += 1;}
		}


		if (nCtrlPointsSelected != 1){std::cout << "you have either selected too many ctrl points or no ctrl point!!!" << std::endl; return;}


		float newWeight = 0.0f;

		std::cout << "new weight: ";
		std::cin >> newWeight;

		if (newWeight <= 0){std::cout << "you have inserted a non-positive weight!!!" << std::endl; return;}

		ControlPoint3D* selectedCtrlPoint = CtrlPoints[SelectedCtrlPointIndex];

		selectedNurbsCurve.setWeight(newWeight, SelectedCtrlPointIndex);
		selectedCtrlPoint -> Update();	


	}



}


void RemoveSelectedCurves(PointArrayQueue<NURBS_Curve>& ActiveNurbs){

	if (IsKeyPressed(KEY_DELETE)){
		
		std::vector<int> indexesToBeDeleted = ActiveNurbs.FindSelectedPoints();

		for (int k = 0; k < indexesToBeDeleted.size(); k++){

			NURBS_Curve& tmpCurve = ActiveNurbs.getItem(indexesToBeDeleted[k]);

			std::vector<ControlPoint3D*> tmpCtrlPoints = tmpCurve.getCtrlPoints();

			for (int j = 0; j < tmpCtrlPoints.size(); j++){tmpCtrlPoints[j] -> MarkAsNotBelongingToCurve();}

			ActiveNurbs.RemoveItem(indexesToBeDeleted[k]);
		}
	}

}











