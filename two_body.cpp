#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<fstream> //enables export to csv files
//#include"twobodystruct.h"

using namespace std;

const double G = 0.00066741;  //True value of .0000000000667408 rounded off to 15 decimal places (precision of double)
const long long int M1 = 1000; //for present analysis
const long long int M2 = 2000; //for present analysis
const double timestep = 0.005; // small time step in seconds

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
		a[0] = G*(body.M)*(body.r[0] - r[0])/(r_*r_*r_);
		a[1] = G*(body.M)*(body.r[1] - r[1])/(r_*r_*r_);
		return;
	}

	void r_update(){
		r[0] = r[0] + v[0]*timestep + (1.0/2)*a[0]*timestep*timestep ;
		r[1] = r[1] + v[1]*timestep + (1.0/2)*a[1]*timestep*timestep ;
	}
	void v_update(Body body){
		v[0] = v[0] + (1.0/2)*a[0]*timestep;
		v[1] = v[1] + (1.0/2)*a[1]*timestep;

		(*this).r_update();
		(*this).acceleration_update(body); // now, a_present = a_(i+1)
		v[0] = v[0] + (1.0/2)*a[0]*timestep;
		v[1] = v[1] + (1.0/2)*a[1]*timestep;
	}

	void print_r()
	{
		cout<<"x = "<<r[0]<<" y = "<<r[1]<<endl;
	}
};

int main(){
	// all the physical properties are in S.I. units
	// 6N equations for N bodies
	// Xj_o, Yj_o, Uj_o, Vj_o are known for j = 1 , 2 

	// LEAPFROG INTEGRAL //
	Body body1(M1,0,0,0,10);
	Body body2(M2,1,0,0,10);
	
	body1.acceleration_update(body2);
	body2.acceleration_update(body1); 
	//sets up initial accelerations of the bodies

	double duration=50; //in seconds
	//cin>>duration;

	int h = duration/timestep; // h =number of iterations
	double trajectory_body_x[2*h]; //first h terms are X coordinate of body 1 and the rest of the terms are for body 2
	double trajectory_body_y[2*h]; //first h terms are Y coordinate of body 1 and the rest of the terms are for body 2
	
	trajectory_body_x[0] = body1.r[0];
	trajectory_body_x[h] = body2.r[0];
	trajectory_body_y[0] = body1.r[1];
	trajectory_body_y[h] = body2.r[1]; 

	body1.print_r();
	body2.print_r();

	for(int i=1; i<h; i++){
		body1.r_update();
		body1.v_update(body2);

		body2.r_update();
		body2.v_update(body1);

		//storing the trajectory in an array//

		trajectory_body_x[i] = body1.r[0];
		trajectory_body_x[i+h] = body2.r[0];
		trajectory_body_y[i] = body1.r[1];
		trajectory_body_y[i+h] = body2.r[1];

		cout<<"body 1 "; body1.print_r();
		cout<<"body 2 ";body2.print_r();
	}

	/*  ofstream myfile;
    	myfile.open ("body1sim.csv");
    	for(int i=0; i<h; i++){
    		myfile << trajectory_body_x[i]<<trajectory_body_y[i]<<"\n" ;
    	}
    	myfile.close();

    ofstream myfile_;
    	myfile.open ("body2sim.csv");
    	for(int i=h; i<2*h; i++){
    		myfile_ << trajectory_body_x[i]<<trajectory_body_y[i]<<"\n";
    	}
    	myfile_.close(); */
      

	/*for (int i = 0; i < h; i++)
	{
		cout<<trajectory_body_x[i]<<" "<<trajectory_body_y[i]<<endl;
	}
	cout<<endl;
	cout<<"For Body 2:"<<endl;
	for (int i = 0; i < h; i++)
	{
		cout<<trajectory_body_x[i+h]<<" "<<trajectory_body_y[i+h]<<endl;
	}*/
}
