#pragma once
using namespace std;

size_t const Window_Width = 1000, Window_Height = 1000; 
size_t const GraphWindowWidth = 600, GraphWindowHeight = 600;
float const VesselSizeCoeff_x = 0.1, VesselSizeCoeff_y = 0.1;
size_t const MaxTime = 12000; // in seconds.

bool const CountersActive = true;
bool const GraphIsActive = false;
size_t const CharacterSizeForCounters = 30;
bool const ChangeColor = true;
size_t const Parametrs_Precision = 4;
size_t const PointsForSphere = 10;


size_t const NumberOfMolecules = 1000;
float const Molecule_Mass = 1.f;
float const Molecule_Radius = 2.f;
float const Jump_Time = 0.1f;
float const Root_Mean_Square_Speed = 40.1f;
float const MaxTemperature = 5 * Molecule_Mass * Root_Mean_Square_Speed * Root_Mean_Square_Speed;
float const MaxPressure = 300;

bool const Vaals_Atraction = true;
float const b = Molecule_Radius * Molecule_Radius;
float const Sphere_Of_Influence_Radius = 100.f * Molecule_Radius;
float const K = 1.1f;
float const Q2 = 100.f;
