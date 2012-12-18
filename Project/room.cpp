//room.cpp room class
//Jesse Hoyt
//Created 12/15/12 2:20pm

#include "room.h"
#include <iostream>
#include <fstream>

room::room(GLfloat depth, GLfloat width, GLfloat height, GLuint wallText, GLuint floorText)
:theFloor(Floor(depth, width, floorText)),ceiling(Floor(depth, width, floorText)), name("room"), height(height), width(width), depth(depth){

wall *tempWall ;
//theFloor = Floor(depth, width, floorText) ;
walls.push_back(wall(width, height, wallText)) ;//front wall
//ceiling = theFloor(depth, width, floorText) ;
ceiling.transform(Translate(0.0,1.0,0.0)) ;
//right wall

tempWall = new wall(depth, height, wallText) ;
tempWall->transform(RotateY(-90)) ;
tempWall->normal = vec3(1.0, 0.0, 0.0) ;//normal set to point into room
walls.push_back(*tempWall) ;

//left wall
tempWall = new wall(depth, height, wallText) ;
tempWall->transform(Translate(width, 0.0, 0.0) * RotateY(-90)) ;
tempWall->normal = vec3(-1.0, 0.0, 0.0) ;//normal set to point into room
walls.push_back(*tempWall) ;

//back wall
tempWall = new wall(width, height, wallText) ;
tempWall->transform(Translate(0.0, 0.0, depth)) ;
tempWall->normal = vec3(0.0, 0.0, -1.0) ;//normal set to point into room
walls.push_back(*tempWall) ;


};
	

	void room::draw(){

		theFloor.draw() ;
		ceiling.draw() ;
		for(std::vector<wall>::iterator it = walls.begin() ; it != walls.end() ; ++it){

			it->draw() ;

		}

	};

	void room::transform(mat4 transformMatrix){

		theFloor.transform(transformMatrix) ;
		ceiling.transform(transformMatrix) ;
		for(std::vector<wall>::iterator it = walls.begin() ; it != walls.end() ; ++it){

			it->transform(transformMatrix) ;

		}

	};

	bool room::inRoom(vec4 camera){
		std::ofstream fout ;
		fout.open("log.txt", std::ios_base::app) ;
		for(std::vector<wall>::iterator it = walls.begin() ; it < walls.end() ; ++it){
			/*fout << "camera: " << camera.x << " " << camera.y << " " << camera.z << std::endl ;
			fout << "normal: " << it->normal.x << " " << it->normal.y << " " << it->normal.z << std::endl ;
*/
			if( it->normal.z == 1){//must be above it to be in room
				for(std::vector<vec4>::iterator it2 = it->boundingBox.begin() ; it2 < it->boundingBox.end() ; ++it2)
					if (camera.z < it2->z){/*fout << "camera: " << camera.x << " " << camera.y << " " << camera.z<< " not above" << "wall: " << it2->x << " " << it2->y << " " << it2->z << std::endl ;*/ return false ;}//camera is above if z is greater than all z in bounding box
			}

			if( it->normal.z == -1){//must be below it to be in room
				for(std::vector<vec4>::iterator it2 = it->boundingBox.begin() ; it2 < it->boundingBox.end() ; ++it2)
					if (camera.z > it2->z) {/*fout << "not below" << std::endl;*/ return false ;}//camera is below if z is less than all z in bounding box
			}

			if( it->normal.x == 1){//must be left of it to be in room
				for(std::vector<vec4>::iterator it2 = it->boundingBox.begin() ; it2 < it->boundingBox.end() ; ++it2)
					if (camera.x < it2->x) {/*fout << "camera: " << camera.x << " " << camera.y << " " << camera.z<< "not left" << "wall: " << it2->x << " " << it2->y << " " << it2->z << std::endl ;*/ return false ;}//camera is left of it if z is greater than all z in bounding box
			}

			if( it->normal.x == -1){//must be right of it to be in room
				for(std::vector<vec4>::iterator it2 = it->boundingBox.begin() ; it2 < it->boundingBox.end() ; ++it2)
					if (camera.x > it2->x) {/*fout << "not right" << std::endl ;*/ return false ;}//camera is right of it if z is less than all z in bounding box
			}
		}
		return true ;

	};
