//floor.cpp floor class, subclass of object


#include "floor.h"

Floor::Floor(GLfloat depth, GLfloat width, GLuint text)
: object(text) {

	name = "floor" ;
	
	normal = vec3(0.0f, 1.0f, 0.0f) ;
	
	points.push_back(vec4(0.0, 0, 0.0,1)) ;
	points.push_back(vec4(width, 0, 0,1)) ;
	points.push_back(vec4(width, 0, depth,1)) ;
	points.push_back(vec4(0, 0, depth,1)) ;

	createBoundingBox() ;
};

