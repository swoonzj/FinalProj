//room.h room class
//Jesse Hoyt
//Created 12/15/12 2:20pm

#include <string>
#include <vector>
#include "Angel.h"
#include "floor.h"
#include "wall.h"

#ifndef _ROOM_H
#define _ROOM_H

class room{

public:
	std::string name ;
	Floor theFloor ;
	Floor ceiling ;
	std::vector<wall> walls ;
	
	GLfloat width ;
	GLfloat height ;
	GLfloat depth ;

	room(GLfloat depth, GLfloat width, GLfloat height, GLuint wallText, GLuint floorText) ;
	
	void draw() ;
	void transform(mat4 transformMatrix) ;
	bool inRoom(vec4 cam) ;
};

#endif
