#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <list>


//#include "../common/network_opcodes.hpp"
#include "network_opcodes.hpp"
#include "GameObject.h"
#include "Chunk.h"
#include "Map.h"
#include "Ball.h"
#include "HUD_Panel.h"

#define GLOBAL_FRAME_RATE 60

#ifdef linux
#include <getopt.h>
#endif // linux

using namespace sf;
using namespace std;

void umiesc_obiekt(Sprite& obiekt,
	Vector2u wys_obiektu,
	Sprite *chunki,
	int ktory,
	Vector2u texture1_size,
	RenderWindow& okno,
	View v)
{
	Vector2f odn1;
	Vector2i odn2;
	odn1 = chunki[ktory].getPosition();
	odn2 = okno.mapCoordsToPixel(odn1, v);
	obiekt.setPosition((odn2.x + (texture1_size.x*sqrt(2) / 2) - (texture1_size.x / 2)),
		(odn2.y - wys_obiektu.y + (texture1_size.y*sqrt(2) / 2) - (texture1_size.y / 2)));
	//0.4142*32/2  CZYLI: biore dane przeksztalcenia
	//--> odejmuje wysokosc zeby rysowanie bylo od lewgo dolnego rogu
	//--> i teraz obnizam rysowanie tekstury do najnizszego punktu rysowania chunka
	//--> uwzgledniam pierwsiatek po prekstalceniu w wymiarach

	//sa przekatne wiec wymiary chunka razy sqrt2 i w jego polowie czyli /2
	//a potem przesuwam o polowe wymiaru chunka
	//czyli przsuniecie jest o polowe zwiekszenia wymiaru przez pierwiastek czyli to 0.41/2
}

