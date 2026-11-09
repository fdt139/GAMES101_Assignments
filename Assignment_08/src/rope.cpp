#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL {

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.

//        Comment-in this part when you implement the constructor
		Vector2D delta = (end - start) / (num_nodes - 1);
        for (int i = 0; i < num_nodes; i++) {
              masses.push_back(new Mass(start + i * delta, node_mass, false));
			  if (i > 0) {
                  springs.push_back(new Spring(masses[i - 1], masses[i], k));
			  }
          }
		  masses[0]->pinned = true;
    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 2): Use Hooke's law to calculate the force on a node
			s->m1->forces += -s->k * (s->m1->position - s->m2->position).unit() * ((s->m1->position - s->m2->position).norm() - s->rest_length);
            s->m2->forces += s->k * (s->m1->position - s->m2->position).unit() * ((s->m1->position - s->m2->position).norm() - s->rest_length);
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position
				m->forces += gravity * m->mass;
				Vector2D acceleration = m->forces / m->mass;
				m->velocity += acceleration * delta_t;
				m->velocity *= 0.99995; // global damping
				m->position += m->velocity * delta_t;
                // TODO (Part 2): Add global damping

            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
			Vector2D delta_p = s->m1->position - s->m2->position;
			Vector2D delta_p_norm = delta_p.unit();
            if (!s->m1->pinned) {
                s->m1->position -= 0.5 * (delta_p_norm * (delta_p.norm() - s->rest_length));
                s->m2->position += 0.5 * (delta_p_norm * (delta_p.norm() - s->rest_length));
            }
            else {
                s->m2->position += delta_p_norm * (delta_p.norm() - s->rest_length);
            }
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                Vector2D temp_position = m->position;
                float damping_factor = 0.00005;
                // TODO (Part 3.1): Set the new position of the rope mass
				m->position = m->position + (1 - damping_factor) * (m->position - m->last_position) + gravity * delta_t * delta_t;
				m->last_position = temp_position;
                // TODO (Part 4): Add global Verlet damping

            }
        }
    }
}
