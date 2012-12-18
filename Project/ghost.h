//ghost.h ghost class


#include "object.h"

#ifndef _GHOST_H
#define _GHOST_H

class ghost : public object{

public:
	ghost(GLfloat height, GLfloat width, GLuint text) ;
	void draw() ;

};

#endif
