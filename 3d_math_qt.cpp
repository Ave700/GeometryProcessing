#include "3d_math_qt.h"
//Creates and returns a scale matrix from given 3 floats
QMatrix4x4 InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ) {
	QMatrix4x4 ScaleTransform(ScaleX, 0.0f, 0.0f, 0.0f,
								 0.0f, ScaleY, 0.0f, 0.0f,
								 0.0f, 0.0f, ScaleZ, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f);

	return ScaleTransform;

}

//Creates and returns a rotation matrix from given 3 vector of floats
QMatrix4x4 InitRotateTransform(float RotateX, float RotateY, float RotateZ) {
	QMatrix4x4 rx, ry, rz;
	//radians boi
	const float x = ToRadian(RotateX);
	const float y = ToRadian(RotateY);
	const float z = ToRadian(RotateZ);
	//rotation in respect to x
	rx = QMatrix4x4(1.0f, 0.0f,     0.0f,     0.0f,
				    0.0f, cosf(x), -sinf(x),  0.0f,
					0.0f, sinf(x),  cosf(x),  0.0f,
					0.0f, 0.0f,     0.0f,     1.0f);
	//rotation in respect to y
	ry = QMatrix4x4(cosf(y), 0.0f,  -sinf(y), 0.0f,
				    0.0f,	 1.0f,  0.0f,     0.0f,
					sinf(y), 0.0f,  cosf(y),  0.0f,
					0.0f,    0.0f,  0.0f,     1.0f);
	//rotation in respect to z
	rz = QMatrix4x4(cosf(z), -sinf(z),  0.0f, 0.0f,
					sinf(z), cosf(z),   0.0f, 0.0f,
					0.0f,    0.0f,      1.0f, 0.0f,
					0.0f,	 0.0f,		0.0f, 1.0f );
	//combined rotation
	QMatrix4x4 RotationTransform = rz * ry * rx;
	return RotationTransform;
}
//Creates and returns a rotation matrix from given quaternion
QMatrix4x4 InitRotateTransform(const QQuaternion& quat) {
	float yy2 = 2.0f * quat.y() * quat.y();
	float xy2 = 2.0f * quat.x() * quat.y();
	float xz2 = 2.0f * quat.x() * quat.z();
	float yz2 = 2.0f * quat.y() * quat.z();
	float zz2 = 2.0f * quat.z() * quat.z();
	float wz2 = 2.0f * quat.scalar() * quat.z();
	float wy2 = 2.0f * quat.scalar() * quat.y();
	float wx2 = 2.0f * quat.scalar() * quat.x();
	float xx2 = 2.0f * quat.x() * quat.x();

	
	QMatrix4x4 RotationTransform((-yy2 - zz2 + 1.0f), (xy2 - wz2), (xz2 + wy2), 0.0f,
		(xy2 + wz2), (-xx2 - zz2 + 1.0f), (yz2 - wx2), 0.0f,
		(xz2 - wy2), (yz2 + wx2), (-xx2 - yy2 + 1.0f), 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f);

	return RotationTransform;
}

//Creates and returns a 4x4 translation matrix with given 3 floats
QMatrix4x4 InitTranslationTransform(float x, float y, float z) {
	//the last colum describes translation
	QMatrix4x4 TranslationTransform(1.0f, 0.0f, 0.0f, x,
									0.0f, 1.0f, 0.0f, y,
									0.0f, 0.0f, 1.0f, z,
									0.0f, 0.0f, 0.0f, 1.0f);
	return TranslationTransform;
}

//Creates and returns a 4x4 translation matrix with given vector
QMatrix4x4 InitTranslationTransform(QVector3D translationVector) {
	//the last colum describes translation
	QMatrix4x4 TranslationTransform(1.0f, 0.0f, 0.0f, translationVector.x(),
									0.0f, 1.0f, 0.0f, translationVector.y(),
									0.0f, 0.0f, 1.0f, translationVector.z(),
									0.0f, 0.0f, 0.0f, 1.0f);
	return TranslationTransform;
}

//Creates and returns 4x4 translation matrix based on camera target/ up vectors
QMatrix4x4 InitCameraTransform(const QVector3D& Target, const QVector3D& Up) {
	QVector3D N = Target;
	N.normalize();
	QVector3D U = Up;
	U = QVector3D::crossProduct(U,N);
	U.normalize();
	QVector3D V = QVector3D::crossProduct(N, U);

	QMatrix4x4 CameraTransform( U.x(), U.y(), U.z(), 0.0f,
								V.x(), V.y(), V.z(), 0.0f,
								N.x(), N.y(), N.z(), 0.0f,
								0.0f,  0.0f,  0.0f,  1.0f);
	return CameraTransform;
}

//Creates projection transformation matrix based on projection information passed in
QMatrix4x4 InitPersProjTransform(const PersProjInfo& p) {
	const float ar = p.Width / p.Height;
	const float zRange = p.zNear - p.zFar;
	const float tanHalfFov = tanf(ToRadian(p.FOV / 2.0f));

	QMatrix4x4 PersProjTransform( (1.0f / (tanHalfFov * ar)), 0.0f, 0.0f, 0.0f,
								   0.0f, (1.0f / tanHalfFov), 0.0f, 0.0f,
								   0.0f, 0.0f, (-p.zNear - p.zFar) / zRange, (2.0f * p.zFar * p.zNear) / zRange,
								   0.0f, 0.0f, 1.0f, 0.0f);

	return PersProjTransform;
}

