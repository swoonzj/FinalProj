//mansion.cpp mansion class
//Jesse Hoyt
//Created 12/15/12 9:00pm

#include "mansion.h"

GLfloat wallspace = 0.01f ;
extern GLuint *texture;

mansion::mansion(GLfloat depth, GLfloat width, GLfloat height, GLuint wallText, GLuint floorText, GLuint groundText)
:name("mansion"){

	Floor *tempFloor = new Floor(10,10,groundText);
	tempFloor->name = "ground" ;

	room *tempRoom = new room(depth/3.0, width*3.0/8.0, height, wallText,floorText) ;//room 1
	tempRoom->name = "room 1" ;
	rooms.push_back(*tempRoom) ;
	
	tempRoom = new room(depth/3.0, width*3.0/8.0, height, wallText,floorText) ;//room 2
	tempRoom->name = "room 2" ;
	tempRoom->transform(Translate(0.0, 0.0, depth*2.0/3.0)) ;//position second room behind first
	rooms.push_back(*tempRoom) ;

	tempRoom = new room(depth, width/4.0, height, wallText,floorText) ;//hallway
	tempRoom->name = "hallway" ;
	tempRoom->transform(Translate(width*3.0/8.0+wallspace, 0.0, 0.0)) ;//position hallway beside rooms, adjust for space between walls
	rooms.push_back(*tempRoom) ;

	tempRoom = new room(depth/3.0, width*3.0/8.0, height, wallText,floorText) ;//room 3
	tempRoom->name = "room 3" ;
	tempRoom->transform(Translate(width*5.0/8.0+2*wallspace, 0.0, 0.0)) ;//position fourth room behind third
	rooms.push_back(*tempRoom) ;
	
	tempRoom = new room(depth/3.0, width*3.0/8.0, height, wallText,floorText) ;//room 4
	tempRoom->name = "room 4" ;
	tempRoom->transform(Translate(width*5.0/8.0+2*wallspace, 0.0, depth*2.0/3.0)) ;//position fourth room behind third
	rooms.push_back(*tempRoom) ;

	transform(Translate(-width/2, 0.0, -1.0)) ;
};

void mansion::draw(){

		
		for(std::vector<room>::iterator it = rooms.begin() ; it != rooms.end() ; ++it){

			it->draw() ;

		}

};

void mansion::transform(mat4 transformMatrix){

		
		for(std::vector<room>::iterator it = rooms.begin() ; it != rooms.end() ; ++it){

			it->transform(transformMatrix) ;

		}

};
