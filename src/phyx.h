#ifndef MY_PHYSX_ENG
#define MY_PHYSX_ENG

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


// define newtonion constants
float GM = 4.8; // 1.3 // 2.8


struct PhysObj{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
    float mass;

    PhysObj(glm::vec3 start_pos, glm::vec3 start_velo, float m , glm::vec3 start_force = glm::vec3(0, 0, 0))
    :position(start_pos), velocity(start_velo), force(start_force), mass(m){
    }
};


// a newtonian universe with a point gravity at 0,0,0
class Physx_Universe_One_GPoint {
private:
	std::vector<PhysObj*> object_ptrs;
	glm::vec3 gravity_point = glm::vec3(0, 0, 0);
 
public:
	// void AddObject   (Object* object) { /* ... */ }
	// void RemoveObject(Object* object) { /* ... */ }
 
    Physx_Universe_One_GPoint(std::vector<PhysObj*>&& obj_ptrs, glm::vec3 grvt)
        :object_ptrs(move(obj_ptrs)), gravity_point(grvt){
    }

	void linear_step(float dt)
	{
		for (PhysObj* obj : object_ptrs) {

            float dist = glm::length(obj->position - gravity_point);
            float dist2 = dist * dist + 0.000001;
			glm::vec3 gravity = -glm::normalize(obj->position - gravity_point) * GM /dist2;
			obj->force += obj->mass * gravity; // apply a force

			obj->velocity += obj->force / obj->mass * dt;
			obj->position += obj->velocity * dt;
 
			obj->force = glm::vec3(0, 0, 0); // reset net force at the end
		}
	}
};


#endif