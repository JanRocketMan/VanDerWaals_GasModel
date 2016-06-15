#pragma once
#include "Graphics.h"
#include <SFML\Window\Event.hpp>
#include <SFML\Graphics\PrimitiveType.hpp>

void CDrawer::Set(const CMolecularGas& StartCondition)
{
	sf::Color MyBlack(0, 0, 0);

	Edges.setPrimitiveType(sf::LinesStrip);
	Edges.resize(5);

	for( size_t i = 0; i < 5; i++ ) Edges[i].color = MyBlack;

	font.loadFromFile("C:/Users/user/Documents/Visual Studio 2013/Projects/Van_Der_Vaals/Van_Der_Vaals/AdobeArabic-Regular.ttf");

	sf::Text sample;
	sample.setFont(font);
	sample.setCharacterSize(CharacterSizeForCounters);
	sample.setColor(MyBlack);

	comments.assign(5, sample);
	comments[0].setRotation(-90);
	comments[2].setRotation(90);

	pressure.assign(4, 0);
	counters.assign(5, 0);
	UpdateCountersAndEdgesPossition(StartCondition);

	sf::CircleShape pattern;
	pattern.setPointCount(PointsForSphere);
	pattern.setRadius(StartCondition.Show_radius());
	pattern.setFillColor(MyBlack);

	sf::Vector2f borders(StartCondition.Show_radius(), StartCondition.Show_radius());
	pattern.setOrigin(pattern.getOrigin() + borders);

	Molecules.assign(StartCondition.Size(), pattern);
}

void CDrawer::UpdateCountersAndEdgesPossition(const CMolecularGas& Condition)
{
	Edges[0].position = Condition.ShowLeftUpPoint();
	Edges[1].position = Condition.ShowLeftDownPoint();
	Edges[2].position = Condition.ShowRightDownPoint();
	Edges[3].position = Condition.ShowRightUpPoint();
	Edges[4].position = Edges[0].position;

	sf::Vector2f temp = (Edges[0].position + Edges[1].position)*0.5f;
	temp.x -= MainWindow.getSize().x * 0.05;
	comments[0].setPosition(temp); // Left Wall
	
	temp = (Edges[1].position + Edges[2].position)*0.5f;
	temp.y += MainWindow.getSize().y * 0.01;
	comments[1].setPosition(temp); // Down Wall

	temp = (Edges[2].position + Edges[3].position)*0.5f;
	temp.x += MainWindow.getSize().x * 0.05;
	comments[2].setPosition(temp); // Right Wall

	temp = (Edges[3].position + Edges[4].position)*0.5f;
	temp.y -= MainWindow.getSize().y * 0.05;
	comments[3].setPosition(temp); // Up Wall

	temp.x = MainWindow.getSize().x * 0.05;
	temp.y = MainWindow.getSize().y * 0.05;
	comments[4].setPosition(temp); // Collision Counter
}

bool CDrawer::DrawCurrent(CMolecularGas& Condition)
{
	MainWindow.setActive(true);
	sf::Color MyWhite(255, 255, 255);
	sf::Color MColor(0, 0, 0);
	float coeff;

	MainWindow.clear(MyWhite);

	sf::Event event;

	while( MainWindow.pollEvent(event) ) {
		if( event.type == sf::Event::Closed ) {
			return false;
		}
		if( event.type == sf::Event::KeyPressed ) {
			if( event.key.code == sf::Keyboard::Down ) {
				Condition.Lower_Upper_Limit();
			}
			if( event.key.code == sf::Keyboard::Up ) {
				Condition.Raise_Upper_Limit();
			}
			if( event.key.code == sf::Keyboard::Left ) {
				Condition.Lower_Right_Limit();
			}
			if( event.key.code == sf::Keyboard::Right ) {
				Condition.Raise_Right_Limit();
			}
		}
	}

	UpdateCountersAndEdgesPossition(Condition);

	MainWindow.draw(Edges);

	for( size_t i = 0; i < Condition.Size(); i++ ) {
		Molecules[i].setPosition(Condition.Show_molecule(i));
	}

	for( size_t i = 0; i < Condition.Size(); i++ ) {
		coeff = Condition.Show_molecule_speed(i) / Root_Mean_Square_Speed;
		if( ChangeColor ) {
			MColor.r = coeff * 255;
			MColor.b = (1 - coeff) * 255;
			Molecules[i].setFillColor(MColor);
		}
		MainWindow.draw(Molecules[i]);
	}
	medium_pressure = 0;
	medium_energy = 0;

	if( CountersActive && CounterMode == true ) {
		for( size_t i = 0; i < 4; i++ ) {
			pressure[i] += Condition.Show_Pressure(i);
			counters[i]++;
		}
		avg_Kin_energy += Condition.Average_Kinetic_Energy;
		// counters[4] += Condition.collision_counter;
		counters[4]++;
	} else {
		std::string buff;
		float value = 0;
		for( size_t i = 0; i < 4; i++ ) {

			if( counters[i] != 0 ) value = round(1000 * (pressure[i] / counters[i])) / 1000;
			else value = 0;

			medium_pressure += value / 4;
			buff = std::to_string(value);
			buff = "Pressure: " + buff;
			if( CountersActive ) comments[i].setString(buff);
			pressure[i] = 0;
			counters[i] = 0;
		}
		if( counters[4] != 0 ) value = round(10 * (avg_Kin_energy / counters[4])) / 10;
		else value = 0;

		medium_energy = value;
		buff = std::to_string(value);
		buff = "Avg Kinetic Energy: " + buff;
		if( CountersActive ) comments[4].setString(buff);

		avg_Kin_energy = 0;
		counters[4] = 0;
	}

	for( size_t i = 0; i < 5; i++ ) {
		MainWindow.draw(comments[i]);
	}
	MainWindow.display();
	return true;
}

void CDrawer::SetCounterMode(bool IsCounting)
{
	CounterMode = IsCounting;
}