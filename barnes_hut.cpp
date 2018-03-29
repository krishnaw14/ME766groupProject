#include<iostream>
#include<math.h>
#include<vector>
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

	int which_subquad_contain(Point p){
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

	/* DEVELOPER NOTES: Define a constructor for the body data-type to initialise the velocity and acceleration vectors; maybe include this in the default constructor */

	Body(){
		/* default constructor to initialise velocity and acceleration vectors among other physical parameters */
		mass = 0;
		coordinates.x = 0;
		coordinates.y = 0;
		velocity[0] = 0;
		velocity[1] = 0;
		acceleration[0] = 0;
		acceleration[1] = 0;
		// now we can use the updateForce() member function as initialisation is done 
	}

	bool in(Quadrant q){
		/* checks whether the body is in a quadrant or not */
		return q.contains(coordinates.x, coordinates.y);
	}

	Body* add(Body a, Body b){
		/* returns a new body with the same parameters as that of the center of mass of bodies A and B */
		Body* newbody = new Body;
		newbody->mass = a.mass + b.mass;
		(newbody->coordinates).x = (a.mass*(a.coordinates.x) + b.mass*(b.coordinates.x))/(newbody->mass) ;
		(newbody->coordinates).y = (a.mass*(a.coordinates.y) + b.mass*(b.coordinates.y))/(newbody->mass) ;

		newbody->velocity[0] = (a.mass*(a.velocity[0]) + b.mass*(b.velocity[0]))/(newbody->mass) ;
		newbody->velocity[1] = (a.mass*(a.velocity[1]) + b.mass*(b.velocity[1]))/(newbody->mass) ;

		newbody->acceleration[0] = (a.mass*(a.acceleration[0]) + b.mass*(b.acceleration[0]))/(newbody->mass) ;
		newbody->acceleration[1] = (a.mass*(a.acceleration[1]) + b.mass*(b.acceleration[1]))/(newbody->mass) ;

		return newbody; 
	}
};

struct BHTree_node{
	/* A Barnes-Hut Tree contains contains some data, a body and a quadrant */
	Body body;
	Quadrant region;
	BHTree_node* NW ; // pointers to the four sub-trees/sub-regions; children nodes
	BHTree_node* NE ;
	BHTree_node* SW ;
	BHTree_node* SE ;
	int number_of_bodies; 

	BHTree_node(){
		/* default constructor */
	}

	BHTree_node(Quadrant q){
		/* constructor to create a Barnes-Hut Tree with no bodies and the given quadrant */
		// might need to do operator overloading = (assignment operator) for the Quadrant data-type
		region = q;
		/* initialising */
		body.mass = 0; 
		body.coordinates.x = 0;
		body.coordinates.y = 0; 

		body.velocity[0] = 0; 
		body.velocity[1] = 0; 

		body.acceleration[0] = 0; 
		body.acceleration[1] = 0; 

		number_of_bodies = 0;

		NW = NE = SW = SE = NULL;
	}

