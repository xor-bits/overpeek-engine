#pragma once

#include "pch.h"

#define STRONG_FORCE_RADIUS		3

#define STRONG_FORCE_STR		20
#define ELECTR_FORCE_STR		STRONG_FORCE_STR / 137.0
//#define GRAV_FORCE_STR			STRONG_FORCE_STR * 10e-39
#define GRAV_FORCE_STR			STRONG_FORCE_STR / 1000000000


#define EULER_NUMBER 2.71828


double closeZero(float a, float b) {
	return abs(a) > abs(b) ? b : a;
}

double farZero(float a, float b) {
	return abs(a) < abs(b) ? b : a;
}

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

			float n = oe::Random::random(0.0, 1.0);
			if (n >= 0.66) { m_charge = 1.0; m_mass = 1835; }
			else if (n >= 0.33) { m_charge = 0.0; m_mass = 1835; }
			else { m_charge = -1.0; m_mass = 1; }
		}

		void preupdate(Particle **particles, unsigned int count, double force_mult, bool strong_force, bool electro_force, bool grav_force) {
			glm::vec2 forces = calcForces(this, particles, count, force_mult, strong_force, electro_force, grav_force);
			applyForceX(forces.x);
			applyForceY(forces.y);

			//m_vel_x *= 1.000 - 0.002 * force_mult;
			//m_vel_y *= 1.000 - 0.002 * force_mult;
		}

		static glm::vec2 calcForces(Particle *from, Particle **particles, double force_mult, unsigned int count, bool strong_force, bool electro_force, bool grav_force) {
			float force_x = 0, force_y = 0;
			for (unsigned int i = 0; i < count; i++)
			{
				if (!particles[i]) continue;
				if (particles[i] == from) continue;

				float distance = pow(abs(particles[i]->m_x - from->m_x), 2) + pow(abs(particles[i]->m_y - from->m_y), 2);


				//Dir
				glm::vec2 direction = glm::vec2(particles[i]->m_x - from->m_x, particles[i]->m_y - from->m_y);
				glm::vec2 normalized = glm::normalize(direction);
				glm::vec2 normalizedMin = glm::vec2(closeZero(normalized.x, direction.x / 2.0), closeZero(normalized.y, direction.y / 2.0));
				//normalVector = glm::normalize(normalVector);

				//The Strong Force (hardest)
				if (strong_force) {
					float r = sqrt(distance);

					if (r < 5) {
						if ((from->proton() || from->neutron()) && (particles[i]->proton() || particles[i]->neutron())) {
							float exponent = -1.02041 * pow(-1 + r, 2);
							float force = -1.64 - 4.0 * pow(EULER_NUMBER, exponent) + (5.0 / r);

							//oe::Logger::out(oe::info, "x: ", 
							//	closeZero(normalized.x, direction.x), ", y: ",
							//	closeZero(normalized.y, direction.y));

							force *= ELECTR_FORCE_STR;

							force_x += normalizedMin.x * -force;
							force_y += normalizedMin.y * -force;

							//Stabilization
							//from->m_vel_x *= 0.9998;
							//from->m_vel_y *= 0.9998;
						}
					}
				}

				//No division by 0
				distance += 1;

				//The Electromagnetic Force
				if (electro_force) {
					//float strength = 8.98755e9 * (particles[i]->m_charge * m_charge) / pow(distance, 2);
					float force = (particles[i]->m_charge * from->m_charge) / distance;

					force *= ELECTR_FORCE_STR;

					force_x += normalizedMin.x * -force;
					force_y += normalizedMin.y * -force;
				}

				//The Gravitational Force
				if (grav_force) {
					float force = (particles[i]->m_mass * from->m_mass) / distance;

					force *= GRAV_FORCE_STR;

					force_x += normalizedMin.x * force;
					force_y += normalizedMin.y * force;
				}
			}

			return glm::vec2(force_x * force_mult, force_y * force_mult);
		}

		void update() {
			m_x += m_vel_x;
			m_y += m_vel_y;

			m_vel_x += m_acc_x / m_mass;
			m_vel_y += m_acc_y / m_mass;

			m_acc_x = 0;
			m_acc_y = 0;
		}

		void render(oe::Renderer *renderer, double zoom, double xo, double yo) {
			float renderSize = 1;
			double rx = (m_x + xo) * zoom;
			double ry = (m_y + yo) * zoom;
			if (m_charge == 1.0) renderer->renderBox(glm::vec2(rx, ry), glm::vec2(renderSize, renderSize), 24, glm::vec4(1.0, 0.0, 0.0, 1.0));
			else if (m_charge == 0.0) renderer->renderBox(glm::vec2(rx, ry), glm::vec2(renderSize, renderSize), 24, glm::vec4(0.7, 0.7, 0.7, 1.0));
			else renderer->renderBox(glm::vec2(rx, ry), glm::vec2(renderSize, renderSize), 24, glm::vec4(0.0, 0.0, 1.0, 1.0));
		}

		float getCharge() { return m_charge; }

		void applyForceX(double force) {
			m_acc_x += force;
		}

		void applyForceY(double force) {
			m_acc_y += force;
		}

		void setX(float x) { m_x = x; }
		void setY(float y) { m_y = y; }

		float getX() { return m_x; }
		float getY() { return m_y; }

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