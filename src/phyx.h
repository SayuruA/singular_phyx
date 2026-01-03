#ifndef MY_PHYSX_ENG
#define MY_PHYSX_ENG

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Objects.h"
#include <memory>

// define newtonion constants
float GM = 4.8; // 1.3 // 2.8

// using namespace glm;


struct PhysObj{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force = glm::vec3(0, 0, 0);
	glm::quat orientation;
	glm::vec3 ang_velocity;
    float mass;

    PhysObj(glm::vec3 start_pos, glm::vec3 start_velo, float m,
		 glm::vec3 ang_velo = glm::vec3(1, 0, 0), glm::vec3 rot = glm::vec3(0, 0, 0))
    
		:position(start_pos), velocity(start_velo), 
		mass(m), orientation(rot), ang_velocity(ang_velo){
    }
};


struct Celestial{
	std::unique_ptr<RenderObj> renderable;
	std::unique_ptr<PhysObj> physObj;
	bool track_locus = false;
	std::unique_ptr<Locus> locus;
	glm::vec3 prev_position;

	Celestial(glm::vec3& start_pos, 
				glm::vec3& start_vel, float mass, 
					float r, bool track, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)){

		renderable.reset(new Sphere(r, 15, std::move(glm::vec3(0.0f)), std::move(color)));
		physObj.reset(new PhysObj(start_pos, start_vel, mass));
		track_locus = track;
		locus.reset(new Locus());
		prev_position = start_pos;
	}
};




// a newtonian universe with a point gravity at 0,0,0
class Universe_With_One_Star {
private:
	std::vector<Celestial*> object_ptrs;
	glm::vec3 gravity_point = glm::vec3(0.0f);
	glm::mat4 view, projection;

	// float FIXED_DT = 0.0080f;
	// int update_every = 10;
	

 
public:
	// void AddObject   (Object* object) { /* ... */ }
	// void RemoveObject(Object* object) { /* ... */ }
 
    Universe_With_One_Star(std::vector<Celestial*>&& obj_ptrs, glm::vec3 grvt, 
		glm::mat4 v, glm::mat4 proj)
        :object_ptrs(move(obj_ptrs)), gravity_point(grvt), view(v), projection(proj){
    }

	// void set_timestep_and_update_frequency(){}

	void update_camera(glm::mat4 v, glm::mat4 proj){
		view = v;
		projection = proj; 
	}

	// take one stochastic step
	void physx_update(float dt)
	{
		for (Celestial* c_obj : object_ptrs) {

			PhysObj* obj = c_obj->physObj.get();
			c_obj->prev_position = obj->position; // store current position

			// compute force
            float dist = glm::length(obj->position - gravity_point);
            float dist2 = dist * dist + 1e-6;

			// check if the object is too close to point of gravity
			if (dist >= 0.01){
				glm::vec3 gravity = -glm::normalize(obj->position - gravity_point) * GM /dist2;
				obj->force += obj->mass * gravity; // apply a force
				// update state
				obj->velocity += obj->force / obj->mass * dt;
				obj->position += obj->velocity * dt;
			//..if too close
			}else{
				obj->position = glm::vec3(0.0f);
				obj->velocity = glm::vec3(0.0f);
			}

			

			

			glm::quat deltaRotation = glm::quat(obj->ang_velocity * dt);
			obj->orientation = glm::normalize(obj->orientation * deltaRotation);

			obj->force = glm::vec3(0.0f); // reset net force at the end
		}
	}

	void frame_update(int frame_counter, const int update_every){
		
		for (Celestial* c_obj : object_ptrs) {
			PhysObj* p_obj = c_obj->physObj.get();
			RenderObj* r_obj  = c_obj->renderable.get();
			Locus* locus = c_obj->locus.get();

			// if (frame_counter % update_every == 0) {
            // 	physx_update(FIXED_DT);
        	// }

			// Interpolation factor [0, 1] within the 20-frame window
			float alpha = (float) (frame_counter % update_every) / update_every;
			glm::vec3 interpolated_position = glm::mix(c_obj->prev_position, p_obj->position, alpha);
			glm::vec3 interpolated_orientation = glm::mix(c_obj->prev_position, p_obj->position, alpha);

			locus->updateLocus(interpolated_position);

			// to do - update model matrix................
			glm::mat4 model_rotate = glm::mat4_cast(p_obj->orientation);
			glm::mat4 model_translate = glm::translate(glm::mat4(1.0f), interpolated_position);
			r_obj->updateModel(model_translate* model_rotate);
		}
		frame_counter++;
	}

	void draw(GLint locModel, GLint locView, GLint locProj, GLint colorLoc){

		glUniformMatrix4fv(locView,  1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(locProj,  1, GL_FALSE, glm::value_ptr(projection));


		for (Celestial* c_obj : object_ptrs) {
			RenderObj* r_obj  = c_obj->renderable.get();
			r_obj->draw(locModel, colorLoc);
			
			if (c_obj->track_locus){
				Locus* locus = c_obj->locus.get();
				locus->draw(locModel, colorLoc);
			}
			
		}
	}

};


#endif