

#include <iostream>
#include <fstream>
#include <SFML/System.hpp>
#include "ncurses.h"
const int maxX = 400;
const int maxY = 400;
void build_wall(int** walls, sf::Vector2i start_pos, sf::Vector2i end_pos);
void build_room(int** walls, sf::Vector2i top_left, sf::Vector2i bottom_right);
void create_opening(int** walls, sf::Vector2i start_pos, sf::Vector2i end_pos);
int main(int argc, char **argv)
{
	std::ofstream walls_file;
	walls_file.open("walls.bb", std::ios::trunc);
	int** walls = new int*[maxX];
	for (int x = 0; x < maxX; x++)
	{
		walls[x] = new int[maxY];
	}
	for (int x = 0; x < maxX; x++)
	{
		for (int y = 0; y < maxY; y++)
		{
			walls[x][y] = -1;
		}
	}
	
	
	build_room(walls, sf::Vector2i(180, 180), sf::Vector2i(220, 220));
	build_room(walls, sf::Vector2i(190, 120), sf::Vector2i(210, 180));
	build_room(walls, sf::Vector2i(140, 150), sf::Vector2i(190, 165));
	build_room(walls, sf::Vector2i(210, 160), sf::Vector2i(250, 170)); 
	//create_opening(walls, sf::Vector2i(195, 180), sf::Vector2i(205, 180));
	//create_opening(walls, sf::Vector2i(190, 165), sf::Vector2i(190, 160));
	for (int x = 0; x < maxX; x++)
	{
		for (int y = 0; y < maxY; y++)
		{
			if (walls[x][y] == 1)
			{
				//std::cout << "#";
				walls_file << 1 << " " << x * 5 << " " << y * 5 << std::endl;
			}
			//else std::cout << ".";
		}
		std::cout << std::endl;
	}
	walls_file.close();
	return 0;
}
void build_wall(int** walls, sf::Vector2i start_pos, sf::Vector2i end_pos)
{
	int x1 = start_pos.x;
	int x2 = end_pos.x;
	int y2 = end_pos.y;
	int y1 = start_pos.y;
	if (x1 > x2) std::swap(x1, x2);
	if (y1 > y2) std::swap(y1, y2);
	for (int x = x1; x <= x2; x++)
	{
		for (int y = y1; y <= y2; y++)
		{
			if (walls[x][y] == -1) walls[x][y] = 1;
			else walls[x][y] = -1;
		}
	}
}
void build_room(int** walls, sf::Vector2i top_left, sf::Vector2i bottom_right)
{
	build_wall(walls, top_left, sf::Vector2i(bottom_right.x, top_left.y));
	build_wall(walls, top_left, sf::Vector2i(top_left.x, bottom_right.y));
	build_wall(walls, bottom_right, sf::Vector2i(bottom_right.x, top_left.y));
	build_wall(walls, bottom_right, sf::Vector2i(top_left.x, bottom_right.y));
}
void create_opening(int** walls, sf::Vector2i start_pos, sf::Vector2i end_pos)
{
	int x1 = start_pos.x;
	int x2 = end_pos.x;
	int y2 = end_pos.y;
	int y1 = start_pos.y;
	if (x1 > x2) std::swap(x1, x2);
	if (y1 > y2) std::swap(y1, y2);
	for (int x = x1; x <= x2; x++)
	{
		for (int y = y1; y <= y2; y++)
		{
			walls[x][y] = -1;
		}
	}
}
