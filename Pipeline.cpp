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



#define TRACKBALLSIZE .7f


static float tbProjectToSphere(float r, float x, float y){
    float d, t, z;
    d = sqrt(x*x + y*y);
    if(d < r * sqrt(.5f)){ //On sphere
        z = sqrt(r*r - d*d);
    }
    else{ //on parabolic sheet
        t = r/ sqrt(2.0);
        z = t*t / d;
    }
    return z;

}

//Initializing the trackball
const QVector3D& Pipeline::tbInit(float x, float y) {


    m_tbMousePos = QVector3D(x, y, tbProjectToSphere(TRACKBALLSIZE,x,y));

	return m_tbMousePos;
}




//Function defining trackball rotation for mouse 
const QQuaternion& Pipeline::Trackball(float x, float y) {
	//Using algorithm described in Sit and Spin section of this website https://www.khronos.org/opengl/wiki/Object_Mouse_Trackball
	//should use a sphere when x or y is less then r and trumpet when greater then r
    //skip if called on same vec
    if(x == m_tbMousePos.x() && y == m_tbMousePos.y()){
        return m_rotQuat;
    }
    //qInfo("X%f\tY:%f", x, y);
	//Create new mouse pos in 3d space with found z coord
    QVector3D newMousePos = QVector3D(x, y, tbProjectToSphere(TRACKBALLSIZE, x, y));

	//Collect stored previous mouse position
	QVector3D originalMousePos = m_tbMousePos;

	//The rotation vector is the cross product between first and last mouse position
    QVector3D rotVec = QVector3D::crossProduct(newMousePos, originalMousePos);
    rotVec.normalize();

    //QVector3D oldToNew = newMousePos - originalMousePos ;
     QVector3D oldToNew = originalMousePos - newMousePos;
    float len = oldToNew.length() / (2.0*TRACKBALLSIZE);

    if(len > 1.0) len = 1.0;
    if(len < - 1.0) len = -1.0;
    float phi = 2.0f * asin(len);
    //qInfo("phi %f",phi);

    //Creating rotation quaternion
    QQuaternion newRotQuat = QQuaternion(cosf(phi / 2.0f), rotVec * sin(phi / 2.0f));


	//Storing new mouse pos into trackball mouse position for next call
    m_tbMousePos = newMousePos;


	//rotating current rotated position by new quaternion 
    m_rotQuat =  newRotQuat * m_rotQuat;

	//This m_rotQuat variable is used to find the rotation matrix during rendering
	return m_rotQuat;
}
