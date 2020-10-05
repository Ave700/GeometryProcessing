#pragma once
#ifndef BASICMESHDATASTRUCTURES_H
#define BASICMESHDATASTRUCTURES_H


#include <memory>
#include "3d_math_qt.h"
//#include <glm\glm\vec2.hpp>
//#include <glm\glm\vec3.hpp>
#include <vector>
//Forward declares to allow circularity
struct Edge;
struct Face;

struct Vertex
{
	
	QVector3D m_pos;
	QVector2D m_tex;
	QVector3D m_normal;

	Vertex() {}

	Vertex(const QVector3D pos, const QVector2D tex, const QVector3D normal) {
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
	std::vector<int> relatedVertIndices; //to store other verts connected by an existing edge
	std::vector<int> relatedEdgeIndices;
	std::vector<int> relatedFaceIndices;
	int thisIndex; //Having it's index stored allows for some loops that otherwise wouldnt be as easy
	//std::vector<int> relatedIndices;
	//std::vector<Edge *> v_Edges;
	//std::vector<Face *> v_Faces;
};

//Holds indices to related vertex and pointers to related faces
struct Edge 
{
	Edge() {
		
		
	}
	Edge(int vertA, int vertB) {
		relatedVertIndices.push_back(vertA);
		relatedVertIndices.push_back(vertB);
	}
	Edge(Vertex *vertA, Vertex *vertB) {
		e_Vertices.push_back(vertA);
		e_Vertices.push_back(vertB);
		
	}
	//int thisIndex;

	std::vector<int> relatedVertIndices;
	std::vector<int> relatedFaceIndices;
	//std::vector<int> relatedIndices;
	//0 is start 1 is end. 
	std::vector<Vertex *> e_Vertices;

	//Since mesh isn't currently volumetric only 2 faces will ever be on an edge
	std::vector<Face *> e_Faces;
};

struct Face
{

	Face() {
		//f_Vertices.clear();
		//f_Edges.clear();
		//f_Normal = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	//Using 3 for triangle mesh
	std::vector<int> relatedVertIndices;
	std::vector<int> relatedEdgeIndices;


	//std::vector<Vertex *> f_Vertices;
	//std::vector<Edge *> f_Edges;
	QVector3D f_CenterPos;
	QVector3D f_Normal;

	//void FindNormal();
	QVector3D CurNormal(QMatrix4x4 WVP);

};


bool doesEdgeExist(Vertex* vertA, Vertex* vertB);
Edge& findCommonEdge(Vertex* vertA, Vertex* vertB);
#endif // !BASICMESHDATASTRUCTURES_H