#pragma once

#include "3d_math_qt.h"
#include "Camera.h"

class Pipeline
{
public:
	Pipeline() {
		m_scale = QVector3D(1.0f, 1.0f, 1.0f);
		m_worldPos = QVector3D(0.0f, 0.0f, 0.0f);
		m_rotateInfo = QVector3D(0.0f, 0.0f, 0.0f);
		m_rotQuat = QQuaternion(cos(M_PI / 4), QVector3D(sin(M_PI / 4)*0, sin(M_PI / 4)*1, sin(M_PI / 4)* 0));
	}
	void Scale(float s) {
		Scale(s, s, s);
	}
	void Scale(float ScaleX, float ScaleY, float ScaleZ) {
		m_scale.setX(ScaleX);
		m_scale.setY(ScaleY);
		m_scale.setZ(ScaleZ);
	}
	;
	void WorldPos(float x, float y, float z) {
		m_worldPos.setX(x);
		m_worldPos.setY(y);
		m_worldPos.setZ(z);
	};
	void Rotate(float theta) {
		Rotate(theta, theta, theta);
	}
	void Rotate(float RotateX, float RotateY, float RotateZ) {
		m_rotateInfo.setX(RotateX);
		m_rotateInfo.setY(RotateY);
		m_rotateInfo.setZ(RotateZ);
	}
	
	void SetPerspectiveProj(const PersProjInfo& p) {
		m_persProjInfo = p;
	}

	void SetCamera(const QVector3D& Pos, const QVector3D& Target, const QVector3D& Up) {
		m_camera.Pos = Pos;
		m_camera.Target = Target;
		m_camera.Up = Up;

	}

	void SetCamera(const Camera& camera ) {
		SetCamera(camera.GetPos(), camera.GetTarget(), camera.GetUp());

	}

	void SetModelCorrection(const QVector3D& newCenter) {
		modelCenter = newCenter;
	}

	const QMatrix4x4& GetModelCorrection();
	const QMatrix4x4& GetWorldTrans();
	const QMatrix4x4& GetViewTrans();
	const QMatrix4x4& GetVPTrans();
	const QMatrix4x4& GetWPTrans();
	const QMatrix4x4& GetWVPTrans();
	const QMatrix4x4& GetProjTrans();
	const QQuaternion& Trackball(float x, float y);
	const QVector3D& tbInit(float x, float y);
private:
	QVector3D modelCenter;
	QVector3D m_scale;
	QVector3D m_worldPos;
	QVector3D m_rotateInfo;
	QVector3D m_tbMousePos;
	QQuaternion m_rotQuat;
	PersProjInfo m_persProjInfo;

	struct {
		QVector3D Pos;
		QVector3D Target;
		QVector3D Up;
	}m_camera;
	//model correction to center model at 0,0,0 
	QMatrix4x4 m_modelCorrection;
	QMatrix4x4 m_transformation;
	QMatrix4x4 m_Vtransformation;
	QMatrix4x4 m_ProjTransformation;
	QMatrix4x4 m_VPtransformation;
	QMatrix4x4 m_WPtransformation;
	QMatrix4x4 m_WVPtransformation;
	
};