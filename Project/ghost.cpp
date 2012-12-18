//ghost.cpp ghost class

#include "ghost.h"

extern void beginBillboard();
extern void endBillboard();

ghost::ghost(GLfloat height, GLfloat width, GLuint text){


	name = "ghost" ;
	texture = text ;

	
		
	points.push_back(vec4(-width/2, 0.0, 0.0, 1.0));  // Point 1 (bottom-left)
	points.push_back(vec4(width/2 , 0, 0, 1.0));  // Point 1 (bottom-right)
	points.push_back(vec4(width/2, height, 0, 1.0));  // Point 1 (top-right)
	points.push_back(vec4(-width/2 , height, 0, 1.0));  // Point 1 (top-left)
		
	normal = vec3(0.0, 0.0, -1.0) ;
		
	createBoundingBox() ;

};

void ghost::draw(){

	if(texture != -1) glBindTexture(GL_TEXTURE_2D, texture) ;
	
	int i = 0 ;
	int size = points.size() ;
	while( i < size ){
			beginBillboard();
			glEnable(GL_BLEND) ; // enable blending and make ghost transparent
			if(texture != -1) glBindTexture(GL_TEXTURE_2D, texture) ;
			glColor4f(1.0, 1.0, 1.0, 0.5) ;
			glBegin(GL_QUADS) ;	
				glNormal3f(normal[0],normal[1],normal[2]);
				if(texture != -1) glTexCoord2f(0.0f, 0.0f); 
				glVertex3f(points[i][0],points[i][1],points[i][2]);// Point 1 (Front-left)
				++i ;
				if(texture != -1) glTexCoord2f(1.0f, 0.0f); 
				glVertex3f(points[i][0],points[i][1],points[i][2]);  // Point 1 (Front-right)
				++i ;
				if(texture != -1) glTexCoord2f(1.0f, 1.0f); 
				glVertex3f(points[i][0],points[i][1],points[i][2]);  // Point 1 (Back-right)
				++i ;
				if(texture != -1) glTexCoord2f(0.0f, 1.0f); 
				glVertex3f(points[i][0],points[i][1],points[i][2]);  // Point 1 (Back-left)
				++i ;
			glEnd();
			glDisable(GL_BLEND) ;
			glColor4f(1.0, 1.0, 1.0, 1.0) ;
			endBillboard();
			
		
	}




}
