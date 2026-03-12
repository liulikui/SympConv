#include <gtest/gtest.h>
#include "SympConv/Box.h"
#include "SympConv/BoxShape.h"
#include "SympConv/Vector.h"
#include "SympConv/AABB.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Box = SympConv::Box;
using BoxShape = SympConv::BoxShape;
using Vec3 = SympConv::Vector3;
using AABB = SympConv::AABB;

TEST(BoxShapeTest, GetLocalSupport) {
    // 创建一个半长为(2, 1.5, 1)的盒子
    Vec3 halfExtents(Real(2.0), Real(1.5), Real(1.0));
    Box box(halfExtents);
    BoxShape shape(box);
    
    // 测试正X方向
    Vec3 dir_x_pos(1, 0, 0);
    Vec3 support_x_pos = shape.GetLocalSupport(dir_x_pos);
    EXPECT_TRUE(FloatEqual(support_x_pos.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(support_x_pos.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(support_x_pos.z, Real(0.0)));
    
    // 测试负X方向
    Vec3 dir_x_neg(-1, 0, 0);
    Vec3 support_x_neg = shape.GetLocalSupport(dir_x_neg);
    EXPECT_TRUE(FloatEqual(support_x_neg.x, Real(-2.0)));
    EXPECT_TRUE(FloatEqual(support_x_neg.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(support_x_neg.z, Real(0.0)));
    
    // 测试正Y方向
    Vec3 dir_y_pos(0, 1, 0);
    Vec3 support_y_pos = shape.GetLocalSupport(dir_y_pos);
    EXPECT_TRUE(FloatEqual(support_y_pos.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(support_y_pos.y, Real(1.5)));
    EXPECT_TRUE(FloatEqual(support_y_pos.z, Real(0.0)));
    
    // 测试负Y方向
    Vec3 dir_y_neg(0, -1, 0);
    Vec3 support_y_neg = shape.GetLocalSupport(dir_y_neg);
    EXPECT_TRUE(FloatEqual(support_y_neg.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(support_y_neg.y, Real(-1.5)));
    EXPECT_TRUE(FloatEqual(support_y_neg.z, Real(0.0)));
    
    // 测试正Z方向
    Vec3 dir_z_pos(0, 0, 1);
    Vec3 support_z_pos = shape.GetLocalSupport(dir_z_pos);
    EXPECT_TRUE(FloatEqual(support_z_pos.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(support_z_pos.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(support_z_pos.z, Real(1.0)));
    
    // 测试负Z方向
    Vec3 dir_z_neg(0, 0, -1);
    Vec3 support_z_neg = shape.GetLocalSupport(dir_z_neg);
    EXPECT_TRUE(FloatEqual(support_z_neg.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(support_z_neg.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(support_z_neg.z, Real(-1.0)));
    
    // 测试斜向方向
    Vec3 dir_diagonal(1, 1, 1);
    Vec3 support_diagonal = shape.GetLocalSupport(dir_diagonal);
    EXPECT_TRUE(FloatEqual(support_diagonal.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(support_diagonal.y, Real(1.5)));
    EXPECT_TRUE(FloatEqual(support_diagonal.z, Real(1.0)));
    
    // 测试负斜向方向
    Vec3 dir_neg_diagonal(-1, -1, -1);
    Vec3 support_neg_diagonal = shape.GetLocalSupport(dir_neg_diagonal);
    EXPECT_TRUE(FloatEqual(support_neg_diagonal.x, Real(-2.0)));
    EXPECT_TRUE(FloatEqual(support_neg_diagonal.y, Real(-1.5)));
    EXPECT_TRUE(FloatEqual(support_neg_diagonal.z, Real(-1.0)));
    
    // 测试零方向向量
    Vec3 dir_zero(0, 0, 0);
    Vec3 support_zero = shape.GetLocalSupport(dir_zero);
    EXPECT_TRUE(FloatEqual(support_zero.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(support_zero.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(support_zero.z, Real(0.0)));
}

TEST(BoxShapeTest, GetLocalSupport_MixedDirections) {
    // 创建一个半长为(3, 2, 1.5)的盒子
    Vec3 halfExtents(Real(3.0), Real(2.0), Real(1.5));
    Box box(halfExtents);
    BoxShape shape(box);
    
    // 测试X正、Y负、Z正方向
    Vec3 dir_mixed1(1, -1, 1);
    Vec3 support_mixed1 = shape.GetLocalSupport(dir_mixed1);
    EXPECT_TRUE(FloatEqual(support_mixed1.x, Real(3.0)));
    EXPECT_TRUE(FloatEqual(support_mixed1.y, Real(-2.0)));
    EXPECT_TRUE(FloatEqual(support_mixed1.z, Real(1.5)));
    
    // 测试X负、Y正、Z负方向
    Vec3 dir_mixed2(-1, 1, -1);
    Vec3 support_mixed2 = shape.GetLocalSupport(dir_mixed2);
    EXPECT_TRUE(FloatEqual(support_mixed2.x, Real(-3.0)));
    EXPECT_TRUE(FloatEqual(support_mixed2.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(support_mixed2.z, Real(-1.5)));
}

TEST(BoxShapeTest, GetLocalInertiaTensor) {
    // 创建一个半长为(1, 1, 1)的盒子（边长为2x2x2）
    Vec3 halfExtents(Real(1.0), Real(1.0), Real(1.0));
    Box box(halfExtents);
    BoxShape shape(box);
    
    // 测试质量为12的情况，此时惯性张量应该为(8, 8, 8)
    // Ix = (1/12) * 12 * (2² + 2²) = 8
    Real mass = Real(12.0);
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    EXPECT_TRUE(FloatEqual(inertia.x, Real(8.0)));
    EXPECT_TRUE(FloatEqual(inertia.y, Real(8.0)));
    EXPECT_TRUE(FloatEqual(inertia.z, Real(8.0)));
    
    // 测试质量为6的情况，此时惯性张量应该为(4, 4, 4)
    // Ix = (1/12) * 6 * (2² + 2²) = 4
    mass = Real(6.0);
    inertia = shape.GetLocalInertiaTensor(mass);
    EXPECT_TRUE(FloatEqual(inertia.x, Real(4.0)));
    EXPECT_TRUE(FloatEqual(inertia.y, Real(4.0)));
    EXPECT_TRUE(FloatEqual(inertia.z, Real(4.0)));
}

TEST(BoxShapeTest, GetLocalInertiaTensor_DifferentSize) {
    // 创建一个半长为(2, 3, 4)的盒子（边长为4x6x8）
    Vec3 halfExtents(Real(2.0), Real(3.0), Real(4.0));
    Box box(halfExtents);
    BoxShape shape(box);
    
    // 计算期望的惯性张量
    // Ix = (1/12) * m * (height² + depth²) = (1/12) * 12 * (6² + 8²) = (36 + 64) = 100
    // Iy = (1/12) * m * (width² + depth²) = (1/12) * 12 * (4² + 8²) = (16 + 64) = 80
    // Iz = (1/12) * m * (width² + height²) = (1/12) * 12 * (4² + 6²) = (16 + 36) = 52
    Real mass = Real(12.0);
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    EXPECT_TRUE(FloatEqual(inertia.x, Real(100.0)));
    EXPECT_TRUE(FloatEqual(inertia.y, Real(80.0)));
    EXPECT_TRUE(FloatEqual(inertia.z, Real(52.0)));
}

TEST(BoxShapeTest, GetLocalBounds) {
    // 创建一个半长为(2, 1.5, 1)的盒子
    Vec3 halfExtents(Real(2.0), Real(1.5), Real(1.0));
    Box box(halfExtents);
    BoxShape shape(box);
    
    // 获取本地坐标系中的AABB
    AABB bounds = shape.GetLocalBounds();
    
    // 验证AABB的最小和最大点
    EXPECT_TRUE(FloatEqual(bounds.mMin.x, Real(-2.0)));
    EXPECT_TRUE(FloatEqual(bounds.mMin.y, Real(-1.5)));
    EXPECT_TRUE(FloatEqual(bounds.mMin.z, Real(-1.0)));
    
    EXPECT_TRUE(FloatEqual(bounds.mMax.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(bounds.mMax.y, Real(1.5)));
    EXPECT_TRUE(FloatEqual(bounds.mMax.z, Real(1.0)));
    
    // 验证AABB的中心
    Vec3 center = bounds.GetCenter();
    EXPECT_TRUE(FloatEqual(center.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(center.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(center.z, Real(0.0)));
    
    // 验证AABB的大小
    Vec3 size = bounds.GetSize();
    EXPECT_TRUE(FloatEqual(size.x, Real(4.0)));
    EXPECT_TRUE(FloatEqual(size.y, Real(3.0)));
    EXPECT_TRUE(FloatEqual(size.z, Real(2.0)));
}

TEST(BoxShapeTest, GetLocalBounds_DifferentSize) {
    // 创建一个半长为(3, 2, 1.5)的盒子
    Vec3 halfExtents(Real(3.0), Real(2.0), Real(1.5));
    Box box(halfExtents);
    BoxShape shape(box);
    
    // 获取本地坐标系中的AABB
    AABB bounds = shape.GetLocalBounds();
    
    // 验证AABB的最小和最大点
    EXPECT_TRUE(FloatEqual(bounds.mMin.x, Real(-3.0)));
    EXPECT_TRUE(FloatEqual(bounds.mMin.y, Real(-2.0)));
    EXPECT_TRUE(FloatEqual(bounds.mMin.z, Real(-1.5)));
    
    EXPECT_TRUE(FloatEqual(bounds.mMax.x, Real(3.0)));
    EXPECT_TRUE(FloatEqual(bounds.mMax.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(bounds.mMax.z, Real(1.5)));
    
    // 验证AABB的大小
    Vec3 size = bounds.GetSize();
    EXPECT_TRUE(FloatEqual(size.x, Real(6.0)));
    EXPECT_TRUE(FloatEqual(size.y, Real(4.0)));
    EXPECT_TRUE(FloatEqual(size.z, Real(3.0)));
}

TEST(BoxShapeTest, CustomSizeConstructor) {
    // 测试自定义尺寸构造（width=2, height=3, depth=4）
    Vec3 halfExtents(Real(1.0), Real(1.5), Real(2.0)); // 半长为0.5*边长
    Box box(halfExtents);
    BoxShape shape(box);
    
    // 验证盒子对象
    const Box& retrievedBox = shape.GetBox();
    EXPECT_TRUE(FloatEqual(retrievedBox.mHalfExtents.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(retrievedBox.mHalfExtents.y, Real(1.5)));
    EXPECT_TRUE(FloatEqual(retrievedBox.mHalfExtents.z, Real(2.0)));
    
    // 验证AABB
    AABB bounds = shape.GetLocalBounds();
    EXPECT_TRUE(FloatEqual(bounds.mMin.x, Real(-1.0)));
    EXPECT_TRUE(FloatEqual(bounds.mMin.y, Real(-1.5)));
    EXPECT_TRUE(FloatEqual(bounds.mMin.z, Real(-2.0)));
    EXPECT_TRUE(FloatEqual(bounds.mMax.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(bounds.mMax.y, Real(1.5)));
    EXPECT_TRUE(FloatEqual(bounds.mMax.z, Real(2.0)));
}

TEST(BoxShapeTest, NegativeSizeConstructor) {
    // 测试非正尺寸构造
    // 注意：这里假设Box构造函数会处理非正尺寸的情况
    Vec3 negativeExtents(Real(-1.0), Real(2.0), Real(3.0));
    Box box(negativeExtents);
    BoxShape shape(box);
    
    // 验证盒子对象（假设Box会将负值修正为正值）
    const Box& retrievedBox = shape.GetBox();
    EXPECT_TRUE(retrievedBox.mHalfExtents.x >= Real(0.0));
    EXPECT_TRUE(retrievedBox.mHalfExtents.y >= Real(0.0));
    EXPECT_TRUE(retrievedBox.mHalfExtents.z >= Real(0.0));
}

TEST(BoxShapeTest, VolumeTest) {
    // 测试体积计算
    // 半长为(1, 1, 1)的盒子，体积应该是8.0
    Vec3 halfExtents(Real(1.0), Real(1.0), Real(1.0));
    Box box(halfExtents);
    BoxShape shape(box);
    
    Real volume = shape.GetVolume();
    EXPECT_TRUE(FloatEqual(volume, Real(8.0)));
    
    // 测试不同尺寸的体积
    Vec3 halfExtents2(Real(2.0), Real(3.0), Real(4.0));
    Box box2(halfExtents2);
    BoxShape shape2(box2);
    
    Real volume2 = shape2.GetVolume();
    EXPECT_TRUE(FloatEqual(volume2, Real(2.0 * 2 * 3.0 * 2 * 4.0 * 2))); // 8 * 2*3*4 = 192
}

TEST(BoxShapeTest, GetterMethods) {
    // 测试getter方法
    Vec3 halfExtents(Real(1.5), Real(2.5), Real(3.5));
    Box box(halfExtents);
    BoxShape shape(box);
    
    // 测试GetBox()
    const Box& retrievedBox = shape.GetBox();
    EXPECT_TRUE(FloatEqual(retrievedBox.mHalfExtents.x, Real(1.5)));
    EXPECT_TRUE(FloatEqual(retrievedBox.mHalfExtents.y, Real(2.5)));
    EXPECT_TRUE(FloatEqual(retrievedBox.mHalfExtents.z, Real(3.5)));
    
    // 测试GetLocalBounds()（间接测试getCenter和getExtents）
    AABB bounds = shape.GetLocalBounds();
    Vec3 center = bounds.GetCenter();
    EXPECT_TRUE(FloatEqual(center.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(center.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(center.z, Real(0.0)));
    
    Vec3 size = bounds.GetSize();
    EXPECT_TRUE(FloatEqual(size.x, Real(3.0))); // 2 * 1.5
    EXPECT_TRUE(FloatEqual(size.y, Real(5.0))); // 2 * 2.5
    EXPECT_TRUE(FloatEqual(size.z, Real(7.0))); // 2 * 3.5
}

} // namespace SympConvTest
