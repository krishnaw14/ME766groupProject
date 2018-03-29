#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string> 
#include <fstream> //enables export to csv files
//#include"twobodystruct.h"

using namespace std;

const double G = 0.00066741;  //True value of .0000000000667408 rounded off to 15 decimal places (precision of double)
const long long int M1 = 1000; //for present analysis
const long long int M2 = 2000; //for present analysis
const double timestep = 0.05; // small time step in seconds

struct Body
{ 
	double M;
	double r[2], v[2], a[2]; // defines acceleration of the body in the x and y directions; a[0] = a_x, a[1] = a_y; and likewise for r and v
	
	Body()
	{
		M=1000; 
		r[0]=0; r[1]=0;
		v[0]=0; v[1]=0;
	}
	Body(double mass, double x, double y, double vel_x, double vel_y){ //constructor
		M = mass;
		r[0] = x;
		r[1] = y;
		v[0] = vel_x;
		v[1] = vel_y;
	}
	
	void acceleration_update( vector<Body> body_array, int n)
	{
		for (int i=0;i<n;i++)
		{
			Body body=body_array[i];
			double r_ = sqrt((body.r[0] - r[0])*(body.r[0] -r[0]) + (body.r[1] - r[1])*(body.r[1] - r[1]));
			if(r_==0)
				continue;
			a[0] += G*(body.M)*(body.r[0] - r[0])/(r_*r_*r_);
			a[1] += G*(body.M)*(body.r[1] - r[1])/(r_*r_*r_);

		}

	}

	void r_update()
	{
		r[0] = r[0] + v[0]*timestep + (1.0/2)*a[0]*timestep*timestep ;
		r[1] = r[1] + v[1]*timestep + (1.0/2)*a[1]*timestep*timestep ;
	}

	void v_update(vector<Body> body_array, int n){
		v[0] = v[0] + (1.0/2)*a[0]*timestep;
		v[1] = v[1] + (1.0/2)*a[1]*timestep;

		(*this).r_update();
		(*this).acceleration_update(body_array, n); // now, a_present = a_(i+1)
		v[0] = v[0] + (1.0/2)*a[0]*timestep;
		v[1] = v[1] + (1.0/2)*a[1]*timestep;
	}

	void print_r()
	{
		cout<<"x = "<<r[0]<<" y = "<<r[1]<<endl;
	}

=======
	/*
	void operator()(double mass, double x, double y, double vel_x, double vel_y){
		M = mass;
		r[0] = x;
		r[1] = y;
		v[0] = vel_x;
		v[1] = vel_y;
		return;
	}*/
};

int main()
{
	int n=5;
	vector<Body> body_array;
	//Body body_array[n]; //Array of bodies. Aim is to determine the trajectories of each body stored in body_array

	//Initialize position, velocity and mass of each body

	Body body1(1*M1, 0, 0, 100, 0);
	Body body2(2*M1,5, 0, -100, 0);
	Body body3(3*M1, 10, 0, 200, 50);
	Body body4(4*M1, 15, 0, 300, -350);
	Body body5(5*M1, 20, 0, 400, 800);

	body_array.push_back(body1);
	body_array.push_back(body2);
	body_array.push_back(body3);
	body_array.push_back(body4);
	body_array.push_back(body5);

	for (int i=0;i<n;i++)
		body_array[i].acceleration_update(body_array, n); //sets up initial accelerations of the bodies

	double duration=10; //in seconds
	//cin>>duration;

	int h = duration/timestep; // h =number of iterations

	for (int i=0;i<n;i++)
	{
		cout<<"Body"+to_string(i);
		body_array[i].print_r();
	}

	for(int i=1; i<h; i++)
	{

		for(int j=0;j<n;j++)
		{
			body_array[j].r_update();
			body_array[j].v_update(body_array, n);

			cout<<"Body "+to_string(j);
		    body_array[j].print_r();
		}

	}

}