void print_help()
{
	cout << "-p --port       default 7000\n"
		"-l --local_port default 8000\n"
		"-i --ip         default localhost\n"
		"-h --help       this message\n";
}
//---------------------------------------------------------------------------------------------------------------------//
int main(int argc, char** argv)
{
	//zmienne które można modyfikować argumentami z konsoli
	unsigned short local_port = 8000;//port na którym aplikacja odbiera połączenia
	unsigned short remote_port = 7000;//port do którego się łączymy
	IpAddress remote_ip = "localhost";//ip do którego się łączymy

#ifdef linux
	{
		static struct option longopts[] =
		{
			{ "port",       required_argument, NULL, 'p' },
		{ "local_port", required_argument, NULL, 'l' },
		{ "ip",         required_argument, NULL, 'i' },
		{ "help",       no_argument,       NULL, 'h' },
		};

		int c;
		while ((c = getopt_long(argc, argv, "p:l:i:h", longopts, NULL)) != -1)
		{
			switch (c)
			{
			case 'p':
				remote_port = stoul(optarg, NULL, 0);
				break;
			case 'l':
				local_port = stoul(optarg, NULL, 0);
				break;
			case 'i':
				remote_ip = optarg;
				break;
			case 'h':
				cout << argv[0] << "\n";
				print_help();
				return 0;
			case '?':
				return 0;
			case ':':
				return 0;
			}
		}
	}
#endif // linux
	//---------------------------------------------------------------------------------------------------------------------//

	bool quit = false;//główny wyłącznik

	//POMOCNE ZMIENNE do lacznosci 
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	UdpSocket socket;
	socket.setBlocking(false);
	socket.bind(local_port);
	Packet send_packet;
	Packet receive_packet;
	IpAddress incomming_ip;
	unsigned short incomming_port;

	//POMOCNE ZMIENNE do renderowania mapy
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	int Frame_Number = 60;
	int map_width = 100, map_height = 100;
	int number_of_chunks = map_width * map_height;
	int speed_of_scrolling = 10;
	float game_zoom = 0.0f; // od -1.5 do 0.5
	int zoom_step = 60; //mnoznik sily zooma
	double object_scroll = speed_of_scrolling * sqrt(2);
	int resolution_width = 1920, resolution_height = 1080;
	srand(time(NULL));
	int random_number1 = rand();

	//obsługa piłeczki
	bool ball_active = false;
	int x = 0;
	int y = 0;
	
	//MODYFIKOWANIE OKNA APLIKACJI
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	RenderWindow oknoAplikacji(VideoMode(resolution_width, resolution_height), "Kelajno", Style::Close);//to opcja resizeable
	oknoAplikacji.setFramerateLimit(Frame_Number);//ustawiam limit fps na 60

	//POMOCNE ZMIENNE 2
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Vector2i mouse_possition = Mouse::getPosition(oknoAplikacji);

	//HUD
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	HUD_Panel hud_panel = HUD_Panel(sf::Vector2u(resolution_width, resolution_height));

	//TEXTURY
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Texture t1; 
	Texture t2; 
	Texture t3;
	t1.loadFromFile("Textures/Terrain/Grunt.png");
	t2.loadFromFile("Textures/Ball/Ball.jpg");
	t3.loadFromFile("Textures/Ball/Active.png");

	sf::Sprite active;
	active.setTexture(t3);

	//MAPA
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Map map = Map(map_width, map_height, t1);

	//TABLICA OBIEKTÓW
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Ball b = Ball(t2, 5, 5, map.get_pointer_to_chunk(5,5)->get_sprite().getPosition().x, map.get_pointer_to_chunk(5, 5)->get_sprite().getPosition().y);
	b.set_sprite(t2, 0,0 );

	//cout << map.get_chunks[5][5].get_sprite().getPosition().x << " " << map.get_chunks[5][5].get_sprite().getPosition().y << "\n";
	//cout << b.get_sprite().getPosition().x << " " << b.get_sprite().getPosition().y << "\n";

	//VIEWS 
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	View map_view = oknoAplikacji.getDefaultView();
	//map_view.setSize(map_view.getSize().x, map_view.getSize().y * 2);
	map_view.setCenter(0, 0);

	View panel_view = oknoAplikacji.getDefaultView();

	//View minimap_view = oknoAplikacji.getDefaultView();
	//minimap_view.setViewport(FloatRect(0.0f, 0.75f, 0.25f, 0.25f));

	//---------------------------------------------------------------------------------------------------------------------//
	Time time;
	Clock clock;

	//OBSŁUGA GRY
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	while (!quit)
	{
		time = clock.restart();//pobranie czasu
		mouse_possition = Mouse::getPosition(oknoAplikacji);//pobieram wspolrzedne myszki w kazdej klatce


		//GŁÓWNA PĘTLA PAKIETÓW
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		while (!socket.receive(receive_packet, incomming_ip, incomming_port))
		{
			while (!receive_packet.endOfPacket())
			{
				Uint8 opcode;
				receive_packet >> opcode;

				switch (opcode)
				{
				case ADD_UNIT_TO_GAME:
				{
					Uint8 BP_jednostki;
					Uint8 x;
					Uint8 y;
					receive_packet >> BP_jednostki >> x >> y;
					break;
				}
				case SET_UNIT_POSITION:
				{
					Uint8 ID_jednostki;
					Uint8 x;
					Uint8 y;
					receive_packet >> ID_jednostki >> x >> y;
					break;
				}
				default:
				{
					break;
				}
				}//end switch
			}
		}

		//OBSŁUGA URZĄDZEŃ WEJŚCIA
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		Event zdarzenie;
		while (oknoAplikacji.pollEvent(zdarzenie))
		{
			switch (zdarzenie.type)
			{
			case Event::Closed:
				quit = true;
				break;
			case Event::KeyPressed:

				switch (zdarzenie.key.code)
				{
				case Keyboard::Escape:
				quit = true;
				break;
				case Keyboard::Num8:
					if (game_zoom < 0.5f)
					{
						game_zoom += 0.1f;
						map_view.setSize(resolution_width + (16 * zoom_step)*game_zoom, (resolution_height*2) + (9 * zoom_step * 2) * game_zoom);
						//object_view.setSize(resolution_width + (16 * zoom_step)*game_zoom, resolution_height + (9 * zoom_step) * game_zoom);

					}
				break;
				case Keyboard::Num2:
					if (game_zoom > -0.5f)
					{
						game_zoom -= 0.1f;
						map_view.setSize(resolution_width + (16 * zoom_step)*game_zoom, (resolution_height * 2) + (9 * zoom_step * 2) * game_zoom);
						//object_view.setSize(resolution_width + (16 * zoom_step)*game_zoom, resolution_height + (9 * zoom_step) * game_zoom);
					}
				break;

				case Keyboard::Up:
					{
						map_view.move(0, -speed_of_scrolling);
						//object_view.move(0, -object_scroll / 2);
					}
				break;				
				
				case Keyboard::Down:
					{
						map_view.move(0, speed_of_scrolling);
						//object_view.move(0, object_scroll / 2);
					}
				break;

				case Keyboard::Right:
					{
						map_view.move(speed_of_scrolling, 0);
						//object_view.move(object_scroll, 0);
					}

				break;

				case Keyboard::Left:
					{
						map_view.move(-speed_of_scrolling, 0);
						//object_view.move(-object_scroll, 0);
					}

				break;

				case Keyboard::Space:
				{
					cout << "Ball status: \n";
					cout << "Active: " << ball_active << " \n";
					cout << "Ball position: x - " << b.get_position_x() << ", y - " << b.get_position_y() << " \n";
					cout << "Sprite position: x - " << b.get_sprite().getPosition().x << ", y - " << b.get_sprite().getPosition().y << " \n";
					if (b.get_current_order() != nullptr) {
						cout << "Order: \n";
						cout << " Start: " << b.get_current_order()->get_start_x() << " " << b.get_current_order()->get_start_y() << " \n";
						cout << " Mid: " << b.get_current_order()->get_mid_x() << " " << b.get_current_order()->get_mid_y() << " \n";
						cout << " Next chunk: " << b.get_current_order()->get_next()->get_position_x() << " " << b.get_current_order()->get_next()->get_position_y()  << " \n";
						cout << " Final: " << b.get_current_order()->get_final_x() << " " << b.get_current_order()->get_final_y() << " \n";
					}
					else {
						cout << "Order: \n No Order! \n";
					}

				}

				break;

				case Keyboard::C:
				{
					if (b.get_current_order() != nullptr) {
						b.cancel(*b.get_current_order());
					}
				}

				break;

				default:
				break;
				}

				break;

			case Event::MouseButtonPressed:
				switch (zdarzenie.mouseButton.button) 
				{
				case Mouse::Left:
				{
					if (true == b.get_sprite().getGlobalBounds().contains(oknoAplikacji.mapPixelToCoords(mouse_possition, map_view))){
						ball_active = true;
					}
					else {
						ball_active = false;
					}
				}
				break;

				case Mouse::Right:
				{
					x = map.get_pointer_to_chunk_with_mouse_on_it(oknoAplikacji, mouse_possition, map_view)->get_position_x();
					y = map.get_pointer_to_chunk_with_mouse_on_it(oknoAplikacji, mouse_possition, map_view)->get_position_y();
					cout << x << " " << y << "\n";



					if (ball_active) {
						if (b.get_current_order() == nullptr) {
							MoveOrder m = MoveOrder(b.get_position_x(), b.get_position_y(), x, y, map);
							//cout << m.get_final_x() << " " << m.get_final_y() << "\n";
							b.give(m, map);
						}


					}
				}
				break;

			break;
			}

				break;
				//------------------------ZOOMOWANIE KAMERY -----------------------//
			case Event::MouseWheelScrolled:
					if (zdarzenie.mouseWheelScroll.delta <= -1)
					{
						if (game_zoom < 0.5f)//oddalanie
						{
							game_zoom += 0.1f;
							map_view.setSize(resolution_width + (16 * zoom_step)*game_zoom, resolution_height + (9 * zoom_step) * game_zoom);
						}
					}
					if (zdarzenie.mouseWheelScroll.delta >= 1)
					{
						if (game_zoom > -1.5f)//przyblizanie
						{
							game_zoom -= 0.1f;
							map_view.setSize(resolution_width + (16 * zoom_step)*game_zoom, resolution_height + (9 * zoom_step) * game_zoom);
						}
					}
					cout << "wheel movement: " << zdarzenie.mouseWheelScroll.delta << endl;
					cout << "mouse x: " << zdarzenie.mouseWheelScroll.x << endl;
					cout << "mouse y: " << zdarzenie.mouseWheelScroll.y << endl;
				break;

			default:
				break;
			}
			
			//--------------- PRZESUWANIE KAMERY --------------------------------------------//
			if (mouse_possition.x == 0) //left
			{
				map_view.move(-speed_of_scrolling, 0);
				//object_view.move(-object_scroll, 0);
			}
			if (mouse_possition.y == 0) //up
			{
				map_view.move(0, -speed_of_scrolling);
				//object_view.move(0, -object_scroll/2);
			}
			if (mouse_possition.x == oknoAplikacji.getSize().x-1) //rigt
			{
				map_view.move(speed_of_scrolling, 0);
				//object_view.move(object_scroll, 0);
			}
			if (mouse_possition.y == oknoAplikacji.getSize().y-1) //down
			{
				map_view.move(0, speed_of_scrolling);
				//object_view.move(0, object_scroll / 2);
			}
			//dla objektow uzywam object_scroll tutaj sqrt(2)*speed_of_scrolling bo po przekstalceniu
			
		}

		//RENDER OBRAZU
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		oknoAplikacji.clear();
		
		oknoAplikacji.setView(map_view);
		for (int i = 0; i < map_width; i++) {
			for (int j = 0; j < map_height; j++) {
				map.get_pointer_to_chunk(i, j)->drawTo(oknoAplikacji);
			}
		}


		b.place_on_chunk(oknoAplikacji, active, *(map.get_pointer_to_chunk(b.get_position_x(),b.get_position_y())), ball_active);

		oknoAplikacji.setView(panel_view);
		hud_panel.drawTo(oknoAplikacji);

		//oknoAplikacji.setView(minimap_view);
		//for (int i = 0; i < map_width; i++) {
		//	for (int j = 0; j < map_height; j++) {
		//		map[i][j].drawTo(oknoAplikacji);
		//	}
		//}

		oknoAplikacji.display();
		socket.send(send_packet, remote_ip, remote_port);//wyslanie pakietu
		send_packet.clear();//czyszczenie pakietu

		b.check_status(map, oknoAplikacji, ball_active);
		//cout << b.get_position_x() << " " << b.get_position_y() << "\n";
		//cout << mouse_possition.x << " " << mouse_possition.y << " \n";
		//cout << map.get_chunks()[1][1].get_sprite().getPosition().x << " " << map.get_chunks()[1][1].get_sprite().getPosition().y << " \n";
	}
	//---------------------------------------------------------------------------------------------------------------------//

	oknoAplikacji.close();

	return EXIT_SUCCESS;
}

//https://www.sfml-dev.org/tutorials/2.4/window-window.php
