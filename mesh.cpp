
#include "mesh.h"
#include "Camera.h"
#include <iterator>
#include <omp.h>
clock_t start, end;
double cpu_time_used;

Mesh::MeshEntry::MeshEntry()
{
	VB = INVALID_OGL_VALUE;
	IB = INVALID_OGL_VALUE;
	NumIndices = 0;
	MaterialIndex = INVALID_MATERIAL;
};

Mesh::MeshEntry::~MeshEntry()
{
	if (VB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &VB);
	}

	if (IB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &IB);
	}
}

void Mesh::MeshEntry::Init(const std::vector<Vertex>& Vertices,
	const std::vector<unsigned int>& Indices)
{
	NumIndices = Indices.size();
	
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);
}

Mesh::Mesh()
{
}

Mesh::~Mesh() {
	Clear();
}

void Mesh::Clear()
{
	for (unsigned int i = 0; i < m_Textures.size(); i++) {
		SAFE_DELETE(m_Textures[i]);
	}
}
bool Mesh::LoadMesh(const std::string& Filename) {
	
	//Get rid of previous mesh
	Clear();

	bool Ret = false;
	Assimp::Importer Importer;
    start = clock();
	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate |
														   aiProcess_GenSmoothNormals |
																    aiProcess_FlipUVs | 
                                                      aiProcess_JoinIdenticalVertices);
    end = clock();
    cpu_time_used = ((double)(end-start)/CLOCKS_PER_SEC);
    qInfo("Mesh loaded from file in: %lfs", cpu_time_used);
    if (pScene) {
		Ret = InitFromScene(pScene, Filename);
	}
	else {
		fprintf(stderr, "Error parsing '%s' : '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}

	return Ret;
}

bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename) {
	//reallocating vectors to new size
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
        start = clock();
		InitMesh(i, paiMesh);
        end = clock();
        cpu_time_used = ((double)(end-start)/CLOCKS_PER_SEC);
        qInfo("Mesh processed in: %lfs", cpu_time_used);
        InitBBox(paiMesh); //may not need to happen here
        //InitBSphere(paiMesh);
	}
	return true;
	//return InitMaterials(pScene, Filename);
}

