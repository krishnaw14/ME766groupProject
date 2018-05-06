#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
using namespace std;

const double G = 1; // change later; use symbolically for now
const double a = 1; // change later; use symbolically for now; defines the geometric scale of the problem s

struct Point{
	double x,y;

	Point(){
		//default constructor
	}

	Point(double x_, double y_){
		x = x_;
		y = y_;
	}
};

double distance(Point a, Point b){
	/* returns the distance between the two input points */
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y)) ;
}

struct Quadrant{
	Point origin; // coordinates of the origin of the quadrant
	double dimension; // side length of the quadrant
	
	Quadrant(){
		/* default constructor */
	}

	Quadrant(double x_, double y_, double dimension_){
		/* constructor */
		origin.x = x_;
		origin.y = y_;
		dimension = dimension_;
	}

	bool contains(double x_ , double y_){

		/* checks whether a point is contained in the quadrant */

		bool x_within_bounds = (x_ >= origin.x) && (x_ <= (origin.x + dimension)) ;
		bool y_within_bounds = (y_ >= origin.y) && (y_ <= (origin.y + dimension)) ;

		if(x_within_bounds && y_within_bounds)
		{
			return true;
		}
		else return false;
	}

	double length(){
		return dimension;
	}

	Quadrant* NW(){
		/* creates and returns the north-west sub-quadrant of the invoking quadrant */
		Quadrant* north_west = new Quadrant;
		north_west->origin.x = origin.x ;
		north_west->origin.y = origin.y + dimension/2.0 ;
		north_west->dimension = dimension/2.0 ;
		return north_west;
	}

	Quadrant* NE(){
		/* creates and returns the north-east sub-quadrant of the invoking quadrant */
		Quadrant* north_east = new Quadrant;
		north_east->origin.x = origin.x + dimension/2.0 ;
		north_east->origin.y = origin.y + dimension/2.0 ;
		north_east->dimension = dimension/2.0 ;
		return north_east;
	}

	Quadrant* SW(){
		/* creates and returns the south-west sub-quadrant of the invoking quadrant */
		Quadrant* south_west = new Quadrant;
		south_west->origin.x = origin.x ;
		south_west->origin.y = origin.y ;		
		south_west->dimension = dimension/2.0 ;
		return south_west;
	}

	Quadrant* SE(){
		/* creates and returns the south-east sub-quadrant of the invoking quadrant */
		Quadrant* south_east = new Quadrant;
		south_east->origin.x = origin.x + dimension/2.0 ;
		south_east->origin.y = origin.y;
		south_east->dimension = dimension/2.0 ;
		return south_east;
	}

	int GetQuadrant(Point p){
		/* A method which tells in which sub-quadrant of the invoking quadrant does the point lie; 
		return 1 for NW, 2 for NE, 3 for SW, 4 for SE */

		if(p.x >= origin.x + dimension/2.0){
			//either in sub-quads 2 or 4
			if(p.y >= origin.y + dimension/2.0){
				return 2;
			}
			else return 4;;
		}
		else{
			if(p.y >= origin.y + dimension/2.0){
				return 1;
			}
			else return 3;
		}
	}

};

struct Body{
	/* represents both internal and external nodes of the Barnes-Hut Tree i.e. both a single body or a group of bodies */
	double mass; // mass of the body or COM of a group of bodies
	Point coordinates; //coordinates of the body or COM of a group of bodies

	std::vector<double> velocity;
	std::vector<double> acceleration;

	Body(){
		mass = 0;
		coordinates.x = 0;
		coordinates.y = 0;
		velocity[0] = 0;
		velocity[1] = 0;
		acceleration[0] = 0;
		acceleration[1] = 0;
	}

	Body(double m, double x_, double y_, double vx, double vy){
		mass = m;
		coordinates.x = x_;
		coordinates.y = y_;

		velocity.push_back(vx);
		velocity.push_back(vy);

		acceleration.push_back(0);
		acceleration.push_back(0);
	}

	bool in(Quadrant q){
		/* checks whether the body is in a quadrant or not */
		return q.contains(coordinates.x, coordinates.y);
	}

