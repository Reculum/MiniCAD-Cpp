#include "ArrayQueue.h"
#include "raymath.h"

#include <cmath>
#include <vector>
#include <array>
#include <stdexcept>



class NURBS_Curve{

	protected:

	std::vector<ControlPoint3D*> CtrlPointsPointers;
	std::vector<Vector3> Tessellation;
	std::vector<float> Nodes;

	float* weights;
	int n;
	int degree;

	public:

	NURBS_Curve(PointArrayQueue& ActivePoints, int p){

		degree = p;

		std::vector<int> SelectedPoints = ActivePoints.FindSelectedPoints();
		n = SelectedPoints.size() - 1;

		if (p < 0 || SelectedPoints.size() < static_cast<size_t>(p + 1)){
			throw std::invalid_argument("Invalid degree or insufficient control points");
		}
    	


		weights = new float[n + 1];
		CtrlPointsPointers.reserve(n + 1);
		Nodes.reserve(n + degree + 2);

		for (int k = 0; k < n + 1; k++){

			CtrlPointsPointers.push_back(&ActivePoints.getItem(SelectedPoints[k]));
			weights[k] = 1.0f;

		}

		int m = n + p + 1;

		for (int k = 0; k <= p; k++){Nodes.push_back(0.0f);}
		for (int k = p + 1; k <= n; k++){Nodes.push_back( (float) (k - p) / (n - p + 1));}
		for (int k = n + 1; k <= m; k++){Nodes.push_back(1.0f);}
		
	}

	NURBS_Curve(const NURBS_Curve&) = delete;
	NURBS_Curve& operator=(const NURBS_Curve&) = delete;

	~NURBS_Curve() {
    	delete[] weights;
	}

	int FindInterval(float u){

		//assumes: 0 <= u < 1 !!!

		if (u == 0){return degree;}

		int i = degree;
		int j = n + 1;

		while (j - i > 1){

			int m = (i + j)/2;

			if (u >= Nodes[m]){i = m;}
			else{j = m;}

		}

		return i;
	}

	Vector3 Eval(float u){

		if (u == 1){return CtrlPointsPointers[n] -> getCoords();}

		//find the starting interval
		int z = FindInterval(u);


		float** values = new float*[n + 1];

		for (int i = 0; i < n + 1; i++){			

			values[i] = new float[degree + 1];

			for (int j = 0; j < degree + 1; j++){
				values[i][j] = 0.0f;
			}			
		}

		values[z][0] = 1.0f;

		float l = 0.0f;
		float r = 0.0f;

		float leftDen;
		float rightDen;

		for (int p = 1; p < degree + 1; p++){

			for (int i = std::max(0, z - p); i <= z; i++){

				leftDen = Nodes[i + p] - Nodes[i];
            	rightDen = Nodes[i + p + 1] - Nodes[i + 1];

				if (leftDen != 0.0f){l = (u - Nodes[i]) / leftDen;}
				else{l = 0.0f;}

				if (rightDen != 0.0f){r = (Nodes[i + p + 1] - u) / rightDen;}
				else{r = 0.0f;}		

				values[i][p] = l * values[i][p - 1] + r * values[i + 1][p - 1];
			}
		}
		
		Vector3 ret = {0.0f, 0.0f, 0.0f};
		Vector3 tmp;

		for (int i = std::max(0, z - degree); i <= z; i++){

			tmp = CtrlPointsPointers[i] -> getCoords();

			ret = ret + tmp * values[i][degree];
		}


		for (int i = 0; i < n + 1; i++){delete[] values[i];}
		delete[] values;


		return ret;	

	}


};
