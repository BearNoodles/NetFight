#pragma once
#include "SFML/Graphics.hpp"

class HealthBar
{
public:
	HealthBar(sf::Vector2f healthBarSize, sf::Vector2f healthBarPosition);
	~HealthBar();

	sf::RectangleShape GetHealthBar();
	sf::RectangleShape GetHealthBarBack();
	void UpdateHealth(float health);
	int GetHealth();
	void SetHealth(float health);

protected:

	sf::RectangleShape m_healthBar;
	sf::RectangleShape m_healthBarBack;
	float m_maxHealth;
	float m_currentHealth;
	float m_healthScale;
};

