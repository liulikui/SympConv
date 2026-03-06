#ifndef SYMPCONV_PHYSIC_WORLD_H
#define SYMPCONV_PHYSIC_WORLD_H

#include "Export.h"

namespace SympConv {

class SYMPCONV_API PhysicWorld
{
public:
	PhysicWorld() = default;
	void Update(float dt);
};

SYMPCONV_API PhysicWorld* CreatePhysicWorld();
SYMPCONV_API void DestroyPhysicWorld(PhysicWorld* world);
}

#endif // SYMPCONV_PHYSIC_WORLD_H