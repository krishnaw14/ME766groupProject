#include<iostream>
using namespace std;

const double G = 0.000000000066741;  //True value of .0000000000667408 rounded off to 15 decimal places (precision of double)
const double M1 = 1; //for present analysis
const double M2 = 1; //for present analysis

// returns force along the x-axis on the specified body//
double Fx(double r,double X1, double X2, int body){
	// r is the distance between the two bodies
	double F = (G*(X2-X1))/(r*r*r) ;
	if(body == 1){
		return F;
	}
	else return -F;
}

//returns force along the y-xis on the specified body//
double Fy(double r,double Y1, double Y2, int body){
	// r is the distance between the two bodies
	double F = (G*(Y2-Y1))/(r*r*r) ;
	if(body == 1){
		return F;
	}
	else return -F;
}
// NOTE : Forces also represent accelerations as masses are 1 unit //

//you would like to make four vectors corresponding to position and the velocity of the two bodies//

//creating a function to update X1//

double X_new(double timestep, ){

}

int main(){
	// all the physical properties are in S.I. units
	// 6N equations for N bodies
	// Xj_o, Yj_o, Uj_o, Vj_o are known for j = 1 , 2 

	//* LEAPFROG INTEGRAL *//

}