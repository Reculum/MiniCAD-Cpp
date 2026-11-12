
#pragma once

#include <raylib.h>
#include "GraphicsElement.h"
#include <iostream>

class ControlPoint3D : public GraphicsElement{

	protected:

		Vector3 coords;
		int TimesMoved = 0;

	public:

		ControlPoint3D(){coords.x = 0.0f; coords.z = 0.0f; coords.y = 0.0f;
			transparent = true;
			selected = false;
		}
		ControlPoint3D(float x, float z, float y){coords.x = x; coords.z = z; coords.y = y;
			transparent = false;
			selected = false;
		}

		Vector3 getCoords(){return coords;}
		int getTimesMoved(){return TimesMoved;}

		void Move(Vector3 newPos){coords = newPos; TimesMoved += 1;}
		
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

		

		void Select() override {selected = true;}
		void Unselect() override {selected = false;}
		bool IsSelected() override {return selected;}

		void MakeInvisible() override {transparent = true;}
		void MakeVisible() override {transparent = true;}
		bool IsVisible() override {return !transparent;}

		void Plot() override{
			if (!transparent){

				if (selected){DrawSphere(coords, 0.05f, (Color) BLUE);}
				else{DrawSphere(coords, 0.05f, (Color) BLACK);}
				
			}
		}

		



};