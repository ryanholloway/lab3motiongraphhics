/// <summary>
/// @author Ryan
/// @date 
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
	if (!deathBuffer.loadFromFile("ASSETS\\death.wav"))
	{
		std::cout << "Death no load";
		return;
	}
	deathSound.setBuffer(deathBuffer);
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
	if (!won)
	{
		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{

				level[col][row].move(-3.5, 0);
				//spritesheet.setPosition(playerShape.getPosition());
			}

		}
		frameTime += clock.restart().asSeconds();
		if (frameTime >= timePerFrame) {
			currentFrame++;
			if (currentFrame >= 8)
				currentFrame = 0;

			frameTime = 0.0f;
		}
		playerShape.setTextureRect(frames[currentFrame]);

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
								deathSound.play();
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
							deathSound.play();
							init();
						}
					}
				}
				if (levelData[col][row] == 2 || levelData[col][row] == 4 || levelData[col][row] == 5)
				{
					if (playerShape.getGlobalBounds().intersects(level[col][row].getGlobalBounds()))
					{
						deathSound.play();
						init();
					}
				}
				if (levelData[col][row] == 3)
				{
					if (playerShape.getGlobalBounds().intersects(level[col][row].getGlobalBounds()))
					{
						boingSound.play();
						velocityY = -20;
					}
				}

				if (levelData[col][row] == 100)
				{
					if (playerShape.getGlobalBounds().intersects(level[col][row].getGlobalBounds()))
					{
						std::cout << "Win";
						won = true;
						endGameText.setString("\t You Win!\nPress R to Restart");
					}
				}
				if (levelData[col][row] == 6)
				{
					if (playerShape.getGlobalBounds().intersects(level[col][row].getGlobalBounds()))
					{
						boingSound.play();
						velocityY = 20;
					}
				}
			}

		}

		if (playerShape.getPosition().y > 600)
		{
			deathSound.play();
			init();
		}

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		if (won)
		{
			init();
		}
	}
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::Black);
	//m_window.draw(background);
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			m_window.draw(level[col][row]);

		}
	}
	m_window.draw(playerShape);
	m_window.draw(endGameText);
	//m_window.draw(spritesheet);

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
	endGameText.setFont(m_ArialBlackfont);
	endGameText.setString("");
	endGameText.setStyle(sf::Text::Italic | sf::Text::Bold);
	endGameText.setPosition(m_window.getSize().x/2.0f-180, 240.0f);
	endGameText.setCharacterSize(40U);
	endGameText.setOutlineColor(sf::Color::Red);
	endGameText.setFillColor(sf::Color::White);
	endGameText.setOutlineThickness(3.0f);
	if (!spritesheetTexture.loadFromFile("ASSETS\\IMAGES\\spritesheet.png"))
	{
		std::cout << "problem loading spritesheet" << std::endl;
	}
	if (!mapItemsTexture.loadFromFile("ASSETS\\IMAGES\\mapItems.png"))
	{
		std::cout << "problem loading mapItems" << std::endl;
	}
	if (!tempBacktexture.loadFromFile("ASSETS\\IMAGES\\backgroundTemp.png"))
	{
		std::cout << "problem loading background" << std::endl;
	}
}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::init()
{
	won = false;
	velocityY = 0;
	view = m_window.getDefaultView();
	playerShape.setSize(sf::Vector2f(20, 20));
	playerShape.setPosition(160, 500);
	background.setSize((sf::Vector2f(m_window.getSize().x, m_window.getSize().y)));
	background.setTexture(&tempBacktexture);
	boingSound.setBuffer(boingBuffer);
	playerShape.setTexture(&spritesheetTexture);
	/*spritesheet.setScale(sf::Vector2f(0.08f, 0.08f));*/
	
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			
			if (levelData[col][row] == 1)
			{
				level[col][row].setTexture(&mapItemsTexture);
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);
				level[col][row].setTextureRect(mapItemsI[3]);
			}
			if (levelData[col][row] == 0)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);
				level[col][row].setFillColor(sf::Color::Transparent);
			}
			if (levelData[col][row] == 2|| levelData[col][row] == 4)
			{
				level[col][row].setTexture(&mapItemsTexture);
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);

				level[col][row].setTextureRect(mapItemsI[2]);
			}
			if (levelData[col][row] == 5)
			{

				level[col][row].setTexture(&mapItemsTexture);
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);

				level[col][row].setTextureRect(mapItemsI[1]);
			}
			if (levelData[col][row] == 3)
			{
				level[col][row].setTexture(&mapItemsTexture);
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);

				level[col][row].setTextureRect(mapItemsI[0]);
			}
			if (levelData[col][row] == 100)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);

				level[col][row].setFillColor(sf::Color::Cyan);
			}
			if (levelData[col][row] == 6)
			{
				level[col][row].setTexture(&mapItemsTexture);
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);

				level[col][row].setTextureRect(mapItemsI[0]);
			}

		}
		std::cout << std::endl;

	}
}
