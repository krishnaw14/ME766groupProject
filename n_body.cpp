#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string> 
#include <fstream> 
#include <ctime>

using namespace std;

const double G = 0.00066741;  //True value of .0000000000667408 rounded off to 15 decimal places (precision of double)
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
	Body(double mass, double x, double y, double vel_x, double vel_y)
	{ //constructor
		M = mass;
		r[0] = x;
		r[1] = y;
		v[0] = vel_x;
		v[1] = vel_y;
	}
	
	void acceleration_update( vector<Body> &body_array, int n)
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

	void v_update(vector<Body> &body_array, int n){
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

};

int main(int argc, char ** argv)
{
	int n=128;
	vector<Body> body_array;

	ifstream dataset;
	double m,x,y,z, vx,vy,vz;

	dataset.open("tab128.txt");

	while(dataset)
	{
		dataset >> m >> x >> y >> z >> vx >> vy >> vz ;
		Body newBody(m, x, y, vx, vy);
		body_array.push_back(newBody);
	}

	for (int i=0;i<n;i++)
		body_array[i].acceleration_update(body_array, n); //sets up initial accelerations of the bodies

	double duration=10; //in seconds
	//cin>>duration;

	int h = duration/timestep; // h =number of iterations

	clock_t time1, time2;
	double total_time;
	time1=clock();

	/*for (int i=0;i<n;i++)
	{
		cout<<"Body"+to_string(i);
		body_array[i].print_r();
	}*/

	for(int i=1; i<=h; i++)
	{

		for(int j=0;j<n;j++)
		{
			body_array[j].r_update();
			body_array[j].v_update(body_array, n);

			//cout<<"Body "+to_string(j);
		    //body_array[j].print_r();
		}
		//body_array[0].print_r();

	}

	time2 = clock();

	total_time =(double)(time2-time1)/(CLOCKS_PER_SEC);
	cout<<"Total Time = "<<total_time<<endl;
	/*for (int i=0;i<n;i++)
	{
		cout<<"Body"+to_string(i);
		body_array[i].print_r();
	}*/

}
