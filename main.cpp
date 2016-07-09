#include "Physics.h"
#include "Graphics.h"
// #include "Graph.h"

int main(int argc, char* args[])
{

	sf::Time Elapsed_Time = sf::seconds(0.f), RecentUpdateTime = sf::seconds(0.f);

	sf::Vector2u const window_size(Window_Width, Window_Height);
	sf::Vector2u const graph_window_size(GraphWindowWidth, GraphWindowHeight);
	sf::Vector2f const x_axis_range(Window_Width*VesselSizeCoeff_x, Window_Width*(1 - VesselSizeCoeff_x));
	sf::Vector2f const y_axis_range(Window_Height*VesselSizeCoeff_y, Window_Height*(1 - VesselSizeCoeff_y));

	CMolecularGas Gas(x_axis_range, y_axis_range);
	CDrawer Drawer(window_size);
	// CGraph Graph;

	Gas.SetStartPosition(NumberOfMolecules);
	Drawer.Set(Gas);

	sf::Clock clock;
	size_t counter = 0;

	if( GraphIsActive ) {
		// Graph.Create(graph_window_size);
		// Graph.Set();
	}

	while( Elapsed_Time.asSeconds() <= MaxTime ) {

		if( Elapsed_Time.asMilliseconds() - RecentUpdateTime.asMilliseconds() >= 2000 ) {
			RecentUpdateTime = Elapsed_Time;
			Drawer.SetCounterMode(false);
		} else Drawer.SetCounterMode(true);

		Gas.MakeStep(Jump_Time);
		if( counter == 4 ) {
			Gas.UpdateSpeed(4*Jump_Time);
			counter = 0;
		} else counter++;

		if( Drawer.DrawCurrent(Gas) == false ) break;

		if( GraphIsActive && Drawer.ShowCounterMode() == false ) {
			// Graph.ShowCondition(Drawer.ShowMediumEnergy(), Drawer.ShowMediumPressure());
		}

		// Graph.UpdateShow();
		Elapsed_Time += clock.restart();
	}

	return 0;
}