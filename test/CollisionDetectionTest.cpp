#include <gtest/gtest.h>
#include "SympConv/CollisionDetection.h"
#include "SympConv/Vector.h"
#include "SympConv/Ray.h"
#include "SympConv/AABB.h"
#include "SympConv/Plane.h"
#include "SympConv/Box.h"
#include "SympConv/Sphere.h"
#include "SympConv/Cylinder.h"
#include "SympConv/Quaternion.h"
#include "TestUtils.h"

namespace SympConvTest {

using Vec3 = SympConv::Vector3;
using Ray = SympConv::Ray;
using AABB = SympConv::AABB;
using Plane = SympConv::Plane;
using Box = SympConv::Box;
using Sphere = SympConv::Sphere;
using Capsule = SympConv::Capsule;
using Cylinder = SympConv::Cylinder;
using Segment = SympConv::Segment;
using Quaternion = SympConv::Quaternion;

TEST(CollisionDetectionTest, RayIntersectsAABB) {
    // 创建一个AABB
    AABB aabb(Vec3(-1.0f, -1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f));
    
    // 从原点发射向AABB的射线
    Ray ray1(Vec3(0.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f));
    float tMin1, tMax1;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray1, aabb, tMin1, tMax1));
    EXPECT_TRUE(FloatEqual(tMin1, 1.0f));
    EXPECT_TRUE(FloatEqual(tMax1, 3.0f));
    // 验证相交点
    Vec3 intersectPoint1 = ray1.GetPoint(tMin1);
    EXPECT_TRUE(FloatEqual(intersectPoint1.x, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.y, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.z, -1.0f)); // 应该在AABB的后表面
    
    // 从AABB内部发射的射线
    Ray ray2(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
    float tMin2, tMax2;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray2, aabb, tMin2, tMax2));
    EXPECT_TRUE(FloatEqual(tMin2, 0.0f));
    EXPECT_TRUE(FloatEqual(tMax2, 1.0f));
    // 验证相交点
    Vec3 intersectPoint2 = ray2.GetPoint(tMax2);
    EXPECT_TRUE(FloatEqual(intersectPoint2.x, 1.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint2.y, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint2.z, 0.0f)); // 应该在AABB的右表面
    
    // 不相交的射线
    Ray ray3(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    float tMin3, tMax3;
    EXPECT_FALSE(SympConv::RayIntersectsAABB(ray3, aabb, tMin3, tMax3));
    
    // Corner case: 射线方向向量有一个分量为0
    Ray ray4(Vec3(0.0f, 0.0f, -2.0f), Vec3(1.0f, 0.0f, 1.0f)); // y分量为0
    float tMin4, tMax4;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray4, aabb, tMin4, tMax4));
    
    // Corner case: 射线刚好擦过AABB的边缘
    Ray ray5(Vec3(0.0f, 1.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f)); // 沿着AABB的上边缘
    float tMin5, tMax5;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray5, aabb, tMin5, tMax5));
    
    // Corner case: 射线与AABB的顶点相交
    Ray ray6(Vec3(2.0f, 2.0f, 2.0f), Vec3(-1.0f, -1.0f, -1.0f)); // 指向AABB的顶点
    float tMin6, tMax6;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray6, aabb, tMin6, tMax6));
    
    // Corner case: 射线方向向量有多个分量为0的情况
    // x和y分量为0，z分量不为0
    Ray ray7(Vec3(0.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f));
    float tMin7, tMax7;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray7, aabb, tMin7, tMax7));
    EXPECT_TRUE(FloatEqual(tMin7, 1.0f));
    EXPECT_TRUE(FloatEqual(tMax7, 3.0f));
    
    // x和z分量为0，y分量不为0
    Ray ray8(Vec3(0.0f, -2.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    float tMin8, tMax8;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray8, aabb, tMin8, tMax8));
    EXPECT_TRUE(FloatEqual(tMin8, 1.0f));
    EXPECT_TRUE(FloatEqual(tMax8, 3.0f));
    
    // y和z分量为0，x分量不为0
    Ray ray9(Vec3(-2.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
    float tMin9, tMax9;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray9, aabb, tMin9, tMax9));
    EXPECT_TRUE(FloatEqual(tMin9, 1.0f));
    EXPECT_TRUE(FloatEqual(tMax9, 3.0f));
    
    // Corner case: 射线原点在AABB的边界上
    Ray ray10(Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
    float tMin10, tMax10;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray10, aabb, tMin10, tMax10));
}

