#pragma once
#include "Physics.h"
#include <math.h>
#include <random>
#include <cassert>

float f_rand_0_1(size_t precision)
{
	size_t max_decimal = (size_t)pow(10, precision);
	float decimal = (rand() % max_decimal);
	decimal /= max_decimal;
	return (float)decimal;
}

sf::Vector2f rand_Vector(sf::Vector2f _x_axis_range, sf::Vector2f _y_axis_range)
{
	sf::Vector2f rand_vector;
	rand_vector.x = (_x_axis_range.x + (_x_axis_range.y - _x_axis_range.x) * f_rand_0_1(Parametrs_Precision));
	rand_vector.y = (_y_axis_range.x + (_y_axis_range.y - _y_axis_range.x) * f_rand_0_1(Parametrs_Precision));
	return rand_vector;
}

float module(sf::Vector2f& vector)
{
	return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

const float module(const sf::Vector2f& vector)
{
	return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

float scalar(sf::Vector2f& first, sf::Vector2f& second)
{
	return (first.x*second.x + first.y*second.y);
}

CMolecularGas::CMolecularGas(sf::Vector2f _x_axis_range, sf::Vector2f _y_axis_range) :
radius(Molecule_Radius), molecule_mass(Molecule_Mass)
{
	x_axis_range.x = _x_axis_range.x + radius;
	x_axis_range.y = _x_axis_range.y - radius;
	y_axis_range.x = _y_axis_range.x + radius;
	y_axis_range.y = _y_axis_range.y - radius;
}


void CMolecularGas::SetStartPosition(size_t NumberOfMolecules, float avg_sq_speed)
{
	float sign;
	srand(0);
	range_of_speed.x = -avg_sq_speed;
	range_of_speed.y = avg_sq_speed;

	for( size_t i = 0; i < NumberOfMolecules; i++ ) {
		MoleculData NewMolecule;
		// NewMolecule.coordinate.x = x_axis_range.x + (x_axis_range.y - x_axis_range.x)*(i + 1.f) / (1 * (NumberOfMolecules + 1.f));
		// NewMolecule.coordinate.y = y_axis_range.x + (y_axis_range.y - y_axis_range.x)*(i + 1.f) / (1 * (NumberOfMolecules + 1.f));
		NewMolecule.coordinate = rand_Vector(x_axis_range, y_axis_range);
		NewMolecule.speed = rand_Vector(range_of_speed, range_of_speed);
		// NewMolecule.speed.x = avg_sq_speed * 0.618;
		// NewMolecule.speed.y = - avg_sq_speed * sqrt(1 - pow(0.618, 2));
		sign = rand() % 2 ? (-1) : 1;
		NewMolecule.speed.x = sign*sqrt(pow(avg_sq_speed, 2) - pow(NewMolecule.speed.y, 2));
		molecules.push_back(NewMolecule);
	}
}

void CMolecularGas::MakeStep(float dt)
{
	pressure.assign(4, 0);
	collision_counter = 0;
	AlreadyCollided.assign(molecules.size(), false);
	Average_Kinetic_Energy = 0;

	for( size_t i = 0; i < molecules.size(); i++ ) {

		if( !AlreadyCollided[i] && single_collision(i, dt) ) {
			AlreadyCollided[i] = true;
		}

		for( size_t j = 0; j < molecules.size(); j++ ) {
			if( i != j && pair_collision(i, j, dt) ) {
				AlreadyCollided[i] = true;
				AlreadyCollided[j] = true;
			}
		}
		Average_Kinetic_Energy += molecule_mass*scalar(molecules[i].speed, molecules[i].speed) / 2;
		if( !AlreadyCollided[i] ) molecule_jump(i, dt);
	}

	pressure[0] = pressure[0] / ((y_axis_range.y - y_axis_range.x) * dt);
	pressure[1] = pressure[1] / ((x_axis_range.y - x_axis_range.x) * dt);
	pressure[2] = pressure[2] / ((y_axis_range.y - y_axis_range.x) * dt);
	pressure[3] = pressure[3] / ((x_axis_range.y - x_axis_range.x) * dt);
	Average_Kinetic_Energy /= molecules.size();
}

void CMolecularGas::UpdateSpeed(float dt)
{
	if( Vaals_Atraction ) {
		for( size_t i = 0; i < molecules.size(); i++ ) {
			molecules[i].speed += molecules[i].acc * (dt);
			molecules[i].acc = VNULL;
		}
	}
}

bool CMolecularGas::single_collision(size_t i, float dt)
{
	sf::Vector2f future_coord = future_position(i, dt);
	if( y_axis_range.y > future_coord.y && future_coord.y > y_axis_range.x && x_axis_range.y > future_coord.x && future_coord.x > x_axis_range.x ) {
		return false;
	}

	if( left_border_collision(future_coord, i, dt) ) {} else if( down_border_collision(future_coord, i, dt) ) {} else if( right_border_collision(future_coord, i, dt) ) {} else if( up_border_collision(future_coord, i, dt) ) {}

	return true;
}

bool CMolecularGas::left_border_collision(sf::Vector2f& future_coord, size_t i, float dt)
{
	float tau = 0.f;
	if( future_coord.x <= x_axis_range.x ) {
		tau = (x_axis_range.x - future_coord.x) / (-molecules[i].speed.x);
		molecule_jump(i, dt - tau);
		molecules[i].speed.x *= (-1);

		pressure[0] += 2 * molecule_mass*abs(molecules[i].speed.x);
		collision_counter++;
		if( tau <= (dt / 4) || !single_collision(i, tau) ) molecule_jump(i, tau);
		return true;
	}
	return false;
}

bool CMolecularGas::down_border_collision(sf::Vector2f& future_coord, size_t i, float dt)
{
	float tau = 0.f;
	if( future_coord.y >= y_axis_range.y ) {
		tau = (future_coord.y - y_axis_range.y) / (molecules[i].speed.y);
		molecule_jump(i, dt - tau);
		molecules[i].speed.y *= (-1);

		pressure[1] += 2 * molecule_mass*abs(molecules[i].speed.y);
		collision_counter++;
		if( tau <= (dt / 4) || !single_collision(i, tau) ) molecule_jump(i, tau);
		return true;
	}
	return false;
}

bool CMolecularGas::right_border_collision(sf::Vector2f& future_coord, size_t i, float dt)
{
	float tau = 0.f;
	if( future_coord.x >= x_axis_range.y ) {
		tau = (future_coord.x - x_axis_range.y) / (molecules[i].speed.x);
		molecule_jump(i, dt - tau);
		molecules[i].speed.x *= (-1);

		pressure[2] += 2 * molecule_mass*abs(molecules[i].speed.x);
		collision_counter++;
		if( tau <= (dt / 4) || !single_collision(i, tau) ) molecule_jump(i, tau);
		return true;
	}
	return false;
}

bool CMolecularGas::up_border_collision(sf::Vector2f& future_coord, size_t i, float dt)
{
	float tau = 0.f;
	if( future_coord.y <= y_axis_range.x ) {
		tau = (y_axis_range.x - future_coord.y) / (-molecules[i].speed.y);
		molecule_jump(i, dt - tau);
		molecules[i].speed.y *= (-1);

		pressure[3] += 2 * molecule_mass*abs(molecules[i].speed.y);
		collision_counter++;
		if( tau <= (dt / 4) || !single_collision(i, tau) ) molecule_jump(i, tau);
		return true;
	}
	return false;
}

bool CMolecularGas::pair_collision(size_t first, size_t second, float dt)
{
	float dist = module(molecules[second].coordinate - molecules[first].coordinate);
	sf::Vector2f center_line = (molecules[second].coordinate - molecules[first].coordinate) / dist;

	if( Vaals_Atraction && dist < 2 * Sphere_Of_Influence_Radius ) {
		if( dist < 2 * radius ) {
			molecules[first].acc += center_line*(K*Q2) / (float)pow(2 * radius, 2);
		} else {
			molecules[first].acc += center_line*(K*Q2) / (float)pow(dist, 2);
		}
	}

	if( AlreadyCollided[second] || AlreadyCollided[first] ) return false;

	float tau = calculate_collision_time(molecules[first].coordinate, molecules[second].coordinate, molecules[first].speed, molecules[second].speed, dt);

	if( tau >= dt )
		return false;

	molecule_jump(first, tau);
	molecule_jump(second, tau);

	float old_first_scalar = scalar(molecules[first].speed, center_line);

	molecules[first].speed -= center_line*scalar(molecules[first].speed, center_line);
	molecules[first].speed += center_line*scalar(molecules[second].speed, center_line);
	molecules[second].speed -= center_line*scalar(molecules[second].speed, center_line);
	molecules[second].speed += center_line*old_first_scalar;

	molecule_jump(first, dt - tau);
	molecule_jump(second, dt - tau);

	collision_counter++;
	return true;
}

float CMolecularGas::calculate_collision_time(sf::Vector2f& fCoord, sf::Vector2f& sCoord, sf::Vector2f& fSpeed, sf::Vector2f& sSpeed, float dt)
{
	if( sCoord.y > fCoord.y + 2 * radius && sCoord.y > fCoord.y + fSpeed.y*dt + 2 * radius
		&& sCoord.y + sSpeed.y*dt > fCoord.y + 2 * radius && sCoord.y + sSpeed.y*dt > fCoord.y + fSpeed.y*dt + 2 * radius )
		return dt;
	if( fCoord.x > sCoord.x + 2 * radius && fCoord.x > sCoord.x + sSpeed.x*dt + 2 * radius
		&& fCoord.x + fSpeed.x*dt > sCoord.x + 2 * radius && fCoord.x + fSpeed.x*dt > sCoord.x + sSpeed.x*dt + 2 * radius )
		return dt;
	if( fCoord.y > sCoord.y + 2 * radius && fCoord.y > sCoord.y + sSpeed.y*dt + 2 * radius
		&& fCoord.y + fSpeed.y*dt > sCoord.y + 2 * radius && fCoord.y + fSpeed.y*dt > sCoord.y + sSpeed.y*dt + 2 * radius )
		return dt;
	if( sCoord.x > fCoord.x + 2 * radius && sCoord.x > fCoord.x + fSpeed.x*dt + 2 * radius
		&& sCoord.x + sSpeed.x*dt > fCoord.x + 2 * radius && sCoord.x + sSpeed.x*dt > fCoord.x + fSpeed.x*dt + 2 * radius )
		return dt;

	float tau = 0.f;

	sf::Vector2f relCoord = sCoord - fCoord, relSpeed = sSpeed - fSpeed;
	float A = pow(module(relSpeed), 2);
	float B = scalar(relCoord, relSpeed);
	float C = pow(module(relCoord), 2) - 4 * pow(radius, 2);

	if( A == 0 ) {
		if( B == 0 && C >= 0 )
			return dt;
		else if( B == 0 && C < 0 )
			return 0;
		else if( -C / (2.f*B) > dt || -C / (2.f*B) < -dt )
			return dt;
		else return (-C / (2.f*B));
	}

	if( pow(B, 2) < A*C && C > 0 )
		return dt;
	if( B > 0 && C > 0 )
		return dt;
	if( C == 0 )
		return 0;

	return (-B - sqrt(pow(B, 2) - A*C)) / A;
}

void CMolecularGas::molecule_jump(size_t i, float dt)
{

	molecules[i].coordinate += molecules[i].speed*(dt);

}

sf::Vector2f CMolecularGas::future_position(size_t i, float dt)
{
	sf::Vector2f future_coord = molecules[i].coordinate;
	sf::Vector2f future_speed = molecules[i].speed;

	future_coord += future_speed * (dt);

	return future_coord;
}

void CMolecularGas::Lower_Upper_Limit()
{
	y_axis_range.x += (y_axis_range.y - y_axis_range.x)*0.01f;
}

void CMolecularGas::Raise_Upper_Limit()
{
	y_axis_range.x -= (y_axis_range.y - y_axis_range.x)*0.01f;
}

void CMolecularGas::Lower_Right_Limit()
{
	x_axis_range.y -= (x_axis_range.y - x_axis_range.x)*0.01f;
}

void CMolecularGas::Raise_Right_Limit()
{
	x_axis_range.y += (x_axis_range.y - x_axis_range.x)*0.01f;
}

const sf::Vector2f& CMolecularGas::Show_molecule(size_t i) const
{
	return molecules[i].coordinate;
}

const float CMolecularGas::Show_molecule_speed(size_t i) const
{
	return module(molecules[i].speed);
}

float CMolecularGas::Show_radius() const
{
	return radius;
}

const sf::Vector2f& CMolecularGas::ShowLeftUpPoint() const
{
	sf::Vector2f edge_point;
	edge_point.x = x_axis_range.x - radius;
	edge_point.y = y_axis_range.x - radius;
	return edge_point;
}

const sf::Vector2f& CMolecularGas::ShowLeftDownPoint() const
{
	sf::Vector2f edge_point;
	edge_point.x = x_axis_range.x - radius;
	edge_point.y = y_axis_range.y + radius;
	return edge_point;
}

const sf::Vector2f& CMolecularGas::ShowRightDownPoint() const
{
	sf::Vector2f edge_point;
	edge_point.x = x_axis_range.y + radius;
	edge_point.y = y_axis_range.y + radius;
	return edge_point;
}

const sf::Vector2f& CMolecularGas::ShowRightUpPoint() const
{
	sf::Vector2f edge_point;
	edge_point.x = x_axis_range.y + radius;
	edge_point.y = y_axis_range.x - radius;
	return edge_point;
}

float CMolecularGas::Show_Pressure(size_t WallNumber) const
{
	return pressure[WallNumber];
}

size_t CMolecularGas::Size() const
{
	return molecules.size();
}