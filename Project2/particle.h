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
		double m_x, m_y, m_z;
		double m_vel_x, m_vel_y, m_vel_z;
		double m_acc_x, m_acc_y, m_acc_z;

		double m_mass;
		double m_charge;

	public:
		Particle(float x, float y, float z, float vel_x, float vel_y, float vel_z, float n, float mass) : Particle(x, y, z, vel_x, vel_y, vel_z, n) {
			m_mass = mass;
		}

		Particle(float x, float y, float z, float vel_x, float vel_y, float vel_z, float n) : Particle(x, y, z, n) {
			m_vel_x = vel_x;
			m_vel_y = vel_y;
			m_vel_z = vel_z;
		}

		Particle(float x, float y, float z, float vel_x, float vel_y, float vel_z) : Particle(x, y, z) {
			m_vel_x = vel_x;
			m_vel_y = vel_y;
			m_vel_z = vel_z;
		}

		Particle(float x, float y, float z, float n) : Particle(x, y, z) {
			m_charge = n;

			if (m_charge == 1.0) { m_charge = 1.0; m_mass = 1835; }
			else if (m_charge == 0.0) { m_charge = 0.0; m_mass = 1835; }
			else if (m_charge == -1.0) { m_charge = -1.0; m_mass = 1; }
		}

		Particle(float x, float y, float z) {
			m_x = x;
			m_y = y;
			m_z = z;
			m_vel_x = 0.0;
			m_vel_y = 0.0;
			m_vel_z = 0.0;
			m_acc_x = 0.0;
			m_acc_y = 0.0;
			m_acc_z = 0.0;

			float n = oe::Random::random(0.0, 1.0);
			if (n >= 0.66) { m_charge = 1.0; m_mass = 1835; }
			else if (n >= 0.33) { m_charge = 0.0; m_mass = 1835; }
			else { m_charge = -1.0; m_mass = 1; }
		}

		void preupdate(Particle **particles, unsigned int count, double force_mult, bool strong_force, bool electro_force, bool grav_force) {
			glm::vec3 forces = calcForces(this, particles, force_mult, count, strong_force, electro_force, grav_force, false);
			applyForceX(forces.x);
			applyForceY(forces.y);
			applyForceZ(forces.z);

			//m_vel_x *= 1.000 - 0.002 * force_mult;
			//m_vel_y *= 1.000 - 0.002 * force_mult;
		}

		static glm::vec3 calcForces(Particle *from, Particle **particles, double force_mult, unsigned int count, bool strong_force, bool electro_force, bool grav_force, bool selector) {
			float force_x = 0, force_y = 0, force_z = 0;

			//DEBUGGING
			//if (!selector) oe::Logger::out("STEP FROM PARTICLE");

			for (unsigned int i = 0; i < count; i++)
			{
				if (!particles[i]) continue;
				if (particles[i] == from) continue;

				//DEBUGGING
				//if (!selector) oe::Logger::out(i);

				float distanceSqr = pow(particles[i]->m_x - from->m_x, 2);
				distanceSqr += pow(particles[i]->m_y - from->m_y, 2);
				distanceSqr += pow(particles[i]->m_z - from->m_z, 2);

				//Dir
				glm::vec3 direction = glm::vec3(particles[i]->m_x - from->m_x, particles[i]->m_y - from->m_y, particles[i]->m_z - from->m_z);
				glm::vec3 normalized = glm::normalize(direction);
				glm::vec3 normalizedMin = glm::vec3(closeZero(normalized.x, direction.x / 2.0), closeZero(normalized.y, direction.y / 2.0), closeZero(normalized.z, direction.z / 2.0));
				//normalVector = glm::normalize(normalVector);

				//No division by 0
				distanceSqr = distanceSqr < 5.0f ? 5.0f : distanceSqr;

				//The Strong Force (hardest)
				if (strong_force) {
					if (distanceSqr < 25) {
						if ((from->proton() || from->neutron()) && (particles[i]->proton() || particles[i]->neutron())) {
							float exponent = -1.02041 * distanceSqr;
							float force = -1.64 - 4.0 * pow(EULER_NUMBER, exponent) + (5.0 / sqrt(distanceSqr));

							//oe::Logger::out("x: ", 
							//	closeZero(normalized.x, direction.x), ", y: ",
							//	closeZero(normalized.y, direction.y));

							force *= ELECTR_FORCE_STR;

							force_x += normalizedMin.x * -force;
							force_y += normalizedMin.y * -force;
							force_z += normalizedMin.z * -force;

							//Stabilization
							//from->m_vel_x *= 0.9998;
							//from->m_vel_y *= 0.9998;
						}
					}
				}

				//The Electromagnetic Force
				if (electro_force) {
					//float strength = 8.98755e9 * (particles[i]->m_charge * m_charge) / pow(distance, 2);
					float force = (particles[i]->m_charge * from->m_charge) / distanceSqr;

					force *= ELECTR_FORCE_STR;

					force_x += normalized.x * -force;
					force_y += normalized.y * -force;
					force_z += normalized.z * -force;
				}

				//The Gravitational Force
				if (grav_force) {
					float force = (particles[i]->m_mass * from->m_mass) / distanceSqr;

					force *= GRAV_FORCE_STR;

					force_x += normalized.x * force;
					force_y += normalized.y * force;
					force_z += normalized.z * force;
				}
			}

			return glm::vec3(force_x * force_mult, force_y * force_mult, force_z * force_mult);
		}

		void update() {
			if (m_mass == INFINITY) return;

			oe::Logger::out(m_vel_x);


			m_x += m_vel_x;
			m_y += m_vel_y;
			m_z += m_vel_z;

			//oe::Logger::out(m_vel_x);

			m_vel_x += m_acc_x / m_mass;
			m_vel_y += m_acc_y / m_mass;
			m_vel_z += m_acc_z / m_mass;

			//oe::Logger::out(m_mass);

			m_acc_x = 0;
			m_acc_y = 0;
			m_acc_z = 0;
		}

		void render(oe::Renderer *renderer, double zoom, double xo, double yo, double zo) {
			float renderSize = 1;
			double rx = (m_x + xo) * zoom;
			double ry = (m_y + yo) * zoom;
			double rz = (m_z + zo) * zoom;
			if (m_charge == 1.0) {
				renderer->pointRenderer->submitVertex(oe::VertexData(glm::vec3(rx, ry, rz), glm::vec2(renderSize), 24, glm::vec4(1.0, 0.0, 0.0, 1.0)));
			}
			else if (m_charge == 0.0) {
				renderer->pointRenderer->submitVertex(oe::VertexData(glm::vec3(rx, ry, rz), glm::vec2(renderSize), 24, glm::vec4(0.7, 0.7, 0.7, 1.0)));
			}
			else {
				renderer->pointRenderer->submitVertex(oe::VertexData(glm::vec3(rx, ry, rz), glm::vec2(renderSize), 24, glm::vec4(0.0, 0.0, 1.0, 1.0)));
			}
		}

		float getCharge() { return m_charge; }

		void applyForceX(double force) {
			m_acc_x += force;
		}

		void applyForceY(double force) {
			m_acc_y += force;
		}

		void applyForceZ(double force) {
			m_acc_z += force;
		}

		void setX(float x) { m_x = x; }
		void setY(float y) { m_y = y; }
		void setZ(float z) { m_z = z; }

		float getX() { return m_x; }
		float getY() { return m_y; }
		float getZ() { return m_z; }

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

		float mass() { return m_mass; }

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