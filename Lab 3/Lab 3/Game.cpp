/// <summary>
/// @author Peter Lowe
/// @date May 2019
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	setupFontAndText(); // load font z
	init(); // load texture
	if (!boingBuffer.loadFromFile("ASSETS\\boing.wav")) {
		return;
	}
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{

			level[col][row].move(-3.5, 0);
		}

	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && velocityY == 0)
	{
		boingSound.play();
		velocityY = -11.8;
	}

	velocityY = velocityY + gravity;
	playerShape.move(0, velocityY);


	gravity = 0.6;

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			if (velocityY >= 0)
			{
				if (levelData[col][row] == 1)
				{

					if (playerShape.getGlobalBounds().intersects(level[col][row].getGlobalBounds()))
					{
						if (playerShape.getPosition().y < level[col][row].getPosition().y)
						{
							gravity = 0;
							velocityY = 0;
							playerShape.setPosition(playerShape.getPosition().x, level[col][row].getPosition().y);
							playerShape.move(0, -playerShape.getGlobalBounds().height);
							break;
						}
						else {
							init();
						}
					}


				}

			}
			if (velocityY < 0)
			{
				if (levelData[col][row] == 1)
				{
					if (playerShape.getGlobalBounds().intersects(level[col][row].getGlobalBounds()))
					{
						init();
					}
				}
			}
			if (levelData[col][row] == 2)
			{
				if (playerShape.getGlobalBounds().intersects(level[col][row].getGlobalBounds()))
				{
					init();
				}
			}
			if (levelData[col][row] == 3)
			{
				if (playerShape.getGlobalBounds().intersects(level[col][row].getGlobalBounds()))
				{
					boingSound.play();
					velocityY=-20;
				}
			}
		}

	}

	if (playerShape.getPosition().y > 600)
	{
		init();
	}


}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			m_window.draw(level[col][row]);

		}
	}
	m_window.draw(playerShape);

	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_welcomeMessage.setFont(m_ArialBlackfont);
	m_welcomeMessage.setString("SFML Game");
	m_welcomeMessage.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_welcomeMessage.setPosition(40.0f, 40.0f);
	m_welcomeMessage.setCharacterSize(80U);
	m_welcomeMessage.setOutlineColor(sf::Color::Red);
	m_welcomeMessage.setFillColor(sf::Color::Black);
	m_welcomeMessage.setOutlineThickness(3.0f);

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::init()
{
	view = m_window.getDefaultView();
	playerShape.setSize(sf::Vector2f(20, 20));
	playerShape.setPosition(160, 500);
	boingSound.setBuffer(boingBuffer);
	
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{

			if (levelData[col][row] == 1)
			{

				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);
				level[col][row].setFillColor(sf::Color::Red);
			}
			if (levelData[col][row] == 0)
			{

				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);
				level[col][row].setFillColor(sf::Color::Black);
			}
			if (levelData[col][row] == 2)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);

				level[col][row].setFillColor(sf::Color::Blue);

			}
			if (levelData[col][row] == 3)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);

				level[col][row].setFillColor(sf::Color::Yellow);
			}

		}
		std::cout << std::endl;

	}
}
