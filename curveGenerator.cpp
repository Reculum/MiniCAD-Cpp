
#include "ArrayQueue.h"
#include <vector>


class NURBS_Curve{

	protected:

	std::vector<ControlPoint3D*> CtrlPointsPointers;
	float* weights;
	int degree;

	public:

	NURBS_Curve(PointArrayQueue& ActivePoints, int _degree){

		std::vector<int> SelectedPoints = ActivePoints.FindSelectedPoints();

		for (int k = 0; k < SelectedPoints.size(); k++){

			CtrlPointsPointers.push_back(&ActivePoints.getItem(SelectedPoints[k]));
		}

		weights = new float[SelectedPoints.size()];

		degree = _degree;
	}


};
