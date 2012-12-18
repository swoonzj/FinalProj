//wall.cpp wall class, subclass of object
//Jesse Hoyt
//created 12/15/12 1:00pm

#include "wall.h"


wall::wall(GLfloat width, GLfloat height, GLuint text){

	name = "wall" ;
	texture = text ;

	int i ;
	for(i = 0 ; i < 16; ++i){ //split for better texturing
		
		points.push_back(vec4(width * i/16, 0.0, 0.0, 1.0));  // Point 1 (bottom-left)
		points.push_back(vec4(width * (i + 1)/16, 0, 0, 1.0));  // Point 1 (bottom-right)
		points.push_back(vec4(width * (i + 1)/16, height, 0, 1.0));  // Point 1 (top-right)
		points.push_back(vec4(width * i/16, height, 0, 1.0));  // Point 1 (top-left)
		
		normal = vec3(0.0, 0.0, 1.0) ;
		
		createBoundingBox() ;
	}

	
};

