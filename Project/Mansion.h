//mansion.h mansion class
//Jesse Hoyt
//created 12/15/12 9:00pm

#include <vector>
#include <string>

#include "room.h"


#ifndef _MANSION_H
#define _MANSION_H

class mansion{

public:

	std::string name ;
	std::vector<room> rooms ;

	mansion(GLfloat depth, GLfloat width, GLfloat height, GLuint wallText, GLuint floorText, GLuint groundText) ;

	void draw() ;
	void transform(mat4 transformMatrix) ;


};

#endif
