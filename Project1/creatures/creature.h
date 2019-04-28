#pragma once

#include "../settings.h"

#define HEADING_UP 0
#define HEADING_DOWN 2
#define HEADING_LEFT 3
#define HEADING_RIGHT 1

namespace oe { class Renderer; }
class Creature {
protected:
	//Variables
	float m_counterToRemoveSwingAnimation = 0;
	int m_id;
	int m_swingDir;
	bool m_bumping;

	//Health and stamina
	float m_health;
	float m_stamina;

	float m_maxHealth;
	float m_maxStamina;

	float m_healthGainRate;
	float m_staminaGainRate;

	float m_staminaRegenCooldown;
	float m_healthRegenCooldown;

	//Private functions
	void clampHPAndSTA();
	void setHeading(float x, float y);

public:
	bool m_item;
	float x, y;
	float old_x, old_y;
	float vel_x, vel_y;
	float acc_x, acc_y;
	unsigned char heading;

	Creature(float x, float y, int id, bool item);

	virtual void update(int index, float divider);
	virtual void ai(float divider) = 0;
	virtual void collide(float divider);
	virtual void submitToRenderer(oe::Renderer *renderer, float renderOffsetX, float renderOffsetY, float corrector);
	virtual void hit(float damageadd, float kbadd);
	virtual void die();
	
	bool canSee(float x, float y);

	inline int getId() { return m_id; }

	inline float getX() { return x - 0.5f; }
	inline float getY() { return y - 0.5f; }

	inline void setX(float n) { x = n + 0.5f; }
	inline void setY(float n) { y = n + 0.5f; }
	inline void addX(float n) { x += n; }
	inline void addY(float n) { y += n; }

	
	inline float getVelX() { return (x - old_x) * UPDATES_PER_SECOND; }
	inline float getVelY() { return (y - old_y) * UPDATES_PER_SECOND; }

	inline float getHealth() { return m_health; }
	inline float getStamina() { return m_stamina; }
	inline float getMaxHealth() { return m_maxHealth; }
	inline float getMaxStamina() { return m_maxStamina; }
	inline float getsHealthGainRate() { return m_healthGainRate; }
	inline float getStaminaGainRate() { return m_staminaGainRate; }

	inline void setHealth(float value) { m_health = value; m_healthRegenCooldown = 0; clampHPAndSTA(); }
	inline void setStamina(float value) { m_stamina = value; m_staminaRegenCooldown = 0; clampHPAndSTA(); }
	inline void setMaxHealth(float value) { m_maxHealth = value; }
	inline void setMaxStamina(float value) { m_maxStamina = value; }
	inline void setHealthGainRate(float value) { m_healthGainRate = value; }
	inline void setStaminaGainRate(float value) { m_staminaGainRate = value; }

	inline void resetHealth() { m_health = m_maxHealth; }
	inline void resetStamina() { m_stamina = m_maxStamina; }
	inline bool dead() { return m_health <= 0 ? true : false; }
	inline bool exhausted() { return m_stamina <= 0 ? true : false; }
};