	void add(Body a){
		/* returns a new body with the same parameters as that of the center of mass of bodies A and B */
		mass = a.mass + mass;
		coordinates.x = (a.mass*(a.coordinates.x) + mass*(coordinates.x))/mass ;
		coordinates.y = (a.mass*(a.coordinates.y) + mass*(coordinates.y))/mass ;

		velocity[0] = (a.mass*(a.velocity[0]) + mass*(velocity[0]))/mass ;
		velocity[1] = (a.mass*(a.velocity[1]) + mass*(velocity[1]))/mass ;

		acceleration[0] = (a.mass*(a.acceleration[0]) + mass*(acceleration[0]))/mass ;
		acceleration[1] = (a.mass*(a.acceleration[1]) + mass*(acceleration[1]))/mass ;

		return ; 
	}

	/*void positionUpdate(){

	}*/
};

struct BHTreeNode{
	Body body;
	Quadrant Region;
	BHTreeNode* NW ; // four sub-trees/sub-regions; children nodes
	BHTreeNode* NE ;
	BHTreeNode* SW ;
	BHTreeNode* SE ;
	int NumberOfBodies; 

	BHTreeNode(){
		/* default constructor */
		NumberOfBodies = 0;
		NW->NumberOfBodies = 0;
		NE->NumberOfBodies = 0;
		SW->NumberOfBodies = 0;
		SE->NumberOfBodies = 0;
		
		NW->Region = *( Region.NW() );
		NE->Region = *( Region.NE() );
		SW->Region = *( Region.SW() );
		SE->Region = *( Region.SE() );
	}

	BHTreeNode(Quadrant Q){
		/* constructor to create a Barnes-Hut Tree with no bodies and the given quadrant */
		Region = Q;
		/* initialising */
		NumberOfBodies = 0;
		NW->Region = *( Region.NW() );
		NE->Region = *( Region.NE() );
		SW->Region = *( Region.SW() );
		SE->Region = *( Region.SE() );

		// 'body' member variable is already initialised because of Body's default constructor
	}

	void insert(Body b){ 
	/*  ALGORITHM
		1. If node x does not contain a body, put the new body b here.

		2. If node x is an internal node, update the center-of-mass and total mass of x. Recursively insert the body b in the appropriate quadrant.

		3. If node x is an external node, say containing a body named c, then there are two bodies b and c in the same region. 
		   Subdivide the region further by creating four children. Then, recursively insert both b and c into the appropriate quadrant(s). 
		   Since b and c may still end up in the same quadrant, there may be several subdivisions during a single insertion.
		   Finally, update the center-of-mass and total mass of x.
	*/
		
		if(NumberOfBodies == 0){ // EMPTY NODE
			body = b;
			NumberOfBodies =  1 ;
			return; 
		}
		else if (NumberOfBodies > 1){ // INTERNAL NODE
			
			/* updating the mass and the position of COM for the invoking node/parent node */
			body.add(b); /* Centre of Mass information updated */
			NumberOfBodies++ ;
			
			/* checking in which subquadrant of the invoking quadrant does the body lie */
			int SubQuad = Region.GetQuadrant(b.coordinates); 

			/* inserting body in the child nodes */
			if(SubQuad == 1){
				return NW->insert(b);
			}
			else if(SubQuad == 2){
				return NE->insert(b);
			}
			else if(SubQuad == 3){
				return SW->insert(b);
			}
			else{
				return SE->insert(b);
			}
		}
		else if (NumberOfBodies == 1){ // EXTERNAL NODE
			/* the single body also has to be relocated into a new sub-quadrant */

			int SubQuad = Region.GetQuadrant(body.coordinates);
			if(SubQuad == 1){
				NW->insert(body);
			}
			else if(SubQuad == 2){
				NE->insert(body);
			}
			else if(SubQuad == 3){
				SW->insert(body);
			}
			else{
				SE->insert(body);
			}

			int SubQuad_ = Region.GetQuadrant(b.coordinates);
			if(SubQuad_ == 1){
				NW->insert(body);
			}
			else if(SubQuad_ == 2){
				NE->insert(body);
			}
			else if(SubQuad_ == 3){
				SW->insert(body);
			}
			else{
				SE->insert(body);
			}

			NumberOfBodies++ ;
			body.add(b); /* updating the COM parameters and mass of the invoking node */
			return;
		}
	}
 
