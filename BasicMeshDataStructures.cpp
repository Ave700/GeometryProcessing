#include "BasicMeshDataStructures.h"


//Takes in the current world view matrix and returns the modifed normal vector
QVector3D Face::CurNormal(QMatrix4x4 WVP) {
	//To include rotating and scaling we must have a 4 vector, by setting w to 1
	//this acts as a simple workaround
	f_Normal.normalize();
	//printf("x: %f\t y: %f\t z: %f\n", f_Normal.x, f_Normal.y, f_Normal.z);
	QVector4D f_Norm4 = QVector4D(f_Normal, 1.0f);
	f_Norm4 = WVP * f_Norm4;
	//to return we must return the 3 vec again since this is what we will 
	//relate to the cameras  position
	return QVector3D(f_Norm4.x(), f_Norm4.y(), f_Norm4.z());
}