//Built around triangle meshes
void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh) {
    //Mesh attribute checks
    if(!paiMesh->HasNormals()){
        qInfo("Mesh does not contain normals");
    }
    if(!paiMesh->HasTangentsAndBitangents()){
        qInfo("Mesh has tangents and bittangets");
    }
    if(paiMesh->HasBones()){
        qInfo("Mesh contains bones");
    }


	m_Mesh = paiMesh;
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	//adding vertices to arrays
    start = clock();
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
        //qInfo("x: %f\t y: %f\t z: %f", paiMesh->mNormals[i].x, paiMesh->mNormals[i].y, paiMesh->mNormals[i].z);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		
		Vertex v(QVector3D(pPos->x, pPos->y, pPos->z), 
				 QVector2D(pTexCoord->x, pTexCoord->y),
                 QVector3D(pNormal->x, pNormal->y, pNormal->z).normalized());

		m_Vertices.push_back(v);
	}
    end = clock();
    cpu_time_used = ((double)(end-start)/CLOCKS_PER_SEC);
    qInfo("Vertices pushed in: %lfs", cpu_time_used);

	//adding indices, edges, and faces to respective arrays -- This is convuluted so i tried to include a lot of comments


    for (uint i = 0; i < paiMesh->mNumFaces; i++) {
		//printf("Processing face %d\n", i);
		const aiFace& aiFace = paiMesh->mFaces[i];
		std::vector<int> thisFaceIndices;

		assert(aiFace.mNumIndices == 3);

		//I find it easier to have these values in vector vs array
		for (int k = 0; k < 3; k++) {
			thisFaceIndices.push_back(aiFace.mIndices[k]);
		}
		//creation of first edges Im sad to do this but it should fix the for loop
		Face newFace;
		if (i == 0) {
			Edge newE01;
			newE01.relatedVertIndices.push_back(thisFaceIndices[0]);
			newE01.relatedVertIndices.push_back(thisFaceIndices[1]);
			newE01.relatedFaceIndices.push_back(m_Faces.size());
			newFace.relatedEdgeIndices.push_back(m_Edges.size());
			m_Edges.push_back(newE01);
			//create new edge for 0 - 2
			Edge newE02;
			newE02.relatedVertIndices.push_back(thisFaceIndices[0]);
			newE02.relatedVertIndices.push_back(thisFaceIndices[2]);
			newE02.relatedFaceIndices.push_back(m_Faces.size());
			newFace.relatedEdgeIndices.push_back(m_Edges.size());
			m_Edges.push_back(newE02);
			//
			Edge newE12;
			newE12.relatedVertIndices.push_back(thisFaceIndices[1]);
			newE12.relatedVertIndices.push_back(thisFaceIndices[2]);
			newE12.relatedFaceIndices.push_back(m_Faces.size());
			newFace.relatedEdgeIndices.push_back(m_Edges.size());
			m_Edges.push_back(newE12);
		}

		//Start new face 
		//Face newFace;
		for (auto currentInd : thisFaceIndices) {
			m_Indices.push_back(currentInd);
			newFace.relatedVertIndices.push_back(currentInd);
		}
		//newFace.FindNormal();
		std::sort(thisFaceIndices.begin(), thisFaceIndices.end());
		
		int curEdgesSize = m_Edges.size();
		bool e01 = false, e02 = false, e12 = false; //dont make edges each loop dumbo
		for (int i = 0; i < curEdgesSize; i++) {
			//printf("size of edges %d\n", m_Edges.size());
			if (m_Edges[i].relatedVertIndices[0] == thisFaceIndices[0] && m_Edges[i].relatedVertIndices[1] == thisFaceIndices[1]) {
				m_Edges[i].relatedFaceIndices.push_back(m_Faces.size());
				newFace.relatedEdgeIndices.push_back(i);
				e01 = true;
				//printf("found existing edge\n");
			}
			if (m_Edges[i].relatedVertIndices[0] == thisFaceIndices[0] && m_Edges[i].relatedVertIndices[1] == thisFaceIndices[2]) {
				m_Edges[i].relatedFaceIndices.push_back(m_Faces.size());
				newFace.relatedEdgeIndices.push_back(i);
				e02 = true;
				//printf("found existing edge\n");
			}
			if (m_Edges[i].relatedVertIndices[0] == thisFaceIndices[1] && m_Edges[i].relatedVertIndices[1] == thisFaceIndices[2]) {
				m_Edges[i].relatedFaceIndices.push_back(m_Faces.size());
				newFace.relatedEdgeIndices.push_back(i);
				//printf("found existing edge\n");
				e12 = true;
			}
		}
		if (i != 0) {
			if (!e01) {
				//create new edge for 0 - 1
				Edge newE01;
				newE01.relatedVertIndices.push_back(thisFaceIndices[0]);
				newE01.relatedVertIndices.push_back(thisFaceIndices[1]);
				newE01.relatedFaceIndices.push_back(m_Faces.size());
				newFace.relatedEdgeIndices.push_back(m_Edges.size());
				m_Edges.push_back(newE01);
			}
			if (!e02) {
				//create new edge for 0 - 2
				Edge newE02;
				newE02.relatedVertIndices.push_back(thisFaceIndices[0]);
				newE02.relatedVertIndices.push_back(thisFaceIndices[2]);
				newE02.relatedFaceIndices.push_back(m_Faces.size());
				newFace.relatedEdgeIndices.push_back(m_Edges.size());
				m_Edges.push_back(newE02);
			}
			if (!e12) {
				//create new edge for 1 - 2
				Edge newE12;
				newE12.relatedVertIndices.push_back(thisFaceIndices[1]);
				newE12.relatedVertIndices.push_back(thisFaceIndices[2]);
				newE12.relatedFaceIndices.push_back(m_Faces.size());
				newFace.relatedEdgeIndices.push_back(m_Edges.size());
				m_Edges.push_back(newE12);
			}
		}

		m_Faces.push_back(newFace);
		//end new face


	}

	FindAllFaceNormals();
	FindAllFacePositions();


	m_Entries[Index].Init(m_Vertices, m_Indices);
    qInfo("Vertices: %d", m_Vertices.size());
    qInfo("Faces: %d", m_Faces.size());
}

bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename) {
	//Filepath modification/translation
	//return true;
	std::string::size_type SlashIndex = Filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = Filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Dir + "/" + Path.data;
				m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

				if (!m_Textures[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}
				else {
					printf("Loaded texture '%s'\n", FullPath.c_str());
				}
			}
		}

		// Load a white texture in case the model does not include its own texture
		if (!m_Textures[i]) {
			m_Textures[i] = new Texture(GL_TEXTURE_2D, "../../Models/white.png");

			Ret = m_Textures[i]->Load();
		}
	}
	return Ret;
}

void Mesh::InitBSphere(const aiMesh* paiMesh){
    //Implementation of Ritters bounding sphere


    const aiVector3D* firstVertex = &(paiMesh->mVertices[0]);
    QVector3D startPoint = QVector3D(firstVertex->x, firstVertex->y, firstVertex->z);



        float maxDis = std::numeric_limits<float>::min();

        //pick a point x on the model
        QVector3D pointX = startPoint;
        QVector3D pointY;

        //search for point y that is largest distance from point x
        for(unsigned int i = 1; i < paiMesh->mNumVertices; i++){
            const aiVector3D* secondVertex = &(paiMesh->mVertices[i]);
            QVector3D temp_pointY = QVector3D(secondVertex->x, secondVertex->y, secondVertex->z);

            float disXtoY =pointX.distanceToPoint(temp_pointY);
            if( disXtoY > maxDis){
                pointY = temp_pointY;
                maxDis = disXtoY;
            }
        }
        //reset maxdis
        maxDis = std::numeric_limits<float>::min();

        //find point z which has largest distance from y
        QVector3D pointZ;
        for(unsigned int i = 0; i < paiMesh->mNumVertices; i++){
            const aiVector3D* thirdVertex = &(paiMesh->mVertices[i]);
            QVector3D temp_pointZ = QVector3D(thirdVertex->x, thirdVertex->y, thirdVertex->z);

            float disYtoZ =pointY.distanceToPoint(temp_pointZ);
            if( disYtoZ > maxDis){
                pointZ = temp_pointZ;
                maxDis = disYtoZ;
            }
        }
        //Setup initial ball with center at midpoint of y and z, and radius half dis between y and z.
        m_BoundingSphere.center = QVector3D((pointY.x()+pointZ.x())/2.0f, (pointY.y()+pointZ.y())/2.0f, (pointY.z()+pointZ.z())/2.0f);
        m_BoundingSphere.radius = maxDis/2.0f;
        bool pointOutside = true;
        while(pointOutside)
        {
            pointOutside = false;
            for(unsigned int i = 0; i < paiMesh->mNumVertices; i++){
                const aiVector3D* secondVertex = &(paiMesh->mVertices[i]);
                QVector3D temp_point = QVector3D(secondVertex->x, secondVertex->y, secondVertex->z);
                if(m_BoundingSphere.center.distanceToPoint(temp_point) > m_BoundingSphere.radius){
                    startPoint = temp_point;
                    qInfo("point outside sphere");
                    i = paiMesh->mNumVertices; //killing loop

                }
            }
        }







    float disCenterToCenter = m_BoundingSphere.center.distanceToPoint(QVector3D(0.0f,0.0f,0.0f));
    qInfo("Distance to center: %f", disCenterToCenter);
    scaleBestFit = TARGETSCALE / disCenterToCenter;
    qInfo("Best fit scale: %f", scaleBestFit);
    m_BoundingSphere.center = -scaleBestFit * m_BoundingSphere.center;


}

