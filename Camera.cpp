
#include "Camera.h"
const static float STEP_SCALE = .25f;
const static float EDGE_STEP = 0.5f; 
const static int MARGIN = 10;
Camera::Camera(int WinWidth, int WinHeight) {
	m_windowHeight = WinHeight;
	m_windowWidth = WinWidth;
	m_pos = QVector3D(0.0f, 0.0f, 0.0f);
	m_target = QVector3D(0.0f, 0.0f, 1.0f);
	m_up = QVector3D(0.0f, 1.0f, 0.0f);
	Init();
}

Camera::Camera(int WinWidth, int WinHeight, const QVector3D& Pos, const QVector3D& Target, const QVector3D& Up) {
	m_windowHeight = WinHeight;
	m_windowWidth = WinWidth;
	m_pos = Pos;
	m_target = Target.normalized();
	m_up = Up.normalized();
	Init();
}

void Camera::Init() {
	QVector3D HTarget(m_target.x(), 0.0f, m_target.z());
	HTarget.normalize();

	if (HTarget.z() >= 0.0f) {
		if (HTarget.x() >= 0.0f) {
			m_AngleH = 360.0f - ToDegree(asinf(HTarget.z()));
		}
		else {
			m_AngleH = 180.0f + ToDegree(asinf(HTarget.z()));
		}
	}
	else
	{
		if (HTarget.x() >= 0.0f) {
			m_AngleH = ToDegree(asinf(-HTarget.z()));
		}
		else {
			m_AngleH = 180.0f - ToDegree(asin(-HTarget.z()));
		}
	}

	m_AngleV = -ToDegree(asin(m_target.y()));
	//Bools to hold if mouse is at or over edge
	m_OnUpperEdge = false;
	m_OnLowerEdge = false;
	m_OnLeftEdge = false;
	m_OnRightEdge = false;
	m_mousePos.setX(m_windowWidth / 2);
	m_mousePos.setY(m_windowHeight / 2);

	//glutWarpPointer(m_mousePos.x(), m_mousePos.y());
}

void Camera::OnMouse(int x, int y) {
	const int DeltaX = x - m_mousePos.x();
	const int DeltaY = y - m_mousePos.y();

	m_mousePos.setX(x);
	m_mousePos.setY(y);

	m_AngleH += (float)DeltaX / 20.0f;
	m_AngleV += (float)DeltaY / 20.0f;

	if (DeltaX == 0) {
		if (x <= MARGIN) {
			m_OnLeftEdge = true;
		}
		else if (x >= (m_windowWidth - MARGIN)) {
			m_OnRightEdge = true;
		}
	}
	else {
		m_OnRightEdge = false;
		m_OnLeftEdge = false;
	}

	if (DeltaY == 0) {
		if (y <= MARGIN) {
			m_OnUpperEdge = true;
		}
		else if (y >= (m_windowHeight - MARGIN)) {
			m_OnLowerEdge = true;
		}
	}
	else {
		m_OnUpperEdge = false;
		m_OnLowerEdge = false;
	}

	Update();
}

bool Camera::OnKeyboard(int Key) {
	bool Res = false;

	switch (Key) {
		case GLUT_KEY_UP:
		{
			m_pos += (m_target * STEP_SCALE);
			Res = true;
		}break; 

		case GLUT_KEY_DOWN:
		{
			m_pos -= (m_target * STEP_SCALE);
			Res = true;
		}break;

		case GLUT_KEY_LEFT:
		{
			QVector3D Left = QVector3D::crossProduct(m_target, m_up);
			Left.normalize();
			Left *= STEP_SCALE;
			m_pos += Left;
			Res = true;
		}break;
		case GLUT_KEY_RIGHT:
		{
			QVector3D Right = QVector3D::crossProduct(m_up, m_target);
			Right.normalize();
			Right *= STEP_SCALE;
			m_pos += Right;
			Res = true;
		}break;

	}
	return Res;
}

void Camera::OnRender() {
	bool ShouldUpdate = false;

	if (m_OnLeftEdge) {
		m_AngleH -= 0.1f;
		ShouldUpdate = true;
	}
	else if (m_OnRightEdge) {
		m_AngleH += 0.1f;
		ShouldUpdate = true;
	}

	//check vertical edges and continue spiing
	if (m_OnUpperEdge) {
		if (m_AngleV > -90.0f) {
			m_AngleV -= 0.1f;
			ShouldUpdate = true;
		}
	}
	else if (m_OnLowerEdge) {
		if (m_AngleV < 90.0f) {
			m_AngleV += 0.1f;
			ShouldUpdate = true;
		}
	}

	if (ShouldUpdate) {
		Update();
	}


}

void Camera::Update() {
	//const QVector3D vAxis = QVector3D(0.0f, 1.0f, 0.0f);

	////rotatin view vector by horizontal angle round vert axis

	//QVector3D View = QVector3D(1.0f, 0.0f, 0.0f);
	//View = glm::rotate(vAxis, m_AngleH, m_up);
	//View = glm::normalize(View);

	//QVector3D hAxis = glm::cross(vAxis, View);
	//hAxis = glm::normalize(hAxis);
	//View = glm::rotate(hAxis, m_AngleV, m_target);
	//View = glm::normalize(View);

	//m_target = View;
	//m_target = glm::normalize(m_target);

	//m_up = glm::cross(m_target, hAxis);
	//m_up = glm::normalize(m_up);
}