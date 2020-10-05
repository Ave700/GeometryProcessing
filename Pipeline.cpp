#include "Pipeline.h"
#include <iostream>

const QMatrix4x4& Pipeline::GetModelCorrection() {
	m_modelCorrection = InitTranslationTransform(modelCenter);
	return m_modelCorrection;
}

const QMatrix4x4& Pipeline::GetProjTrans()
{
	m_ProjTransformation = InitPersProjTransform(m_persProjInfo);
	return m_ProjTransformation;
}

const QMatrix4x4& Pipeline::GetWorldTrans()
{
	QMatrix4x4 ScaleTrans, RotateTrans, TranslationTrans;

	ScaleTrans = InitScaleTransform(m_scale.x(), m_scale.y(), m_scale.z());
	RotateTrans = InitRotateTransform(m_rotQuat);
	TranslationTrans = InitTranslationTransform(m_worldPos.x(), m_worldPos.y(), m_worldPos.z());

	//model correcction should happen before rotation so model rotates round origin
	m_transformation = TranslationTrans  * RotateTrans * m_modelCorrection * ScaleTrans  ;
	return m_transformation;
}

const QMatrix4x4& Pipeline::GetViewTrans() {
	QMatrix4x4 CameraTranslationTrans, CameraRotateTrans;
	CameraTranslationTrans = InitTranslationTransform(m_camera.Pos);
	CameraRotateTrans = InitCameraTransform(m_camera.Target, m_camera.Up);

	m_Vtransformation = CameraRotateTrans * CameraTranslationTrans;
	return m_Vtransformation;
}

const QMatrix4x4& Pipeline::GetVPTrans() {
	GetViewTrans();
	GetProjTrans();

	m_VPtransformation = m_ProjTransformation * m_Vtransformation;
	return m_VPtransformation;
}

const QMatrix4x4& Pipeline::GetWPTrans()
{
	QMatrix4x4 PersProjTrans;
	GetWorldTrans();
	PersProjTrans = InitPersProjTransform(m_persProjInfo);

	m_WPtransformation = PersProjTrans * m_transformation;
	return m_WPtransformation;

}

const QMatrix4x4& Pipeline::GetWVPTrans() {
	GetWorldTrans();
	GetVPTrans();

	m_WVPtransformation = m_VPtransformation * m_transformation;
	return m_WVPtransformation;
}


//Initializing the trackball
const QVector3D& Pipeline::tbInit(float x, float y) {
	float z;
	float x2 = pow((float)x, 2);
	float y2 = pow((float)y, 2);
	//printf("xsq:%f\t ysq:%f\n", x2, y2);
	float r = .5f;

	if (x2 + y2 <= pow(r, 2) / 2.0f) {
		//printf("hey\n");
		z = sqrt(pow(r, 2) - (x2 + y2));
	}
	else {
		z = (pow(r, 2) / 2) / sqrt(x2 + y2);
		//printf("hp\n");
	}

	m_tbMousePos = QVector3D(x, y, z);

	return m_tbMousePos;
}

//Function defining trackball rotation for mouse 
const QQuaternion& Pipeline::Trackball(float x, float y) {
	//Using algorithm described in Sit and Spin section of this website https://www.khronos.org/opengl/wiki/Object_Mouse_Trackball
	//should use a sphere when x or y is less then r and trumpet when greater then r
	float z;
	//x^2 and y^2 variables
	float x2 = pow(x, 2.0f);
	float y2 = pow(y, 2.0f);
	//Describing screen space as -.5 to .5 or radius 5. This was an intuitive guess and may be inaccurate
	float r = .5f;
	//Choosing trumpet or ball as rotation space (read artice above)
	if (x2 + y2 <= pow(r,2.0f)/2.0f) {
		z = sqrt(pow(r, 2.0f) - (x2 + y2));
	}
	else {
		z = (pow(r, 2.0f) / 2.0f) / sqrt(x2 + y2);
	}
	//Create new mouse pos in 3d space with found z coord
	QVector3D newMousePos = QVector3D(x, y, z);
	newMousePos.normalize();

	//Collect stored previous mouse position
	QVector3D originalMousePos = m_tbMousePos;
	originalMousePos.normalize();

	//The rotation vector is the cross product between first and last mouse position
	QVector3D rotVec = QVector3D::crossProduct(originalMousePos, newMousePos);
	rotVec.normalize();

	//Collecting angle between first and last position 
	float theta = acosf(QVector3D::dotProduct(originalMousePos, newMousePos));

	//Storing new mouse pos into trackball mouse position for next call
	m_tbMousePos = QVector3D(x, y, z);

	
	//Creating rotation quaternion
	QQuaternion newRotQuat = QQuaternion(cosf(theta / 2), rotVec * sin(theta / 2));

	//rotating current rotated position by new quaternion 
	m_rotQuat *= newRotQuat;
	
	//This m_rotQuat variable is used to find the rotation matrix during rendering
	return m_rotQuat;
}
