#include "HealthBar.h"



HealthBar::HealthBar(sf::Vector2f healthBarSize, sf::Vector2f healthBarPosition)
{
	m_healthBar.setSize(healthBarSize);
	m_healthBar.setPosition(healthBarPosition);
	m_healthBar.setFillColor(sf::Color::Red);

	m_healthBarBack.setSize(healthBarSize);
	m_healthBarBack.setPosition(healthBarPosition);
	m_healthBarBack.setFillColor(sf::Color(100, 100, 100));
	m_healthBarBack.setOutlineColor(sf::Color::Blue);
	m_healthBarBack.setOutlineThickness(5.0f);

	
	m_maxHealth = 1000.0f;
	m_currentHealth = m_maxHealth;

	m_healthScale = m_maxHealth / healthBarSize.x;
}

sf::RectangleShape HealthBar::GetHealthBar()
{
	return m_healthBar;
}

sf::RectangleShape HealthBar::GetHealthBarBack()
{
	return m_healthBarBack;
}

void HealthBar::SetHealth(float health)
{

}

void HealthBar::UpdateHealth(float health)
{
	m_currentHealth = health;
	float newHealthSize = m_currentHealth / m_healthScale;

	if (newHealthSize < 0)
	{
		newHealthSize = 0;
	}
	m_healthBar.setSize(sf::Vector2f(newHealthSize, 50.0f));
}

int HealthBar::GetHealth()
{
	return m_currentHealth;
}


HealthBar::~HealthBar()
{
}