void Mesh::InitBBox(const aiMesh* paiMesh) {
	//go through every vertices and find max and min x, y and z values
	//set min to max and max to min so anything smaller or greater will be saved. otherwise it will always be smallest
	float min = std::numeric_limits<float>::max();
	float max = std::numeric_limits<float>::min();

	float minX = min, minY = min, minZ = min;
	float maxX = max, maxY = max, maxZ = max;

	//seems good for parralellization
	//finding the smallest and greates x y and z values
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* curVertex = &(paiMesh->mVertices[i]);
		if (curVertex->x < minX){
			minX = curVertex->x;
		}
		else if (curVertex->x > maxX) {
			maxX = curVertex->x;
		}

		if (curVertex->y < minY) {
			minY = curVertex->y;
		}
		else if (curVertex->y > maxY) {
			maxY = curVertex->y;
		}

		if (curVertex->z < minZ) {
			minZ = curVertex->z;
		}
		else if (curVertex->z > maxZ) {
			maxZ = curVertex->z;
		}
	}

    //To find best fit scale divide bestfit by current avg dim
    //Result is the scale needed
    float avgDim = pow(maxX,2)+pow(minX,2) + pow(maxY,2) + pow(minY,2)+ pow(maxZ,2)+ pow(minZ,2);
    avgDim = sqrt(avgDim);
    scaleBestFit = TARGETSCALE / avgDim;
    //qInfo("Best fit scale: %f", scaleBestFit);


	//Building the actual bounding box dimensions 
	//defined at minx,miny,minz then moving around the face where y = min then moving to face y = max
	myBox.Vertices[0] = QVector3D(minX, minY, minZ);
	myBox.Vertices[1] = QVector3D(maxX, minY, minZ);
	myBox.Vertices[2] = QVector3D(maxX, minY, maxZ);
	myBox.Vertices[3] = QVector3D(minX, minY, maxZ);
	myBox.Vertices[4] = QVector3D(minX, maxY, minZ);
	myBox.Vertices[5] = QVector3D(maxX, maxY, minZ);
	myBox.Vertices[6] = QVector3D(maxX, maxY, maxZ);
	myBox.Vertices[7] = QVector3D(minX, maxY, maxZ);
	FindCenterBBox(); //finally calling center
}

//Finds average of all x, y, and z points from bounding box.
void Mesh::FindCenterBBox() {

	float sumX = 0, sumY = 0, sumZ = 0;
	for (int i = 0; i < 8; i++) {
		sumX += myBox.Vertices[i].x();
		sumY += myBox.Vertices[i].y();
		sumZ += myBox.Vertices[i].z();
	}
	
	//negates the adjusted center and times it by 2 to move to the models center instead of its base.
	//must be multiplied by scalar
	BBoxCenter = -QVector3D(sumX/8,sumY / 8,sumZ / 8);

    //Scaling bboxcenter vector to proper scale
    BBoxCenter = scaleBestFit * BBoxCenter;
	
}

void Mesh::DrawBBox() {
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 4; i++) {
		glVertex3f(myBox.Vertices[i].x(), myBox.Vertices[i].y(), myBox.Vertices[i].z());
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
	for (int i = 4; i < 8; i++) {
		glVertex3f(myBox.Vertices[i].x(), myBox.Vertices[i].y(), myBox.Vertices[i].z());
	}
	glEnd();
	//crossing through center
	glBegin(GL_LINE_LOOP);
	glVertex3f(myBox.Vertices[1].x(), myBox.Vertices[1].y(), myBox.Vertices[1].z());
	glVertex3f(myBox.Vertices[7].x(), myBox.Vertices[7].y(), myBox.Vertices[7].z());
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(myBox.Vertices[0].x(), myBox.Vertices[0].y(), myBox.Vertices[0].z());
	glVertex3f(myBox.Vertices[6].x(), myBox.Vertices[6].y(), myBox.Vertices[6].z());
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(myBox.Vertices[2].x(), myBox.Vertices[2].y(), myBox.Vertices[2].z());
	glVertex3f(myBox.Vertices[4].x(), myBox.Vertices[4].y(), myBox.Vertices[4].z());
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(myBox.Vertices[3].x(), myBox.Vertices[3].y(), myBox.Vertices[3].z());
	glVertex3f(myBox.Vertices[5].x(), myBox.Vertices[5].y(), myBox.Vertices[5].z());
	glEnd();
}


void Mesh::Render() {
	
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

		glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
	}
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

}

