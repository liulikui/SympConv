#include "SympConv/PhysicWorld.h"

namespace SympConv {

void PhysicWorld::Update(float dt) {
	// 空实现，后续可以添加物理模拟逻辑
}

SYMPCONV_API PhysicWorld* CreatePhysicWorld() {
	return new PhysicWorld();
}

SYMPCONV_API void DestroyPhysicWorld(PhysicWorld* world) {
	delete world;
}

}