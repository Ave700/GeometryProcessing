#pragma once

#include <qquaternion.h>
#include <QVector3D>
#include <QVector2D>
#include <qmatrix4x4.h>
#include <qmatrix.h>
#include "utilities.h"
#include <qmath.h>
//#define M_PI 3.14

#define ToRadian(x) ((x)*M_PI/180.0f)
#define ToDegree(x) ((x) *180.0f/M_PI)

struct PersProjInfo {
	float FOV;
	float Width;
	float Height;
	float zNear;
	float zFar;
};

QMatrix4x4 InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
QMatrix4x4 InitRotateTransform(float RotateX, float RotateY, float RotateZ);
QMatrix4x4 InitRotateTransform(const QQuaternion& quat);
QMatrix4x4 InitTranslationTransform(float x, float y, float z);
QMatrix4x4 InitTranslationTransform(QVector3D translationVector);
QMatrix4x4 InitCameraTransform(const QVector3D& Target, const QVector3D& Up);
QMatrix4x4 InitPersProjTransform(const PersProjInfo& p);
