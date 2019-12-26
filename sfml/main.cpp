#include <SFML\Graphics.hpp>
#include <iostream>
#include <time.h>
#include <string>

const int boardSize = 5;
char board[boardSize][boardSize];
char answer[boardSize][boardSize];
std::vector<int> horiz[boardSize];
std::vector<int> vert[boardSize];

void createBoard() {
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			board[j][i] = '#';
		}
	}
}

void createAnswer() {
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			answer[j][i] = rand() % 2 ? 'X' : 'O';
		}
	}
}

void createHelp() {
	// horizontal help.
	int counter = 0;
	for (int i = 0; i < boardSize; i++) {
		counter = 0;
		for (int j = 0; j < boardSize; j++) {
			if (answer[j][i] == 'O') {
				counter++;
				if (j == boardSize - 1) {
					horiz[i].push_back(counter);
				}
			}
			else {
				if (counter > 0) {
					horiz[i].push_back(counter);
				}
				counter = 0;
			}
		}
	}
	// vertical help.
	for (int i = 0; i < boardSize; i++) {
		counter = 0;
		for (int j = 0; j < boardSize; j++) {
			if (answer[i][j] == 'O') {
				counter++;
				if (j == boardSize - 1) {
					vert[i].push_back(counter);
				}
			}
			else {
				if (counter > 0) {
					vert[i].push_back(counter);
				}
				counter = 0;
			}
		}
	}
}

bool checkBoard() {
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			if ((board[j][i] == 'O' && answer[j][i] != 'O') ||
				((board[j][i] == 'X' || board[j][i] == '#')) && answer[j][i] == 'O') {
				return false;
			}
		}
	}
	return true;
}

int main()
{
	srand(time(NULL));
	createAnswer();
	createHelp();
	createBoard();

	sf::RenderWindow window(sf::VideoMode(600, 600), "Nonogram");
	sf::RectangleShape shapes[boardSize][boardSize];
	sf::Font font;
	sf::Text helpTextsHoriz[boardSize];
	sf::Text helpTextsVert[boardSize];
	sf::Text confirmButton;
	sf::Text endText;

	bool playing = true;
	bool victory = false;

	if (!font.loadFromFile("ARI.ttf"))
	{
		std::cout << "font didnt load";
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			shapes[j][i] = sf::RectangleShape(sf::Vector2f(60.f, 60.f));
			shapes[j][i].setOrigin(-(140 + (j * 65)), -(175 + (i * 65)));
		}
	}
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (confirmButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
				if (event.type == sf::Event::MouseButtonPressed) {
					playing = false;
					victory = checkBoard();
				}
			}

			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					shapes[j][i].setOutlineColor(sf::Color::Blue);
					shapes[j][i].setOutlineThickness(1.0f);
					// mouse over square
					if (shapes[j][i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
						shapes[j][i].setFillColor(sf::Color(0.1f, 0.1f, 0.1f, 25));
						// mouse left click square
						if (event.type == sf::Event::MouseButtonPressed) {
							if (event.mouseButton.button == sf::Mouse::Left && board[j][i] != 'O') {
								board[j][i] = 'O';
							}
							else if (event.mouseButton.button == sf::Mouse::Left && board[j][i] == 'O') {
								board[j][i] = '#';
							}
							else if (event.mouseButton.button == sf::Mouse::Right && board[j][i] != 'X') {
								board[j][i] = 'X';
							}
							else if (event.mouseButton.button == sf::Mouse::Right && board[j][i] == 'X') {
								board[j][i] = '#';
							}
						}
						
					}

					shapes[j][i].setOutlineColor(sf::Color::Blue);
					if (board[j][i] == 'O') {
						shapes[j][i].setFillColor(sf::Color::Blue);
					}
					if (board[j][i] == 'X') {
						shapes[j][i].setFillColor(sf::Color(sf::Color::Blue.r, sf::Color::Blue.g, sf::Color::Blue.b, 25));
					}
					if (board[j][i] == '#') {
						shapes[j][i].setFillColor(sf::Color::White);
					}
				}
			}
		}

		for (int i = 0; i < boardSize; i++) {
			std::string str1, str2;
			for (int j = 0; j < horiz[i].size(); j++) {
				str1 += std::to_string(horiz[i][j]) + "  ";
			}
			for (int j = 0; j < vert[i].size(); j++) {
				str2 += std::to_string(vert[i][j]) + "\n";
			}
			helpTextsHoriz[i].setString(str1);
			helpTextsHoriz[i].setFont(font);
			helpTextsHoriz[i].setFillColor(sf::Color::Blue);
			helpTextsHoriz[i].setPosition(132 - helpTextsHoriz[i].getLocalBounds().width, 185 + (i * 65));
			helpTextsVert[i].setString(str2);
			helpTextsVert[i].setFont(font);
			helpTextsVert[i].setFillColor(sf::Color::Blue);
			helpTextsVert[i].setPosition(165 + (i * 65), 187 - helpTextsVert[i].getLocalBounds().height);
		}

		confirmButton.setString("Confirm");
		confirmButton.setFont(font);
		confirmButton.setFillColor(sf::Color::Blue);
		confirmButton.setPosition(250, 525);

		if (victory) {
			endText.setString("WIN!");
		}
		else {
			endText.setString("LOSE!");
		}
		endText.setFont(font);
		endText.setFillColor(sf::Color::Blue);
		endText.setPosition(250, 300);
		endText.setScale(1.5, 1.5);

		window.clear(sf::Color::White);

		if (playing) {
			for (int i = 0; i < boardSize; i++) {
				window.draw(helpTextsHoriz[i]);
				window.draw(helpTextsVert[i]);
			}
			for (int i = 0; i < boardSize; i++) {
				for (int j = 0; j < boardSize; j++) {
					window.draw(shapes[i][j]);
				}
			}
			window.draw(confirmButton);
		}
		else {
			window.draw(endText);
		}

		window.display();
	}

	return 0;
}