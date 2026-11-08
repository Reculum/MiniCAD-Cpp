
#pragma once

#include <raylib.h>
#include <iostream>

class ControlPoint3D{

	protected:

		Vector3 coords;
		bool transparent;
		bool selected;

	public:

		ControlPoint3D(){coords.x = 0.0f; coords.z = 0.0f; coords.y = 0.0f;
			transparent = true;
			selected = false;
		}
		ControlPoint3D(float x, float y, float z){coords.x = x; coords.z = y; coords.y = z;
			transparent = false;
			selected = false;
		}

		Vector3 getCoords(){return coords;}

		void Plot(){
			if (!transparent){

				if (selected){DrawSphere(coords, 0.05f, (Color) BLUE);}
				else{DrawSphere(coords, 0.05f, (Color) BLACK);}
				
			}
		}

		void Select(){selected = true;}
		bool IsSelected(){return selected;}

		void MakeInvisible(){transparent = true;}
		bool IsVisible(){return !transparent;}

		void ControlPointFromInput(){

			char comps[] = "xyz";
			float values[3]; 

			for (int k = 0; k < 3; k++){

				std::cout << comps[k] << ": ";
				std::cin >> values[k];
			}

			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;

			coords.x = values[0];
			coords.z = values[1];
			coords.y = values[2];
			transparent = false;

		}



};