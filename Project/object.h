//object.h object class
//Jesse Hoyt
//last modified 12/15/12 12:04 pm

#include "Angel.h"
#include <vector>
#include <string>

#ifndef _OBJECT_H
#define _OBJECT_H

class object{

public:
std::string name ;
std::vector<vec4> points ;
GLuint texture ;
vec3 normal ;
std::vector<vec4> boundingBox ;

object() ;
object(std::vector<vec4>* pnts, GLuint tex) ;
object(std::vector<vec4>* pnts) ;
object(GLuint tex) ;

void draw() ;
void addPoints(vec3) ;
void addPoints(vec4) ;
void addPoints(std::vector<vec4>) ;
void transform(mat4) ;
void createBoundingBox() ;
GLfloat maxPoint(char) ;
GLfloat minPoint(char) ;

} ;

#endif