	void insert(BHTree_node* Tree,Body b){ 
		if(Tree == NULL){ // for an empty node
			/* DEVELOPER NOTE: What if the Tree pointer points to a tree with zero bodies; Tree != NULL but the same task should be performed */
			Quadrant q(0,0,a);
			/* a defines the geometric scale of the problem, is a constant and is defined along with G at the start */
			Tree = new BHTree_node(q);
			Tree->body = b;
			Tree->number_of_bodies =  1 ;
			Tree->NW = Tree->NE = Tree->SW = Tree->SE = NULL; //maybe need to delete; will be already done by the constructor
			
			//Pending: initialise the quadrant info

			/* initialise the quadrant info using the constructor BHTree_node(Quadrant q) */

			/* DEVELOPER NOTES : OH, GOT IT; THE MEMBER FUNCTIONS NW, NE ,SW ,SE OF THE STRUCT QUADRANT WILL NOW GET USED */

			return; 
		}
		else{
			// updating the mass and the position of COM for the invoking node/parent node
			
			Tree->body.coordinates.x = ( (Tree->body.mass)*(Tree->body.coordinates.x) + (b.mass)*(b.coordinates.x) )/(b.mass +Tree->body.mass);
			Tree->body.coordinates.y = ( (Tree->body.mass)*(Tree->body.coordinates.y) + (b.mass)*(b.coordinates.y) )/(b.mass + Tree->body.mass);
			Tree->body.mass =  Tree->body.mass + b.mass;
			Tree->number_of_bodies = Tree->number_of_bodies + 1 ;

			/* DEVELOPER NOTES: No need to initialise velocity and acceleration vectors for any node except the external nodes; memory and time inefficient */
			Tree->body.velocity[0] = ( (Tree->body.mass)*(Tree->body.velocity[0]) + (b.mass)*(b.velocity[0]) )/(b.mass +Tree->body.mass);
			Tree->body.velocity[1] = ( (Tree->body.mass)*(Tree->body.velocity[1]) + (b.mass)*(b.velocity[1]) )/(b.mass +Tree->body.mass);

			Tree->body.acceleration[0] = ( (Tree->body.mass)*(Tree->body.acceleration[0]) + (b.mass)*(b.acceleration[0]) )/(b.mass +Tree->body.mass);
			Tree->body.acceleration[1] = ( (Tree->body.mass)*(Tree->body.acceleration[1]) + (b.mass)*(b.acceleration[1]) )/(b.mass +Tree->body.mass);

			/* checking in which subquadrant of the invoking quadrant does the body lie */
			int subquad = Tree->region.which_subquad_contain(b.coordinates); 

			/* inserting body in the child nodes */

			if(subquad == 1){
				return insert(Tree->NW, b);
			}
			else if(subquad == 2){
				return insert(Tree->NE, b);
			}
			else if(subquad == 3){
				return insert(Tree->SW, b);
			}
			else{
				return insert(Tree->SE, b);
			}
		}
	}


		/* DEVELOPER NOTE: Modify the default constructor such that while initialising the body data type; accelerations are equal to zero; 
			because we are using += in the updateForce() member function */


	void updateForce(BHTree_node* Tree,Body b){
		/* 
		Visit each node starting from the root node
		
		Check if it is an internal node or external node
		
		If internal node and s/d < tolerance value (where s is the dimension of the region representing the node), compute
		force exerted by this body (stationed at the COM of the group of bodies) on the input body
		
		If s/d > tolerance value; recurse for the children nodes
		
		If external node is reached; compute the force acting on the input body due to the body represented by this node 
		*/

		if (Tree->number_of_bodies > 1){
			/* for internal node */
			if( (Tree->region.dimension)/(distance(b.coordinates, Tree->body.coordinates)) < 0.5 ){
				/* update force due to the body at the node */
				double distance_ = distance(b.coordinates, Tree->body.coordinates);
				b.acceleration[0] +=  G*(Tree->body.mass)*(Tree->body.coordinates.x - b.coordinates.x)/(distance_*distance_*distance_);
				b.acceleration[1] +=  G*(Tree->body.mass)*(Tree->body.coordinates.y - b.coordinates.y)/(distance_*distance_*distance_);
				return;
			}
			else{
				// recursing on children
				updateForce(Tree->NW, b);
				updateForce(Tree->NE, b);
				updateForce(Tree->SW, b);
				updateForce(Tree->SE, b);
			}

		}
		else if (Tree->number_of_bodies = 1){ 
			/* for populated external nodes */
			double distance_ = distance(b.coordinates, Tree->body.coordinates);
			b.acceleration[0] +=  G*(Tree->body.mass)*(Tree->body.coordinates.x - b.coordinates.x)/(distance_*distance_*distance_);
			b.acceleration[1] +=  G*(Tree->body.mass)*(Tree->body.coordinates.y - b.coordinates.y)/(distance_*distance_*distance_);
			return;
		}
	}
};

int main(){
	/*
 	For each step of the simulation, create a new Barnes-Hut tree from scratch, and insert all of the bodies.
 	Since the Barnes-Hut tree represents a finite region in the plane, only insert those bodies that are inside the boundaries. 
 	After inserting all of the bodies, reset the net forces acting on each body and call updateForce for each body to re-calculate them. 
 	Then, update the positions of the bodies and plot them using Turtle graphics.
	*/
}