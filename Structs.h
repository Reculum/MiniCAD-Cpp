
#include<array>

class Vector3{

	protected:
		
		std::array<double, 3> comps;

	public:

		Vector3(){comps[0] = 0.0f; comps[1] = 0.0f; comps[2] = 0.0f;}
		Vector3(double x, double y, double z){comps[0] = x; comps[1] = y; comps[2] = z;}

		double x(){return comps[0];}
		double y(){return comps[1];}
		double z(){return comps[2];}

		Vector3 operator+(Vector3& w){
			return Vector3(comps[0] + w.x(), comps[1] + w.y(), comps[2] + w.z());
		}

		Vector3 operator-(Vector3& w){
			return Vector3(comps[0] - w.x(), comps[1] - w.y(), comps[2] - w.z());
		}

		Vector3 operator*(double c){
			return Vector3(comps[0] * c, comps[1] * c, comps[2] * c);
		}

};