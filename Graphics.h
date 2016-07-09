#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Physics.h"

class CDrawer {
public:

	CDrawer(sf::Vector2u const window_size, size_t pixel_depth = 32):
		medium_pressure(0), medium_energy(0) { 
		MainWindow.create(sf::VideoMode(window_size.x, window_size.y, pixel_depth), "Ideal Gas."); 
	}
	~CDrawer() { MainWindow.close(); }

	void Set(const CMolecularGas& StartCondition);
	bool DrawCurrent(CMolecularGas& Condition);

	void UpdateCountersAndEdgesPossition(const CMolecularGas& Condition);
	void SetCounterMode(bool IsCounting);

	float const ShowMediumPressure() { return medium_pressure; }
	float const ShowMediumEnergy() { return medium_energy; }
	bool const ShowCounterMode() { return CounterMode; }
private:
	sf::RenderWindow MainWindow;

	std::vector<sf::CircleShape> Molecules;
	sf::VertexArray Edges;

	std::vector<sf::Text> comments;
	std::vector<float> pressure;
	std::vector<size_t> counters;

	float avg_Kin_energy;
	float medium_pressure;
	float medium_energy;
	sf::Font font;
	bool CounterMode;
};
