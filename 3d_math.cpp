//#include "3d_math.h"
//
//void Matrix4f::InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
//{
//
//    m[0][0] = ScaleX; m[1][0] = 0.0f;   m[2][0] = 0.0f;   m[3][0] = 0.0f;
//    m[0][1] = 0.0f;   m[1][1] = ScaleY; m[2][1] = 0.0f;   m[3][1] = 0.0f;
//    m[0][2] = 0.0f;   m[1][2] = 0.0f;   m[2][2] = ScaleZ; m[3][2] = 0.0f;
//    m[0][3] = 0.0f;   m[1][3] = 0.0f;   m[2][3] = 0.0f;   m[3][3] = 1.0f;
//}
//
//void Matrix4f::InitRotateTransform(float RotateX, float RotateY, float RotateZ)
//{
//    Matrix4f rx, ry, rz;
//
//    const float x = ToRadian(RotateX);
//    const float y = ToRadian(RotateY);
//    const float z = ToRadian(RotateZ);
//
//
//    rx.m[0][0] = 1.0f; rx.m[1][0] = 0.0f;    rx.m[2][0] = 0.0f;     rx.m[3][0] = 0.0f;
//    rx.m[0][1] = 0.0f; rx.m[1][1] = cosf(x); rx.m[2][1] = -sinf(x); rx.m[3][1] = 0.0f;
//    rx.m[0][2] = 0.0f; rx.m[1][2] = sinf(x); rx.m[2][2] = cosf(x);  rx.m[3][2] = 0.0f;
//    rx.m[0][3] = 0.0f; rx.m[1][3] = 0.0f;    rx.m[2][3] = 0.0f;     rx.m[3][3] = 1.0f;
//
//    ry.m[0][0] = cosf(y); ry.m[1][0] = 0.0f; ry.m[2][0] = -sinf(y); ry.m[3][0] = 0.0f;
//    ry.m[0][1] = 0.0f;    ry.m[1][1] = 1.0f; ry.m[2][1] = 0.0f;     ry.m[3][1] = 0.0f;
//    ry.m[0][2] = sinf(y); ry.m[1][2] = 0.0f; ry.m[2][2] = cosf(y);  ry.m[3][2] = 0.0f;
//    ry.m[0][3] = 0.0f;    ry.m[1][3] = 0.0f; ry.m[2][3] = 0.0f;     ry.m[3][3] = 1.0f;
//
//    rz.m[0][0] = cosf(z); rz.m[1][0] = -sinf(z); rz.m[2][0] = 0.0f; rz.m[3][0] = 0.0f;
//    rz.m[0][1] = sinf(z); rz.m[1][1] = cosf(z);  rz.m[2][1] = 0.0f; rz.m[3][1] = 0.0f;
//    rz.m[0][2] = 0.0f;    rz.m[1][2] = 0.0f;     rz.m[2][2] = 1.0f; rz.m[3][2] = 0.0f;
//    rz.m[0][3] = 0.0f;    rz.m[1][3] = 0.0f;     rz.m[2][3] = 0.0f; rz.m[3][3] = 1.0f;
//
//    *this = rz * ry * rx;
//}
//
//void Matrix4f::InitRotateTransform(const glm::quat& quat) {
//    float yy2 = 2.0f * quat.y * quat.y;
//    float xy2 = 2.0f * quat.x * quat.y;
//    float xz2 = 2.0f * quat.x * quat.z;
//    float yz2 = 2.0f * quat.y * quat.z;
//    float zz2 = 2.0f * quat.z * quat.z;
//    float wz2 = 2.0f * quat.w * quat.z;
//    float wy2 = 2.0f * quat.w * quat.y;
//    float wx2 = 2.0f * quat.w * quat.x;
//    float xx2 = 2.0f * quat.x * quat.x;
//    m[0][0] = -yy2 - zz2 + 1.0f;
//    m[1][0] = xy2 + wz2;
//    m[2][0] = xz2 - wy2;
//    m[3][0] = 0;
//    m[0][1] = xy2 - wz2;
//    m[1][1] = -xx2 - zz2 + 1.0f;
//    m[2][1] = yz2 + wx2;
//    m[3][1] = 0;
//    m[0][2] = xz2 + wy2;
//    m[1][2] = yz2 - wx2;
//    m[2][2] = -xx2 - yy2 + 1.0f;
//    m[3][2] = 0.0f;
//    m[0][3] = m[1][3] = m[2][3] = 0;
//    m[3][3] = 1.0f;
//}
//
//void Matrix4f::InitTranslationTransform(float x, float y, float z)
//{
//
//
//    m[0][0] = 1.0f; m[1][0] = 0.0f; m[2][0] = 0.0f; m[3][0] = x;
//    m[0][1] = 0.0f; m[1][1] = 1.0f; m[2][1] = 0.0f; m[3][1] = y;
//    m[0][2] = 0.0f; m[1][2] = 0.0f; m[2][2] = 1.0f; m[3][2] = z;
//    m[0][3] = 0.0f; m[1][3] = 0.0f; m[2][3] = 0.0f; m[3][3] = 1.0f;
//}
//
//void Matrix4f::InitPersProjTransform(const PersProjInfo& p)
//{
//    const float ar = p.Width / p.Height;
//    const float zRange = p.zNear - p.zFar;
//    const float tanHalfFOV = tanf(ToRadian(p.FOV / 2.0f));
//
//    m[0][0] = 1.0f / (tanHalfFOV * ar); m[1][0] = 0.0f;            m[2][0] = 0.0f;            m[3][0] = 0.0;
//    m[0][1] = 0.0f;                   m[1][1] = 1.0f / tanHalfFOV; m[2][1] = 0.0f;            m[3][1] = 0.0;
//    m[0][2] = 0.0f;                   m[1][2] = 0.0f;            m[2][2] = (-p.zNear - p.zFar) / zRange; m[3][2] = 2.0f * p.zFar * p.zNear / zRange;
//    m[0][3] = 0.0f;                   m[1][3] = 0.0f;            m[2][3] = 1.0f;            m[3][3] = 0.0;
//
//}
//
//void Matrix4f::InitCameraTransform(const glm::vec3& Target, const glm::vec3& Up) {
//    glm::vec3 N = Target;
//    N = glm::normalize(N);
//    glm::vec3 U = Up;
//    U = glm::cross(U, Target);
//    U = glm::normalize(U);
//    glm::vec3 V = glm::cross(N, U);
//
//    m[0][0] = U.x; m[1][0] = U.y; m[2][0] = U.z; m[3][0] = 0.0f;
//    m[0][1] = V.x; m[1][1] = V.y; m[2][1] = V.z; m[3][1] = 0.0f;
//    m[0][2] = N.x; m[1][2] = N.y; m[2][2] = N.z; m[3][2] = 0.0f;
//    m[0][3] = 0.0f; m[1][3] = 0.0f; m[2][3] = 0.0f; m[3][3] = 1.0f;
//};
//
////glm::vec3 Rotate(float Angle, const glm::vec3& Axe)