void Mesh::RenderWireFrame(){
    //Scale up slightly to avoid weak looking lines when not hollow
    float scaleFact = 1.0001;
    for(uint i = 0 ;i < m_Faces.size() ; i++){
        glBegin(GL_LINE_LOOP);

        for(uint j = 0; j < m_Faces[i].relatedVertIndices.size(); j++){
            Vertex* temp_v = &m_Vertices[m_Faces[i].relatedVertIndices[j]];
            glVertex3f(temp_v->m_pos.x()*scaleFact,temp_v->m_pos.y()*scaleFact,temp_v->m_pos.z()*scaleFact);
        }
        glEnd();
    }

}
void Mesh::FindAllFaceNormals() {

	//To find normal of surface with 3 points first find vector from one point (start)
	//to one of the remaining points (end1), and then again from start to the other point(end2)
    for (uint i = 0; i < m_Faces.size(); i++) {

		QVector3D Vec01 = m_Vertices[m_Faces[i].relatedVertIndices[0]].m_pos - m_Vertices[m_Faces[i].relatedVertIndices[1]].m_pos;
		QVector3D Vec02 = m_Vertices[m_Faces[i].relatedVertIndices[0]].m_pos - m_Vertices[m_Faces[i].relatedVertIndices[2]].m_pos;

		//Then find the cross product of these two vectors that lie in the plan, ez
		m_Faces[i].f_Normal = QVector3D::crossProduct(Vec01, Vec02);
	}

}

//Should work for higher order polygons
void Mesh::FindAllFacePositions() {
	//For all faces sum the values of all related vertices
    for (uint i = 0; i < m_Faces.size(); i ++) {
		QVector3D sumVector(.0f, .0f, .0f); //summing vector
        for (auto curFaceVert : m_Faces[i].relatedVertIndices) {
			sumVector = sumVector + m_Vertices[curFaceVert].m_pos;
		}

		//divide results by number of related vertices
        m_Faces[i].f_CenterPos = sumVector / (float)m_Faces[i].relatedVertIndices.size();
        //qInfo("FacePos1 X: %f\tY: %f\tZ: %f\n", curFace.x(), curFace.y(), curFace.z() );
	}
}

void Mesh::FindCurrentContourEdges(QMatrix4x4 WVP, QVector3D CameraPos) {
	//First wipe all previous contour edges
	m_ContourEdges.clear();

	//Must find all edges where one surface normal points towards camera and other points away
    for (uint i = 0; i < m_Edges.size(); i++) {
		//printf("loading contour");
        //qInfo("Edge %d has %d faces attached\n", i,m_Edges[i].relatedFaceIndices.size() );
		if (m_Edges[i].relatedFaceIndices.size() > 1) {
			if (m_Edges[i].relatedFaceIndices.size() > 2) {
                //qInfo("Error edge %d has %d faces attached\n", i,m_Edges[i].relatedFaceIndices.size());
			}

			//Using the center of the faces to roughly determine viewpoint vector
			QVector4D facePos0 = WVP * QVector4D(m_Faces[m_Edges[i].relatedFaceIndices[0]].f_CenterPos,1);
            //qInfo("Face0 Center X: %f\tY: %f\tZ: %f\n", m_Faces[m_Edges[i].relatedFaceIndices[1]].f_CenterPos.x(), m_Faces[m_Edges[i].relatedFaceIndices[0]].f_CenterPos.y(), m_Faces[m_Edges[i].relatedFaceIndices[0]].f_CenterPos.z() );
			QVector4D facePos1 = WVP * QVector4D(m_Faces[m_Edges[i].relatedFaceIndices[1]].f_CenterPos,1);

			//find vector from face center to viewer
            QVector3D face0toViewer = QVector3D(-(CameraPos) - QVector3D(facePos0.x(), facePos0.y(), facePos0.z()));
            face0toViewer.normalize();
            QVector3D face1toViewer = QVector3D(-(CameraPos) - QVector3D(facePos1.x(), facePos1.y(), facePos1.z()));
            face0toViewer.normalize();

                //qInfo("Face2View1 X: %f\tY: %f\tZ: %f\n", face0toViewer.x(), face0toViewer.y(), face0toViewer.z() );
                //qInfo("Face2View2 X: %f\tY: %f\tZ: %f\n", face1toViewer.x(), face1toViewer.y(), face1toViewer.z() );

			//find dot of cur face norm with vector from face to viewer
			
			float dotNorm1 = QVector3D::dotProduct(m_Faces[m_Edges[i].relatedFaceIndices[0]].CurNormal(WVP), face0toViewer);
			float dotNorm2 = QVector3D::dotProduct(m_Faces[m_Edges[i].relatedFaceIndices[1]].CurNormal(WVP), face1toViewer);
		
			
			if (dotNorm1 >= 0) {
					if (dotNorm2 <= 0) {
							//printf("found Contour\n");
							m_ContourEdges.push_back(&m_Edges[i]);
						}
					}
			else if (dotNorm1 <= 0) {
						if (dotNorm2 >= 0) {
							//printf("found Contour\n");
							m_ContourEdges.push_back(&m_Edges[i]);
						}
					}

		}
		else {
			//This happens if the edge only has 1 face attached, therefor either assignment is wrong or this is a contour edge
			m_ContourEdges.push_back(&m_Edges[i]);
		}
	}
}