TEST(CollisionDetectionTest, RayIntersectsPlane) {
    // 创建一个平面（z=0）
    Plane plane(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));
    
    // 从z=-1向z=1发射的射线
    Ray ray1(Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t1;
    EXPECT_TRUE(SympConv::RayIntersectsPlane(ray1, plane, t1));
    EXPECT_TRUE(FloatEqual(t1, 1.0f));
    // 验证相交点
    Vec3 intersectPoint1 = ray1.GetPoint(t1);
    EXPECT_TRUE(FloatEqual(intersectPoint1.x, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.y, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.z, 0.0f)); // 应该在平面上
    
    // 平行于平面的射线
    Ray ray2(Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
    float t2;
    EXPECT_FALSE(SympConv::RayIntersectsPlane(ray2, plane, t2));
    
    // 从平面后面发射的射线（反方向）
    Ray ray3(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t3;
    EXPECT_FALSE(SympConv::RayIntersectsPlane(ray3, plane, t3));
    
    // 非AliasAxis情况：倾斜平面和倾斜射线相交
    Plane inclinedPlane(Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));
    Ray inclinedRay(Vec3(1.0f, 1.0f, -1.0f), Vec3(-1.0f, -1.0f, 1.0f));
    float t4;
    EXPECT_TRUE(SympConv::RayIntersectsPlane(inclinedRay, inclinedPlane, t4));
    // 验证相交点
    Vec3 intersectPoint4 = inclinedRay.GetPoint(t4);
    // 相交点应该在平面上，满足平面方程：x + y + z = 0
    EXPECT_TRUE(FloatEqual(intersectPoint4.x + intersectPoint4.y + intersectPoint4.z, 0.0f));
    
    // 非AliasAxis情况：倾斜平面和倾斜射线不相交（平行）
    Ray parallelRay(Vec3(1.0f, 1.0f, -1.0f), Vec3(1.0f, 1.0f, 0.0f));
    float t5;
    EXPECT_FALSE(SympConv::RayIntersectsPlane(parallelRay, inclinedPlane, t5));
    
    // 非AliasAxis情况：倾斜平面和倾斜射线不相交（方向相反）
    Ray oppositeRay(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));
    float t6;
    EXPECT_FALSE(SympConv::RayIntersectsPlane(oppositeRay, inclinedPlane, t6));
    
    // Corner case: 射线原点在平面上
    Ray ray7(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    float t7;
    EXPECT_TRUE(SympConv::RayIntersectsPlane(ray7, plane, t7));
    EXPECT_TRUE(FloatEqual(t7, 0.0f));
    
    // Corner case: 射线几乎平行于平面
    Ray ray8(Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 1.0f, 1e-7f)); // z分量很小
    float t8;
    EXPECT_FALSE(SympConv::RayIntersectsPlane(ray8, plane, t8));
    
    // Corner case: 射线刚好擦过平面（t接近0）
    Ray ray9(Vec3(0.0f, 0.0f, 1e-7f), Vec3(0.0f, 0.0f, -1.0f));
    float t9;
    EXPECT_TRUE(SympConv::RayIntersectsPlane(ray9, plane, t9));
    
    // Corner case: 射线与平面垂直
    Ray ray10(Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t10;
    EXPECT_TRUE(SympConv::RayIntersectsPlane(ray10, plane, t10));
    EXPECT_TRUE(FloatEqual(t10, 1.0f));
    
    // Corner case: 射线方向与平面法线相反
    Ray ray11(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t11;
    EXPECT_FALSE(SympConv::RayIntersectsPlane(ray11, plane, t11));
}

TEST(CollisionDetectionTest, RayIntersectsSphere) {
    // 球体中心在原点，半径1
    Vec3 center(0.0f, 0.0f, 0.0f);
    float radius = 1.0f;
    
    // 从z=-2向z=2发射的射线
    Ray ray1(Vec3(0.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t1, t2;
    EXPECT_TRUE(SympConv::RayIntersectsSphere(ray1, center, radius, t1, t2));
    EXPECT_TRUE(FloatEqual(t1, 1.0f));
    EXPECT_TRUE(FloatEqual(t2, 3.0f));
    // 验证相交点
    Vec3 intersectPoint1 = ray1.GetPoint(t1);
    Vec3 intersectPoint2 = ray1.GetPoint(t2);
    EXPECT_TRUE(FloatEqual(intersectPoint1.x, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.y, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.z, -1.0f)); // 应该在球体的后表面
    EXPECT_TRUE(FloatEqual(intersectPoint2.x, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint2.y, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint2.z, 1.0f)); // 应该在球体的前表面
    // 验证相交点到球心的距离等于半径
    EXPECT_TRUE(FloatEqual((intersectPoint1 - center).Length(), radius));
    EXPECT_TRUE(FloatEqual((intersectPoint2 - center).Length(), radius));
    
    // 不相交的射线
    Ray ray2(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    float t3, t4;
    EXPECT_FALSE(SympConv::RayIntersectsSphere(ray2, center, radius, t3, t4));
    
    // Corner case: 射线原点在球体内
    Ray ray3(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
    float t5, t6;
    EXPECT_TRUE(SympConv::RayIntersectsSphere(ray3, center, radius, t5, t6));
    EXPECT_TRUE(FloatEqual(t5, 1.0f));
    
    // Corner case: 射线与球体相切
    Ray ray4(Vec3(1.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f)); // 水平距离刚好等于半径
    float t7, t8;
    EXPECT_TRUE(SympConv::RayIntersectsSphere(ray4, center, radius, t7, t8));
    EXPECT_TRUE(FloatEqual(t7, t8)); // 相切时两个t值相等
    
    // Corner case: 射线原点在球体表面
    Ray ray5(Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    float t9, t10;
    EXPECT_TRUE(SympConv::RayIntersectsSphere(ray5, center, radius, t9, t10));
    EXPECT_TRUE(FloatEqual(t9, 0.0f));
    
    // Corner case: 射线穿过球体中心
    Ray ray6(Vec3(2.0f, 0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
    float t11, t12;
    EXPECT_TRUE(SympConv::RayIntersectsSphere(ray6, center, radius, t11, t12));
    EXPECT_TRUE(FloatEqual(t11, 1.0f));
    EXPECT_TRUE(FloatEqual(t12, 3.0f));
    
    // Corner case: 射线原点在球体内
    Ray ray7(Vec3(0.5f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
    float t13, t14;
    EXPECT_TRUE(SympConv::RayIntersectsSphere(ray7, center, radius, t13, t14));
    EXPECT_TRUE(FloatEqual(t13, 0.5f)); // 从内部到表面的距离
}

TEST(CollisionDetectionTest, RayIntersectsTriangle) {
    // 三角形在z=0平面上
    Vec3 v0(0.0f, 0.0f, 0.0f);
    Vec3 v1(1.0f, 0.0f, 0.0f);
    Vec3 v2(0.0f, 1.0f, 0.0f);
    
    // 从z=-1向三角形中心发射的射线
    Ray ray1(Vec3(0.3f, 0.3f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t, u, v;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray1, v0, v1, v2, t, u, v));
    EXPECT_TRUE(FloatEqual(t, 1.0f));
    EXPECT_TRUE(FloatEqual(u, 0.3f));
    EXPECT_TRUE(FloatEqual(v, 0.3f));
    // 验证相交点
    Vec3 intersectPoint = ray1.GetPoint(t);
    EXPECT_TRUE(FloatEqual(intersectPoint.x, 0.3f));
    EXPECT_TRUE(FloatEqual(intersectPoint.y, 0.3f));
    EXPECT_TRUE(FloatEqual(intersectPoint.z, 0.0f)); // 应该在z=0平面上
    // 验证相交点是否在三角形内部（通过重心坐标）
    EXPECT_TRUE(u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f);
    
    // Corner case: 射线与三角形的边相交
    Ray rayEdge(Vec3(0.5f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)); // 沿着三角形的底边
    float tEdge, uEdge, vEdge;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(rayEdge, v0, v1, v2, tEdge, uEdge, vEdge));
    EXPECT_TRUE(FloatEqual(tEdge, 1.0f));
    EXPECT_TRUE(FloatEqual(uEdge, 0.5f));
    EXPECT_TRUE(FloatEqual(vEdge, 0.0f));
    
    // Corner case: 射线与三角形的顶点相交
    Ray rayVertex(Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)); // 指向三角形的顶点
    float tVertex, uVertex, vVertex;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(rayVertex, v0, v1, v2, tVertex, uVertex, vVertex));
    EXPECT_TRUE(FloatEqual(tVertex, 1.0f));
    EXPECT_TRUE(FloatEqual(uVertex, 0.0f));
    EXPECT_TRUE(FloatEqual(vVertex, 0.0f));
    
    // Corner case: 射线原点在三角形平面上但在外部
    Ray rayPlaneOutside(Vec3(2.0f, 2.0f, 0.0f), Vec3(-1.0f, -1.0f, 0.0f)); // 在平面上但在外部
    float tPlane, uPlane, vPlane;
    EXPECT_FALSE(SympConv::RayIntersectsTriangle(rayPlaneOutside, v0, v1, v2, tPlane, uPlane, vPlane));
    
    // Corner case: 射线几乎平行于三角形
    Ray rayAlmostParallel(Vec3(0.5f, 0.5f, -1.0f), Vec3(1.0f, 1.0f, 1e-7f)); // z分量很小
    float tParallel, uParallel, vParallel;
    EXPECT_FALSE(SympConv::RayIntersectsTriangle(rayAlmostParallel, v0, v1, v2, tParallel, uParallel, vParallel));
    
    // 从三角形外部发射的射线
    Ray ray2(Vec3(2.0f, 2.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t2, u2, vParam2;
    EXPECT_FALSE(SympConv::RayIntersectsTriangle(ray2, v0, v1, v2, t2, u2, vParam2));
    
    // 三角形在x=0平面上
    Vec3 v3(0.0f, 0.0f, 0.0f);
    Vec3 v4(0.0f, 1.0f, 0.0f);
    Vec3 v5(0.0f, 0.0f, 1.0f);
    Ray ray3(Vec3(-1.0f, 0.3f, 0.3f), Vec3(1.0f, 0.0f, 0.0f));
    float t3, u3, vParam3;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray3, v3, v4, v5, t3, u3, vParam3));
    
    // 三角形在y=0平面上
    Vec3 v6(0.0f, 0.0f, 0.0f);
    Vec3 v7(1.0f, 0.0f, 0.0f);
    Vec3 v8(0.0f, 0.0f, 1.0f);
    Ray ray4(Vec3(0.3f, -1.0f, 0.3f), Vec3(0.0f, 1.0f, 0.0f));
    float t4, u4, vParam4;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray4, v6, v7, v8, t4, u4, vParam4));
    
    // 非AliasAxis的三角形
    Vec3 v9(0.0f, 0.0f, 0.0f);
    Vec3 v10(1.0f, 1.0f, 0.0f);
    Vec3 v11(0.0f, 1.0f, 1.0f);
    Ray ray5(Vec3(0.3f, 0.3f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t5, u5, vParam5;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray5, v9, v10, v11, t5, u5, vParam5));
    
    // 三角形和射线平行的情况
    Vec3 v12(0.0f, 0.0f, 0.0f);
    Vec3 v13(1.0f, 0.0f, 0.0f);
    Vec3 v14(0.0f, 1.0f, 0.0f);
    Ray ray6(Vec3(0.5f, 0.5f, 1.0f), Vec3(1.0f, 1.0f, 0.0f)); // 平行于z=0平面
    float t6, u6, vParam6;
    EXPECT_FALSE(SympConv::RayIntersectsTriangle(ray6, v12, v13, v14, t6, u6, vParam6));
    
    // 细长的三角形
    Vec3 v15(0.0f, 0.0f, 0.0f);
    Vec3 v16(10.0f, 0.1f, 0.0f);
    Vec3 v17(0.0f, 0.1f, 0.0f);
    Ray ray7(Vec3(5.0f, 0.05f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t7, u7, vParam7;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray7, v15, v16, v17, t7, u7, vParam7));
    
    // 非AliasAxis的射线
    Vec3 v18(0.0f, 0.0f, 0.0f);
    Vec3 v19(1.0f, 0.0f, 0.0f);
    Vec3 v20(0.0f, 1.0f, 0.0f);
    Ray ray8(Vec3(1.0f, 1.0f, 1.0f), Vec3(-1.0f, -1.0f, -1.0f)); // 斜向射线
    float t8, u8, vParam8;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray8, v18, v19, v20, t8, u8, vParam8));
    
    // 很长的射线
    Vec3 v21(0.0f, 0.0f, 0.0f);
    Vec3 v22(1.0f, 0.0f, 0.0f);
    Vec3 v23(0.0f, 1.0f, 0.0f);
    Ray ray9(Vec3(0.5f, 0.5f, -1000.0f), Vec3(0.0f, 0.0f, 1.0f)); // 长射线
    float t9, u9, vParam9;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray9, v21, v22, v23, t9, u9, vParam9));
    
    // 射线和三角形都不是AliasAxis的情况
    Vec3 v24(1.0f, 1.0f, 1.0f);
    Vec3 v25(2.0f, 2.0f, 1.0f);
    Vec3 v26(1.0f, 2.0f, 2.0f);
    Ray ray10(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)); // 斜向射线
    float t10, u10, vParam10;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray10, v24, v25, v26, t10, u10, vParam10));
    
    // Corner case: 射线从三角形的边缘擦过
    Ray rayEdgeGrazing(Vec3(1.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float tEdgeGrazing, uEdgeGrazing, vEdgeGrazing;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(rayEdgeGrazing, v0, v1, v2, tEdgeGrazing, uEdgeGrazing, vEdgeGrazing));
    
    // Corner case: 三角形退化（三点共线）
    Vec3 v27(0.0f, 0.0f, 0.0f);
    Vec3 v28(1.0f, 0.0f, 0.0f);
    Vec3 v29(2.0f, 0.0f, 0.0f);
    Ray rayDegenerate(Vec3(1.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float tDegenerate, uDegenerate, vDegenerate;
    EXPECT_FALSE(SympConv::RayIntersectsTriangle(rayDegenerate, v27, v28, v29, tDegenerate, uDegenerate, vDegenerate));
}

TEST(CollisionDetectionTest, RayIntersectsSegment) {
    // 线段在z=0平面上
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(1.0f, 1.0f, 0.0f);
    Segment segment(start, end);
    
    // 从(0.5, 0.5, -1)向z轴正方向发射的射线
    Ray ray1(Vec3(0.5f, 0.5f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t, s;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray1, segment, t, s));
    EXPECT_TRUE(FloatEqual(t, 1.0f));
    EXPECT_TRUE(FloatEqual(s, 0.5f));
    // 验证相交点
    Vec3 intersectPoint = ray1.GetPoint(t);
    EXPECT_TRUE(FloatEqual(intersectPoint.x, 0.5f));
    EXPECT_TRUE(FloatEqual(intersectPoint.y, 0.5f));
    EXPECT_TRUE(FloatEqual(intersectPoint.z, 0.0f)); // 应该在线段上
    // 验证相交点是否在线段上（通过参数s）
    EXPECT_TRUE(s >= 0.0f && s <= 1.0f);
    
    // Corner case: 射线与线段的端点相交
    Ray rayEndpoint(Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)); // 指向线段的起点
    float tEndpoint, sEndpoint;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(rayEndpoint, segment, tEndpoint, sEndpoint));
    EXPECT_TRUE(FloatEqual(sEndpoint, 0.0f));
    
    // Corner case: 射线与线段的中点相交
    Ray rayMidpoint(Vec3(0.5f, 0.5f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)); // 指向线段的中点
    float tMidpoint, sMidpoint;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(rayMidpoint, segment, tMidpoint, sMidpoint));
    EXPECT_TRUE(FloatEqual(sMidpoint, 0.5f));
    
    // Corner case: 射线原点在线段上
    Ray rayOriginOnSegment(Vec3(0.5f, 0.5f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)); // 原点在线段上
    float tOrigin, sOrigin;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(rayOriginOnSegment, segment, tOrigin, sOrigin));
    EXPECT_TRUE(FloatEqual(tOrigin, 0.0f));
    
    // Corner case: 射线方向与线段方向垂直
    Ray rayPerpendicular(Vec3(0.5f, 1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f)); // 垂直向下，起点在线段正上方
    float tPerpendicular, sPerpendicular;
    // 这条射线应该与线段相交，因为它从线段上方垂直向下
    EXPECT_TRUE(SympConv::RayIntersectsSegment(rayPerpendicular, segment, tPerpendicular, sPerpendicular));
    
    // Corner case: 射线与线段平行
    Ray rayParallelSegment(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 0.0f)); // 平行于线段，起点在线段起点
    float tParallelSeg, sParallelSeg;
    // 这条射线与线段平行，不应该相交（因为它们重合，但是射线方向与线段方向相同）
    EXPECT_FALSE(SympConv::RayIntersectsSegment(rayParallelSegment, segment, tParallelSeg, sParallelSeg));
    
    // 不相交的射线
    Ray ray2(Vec3(2.0f, 2.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t2, s2;
    EXPECT_FALSE(SympConv::RayIntersectsSegment(ray2, segment, t2, s2));
    
    // 线段在x=0平面上
    Vec3 start2(0.0f, 0.0f, 0.0f);
    Vec3 end2(0.0f, 1.0f, 1.0f);
    Segment segment2(start2, end2);
    Ray ray3(Vec3(-1.0f, 0.5f, 0.5f), Vec3(1.0f, 0.0f, 0.0f));
    float t3, s3;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray3, segment2, t3, s3));
    
    // 线段在y=0平面上
    Vec3 start3(0.0f, 0.0f, 0.0f);
    Vec3 end3(1.0f, 0.0f, 1.0f);
    Segment segment3(start3, end3);
    Ray ray4(Vec3(0.5f, -1.0f, 0.5f), Vec3(0.0f, 1.0f, 0.0f));
    float t4, s4;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray4, segment3, t4, s4));
    
    // 非AliasAxis的线段
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(1.0f, 1.0f, 1.0f);
    Segment segment4(start4, end4);
    Ray ray5(Vec3(0.5f, 0.5f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)); // 调整射线起点，确保与线段相交
    float t5, s5;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray5, segment4, t5, s5));
    
    // 线段和射线平行的情况
    Vec3 start5(0.0f, 0.0f, 0.0f);
    Vec3 end5(1.0f, 1.0f, 0.0f);
    Segment segment5(start5, end5);
    Ray ray6(Vec3(2.0f, 2.0f, 1.0f), Vec3(1.0f, 1.0f, 0.0f)); // 平行于线段
    float t6, s6;
    EXPECT_FALSE(SympConv::RayIntersectsSegment(ray6, segment5, t6, s6));
    
    // 细长的线段
    Vec3 start6(0.0f, 0.0f, 0.0f);
    Vec3 end6(10.0f, 0.1f, 0.0f);
    Segment segment6(start6, end6);
    Ray ray7(Vec3(5.0f, 0.05f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t7, s7;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray7, segment6, t7, s7));
    
    // 非AliasAxis的射线
    Vec3 start7(0.0f, 0.0f, 0.0f);
    Vec3 end7(1.0f, 1.0f, 0.0f);
    Segment segment7(start7, end7);
    Ray ray8(Vec3(1.0f, 1.0f, 1.0f), Vec3(-1.0f, -1.0f, -1.0f)); // 斜向射线
    float t8, s8;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray8, segment7, t8, s8));
    
    // 很长的射线
    Vec3 start8(0.0f, 0.0f, 0.0f);
    Vec3 end8(1.0f, 1.0f, 0.0f);
    Segment segment8(start8, end8);
    Ray ray9(Vec3(0.5f, 0.5f, -1000.0f), Vec3(0.0f, 0.0f, 1.0f)); // 长射线
    float t9, s9;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray9, segment8, t9, s9));
    
    // 射线和线段都不是AliasAxis的情况
    Vec3 start9(1.0f, 1.0f, 0.0f);
    Vec3 end9(2.0f, 2.0f, 0.0f);
    Segment segment9(start9, end9);
    Ray ray10(Vec3(1.5f, 1.5f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)); // 调整线段和射线，确保相交
    float t10, s10;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray10, segment9, t10, s10));
    
    // Corner case: 线段长度为0（退化为点）
    Vec3 point(0.5f, 0.5f, 0.0f);
    Segment pointSegment(point, point);
    Ray rayPoint(Vec3(0.5f, 0.5f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float tPoint, sPoint;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(rayPoint, pointSegment, tPoint, sPoint));
}

