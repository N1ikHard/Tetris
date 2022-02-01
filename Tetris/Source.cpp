#include <SFML/Graphics.hpp>
#include<time.h>
using namespace sf;

//Размеры поля по высоте и длине
const int M = 20;
const int N = 10;

//Массив-поле игры
int field[M][N] = { 0 };

// Координаты  фигур , а- изменяемые координаты , b - принимающение значения
struct  Point { int x, y; } a[4] , b[4];

//Координаты фигур из квадратов строятся по принципу таблицы 2х7
int figures[7][4] = {

	1,3,5,7, //I			__ __
	2,4,5,7, //Z		   | 0| 1|
	3,4,5,6, //S		   | 2| 3|
	3,5,4,7, //T		   | 4| 5|
	2,3,5,7, //L		   | 6| 7|
	3,5,7,6, //J		   -------
	2,3,4,5, //O

};

//Проверка на выход за поля
bool Check(){

	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x > N || a[i].y >= M) return 0;
		else if (field[a[i].y][a[i].x]) return 0;
	return 1;
}

int main() 
{
 
    #pragma region Установка размеров экрана , загрузка текстур , инициализация спрайтов , рандомайзера 



	RenderWindow window(VideoMode(320, 480), "Tetris");
	Texture t , t2 ,t3;
	t.loadFromFile("images/tiles.png");
	t2.loadFromFile("images/background.png");
	t3.loadFromFile("images/frame.png");
	Sprite s(t) , back(t2) , f(t3);
	s.setTextureRect(IntRect(0, 0, 18, 18));
	srand(time(0));
	#pragma endregion
	
	#pragma region Координата для движения по оси Х , переменная вращения ,  цвета , счетчика времени и временного лимита , инициализация таймера.
	int dx = 0; bool rotate = 0; int colorNum = 1;
	float timer = 0, delay = 0.3;
	Clock clock;
	#pragma endregion

	while (window.isOpen()) {

	#pragma region 	Счет времени
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
	#pragma endregion

		Event e;
		while (window.pollEvent(e)) 
			{

			if (e.type == Event::Closed)
				window.close();

			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::Up)
					rotate = true;
				else if (e.key.code == Keyboard::Left)
					dx = -1;
				else if (e.key.code == Keyboard::Right)
					dx = 1;
			}

		//Ускорение падения фигуры
		if (Keyboard::isKeyPressed(Keyboard::Down))
			delay = 0.05;

	#pragma region Перемещение фигуры



		for (int i = 0; i < 4; i++)
		{	
			b[i] = a[i]; 
				a[i].x += dx;
		}
		if (!Check())
			for (int i = 0; i < 4; i++)
			a[i] = b[i];
#pragma endregion
	#pragma region Вращение фигур

		//Если фигуру можно повернуть
		if (rotate)
		{
			//Берем второй элемент фигуры за центр и меняем клетки вокруг него 
			Point p = a[1];	 
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			//Если не выходим за пределы поля , то 
			if (!Check())
				for (int i = 0; i < 4; i++)
					a[i] = b[i];
		}
	#pragma endregion
		

	#pragma region Действия выполняемые за работу таймера

		if (timer > delay)
		{
			//Фигура принимает значения координат , координаты меняются
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				a[i].y += 1;
			}

			//Установка цвета фигуры
			if (!Check()) 
			{
				for (int i = 0; i < 4; i++)
					field[b[i].y][b[i].x] = colorNum;
				
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
	#pragma endregion
		
		//Проверка на собранную линию
		int k = M - 1;
		for (int i = M - 1; i > 0; i--)
		{
				int count = 0;
				for (int j = 0; j < N; j++) 
				{
				if (field[i][j])count++;
					field[k][j] = field[i][j];
				}
				if (count < N)
					k--;
		}

			 //Обнуляем все параметры
			dx = 0; rotate = 0, delay = 0.3;

			//Рисуем рамку 
			window.clear(Color::White);
			window.draw(back);


			for (int i = 0; i < M; i++)
				for (int j = 0; j < N; j++)
				{
					if (field[i][j] == 0) continue;
					s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
					s.setPosition(j * 18, i * 18);
					s.move(28, 31);
					window.draw(s);
				}

			for (int i = 0; i < 4; i++)
			{
				s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
				s.setPosition(a[i].x * 18, a[i].y * 18);
				s.move(28, 31);
				window.draw(s);

			}
			window.draw(f);
			window.display();
		
	}
}