void Mesh::FindCurrentContourVertices(QMatrix4x4 WVP, QVector3D CameraPos) {
    float zeroTolerance = .1f;
	//First wipe all current vertices 
	m_ControurVertices.clear();

	for (auto curVert : m_Vertices) {
		//rotate vector by world transforml
		QVector4D vertexPos = WVP * QVector4D(curVert.m_pos, 1);
		//find the vector from this vertex to the camera
       // QVector3D vertexToCam = QVector3D(((-1.0f*(CameraPos)) - QVector3D(vertexPos.x(), vertexPos.y(), vertexPos.z()))).normalized();
        QVector3D vertexToCam = QVector3D(((-1.0f*(CameraPos)) - QVector3D(vertexPos.x(), vertexPos.y(), vertexPos.z())));
        vertexToCam.normalize();
        //qInfo("Vert to cam: x:%f\t y: %f\t z: %f", vertexToCam.x(), vertexToCam.y(), vertexToCam.z());
		//Need to rotate the current vertex normal by the world view matrix
        QVector4D rotatedNorm4D = WVP * QVector4D(curVert.m_normal, 1);
        QVector3D rotatedNorm = QVector3D(rotatedNorm4D.x(),rotatedNorm4D.y(),rotatedNorm4D.z()).normalized() ;
        //qInfo("RotNorm x:%f\t y: %f\t z: %f", rotatedNorm.x(),rotatedNorm.y(), rotatedNorm.z() );
		//find dot product of normal dot camera view
        float nDotv = QVector3D::dotProduct(rotatedNorm, vertexToCam);
        //printf("ndot value:%f\n", nDotv);
        if (nDotv < zeroTolerance && nDotv > (-1.0*zeroTolerance)) {
			
			m_ControurVertices.push_back(curVert);
			//printf("found contour Vertex\n");
		}


	}
    //printf("found %d contour verts\n", m_ControurVertices.size());
}



void Mesh::DrawContourByObjSpace(QMatrix4x4 WVP, QVector3D CameraTarget) {
	FindCurrentContourEdges(WVP, CameraTarget);
	//set color black
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	//printf("size of contour %d\n ", m_ContourEdges.size());
    for (uint i = 0; i < m_ContourEdges.size(); i++) {
		
        float scaleFact = 1.01f;
		glVertex3f(m_Vertices[m_ContourEdges[i]->relatedVertIndices[0]].m_pos.x() * scaleFact,
			m_Vertices[m_ContourEdges[i]->relatedVertIndices[0]].m_pos.y() * scaleFact,
			m_Vertices[m_ContourEdges[i]->relatedVertIndices[0]].m_pos.z() * scaleFact);

		glVertex3f(m_Vertices[m_ContourEdges[i]->relatedVertIndices[1]].m_pos.x() * scaleFact,
			m_Vertices[m_ContourEdges[i]->relatedVertIndices[1]].m_pos.y() * scaleFact,
			m_Vertices[m_ContourEdges[i]->relatedVertIndices[1]].m_pos.z() * scaleFact);
		
	}
	glEnd();
}

void Mesh::DrawContourByVertex(QMatrix4x4 WVP, QVector3D CameraTarget) {
	
	FindCurrentContourVertices(WVP, CameraTarget);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	for (auto curVert : m_ControurVertices) {
		glVertex3f(curVert.m_pos.x(), curVert.m_pos.y(), curVert.m_pos.z());
	}
	glEnd();
}
