

#include <raylib.h>
#include <iostream>

class ControlPoint3D{

	protected:

		Vector3 coords;
		bool transparent;

	public:

		ControlPoint3D(){coords.x = 0.0f; coords.z = 0.0f; coords.y = 0.0f; transparent = false;}
		ControlPoint3D(float x, float y, float z){coords.x = x; coords.z = y; coords.y = z; transparent = false;}

		void Plot(){
			if (!transparent){
				DrawSphere(coords, 0.05f, (Color) BLACK);
			}
		}

		void MakeInvisible(){transparent = true;}
		bool IsVisible(){return !transparent;}

		void ControlPointFromInput(){

			char comps[] = "xyz";
			float values[3]; 

			for (int k = 0; k < 3; k++){

				std::cout << comps[k] << ": ";
				std::cin >> values[k];
			}

			coords.x = values[0];
			coords.z = values[1];
			coords.y = values[2];
		}



};