	void updateForce(Body b){
		/* ALGORITHM
		1. If the current node is an external node (and it is not body b), calculate the force exerted by the current node on b, and add this amount to b’s net force.
		2. Otherwise, calculate the ratio s/d. If s/d < θ, treat this internal node as a single body, and calculate the force it exerts on body b, 
		   and add this amount to b’s net force.
		3. Otherwise, run the procedure recursively on each of the current node’s children.
		*/

		if (NumberOfBodies == 0) return; // EMPTY NODE

		else if (NumberOfBodies == 1){ // EXTERNAL NODE
			double distance_ = distance(b.coordinates, body.coordinates);
			b.acceleration[0] +=  G*(body.mass)*(body.coordinates.x - b.coordinates.x)/(distance_*distance_*distance_);
			b.acceleration[1] +=  G*(body.mass)*(body.coordinates.y - b.coordinates.y)/(distance_*distance_*distance_);
			return;
		}

		else if (NumberOfBodies > 1){ // INTERNAL NODE
			
			if( (Region.dimension)/(distance(b.coordinates, body.coordinates) ) < 0.5 ){
				/* update force due to the body at the node */
				double distance_ = distance(b.coordinates, body.coordinates);
				b.acceleration[0] +=  G*(body.mass)*(body.coordinates.x - b.coordinates.x)/(distance_*distance_*distance_);
				b.acceleration[1] +=  G*(body.mass)*(body.coordinates.y - b.coordinates.y)/(distance_*distance_*distance_);
				return;
			}
			else{
				// recursing on children
				NW->updateForce(b);
				NE->updateForce(b);
				SW->updateForce(b);
				SE->updateForce(b);
			}
		}

	}

	/* WRITING A DESTRUCTOR FOR THE BHTreeNode DATATYPE */
	~BHTreeNode(){
		delete NW;
		delete NE;
		delete SW;
		delete SE;
	}
};

int main(){
	/*
 	For each step of the simulation, create a new Barnes-Hut tree from scratch, and insert all of the bodies.
 	Since the Barnes-Hut tree represents a finite region in the plane, only insert those bodies that are inside the boundaries. 
 	After inserting all of the bodies, reset the net forces acting on each body and call updateForce for each body to re-calculate them. 
 	Then, update the positions of the bodies and plot them using Turtle graphics.
	*/
	double timestep=0.01;
	double duration=0.01;

	int numberOfIterations = duration/timestep; // number of iterations
	int n = 128;
	cout<<numberOfIterations<<endl;;

	std::vector<Body> Bodies; 
	//initialise the bodies
	
	Body body1(0.0078125,-0.561583,-0.0653953,-0.106656,-0.119243);
	Body body2(0.0078125,4.85715,-0.32989,0.388497,0.20792);

	Bodies.push_back(body1);
	Bodies.push_back(body2);
	/*ifstream dataset;
	double m,x,y,z, vx,vy,vz;
	
	dataset.open("tab128.txt");

	while(dataset)
	{
		dataset >> m >> x >> y >> z >> vx >> vy >> vz ;
		Body newBody(m, x, y, vx, vy);
		Bodies.push_back(newBody);
	} */

	for(int j=0; j<2; j++){
		cout<<Bodies[j].mass<<" "<<Bodies[j].coordinates.x<<" "<<Bodies[j].coordinates.y<<endl;
	}

	for(int i=0; i<numberOfIterations; i++){
		BHTreeNode tree;
		for(int j=0; j<2; j++){
			tree.insert(Bodies[j]);
		}

		for(int j=0; j<2; j++){
			tree.updateForce(Bodies[j]);
		}
		// now Bodies[i].acceleration[] is a populated vector

		//writing code to update positions
		for(int j=0; j<2; j++){
			Bodies[j].coordinates.x = Bodies[j].coordinates.x + timestep*Bodies[j].velocity[0] + (1/2)*timestep*timestep*Bodies[j].acceleration[0];
			Bodies[j].coordinates.y = Bodies[j].coordinates.y + timestep*Bodies[j].velocity[1] + (1/2)*timestep*timestep*Bodies[j].acceleration[1];

			Bodies[j].velocity[0] = Bodies[j].velocity[0] + timestep*Bodies[j].acceleration[0];
			Bodies[j].velocity[1] = Bodies[j].velocity[1] + timestep*Bodies[j].acceleration[1];
		}
		//delete tree;
	}
} 

/* 																			DEVELOPER NOTES 
1. COMPUTE BOUNDING BOX AROUND ALL THE BODIES
2. IN updateForce() FUNCTION; 0/0 FORM MIGHT BE ENCOUNTERED; IF THE COM OF A GROUP LIES AT THE BODY'S coordinates ; ALSO ONE BODY CANNOT EXERT A FORCE ON ITSELF 
*/