#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Constants.h"

sf::Vector2f const VNULL(0.f, 0.f);

struct MoleculData {
	sf::Vector2f coordinate;
	sf::Vector2f speed;
	sf::Vector2f acc;
};

class CMolecularGas {
public:
	CMolecularGas(sf::Vector2f _x_axis_range, sf::Vector2f _y_axis_range);

	~CMolecularGas() {}

	void SetStartPosition(size_t NumberOfMolecules, float avg_sq_speed = Root_Mean_Square_Speed);
	void MakeStep(float dt = Jump_Time);
	void UpdateSpeed(float dt);

	void Lower_Upper_Limit();
	void Raise_Upper_Limit();
	void Lower_Right_Limit();
	void Raise_Right_Limit();

	const sf::Vector2f& Show_molecule(size_t i) const;
	const float Show_molecule_speed(size_t i) const;
	float Show_radius() const;
	const sf::Vector2f& ShowLeftUpPoint() const;
	const sf::Vector2f& ShowLeftDownPoint() const;
	const sf::Vector2f& ShowRightDownPoint() const;
	const sf::Vector2f& ShowRightUpPoint() const;
	float Show_Pressure(size_t WallNumber) const;

	size_t Size() const;
	size_t collision_counter;
	float Average_Kinetic_Energy;
private:
	bool single_collision(size_t i, float dt);
	bool left_border_collision(sf::Vector2f& future_coord, size_t i, float dt);
	bool down_border_collision(sf::Vector2f& future_coord, size_t i, float dt);
	bool right_border_collision(sf::Vector2f& future_coord, size_t i, float dt);
	bool up_border_collision(sf::Vector2f& future_coord, size_t i, float dt);

	bool pair_collision(size_t first, size_t second, float dt);
	float calculate_collision_time(sf::Vector2f& fCoord, sf::Vector2f& sCoord, sf::Vector2f& fSpeed, sf::Vector2f& sSpeed, float dt);
	bool border_collision(float coordinate, float border);

	void molecule_jump(size_t i, float dt);
	sf::Vector2f future_position(size_t i, float dt);

	std::vector<MoleculData> molecules;
	std::vector<bool> AlreadyCollided;

	float molecule_mass;
	float radius;

	std::vector<float> pressure;
	sf::Vector2f x_axis_range;
	sf::Vector2f y_axis_range;
	sf::Vector2f range_of_speed;
};

float f_rand_0_1(size_t precision);
sf::Vector2f rand_Vector(sf::Vector2f _x_axis_range, sf::Vector2f _y_axis_range);
