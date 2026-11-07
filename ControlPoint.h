

#include <raylib.h>
#include <iostream>

class ControlPoint3D{

	protected:

		Vector3 coords;

	public:

		ControlPoint3D(){coords.x = 0.0f; coords.y = 0.0f; coords.z = 0.0f;}
		ControlPoint3D(float x, float y, float z){coords.x = x; coords.y = y; coords.z = z;}

		void Plot(){DrawSphere(coords, 0.05f, (Color) BLACK);}

		void ControlPointFromInput(){

			char comps[] = "xyz";
			float values[3]; 

			for (int k = 0; k < 3; k++){

				std::cout << comps[k] << ": ";
				std::cin >> values[k];
			}

			coords.x = values[0];
			coords.y = values[1];
			coords.z = values[2];
		}



};