TEST(CollisionDetectionTest, AABBIntersectsAABB) {
    // 两个相交的AABB
    AABB aabb1(Vec3(-1.0f, -1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f));
    AABB aabb2(Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
    EXPECT_TRUE(SympConv::AABBIntersectsAABB(aabb1, aabb2));
    
    // 两个边界接触的AABB（视为相交）
    AABB aabb3(Vec3(-2.0f, -2.0f, -2.0f), Vec3(-1.0f, -1.0f, -1.0f));
    EXPECT_TRUE(SympConv::AABBIntersectsAABB(aabb1, aabb3));
    
    // 两个不相交的AABB
    AABB aabb4(Vec3(-3.0f, -3.0f, -3.0f), Vec3(-2.0f, -2.0f, -2.0f));
    EXPECT_FALSE(SympConv::AABBIntersectsAABB(aabb1, aabb4));
    
    // Corner case: 两个AABB在一个轴上接触
    AABB aabb5(Vec3(-1.0f, -1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f));
    AABB aabb6(Vec3(1.0f, 0.0f, 0.0f), Vec3(2.0f, 0.0f, 0.0f));
    EXPECT_TRUE(SympConv::AABBIntersectsAABB(aabb5, aabb6));
}

TEST(CollisionDetectionTest, PointInAABB) {
    AABB aabb(Vec3(-1.0f, -1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f));
    
    // AABB内部的点
    Vec3 point1(0.0f, 0.0f, 0.0f);
    EXPECT_TRUE(SympConv::PointInAABB(point1, aabb));
    
    // AABB边界上的点
    Vec3 point2(1.0f, 0.0f, 0.0f);
    EXPECT_TRUE(SympConv::PointInAABB(point2, aabb));
    
    // AABB外部的点
    Vec3 point3(2.0f, 0.0f, 0.0f);
    EXPECT_FALSE(SympConv::PointInAABB(point3, aabb));
    
    // Corner case: 点在AABB的角点上
    Vec3 point4(1.0f, 1.0f, 1.0f);
    EXPECT_TRUE(SympConv::PointInAABB(point4, aabb));
}

TEST(CollisionDetectionTest, PointOnPlane) {
    // z=0平面
    Plane plane(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));
    
    // 平面上的点
    Vec3 point1(1.0f, 1.0f, 0.0f);
    EXPECT_TRUE(SympConv::PointOnPlane(point1, plane));
    
    // 平面上方的点
    Vec3 point2(1.0f, 1.0f, 1.0f);
    EXPECT_TRUE(SympConv::PointOnPlanePositiveSide(point2, plane));
    EXPECT_FALSE(SympConv::PointOnPlaneNegativeSide(point2, plane));
    
    // 平面下方的点
    Vec3 point3(1.0f, 1.0f, -1.0f);
    EXPECT_FALSE(SympConv::PointOnPlanePositiveSide(point3, plane));
    EXPECT_TRUE(SympConv::PointOnPlaneNegativeSide(point3, plane));
    
    // Corner case: 点在平面上
    Vec3 point4(1.0f, 1.0f, 0.0f);
    EXPECT_TRUE(SympConv::PointOnPlane(point4, plane));
    
    // Corner case: 点在平面的正方向
    Vec3 point5(1.0f, 1.0f, 0.1f);
    EXPECT_TRUE(SympConv::PointOnPlanePositiveSide(point5, plane));
    
    // Corner case: 点在平面的负方向
    Vec3 point6(1.0f, 1.0f, -0.1f);
    EXPECT_TRUE(SympConv::PointOnPlaneNegativeSide(point6, plane));
}

