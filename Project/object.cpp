//object.cpp object class
//Jesse Hoyt
//last modified 12/15/12 12:04 pm

#include "object.h"
#include <stdlib.h>
#include "Angel.h"
#include <vector>
#include <string>


object::object()
:name("object"), texture(-1), normal(vec3(0.0)) {

	
	points = *new std::vector<vec4> ;
	boundingBox = *new std::vector<vec4> (8) ;
} ;

object::object(std::vector<vec4> *pnts)
:name("object"), texture(-1), points(*pnts), normal(vec3(0.0)){

	boundingBox = *new std::vector<vec4> (8);
	createBoundingBox() ;//points have been updated, must recalculate bbox
} ;

object::object(std::vector<vec4> *pnts, GLuint tex)
:name("object"), texture(tex), points(*pnts), normal(vec3(0.0)){
	boundingBox = *new std::vector<vec4> (8);
	createBoundingBox() ;//points have been updated, must recalculate bbox
} ;

object::object(GLuint tex)
:name("object"), texture(tex), normal(vec3(0.0)){

	points = *new std::vector<vec4> ;
	boundingBox = *new std::vector<vec4> (8);

}

void object::draw(){

	if(texture != -1) glBindTexture(GL_TEXTURE_2D, texture) ;
	
	int i = 0 ;
	int size = points.size() ;
	while( i < size ){
		
			if(texture != -1) glBindTexture(GL_TEXTURE_2D, texture) ;
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
		
	}

} ;

void object::addPoints(vec3 newPoint){

	points.push_back(vec4(newPoint)) ;

	createBoundingBox() ;//points have been updated, must recalculate bbox
};

void object::addPoints(vec4 newPoint){

	points.push_back(newPoint) ;

	createBoundingBox() ;//points have been updated, must recalculate bbox
};

void object::addPoints(std::vector<vec4> newPoints){

	int i ;
	int size = newPoints.size() ;
	for(i = 0 ; i < size ; ++i) points.push_back(newPoints[i]) ;

	createBoundingBox() ;//points have been updated, must recalculate bbox
};

void object::transform(mat4 transformMatrix){

	int i ;
	int size = points.size() ;
	for(i = 0 ; i < size ; ++i) points[i] = transformMatrix * points[i] ;

	createBoundingBox() ;//points have been updated, must recalculate bbox
};

//max(char axis) finds maximum value for given axis
GLfloat object::maxPoint(char axis){

	int i ;
	int size = points.size() ;
	GLfloat max = 0.0 ;
	int ax = (axis == 'x')?0:(axis == 'y')?1:2 ;

	for(i = 0 ; i < size ; ++i){

		if( points[i][ax] > max) max = points[i][ax] ;

	}

	return max ;

};

//min(char axis) finds minimum value for given axis
GLfloat object::minPoint(char axis){

	int i ;
	int size = points.size() ;
	GLfloat min = 1000000.0 ;
	int ax = (axis == 'x')?0:(axis == 'y')?1:2 ;

	for(i = 0 ; i < size ; ++i){

		if( points[i][ax] < min) min = points[i][ax] ;

	}

	return min ;

};

void object::createBoundingBox(){

	GLfloat maxX = maxPoint('x') ;
	GLfloat maxY = maxPoint('y') ;
	GLfloat maxZ = maxPoint('z') ;

	GLfloat minX = minPoint('x') ;
	GLfloat minY = minPoint('y') ;
	GLfloat minZ = minPoint('z') ;


	boundingBox[0] = (vec4(minX, minY, minZ, 1)) ;
	boundingBox[1] = (vec4(minX, minY, maxZ, 1)) ;

	boundingBox[2] = (vec4( minX, maxY, minZ, 1)) ;
	boundingBox[3] = (vec4( minX, maxY, maxZ, 1)) ;

	boundingBox[4] = (vec4(maxX, minY, minZ, 1)) ;
	boundingBox[5] = (vec4(maxX, minY, maxZ, 1)) ;

	boundingBox[6] = (vec4(maxX, maxY, minZ, 1)) ;
	boundingBox[7] = (vec4(maxX, maxY, maxZ, 1)) ;

	

} ;
