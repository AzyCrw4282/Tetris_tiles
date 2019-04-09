#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;//namespace sf to avoid naming conflicts

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };//sets array value to 0 to beign with and changes for each movement of block

//class initialisation
struct Point
{
	int x, y;
} 
a[4], b[4];

int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;//checks if it exceeds boundary
		else if (field[a[i].y][a[i].x]) return 0;// if this is true for an exisitng location on frame then normal return

	return 1;//return if all good
};


int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(320, 480), "The Game!");

	Texture t1, t2, t3;
	t1.loadFromFile("images/tiles.png");
	t2.loadFromFile("images/background.png");
	t3.loadFromFile("images/frame.png");

	//Applies drawable representation of a texture
	Sprite s(t1), background(t2), frame(t3);//creates a spritesheet for tiles, background and frame

	int dx = 0; bool rotate = 0; int colorNum = 1; //All variables for display
	float timer = 0, delay = 0.3;

	Clock clock;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event e;
		//Will handle all events via queue polling and will handle appropriately
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::Up) rotate = true;//Sets rotation
				else if (e.key.code == Keyboard::Left) dx = -1;//Applies movement left
				else if (e.key.code == Keyboard::Right) dx = 1;//Applies movement right
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;//increase delay

		//// <- Move -> ///
		for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }//changes movement coordinates with a for loop
			if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];//sets the object to the current object

		//////Rotate//////
		if (rotate)
		{
			Point p = a[1]; //center of rotation
			for (int i = 0; i < 4; i++)
			{
				//This performs a clockwise rotation and mapping it to exact coordinates
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
		}
	
		///////Tick//////
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }//moves it down in y for each tick

			if (!check())
			{
				for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;

				colorNum = 1 + rand() % 7;
				int n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0;
		}

		///////check lines//////////
		//This handles the movement in y and counts the remaining lines for a given object
		//if at the bottom then set to start from top agian
		int k = M - 1;
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < N; j++)
			{
				if (field[i][j]) count++;
				field[k][j] = field[i][j];
			}
			if (count < N) k--;// does the counting by decrementing it
		}

		dx = 0; rotate = 0; delay = 0.3;

		/////////draw//////////
		window.clear(Color::White);
		window.draw(background);

		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{//Does the creating and drawing
				if (field[i][j] == 0) continue;
				s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));//sets height and width for the grid
				s.setPosition(j * 18, i * 18);
				s.move(28, 31); //offset
				window.draw(s);
			}

		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			s.setPosition(a[i].x * 18, a[i].y * 18);
			s.move(28, 31); //offset
			window.draw(s);
		}

		window.draw(frame);
		window.display();
	}

	return 0;
}











/*
int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}

*/