TEST(CollisionDetectionTest, RayIntersectsBox) {
    // 创建一个盒子
    Box box(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    
    // 从盒子前面发射的射线
    Ray ray1(Vec3(0.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t1;
    EXPECT_TRUE(SympConv::RayIntersectsBox(ray1, box, t1));
    EXPECT_TRUE(FloatEqual(t1, 1.0f)); // 射线从z=-2到盒子表面z=-1，距离为1
    // 验证相交点
    Vec3 intersectPoint1 = ray1.GetPoint(t1);
    // 盒子的范围是从(-1,-1,-1)到(1,1,1)，所以相交点应该在z=-1处
    EXPECT_TRUE(FloatEqual(intersectPoint1.x, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.y, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.z, -1.0f));
    
    // 从盒子后面发射的射线
    Ray ray2(Vec3(0.0f, 0.0f, 2.0f), Vec3(0.0f, 0.0f, -1.0f));
    float t2;
    EXPECT_TRUE(SympConv::RayIntersectsBox(ray2, box, t2));
    EXPECT_TRUE(FloatEqual(t2, 1.0f)); // 射线从z=2到盒子表面z=1，距离为1
    // 验证相交点
    Vec3 intersectPoint2 = ray2.GetPoint(t2);
    // 相交点应该在z=1处
    EXPECT_TRUE(FloatEqual(intersectPoint2.x, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint2.y, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint2.z, 1.0f));
    
    // 不相交的射线
    Ray ray3(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    float t3;
    EXPECT_FALSE(SympConv::RayIntersectsBox(ray3, box, t3));
    
    // 非AliasAxis情况：射线与旋转的盒子相交
    Box rotatedBox(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Quaternion(0.0f, 0.0f, 0.7071f, 0.7071f)); // 绕y轴旋转45度
    Ray ray4(Vec3(2.0f, 0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
    float t4;
    EXPECT_TRUE(SympConv::RayIntersectsBox(ray4, rotatedBox, t4));
    EXPECT_TRUE(t4 >= 0.0f); // t值应该非负
    
    // 非AliasAxis情况：射线与旋转的盒子不相交
    Ray ray5(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    float t5;
    EXPECT_FALSE(SympConv::RayIntersectsBox(ray5, rotatedBox, t5));
}

TEST(CollisionDetectionTest, BoxIntersectsBox) {
    // 创建两个相交的盒子
    Box box1(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    Box box2(Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(SympConv::BoxIntersectsBox(box1, box2));
    
    // 创建两个不相交的盒子
    Box box3(Vec3(3.0f, 3.0f, 3.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_FALSE(SympConv::BoxIntersectsBox(box1, box3));
    
    // 非AliasAxis情况：两个旋转的盒子相交
    Box rotatedBox1(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Quaternion(0.0f, 0.0f, 0.7071f, 0.7071f)); // 绕y轴旋转45度
    Box rotatedBox2(Vec3(0.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f), Quaternion(0.0f, 0.7071f, 0.0f, 0.7071f)); // 绕x轴旋转45度
    EXPECT_TRUE(SympConv::BoxIntersectsBox(rotatedBox1, rotatedBox2));
    
    // 非AliasAxis情况：两个旋转的盒子不相交
    Box rotatedBox3(Vec3(3.0f, 3.0f, 3.0f), Vec3(1.0f, 1.0f, 1.0f), Quaternion(0.0f, 0.0f, 0.7071f, 0.7071f)); // 绕y轴旋转45度
    EXPECT_FALSE(SympConv::BoxIntersectsBox(rotatedBox1, rotatedBox3));
}

TEST(CollisionDetectionTest, SphereIntersectsBox) {
    // 创建一个球体和一个盒子
    Sphere sphere(Vec3(0.0f, 0.0f, 0.0f), 1.0f);
    Box box(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(SympConv::SphereIntersectsBox(sphere, box));
    
    // 创建一个不相交的球体和盒子
    Sphere sphere2(Vec3(3.0f, 0.0f, 0.0f), 1.0f);
    EXPECT_FALSE(SympConv::SphereIntersectsBox(sphere2, box));
    
    // 非AliasAxis情况：球体与旋转的盒子相交
    Box rotatedBox(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Quaternion(0.0f, 0.0f, 0.7071f, 0.7071f)); // 绕y轴旋转45度
    Sphere sphere3(Vec3(1.0f, 0.0f, 0.0f), 0.5f);
    EXPECT_TRUE(SympConv::SphereIntersectsBox(sphere3, rotatedBox));
    
    // 非AliasAxis情况：球体与旋转的盒子不相交
    Sphere sphere4(Vec3(2.0f, 2.0f, 2.0f), 0.5f);
    EXPECT_FALSE(SympConv::SphereIntersectsBox(sphere4, rotatedBox));
}

TEST(CollisionDetectionTest, SphereIntersectsSphere) {
    // 创建两个相交的球体
    Sphere sphere1(Vec3(0.0f, 0.0f, 0.0f), 1.0f);
    Sphere sphere2(Vec3(1.0f, 0.0f, 0.0f), 1.0f);
    EXPECT_TRUE(SympConv::SphereIntersectsSphere(sphere1, sphere2));
    
    // 创建两个不相交的球体
    Sphere sphere3(Vec3(3.0f, 0.0f, 0.0f), 1.0f);
    EXPECT_FALSE(SympConv::SphereIntersectsSphere(sphere1, sphere3));
    
    // Corner case: 两个球体相切
    Sphere sphere4(Vec3(0.0f, 0.0f, 0.0f), 1.0f);
    Sphere sphere5(Vec3(2.0f, 0.0f, 0.0f), 1.0f);
    EXPECT_TRUE(SympConv::SphereIntersectsSphere(sphere4, sphere5));
    
    // Corner case: 一个球体完全包含另一个球体
    Sphere sphere6(Vec3(0.0f, 0.0f, 0.0f), 2.0f);
    Sphere sphere7(Vec3(0.0f, 0.0f, 0.0f), 1.0f);
    EXPECT_TRUE(SympConv::SphereIntersectsSphere(sphere6, sphere7));
}

TEST(CollisionDetectionTest, RayIntersectsCapsule) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    float radius = 1.0f;
    Capsule capsule(start, end, radius);
    
    // 从胶囊体前面发射的射线
    Ray ray1(Vec3(0.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t1;
    EXPECT_TRUE(SympConv::RayIntersectsCapsule(ray1, capsule, t1));
    EXPECT_TRUE(FloatEqual(t1, 1.0f)); // 射线从z=-2到胶囊体表面z=-1，距离为1
    
    // 从胶囊体后面发射的射线
    Ray ray2(Vec3(0.0f, 0.0f, 4.0f), Vec3(0.0f, 0.0f, -1.0f));
    float t2;
    EXPECT_TRUE(SympConv::RayIntersectsCapsule(ray2, capsule, t2));
    EXPECT_TRUE(FloatEqual(t2, 1.0f)); // 射线从z=4到胶囊体表面z=3，距离为1
    
    // 不相交的射线
    Ray ray3(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    float t3;
    EXPECT_FALSE(SympConv::RayIntersectsCapsule(ray3, capsule, t3));
    
    // 非AliasAxis情况：射线与倾斜的胶囊体相交
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(2.0f, 2.0f, 2.0f);
    float radius4 = 0.2f; // 减小半径，使胶囊体更细
    Capsule capsule4(start4, end4, radius4);
    
    Ray ray4(Vec3(1.0f, 0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f));
    float t4;
    EXPECT_TRUE(SympConv::RayIntersectsCapsule(ray4, capsule4, t4));
    EXPECT_TRUE(t4 >= 0.0f); // t值应该非负
    
    // 非AliasAxis情况：射线与倾斜的胶囊体不相交
    Ray ray5(Vec3(5.0f, 5.0f, 5.0f), Vec3(1.0f, 0.0f, 0.0f)); // 方向沿着x轴正方向，远离胶囊体
    float t5;
    EXPECT_FALSE(SympConv::RayIntersectsCapsule(ray5, capsule4, t5));
    
    // Corner case: 射线与胶囊体的端点相交
    Ray rayCapsuleEndpoint(Vec3(1.0f, 0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f)); // 指向胶囊体的起点
    float tCapsuleEndpoint;
    EXPECT_TRUE(SympConv::RayIntersectsCapsule(rayCapsuleEndpoint, capsule, tCapsuleEndpoint));
    EXPECT_TRUE(FloatEqual(tCapsuleEndpoint, 0.0f)); // 射线原点在胶囊体表面
    
    // Corner case: 射线与胶囊体的轴线相交
    Ray rayCapsuleAxis(Vec3(0.0f, 0.5f, 1.0f), Vec3(0.0f, -1.0f, 0.0f)); // 垂直于轴线
    float tCapsuleAxis;
    EXPECT_TRUE(SympConv::RayIntersectsCapsule(rayCapsuleAxis, capsule, tCapsuleAxis));
    EXPECT_TRUE(FloatEqual(tCapsuleAxis, 1.5f)); // 射线从y=0.5到胶囊体表面y=-0.5，距离为1.5
    
    // Corner case: 射线与胶囊体相切
    Ray rayCapsuleTangent(Vec3(1.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)); // 距离刚好等于半径，从胶囊体前方发射
    float tCapsuleTangent;
    EXPECT_TRUE(SympConv::RayIntersectsCapsule(rayCapsuleTangent, capsule, tCapsuleTangent));
    EXPECT_TRUE(tCapsuleTangent >= 0.0f); // t值应该非负
    
    // Corner case: 射线原点在胶囊体内
    Ray rayCapsuleInside(Vec3(0.0f, 0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f)); // 原点在胶囊体内
    float tCapsuleInside;
    EXPECT_TRUE(SympConv::RayIntersectsCapsule(rayCapsuleInside, capsule, tCapsuleInside));
    EXPECT_TRUE(FloatEqual(tCapsuleInside, 1.0f)); // 射线从胶囊体中心到表面，距离为1
    
    // Corner case: 射线几乎平行于胶囊体轴线
    Ray rayCapsuleAlmostParallel(Vec3(1.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f)); // 几乎平行
    float tCapsuleParallel;
    EXPECT_TRUE(SympConv::RayIntersectsCapsule(rayCapsuleAlmostParallel, capsule, tCapsuleParallel));
    EXPECT_TRUE(tCapsuleParallel >= 0.0f); // t值应该非负
}

TEST(CollisionDetectionTest, PlaneIntersectsCapsule) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    float radius = 1.0f;
    Capsule capsule(start, end, radius);
    
    // 与胶囊体相交的平面
    Plane plane1(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
    EXPECT_TRUE(SympConv::PlaneIntersectsCapsule(plane1, capsule));
    
    // 与胶囊体不相交的平面
    Plane plane2(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 2.0f, 1.0f));
    EXPECT_FALSE(SympConv::PlaneIntersectsCapsule(plane2, capsule));
    
    // 非AliasAxis情况：平面与倾斜的胶囊体相交
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(2.0f, 2.0f, 2.0f);
    float radius4 = 0.5f;
    Capsule capsule4(start4, end4, radius4);
    
    Plane plane3(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(SympConv::PlaneIntersectsCapsule(plane3, capsule4));
    
    // 非AliasAxis情况：平面与倾斜的胶囊体不相交
    Plane plane4(Vec3(1.0f, 1.0f, 1.0f), Vec3(3.0f, 3.0f, 3.0f));
    EXPECT_FALSE(SympConv::PlaneIntersectsCapsule(plane4, capsule4));
}

TEST(CollisionDetectionTest, BoxIntersectsCapsule) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    float radius = 1.0f;
    Capsule capsule(start, end, radius);
    
    // 与胶囊体相交的盒子
    Box box1(Vec3(0.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(SympConv::BoxIntersectsCapsule(box1, capsule));
    
    // 与胶囊体不相交的盒子
    Box box2(Vec3(2.0f, 2.0f, 1.0f), Vec3(0.5f, 0.5f, 0.5f));
    EXPECT_FALSE(SympConv::BoxIntersectsCapsule(box2, capsule));
    
    // 非AliasAxis情况：盒子与倾斜的胶囊体相交
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(2.0f, 2.0f, 2.0f);
    float radius4 = 0.5f;
    Capsule capsule4(start4, end4, radius4);
    
    Box box3(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(SympConv::BoxIntersectsCapsule(box3, capsule4));
    
    // 非AliasAxis情况：盒子与倾斜的胶囊体不相交
    Box box4(Vec3(4.0f, 4.0f, 4.0f), Vec3(0.5f, 0.5f, 0.5f));
    EXPECT_FALSE(SympConv::BoxIntersectsCapsule(box4, capsule4));
    
    // 非AliasAxis情况：旋转的盒子与倾斜的胶囊体相交
    Box rotatedBox(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f), Quaternion(0.0f, 0.0f, 0.7071f, 0.7071f)); // 绕y轴旋转45度
    EXPECT_TRUE(SympConv::BoxIntersectsCapsule(rotatedBox, capsule4));
}

TEST(CollisionDetectionTest, SphereIntersectsCapsule) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    float radius = 1.0f;
    Capsule capsule(start, end, radius);
    
    // 与胶囊体相交的球体
    Sphere sphere1(Vec3(0.0f, 0.0f, 1.0f), 0.5f);
    EXPECT_TRUE(SympConv::SphereIntersectsCapsule(sphere1, capsule));
    
    // 与胶囊体不相交的球体
    Sphere sphere2(Vec3(2.0f, 2.0f, 1.0f), 0.5f);
    EXPECT_FALSE(SympConv::SphereIntersectsCapsule(sphere2, capsule));
    
    // 非AliasAxis情况：球体与倾斜的胶囊体相交
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(2.0f, 2.0f, 2.0f);
    float radius4 = 0.5f;
    Capsule capsule4(start4, end4, radius4);
    
    Sphere sphere3(Vec3(1.0f, 1.0f, 1.0f), 0.3f);
    EXPECT_TRUE(SympConv::SphereIntersectsCapsule(sphere3, capsule4));
    
    // 非AliasAxis情况：球体与倾斜的胶囊体不相交
    Sphere sphere4(Vec3(3.0f, 3.0f, 3.0f), 0.3f);
    EXPECT_FALSE(SympConv::SphereIntersectsCapsule(sphere4, capsule4));
}

TEST(CollisionDetectionTest, CapsuleIntersectsCapsule) {
    // 相交的两个胶囊体
    Vec3 start1(0.0f, 0.0f, 0.0f);
    Vec3 end1(0.0f, 0.0f, 2.0f);
    float radius1 = 1.0f;
    Capsule capsule1(start1, end1, radius1);
    
    Vec3 start2(1.0f, 0.0f, 1.0f);
    Vec3 end2(1.0f, 0.0f, 3.0f);
    float radius2 = 1.0f;
    Capsule capsule2(start2, end2, radius2);
    
    EXPECT_TRUE(SympConv::CapsuleIntersectsCapsule(capsule1, capsule2));
    
    // 不相交的两个胶囊体
    Vec3 start3(3.0f, 0.0f, 0.0f);
    Vec3 end3(3.0f, 0.0f, 2.0f);
    float radius3 = 0.5f;
    Capsule capsule3(start3, end3, radius3);
    
    EXPECT_FALSE(SympConv::CapsuleIntersectsCapsule(capsule1, capsule3));
    
    // 非AliasAxis情况：两个胶囊体在x-y平面内相交
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(2.0f, 2.0f, 0.0f);
    float radius4 = 0.5f;
    Capsule capsule4(start4, end4, radius4);
    
    Vec3 start5(2.0f, 0.0f, 0.0f);
    Vec3 end5(0.0f, 2.0f, 0.0f);
    float radius5 = 0.5f;
    Capsule capsule5(start5, end5, radius5);
    
    EXPECT_TRUE(SympConv::CapsuleIntersectsCapsule(capsule4, capsule5));
    
    // 非AliasAxis情况：两个胶囊体在空间中交叉
    Vec3 start6(0.0f, 0.0f, 0.0f);
    Vec3 end6(2.0f, 2.0f, 2.0f);
    float radius6 = 0.5f;
    Capsule capsule6(start6, end6, radius6);
    
    Vec3 start7(2.0f, 0.0f, 0.0f);
    Vec3 end7(0.0f, 2.0f, 2.0f);
    float radius7 = 0.5f;
    Capsule capsule7(start7, end7, radius7);
    
    EXPECT_TRUE(SympConv::CapsuleIntersectsCapsule(capsule6, capsule7));
    
    // 非AliasAxis情况：两个胶囊体倾斜不相交
    Vec3 start8(0.0f, 0.0f, 0.0f);
    Vec3 end8(1.0f, 1.0f, 1.0f);
    float radius8 = 0.3f;
    Capsule capsule8(start8, end8, radius8);
    
    Vec3 start9(2.0f, 2.0f, 2.0f);
    Vec3 end9(3.0f, 3.0f, 3.0f);
    float radius9 = 0.3f;
    Capsule capsule9(start9, end9, radius9);
    
    EXPECT_FALSE(SympConv::CapsuleIntersectsCapsule(capsule8, capsule9));
    
    // Corner case: 两个胶囊体相切
    Vec3 start10(0.0f, 0.0f, 0.0f);
    Vec3 end10(0.0f, 0.0f, 2.0f);
    float radius10 = 1.0f;
    Capsule capsule10(start10, end10, radius10);
    
    Vec3 start11(2.0f, 0.0f, 1.0f);
    Vec3 end11(2.0f, 0.0f, 3.0f);
    float radius11 = 1.0f;
    Capsule capsule11(start11, end11, radius11);
    
    EXPECT_TRUE(SympConv::CapsuleIntersectsCapsule(capsule10, capsule11));
    
    // Corner case: 两个胶囊体端点接触
    Vec3 start12(0.0f, 0.0f, 0.0f);
    Vec3 end12(0.0f, 0.0f, 2.0f);
    float radius12 = 1.0f;
    Capsule capsule12(start12, end12, radius12);
    
    Vec3 start13(0.0f, 0.0f, 2.0f);
    Vec3 end13(0.0f, 0.0f, 4.0f);
    float radius13 = 1.0f;
    Capsule capsule13(start13, end13, radius13);
    
    EXPECT_TRUE(SympConv::CapsuleIntersectsCapsule(capsule12, capsule13));
}

TEST(CollisionDetectionTest, RayIntersectsCylinder) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    float radius = 1.0f;
    Cylinder cylinder(start, end, radius);
    
    // 从圆柱体前面发射的射线
    Ray ray1(Vec3(0.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t1;
    EXPECT_TRUE(SympConv::RayIntersectsCylinder(ray1, cylinder, t1));
    EXPECT_TRUE(FloatEqual(t1, 1.0f)); // 射线从z=-2到圆柱体表面z=-1，距离为1
    
    // 从圆柱体后面发射的射线
    Ray ray2(Vec3(0.0f, 0.0f, 4.0f), Vec3(0.0f, 0.0f, -1.0f));
    float t2;
    EXPECT_TRUE(SympConv::RayIntersectsCylinder(ray2, cylinder, t2));
    EXPECT_TRUE(FloatEqual(t2, 1.0f)); // 射线从z=4到圆柱体表面z=3，距离为1
    
    // 不相交的射线
    Ray ray3(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    float t3;
    EXPECT_FALSE(SympConv::RayIntersectsCylinder(ray3, cylinder, t3));
    
    // 非AliasAxis情况：射线与倾斜的圆柱体相交
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(2.0f, 2.0f, 2.0f);
    float radius4 = 0.2f; // 减小半径，使圆柱体更细
    Cylinder cylinder4(start4, end4, radius4);
    
    Ray ray4(Vec3(1.0f, 0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f));
    float t4;
    EXPECT_TRUE(SympConv::RayIntersectsCylinder(ray4, cylinder4, t4));
    EXPECT_TRUE(t4 >= 0.0f); // t值应该非负
    
    // 非AliasAxis情况：射线与倾斜的圆柱体不相交
    Ray ray5(Vec3(5.0f, 5.0f, 5.0f), Vec3(1.0f, 0.0f, 0.0f)); // 方向沿着x轴正方向，远离圆柱体
    float t5;
    EXPECT_FALSE(SympConv::RayIntersectsCylinder(ray5, cylinder4, t5));
    
    // Corner case: 射线与圆柱体的端点相交
    Ray rayCylinderEndpoint(Vec3(1.0f, 0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f)); // 指向圆柱体的起点
    float tCylinderEndpoint;
    EXPECT_TRUE(SympConv::RayIntersectsCylinder(rayCylinderEndpoint, cylinder, tCylinderEndpoint));
    EXPECT_TRUE(FloatEqual(tCylinderEndpoint, 0.0f)); // 射线原点在圆柱体表面
    
    // Corner case: 射线与圆柱体的轴线相交
    Ray rayCylinderAxis(Vec3(0.0f, 0.5f, 1.0f), Vec3(0.0f, -1.0f, 0.0f)); // 垂直于轴线
    float tCylinderAxis;
    EXPECT_TRUE(SympConv::RayIntersectsCylinder(rayCylinderAxis, cylinder, tCylinderAxis));
    EXPECT_TRUE(FloatEqual(tCylinderAxis, 1.5f)); // 射线从y=0.5到圆柱体表面y=-0.5，距离为1.5
    
    // Corner case: 射线与圆柱体相切
    Ray rayCylinderTangent(Vec3(1.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)); // 距离刚好等于半径，从圆柱体前方发射
    float tCylinderTangent;
    EXPECT_TRUE(SympConv::RayIntersectsCylinder(rayCylinderTangent, cylinder, tCylinderTangent));
    EXPECT_TRUE(tCylinderTangent >= 0.0f); // t值应该非负
    
    // Corner case: 射线原点在圆柱体内
    Ray rayCylinderInside(Vec3(0.0f, 0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f)); // 原点在圆柱体内
    float tCylinderInside;
    EXPECT_TRUE(SympConv::RayIntersectsCylinder(rayCylinderInside, cylinder, tCylinderInside));
    EXPECT_TRUE(FloatEqual(tCylinderInside, 1.0f)); // 射线从圆柱体中心到表面，距离为1
    
    // Corner case: 射线几乎平行于圆柱体轴线
    Ray rayCylinderAlmostParallel(Vec3(1.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f)); // 几乎平行
    float tCylinderParallel;
    EXPECT_TRUE(SympConv::RayIntersectsCylinder(rayCylinderAlmostParallel, cylinder, tCylinderParallel));
    EXPECT_TRUE(tCylinderParallel >= 0.0f); // t值应该非负
}

TEST(CollisionDetectionTest, PlaneIntersectsCylinder) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    float radius = 1.0f;
    Cylinder cylinder(start, end, radius);
    
    // 与圆柱体相交的平面
    Plane plane1(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
    EXPECT_TRUE(SympConv::PlaneIntersectsCylinder(plane1, cylinder));
    
    // 与圆柱体不相交的平面
    Plane plane2(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 2.0f, 1.0f));
    EXPECT_FALSE(SympConv::PlaneIntersectsCylinder(plane2, cylinder));
    
    // 非AliasAxis情况：平面与倾斜的圆柱体相交
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(2.0f, 2.0f, 2.0f);
    float radius4 = 0.5f;
    Cylinder cylinder4(start4, end4, radius4);
    
    Plane plane3(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(SympConv::PlaneIntersectsCylinder(plane3, cylinder4));
    
    // 非AliasAxis情况：平面与倾斜的圆柱体不相交
    Plane plane4(Vec3(1.0f, 1.0f, 1.0f), Vec3(3.0f, 3.0f, 3.0f));
    EXPECT_FALSE(SympConv::PlaneIntersectsCylinder(plane4, cylinder4));
}

TEST(CollisionDetectionTest, SphereIntersectsCylinder) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    float radius = 1.0f;
    Cylinder cylinder(start, end, radius);
    
    // 与圆柱体相交的球体
    Sphere sphere1(Vec3(0.0f, 0.0f, 1.0f), 0.5f);
    EXPECT_TRUE(SympConv::SphereIntersectsCylinder(sphere1, cylinder));
    
    // 与圆柱体不相交的球体
    Sphere sphere2(Vec3(2.0f, 2.0f, 1.0f), 0.5f);
    EXPECT_FALSE(SympConv::SphereIntersectsCylinder(sphere2, cylinder));
    
    // 非AliasAxis情况：球体与倾斜的圆柱体相交
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(2.0f, 2.0f, 2.0f);
    float radius4 = 0.5f;
    Cylinder cylinder4(start4, end4, radius4);
    
    Sphere sphere3(Vec3(1.0f, 1.0f, 1.0f), 0.3f);
    EXPECT_TRUE(SympConv::SphereIntersectsCylinder(sphere3, cylinder4));
    
    // 非AliasAxis情况：球体与倾斜的圆柱体不相交
    Sphere sphere4(Vec3(3.0f, 3.0f, 3.0f), 0.3f);
    EXPECT_FALSE(SympConv::SphereIntersectsCylinder(sphere4, cylinder4));
}

TEST(CollisionDetectionTest, CylinderIntersectsCylinder) {
    // 相交的两个圆柱体
    Vec3 start1(0.0f, 0.0f, 0.0f);
    Vec3 end1(0.0f, 0.0f, 2.0f);
    float radius1 = 1.0f;
    Cylinder cylinder1(start1, end1, radius1);
    
    Vec3 start2(1.0f, 0.0f, 1.0f);
    Vec3 end2(1.0f, 0.0f, 3.0f);
    float radius2 = 1.0f;
    Cylinder cylinder2(start2, end2, radius2);
    
    EXPECT_TRUE(SympConv::CylinderIntersectsCylinder(cylinder1, cylinder2));
    
    // 不相交的两个圆柱体
    Vec3 start3(3.0f, 0.0f, 0.0f);
    Vec3 end3(3.0f, 0.0f, 2.0f);
    float radius3 = 0.5f;
    Cylinder cylinder3(start3, end3, radius3);
    
    EXPECT_FALSE(SympConv::CylinderIntersectsCylinder(cylinder1, cylinder3));
    
    // 非AliasAxis情况：两个圆柱体在x-y平面内相交
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(2.0f, 2.0f, 0.0f);
    float radius4 = 0.5f;
    Cylinder cylinder4(start4, end4, radius4);
    
    Vec3 start5(2.0f, 0.0f, 0.0f);
    Vec3 end5(0.0f, 2.0f, 0.0f);
    float radius5 = 0.5f;
    Cylinder cylinder5(start5, end5, radius5);
    
    EXPECT_TRUE(SympConv::CylinderIntersectsCylinder(cylinder4, cylinder5));
    
    // 非AliasAxis情况：两个圆柱体在空间中交叉
    Vec3 start6(0.0f, 0.0f, 0.0f);
    Vec3 end6(2.0f, 2.0f, 2.0f);
    float radius6 = 0.5f;
    Cylinder cylinder6(start6, end6, radius6);
    
    Vec3 start7(2.0f, 0.0f, 0.0f);
    Vec3 end7(0.0f, 2.0f, 2.0f);
    float radius7 = 0.5f;
    Cylinder cylinder7(start7, end7, radius7);
    
    EXPECT_TRUE(SympConv::CylinderIntersectsCylinder(cylinder6, cylinder7));
    
    // 非AliasAxis情况：两个圆柱体倾斜不相交
    Vec3 start8(0.0f, 0.0f, 0.0f);
    Vec3 end8(1.0f, 1.0f, 1.0f);
    float radius8 = 0.3f;
    Cylinder cylinder8(start8, end8, radius8);
    
    Vec3 start9(2.0f, 2.0f, 2.0f);
    Vec3 end9(3.0f, 3.0f, 3.0f);
    float radius9 = 0.3f;
    Cylinder cylinder9(start9, end9, radius9);
    
    EXPECT_FALSE(SympConv::CylinderIntersectsCylinder(cylinder8, cylinder9));
    
    // Corner case: 两个圆柱体相切
    Vec3 start10(0.0f, 0.0f, 0.0f);
    Vec3 end10(0.0f, 0.0f, 2.0f);
    float radius10 = 1.0f;
    Cylinder cylinder10(start10, end10, radius10);
    
    Vec3 start11(2.0f, 0.0f, 1.0f);
    Vec3 end11(2.0f, 0.0f, 3.0f);
    float radius11 = 1.0f;
    Cylinder cylinder11(start11, end11, radius11);
    
    EXPECT_TRUE(SympConv::CylinderIntersectsCylinder(cylinder10, cylinder11));
    
    // Corner case: 两个圆柱体端点接触
    Vec3 start12(0.0f, 0.0f, 0.0f);
    Vec3 end12(0.0f, 0.0f, 2.0f);
    float radius12 = 1.0f;
    Cylinder cylinder12(start12, end12, radius12);
    
    Vec3 start13(0.0f, 0.0f, 2.0f);
    Vec3 end13(0.0f, 0.0f, 4.0f);
    float radius13 = 1.0f;
    Cylinder cylinder13(start13, end13, radius13);
    
    EXPECT_TRUE(SympConv::CylinderIntersectsCylinder(cylinder12, cylinder13));
}

} // namespace SympConvTest