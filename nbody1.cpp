#include<iostream>
#include<stdlib.h>
#include<math.h>
using namespace std;

const double G = 0.000000000066741;  //True value of .0000000000667408 rounded off to 15 decimal places (precision of double)
const double M1 = 1; //for present analysis
const double M2 = 1; //for present analysis
const double timestep = 0.025; // small time step in seconds

struct Body{ 
	double M;
	double r[2], v[2], a[2]; // defines acceleration of the body in the x and y directions; a[0] = a_x, a[1] = a_y; and likewise for r and v
	
	Body(double mass, double x, double y, double vel_x, double vel_y){ //constructor
		M = mass;
		r[0] = x;
		r[1] = y;
		v[0] = vel_x;
		v[1] = vel_y;
	}
	
	void acceleration_update(Body body){
		double r_ = sqrt((body.r[0] - r[0])*(body.r[0] -r[0]) + (body.r[1] - r[1])*(body.r[1] - r[1]));
		a[0] = G*(body.r[0] - r[0])/(r_*r_*r_);
		a[1] = G*(body.r[1] - r[1])/(r_*r_*r_);
		return;
	}

	void r_update(Body body){
		r[0] = r[0] + v[0]*timestep + (1/2)*a[0]*timestep*timestep ;
		r[1] = r[1] + v[1]*timestep + (1/2)*a[1]*timestep*timestep ;
		return;
	}
	void v_update(Body body){
		v[0] = v[0] + (1/2)*a[0]*timestep;
		v[1] = v[1] + (1/2)*a[1]*timestep;

		(*this).acceleration_update(body); // now, a_present = a_(i+1)
		v[0] = v[0] + (1/2)*a[0]*timestep;
		v[1] = v[1] + (1/2)*a[1]*timestep;
		return;
	}
};
int main(){
	// all the physical properties are in S.I. units
	// 6N equations for N bodies
	// Xj_o, Yj_o, Uj_o, Vj_o are known for j = 1 , 2 

	// LEAPFROG INTEGRAL //
	Body body1(1,0,0,1,1);
	Body body2(1,5,0,-3,4);
	
	body1.acceleration_update(body2);
	body2.acceleration_update(body1); 
	//sets up initial accelerations of the bodies

	double duration; //in seconds
	cin>>duration;

	int h = duration/timestep; // h =number of iterations
	double trajectory_body_x[2*h]; //first h terms are X coordinate of body 1 and the rest of the terms are for body 2
	double trajectory_body_y[2*h]; //first h terms are Y coordinate of body 1 and the rest of the terms are for body 2
	
	trajectory_body_x[0] = body1.r[0];
	trajectory_body_x[h] = body2.r[0];
	trajectory_body_y[0] = body1.r[1];
	trajectory_body_y[h] = body2.r[1]; 

	for(int i=1; i<h; i++){
		body1.r_update(body2);
		body1.v_update(body2);

		body2.r_update(body1);
		body2.v_update(body1);

		//storing the trajectory//

		trajectory_body_x[i] = body1.r[0];
		trajectory_body_x[i+h] = body2.r[0];
		trajectory_body_y[i] = body1.r[1];
		trajectory_body_y[i+h] = body2.r[1];
	}

	for (int i = 0; i < h; i++)
	{
		cout<<trajectory_body_x[i]<<" "<<trajectory_body_y[i]<<endl;
	}
	cout<<endl;
	cout<<"For Body 2:"<<endl;
	for (int i = 0; i < h; i++)
	{
		cout<<trajectory_body_x[i+h]<<" "<<trajectory_body_y[i+h]<<endl;
	}
}
