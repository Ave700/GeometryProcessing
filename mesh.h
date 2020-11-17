#pragma once
#ifndef MESH_H
#define MESH_H
#include "globals.h"
#include <map>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QVector3D>
#include "BasicMeshDataStructures.h"
#include "texture.h"
#include "utilities.h"

#define TARGETSCALE .75f


class Mesh
{
public:
	Mesh();

	~Mesh();

	bool LoadMesh(const std::string& Filename);
	
	void Render();
    void RenderWireFrame();
	void DrawContourByObjSpace(QMatrix4x4 WVP, QVector3D CameraPos);
	void DrawContourByVertex(QMatrix4x4 WVP, QVector3D CameraPos);
	//important to scale this vector otherwise does not function properly
    QVector3D getBBoxCenter() {
        return BBoxCenter;
	}
    QVector3D getBSphereCenter(){
        return m_BoundingSphere.center;
    }

    float getScaleBestFit() {
        return scaleBestFit;
    }



private:

	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void InitBBox(const aiMesh* paiMesh);
    void InitBSphere(const aiMesh* paiMesh);
	void FindAllFaceNormals();
	void FindAllFacePositions();
	void FindCenterBBox();
	void DrawBBox();
    void DrawBSphere();
	void FindCurrentContourEdges(QMatrix4x4 WVP, QVector3D CameraPos);
	void FindCurrentContourVertices(QMatrix4x4 WVP, QVector3D CameraPos);
	void Clear();


#define INVALID_MATERIAL 0xFFFFFFFF
	struct BoundingBox {
		QVector3D Vertices[8];
	};
    struct BoundingSphere{
        float radius;
        QVector3D center;
    };

	const aiMesh* m_Mesh;
	struct MeshEntry {
		MeshEntry();

		~MeshEntry();

		void Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices);

		GLuint VB;
		GLuint IB;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

    //bounding sphere for this mesh
    BoundingSphere m_BoundingSphere;

	//binding box for this mesh
	BoundingBox myBox;
	QVector3D BBoxCenter;
	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;

	//data structures to facilitate mesh modification
	//this is needed since assimp imports in it's own data format, we need it in ours.
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Face> m_Faces;
	std::vector<Edge> m_Edges;

	//Vector to hold edges marked as contour lines
	std::vector<Edge*> m_ContourEdges;
	std::vector<Vertex> m_ControurVertices;

    //Scale holds calculated scale for best fit
    float scaleBestFit;
};

#endif //MESH_H
