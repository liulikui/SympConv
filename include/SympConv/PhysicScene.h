#ifndef SYMPCONV_PHYSIC_SCENE_H
#define SYMPCONV_PHYSIC_SCENE_H

namespace SympConv {
class IPhysicScene {
public:
	virtual ~IPhysicScene() = default;
	virtual void Update(float dt) = 0;
};
}

#endif // SYMPCONV_PHYSIC_SCENE_H