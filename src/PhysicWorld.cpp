#include "SympConv/PhysicWorld.h"

namespace SympConv {


SYMPCONV_API PhysicWorld* CreatePhysicWorld() {
	return new PhysicWorld();
}

SYMPCONV_API void DestroyPhysicWorld(PhysicWorld* world) {
	delete world;
}

}