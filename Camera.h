#pragma once
#include "3d_math_qt.h"
#include <glut.h>
class Camera
{
public:
	Camera();
	Camera(int WinWidth, int WinHeight);
	Camera(int WinWidth, int WinHeight, const QVector3D& Pos, const QVector3D& Target, const QVector3D& Up);
	bool OnKeyboard(int Key);
	void OnMouse(int x, int y);
	void OnRender();
	const QVector3D& GetPos() const{ return m_pos;}
	const QVector3D& GetTarget() const{ return m_target;}
	const QVector3D& GetUp() const { return m_up;}

private:
	void Init();
	void Update();
	int m_windowWidth;
	int m_windowHeight;

	QVector3D m_pos;
	QVector3D m_target;
	QVector3D m_up;

	float m_AngleH; //for mouse camera rotations
	float m_AngleV;

	bool m_OnUpperEdge;
	bool m_OnLowerEdge;
	bool m_OnRightEdge;
	bool m_OnLeftEdge;

	QVector2D m_mousePos;
};
