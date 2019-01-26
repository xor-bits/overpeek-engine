#pragma once

#include "../engine.h"

#define STRONG_FORCE_RADIUS		1

#define STRONG_FORCE_STR		5
#define ELECTR_FORCE_STR		STRONG_FORCE_STR / 137.0
//#define GRAV_FORCE_STR			STRONG_FORCE_STR * 10e-39
#define GRAV_FORCE_STR			STRONG_FORCE_STR / 1000000000


class Particle {
	private:
		double m_x, m_y;
		double m_vel_x, m_vel_y;
		double m_acc_x, m_acc_y;

		double m_mass;
		double m_charge;

	public:
		Particle(float x, float y, float vel_x, float vel_y, float n, float mass) : Particle(x, y, vel_x, vel_y, n) {
			m_mass = mass;
		}

		Particle(float x, float y, float vel_x, float vel_y, float n) : Particle(x, y, n) {
			m_vel_x = vel_x;
			m_vel_y = vel_y;
		}

		Particle(float x, float y, float vel_x, float vel_y) : Particle(x, y) {
			m_vel_x = vel_x;
			m_vel_y = vel_y;
		}

		Particle(float x, float y, float n) : Particle(x, y) {
			m_charge = n;

			if (m_charge == 1.0) { m_charge = 1.0; m_mass = 1835; }
			else if (m_charge == 0.0) { m_charge = 0.0; m_mass = 1835; }
			else if (m_charge == -1.0) { m_charge = -1.0; m_mass = 1; }
		}

		Particle(float x, float y) {
			m_x = x;
			m_y = y;
			m_vel_x = 0.0;
			m_vel_y = 0.0;
			m_acc_x = 0.0;
			m_acc_y = 0.0;

			float n = tools::Random::random(0.0, 1.0);
			if (n >= 0.66) { m_charge = 1.0; m_mass = 1835; }
			else if (n >= 0.33) { m_charge = 0.0; m_mass = 1835; }
			else { m_charge = -1.0; m_mass = 1; }
		}

		void preupdate(Particle **particles, unsigned int count, bool strong_force, bool electro_force, bool grav_force) {

			for (unsigned int i = 0; i < count; i++)
			{
				if (particles[i] == this) continue;

				float distance = sqrt(
					pow(abs(particles[i]->m_x - m_x), 2) +
					pow(abs(particles[i]->m_y - m_y), 2)
				);
				glm::vec2 normalVector = glm::vec2(particles[i]->m_x - m_x, particles[i]->m_y - m_y);

				//The Strong Force
				if (strong_force) {
					if (particles[i]->m_charge == 1.0 || particles[i]->m_charge == 0.0) {
						if (m_charge == 1.0 || m_charge == 0.0) {
							if (distance <= STRONG_FORCE_RADIUS) {
								//normalVector = glm::normalize(normalVector);
								applyForceX(normalVector.x * STRONG_FORCE_STR);
								applyForceY(normalVector.y * STRONG_FORCE_STR);

								m_vel_x *= 0.99;
								m_vel_y *= 0.99;
							}
						}
					}
				}

				//No division by 0
				if (pow(distance, 2) == 0) continue;

				//The Electromagnetic Force
				if (electro_force) {
					float strength = (particles[i]->m_charge * m_charge) / pow(distance, 2);
					normalVector = rotatePoint(0.0, 0.0, 0.1, normalVector);
					applyForceX(normalVector.x * strength * -ELECTR_FORCE_STR);
					applyForceY(normalVector.y * strength * -ELECTR_FORCE_STR);
				}

				//The Gravitational Force
				if (grav_force) {
					if (particles[i]->m_charge == 1.0 || particles[i]->m_charge == 0.0) {
						if (m_charge == 1.0 || m_charge == 0.0) {
							float force = (particles[i]->m_mass * m_mass) / pow(distance, 2);
							applyForceX(normalVector.x * force * GRAV_FORCE_STR);
							applyForceY(normalVector.y * force * GRAV_FORCE_STR);
						}
					}
				}
			}
		}

		void update() {
			m_x += m_vel_x;
			m_y += m_vel_y;

			m_vel_x += m_acc_x;
			m_vel_y += m_acc_y;

			m_vel_x *= 0.998;
			m_vel_y *= 0.998;

			m_acc_x = 0;
			m_acc_y = 0;
		}

		void render(graphics::Renderer *renderer, double zoom, double xo, double yo) {
			float renderSize = 1;
			double rx = (m_x + xo) * zoom;
			double ry = (m_y + yo) * zoom;
			if (m_charge == 1.0) renderer->renderBox(rx, ry, renderSize, renderSize, 0, 24, glm::vec4(1.0, 0.0, 0.0, 1.0));
			else if (m_charge == 0.0) renderer->renderBox(rx, ry, renderSize, renderSize, 0, 24, glm::vec4(0.7, 0.7, 0.7, 1.0));
			else renderer->renderBox(rx, ry, renderSize, renderSize, 0, 24, glm::vec4(0.0, 0.0, 1.0, 1.0));
		}

		void applyForceX(double force) {
			m_acc_x += force / m_mass;
		}

		void applyForceY(double force) {
			m_acc_y += force / m_mass;
		}

		double getPotentialEnergy() {
			return 1 - getKineticEnergy();
			//return m_mass * sqrt(pow(m_acc_x, 2) + pow(m_acc_y, 2));
		}

		double getKineticEnergy() {
			return 0.5 * m_mass * (pow(m_vel_x, 2) + pow(m_vel_y, 2));
		}

		bool electron() {
			if (m_charge == -1.0) return true;
			
			return false;
		}

		bool proton() {
			if (m_charge == 1.0) return true;

			return false;
		}

		bool neutron() {
			if (m_charge == 0.0) return true;

			return false;
		}

		glm::vec2 rotatePoint(float cx, float cy, float angle, glm::vec2 p)
		{
			float s = sin(angle);
			float c = cos(angle);

			// translate point back to origin:
			p.x -= cx;
			p.y -= cy;

			// rotate point
			float xnew = p.x * c - p.y * s;
			float ynew = p.x * s + p.y * c;

			// translate point back:
			p.x = xnew + cx;
			p.y = ynew + cy;
			return p;
		}
};