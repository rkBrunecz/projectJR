#include "UI.h"

sf::Vector2i UI::getNewMapParams(std::string* str)
{
	sf::RenderWindow window(sf::VideoMode(400, 225, 0), "New Map", sf::Style::Close);
	sf::Font font;
	if (!font.loadFromFile("bin/Font/arial.ttf"))
		exit(EXIT_FAILURE);

	//LOCAL VARIABLES
	sf::Text Width("Width:", font, 20), Height("Height:", font, 20), FileName("File Name:", font, 20);
	sf::Text enteredW, enteredH, enteredName;
	sf::RectangleShape textField(sf::Vector2f(window.getSize().x * 0.5, 25)), textField2(sf::Vector2f(window.getSize().x * 0.5, 25)), textField3(sf::Vector2f(window.getSize().x * 0.5, 25));
	sf::RectangleShape bar(sf::Vector2f(3, 20));
	sf::Clock barBlink;
	std::string width, height, fileName;
	bool valuesEntered = false, enteringWidth = false, enteringHeight = false, enteringFileName = true;

	//Set the position for the Width text. Set it to the center of the screen. Additionally, set the color of the text to white.
	FileName.setPosition((window.getSize().x / 2) - (Width.getLocalBounds().width / 2) - 160, (window.getSize().y / 2) - 75);
	FileName.setColor(sf::Color::White);
	
	Width.setPosition((window.getSize().x / 2) - (Width.getLocalBounds().width / 2) - 124, (window.getSize().y / 2) - 35);
	Width.setColor(sf::Color::White);

	Height.setPosition((window.getSize().x / 2) - (Height.getLocalBounds().width / 2) - 124, (window.getSize().y / 2));
	Height.setColor(sf::Color::White);


	//Set up properties of the enteredName text field
	enteredName.setFont(font);
	enteredName.setColor(sf::Color(0, 155, 0, 255));
	enteredName.setCharacterSize(20);
	enteredName.setPosition((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15, (window.getSize().y / 2) - 73);
	enteredName.setColor(sf::Color::Black);

	bar.setPosition((window.getSize().x / 2) - (textField3.getLocalBounds().width / 2) + 17, (window.getSize().y / 2) - 73);

	//Set up properties of the enteredW text field
	enteredW.setFont(font);
	enteredW.setColor(sf::Color(0, 155, 0, 255));
	enteredW.setCharacterSize(20);
	enteredW.setPosition((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15, (window.getSize().y / 2) - 33);
	enteredW.setColor(sf::Color::Black);

	//Set up properties of the enteredH text field
	enteredH.setFont(font);
	enteredH.setColor(sf::Color(0, 0, 0, 255));
	enteredH.setCharacterSize(20);
	enteredH.setPosition((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15, (window.getSize().y / 2) + 7);
	enteredH.setColor(sf::Color::Black);

	//Set up properties of the text field
	textField.setFillColor(sf::Color::White);
	textField.setPosition((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15, (window.getSize().y / 2) - 35);

	textField2.setFillColor(sf::Color::White);
	textField2.setPosition((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15, (window.getSize().y / 2) + 5);

	textField3.setFillColor(sf::Color::White);
	textField3.setPosition((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15, (window.getSize().y / 2) - 75);

	while (!valuesEntered)
	{
		window.clear(sf::Color(205, 201, 201, 255));

		if (barBlink.getElapsedTime() >= sf::seconds(0.5f))
		{
			barBlink.restart();

			if (bar.getFillColor().a == 0)
				bar.setFillColor(sf::Color(0, 0, 0, 255));
			else
				bar.setFillColor(sf::Color(0, 0, 0, 0));
		}

		//Poll keyboard events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				return sf::Vector2i(-1, -1);
					
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::BackSpace)
				{
					if (width.length() > 0 && enteringWidth)
						width.erase(width.length() - 1);
					if (height.length() > 0 && enteringHeight)
						height.erase(height.length() - 1);
					if (fileName.length() > 0 && enteringFileName)
						fileName.erase(fileName.length() - 1, 1);

					enteredW.setString(width);
					enteredH.setString(height);
					enteredName.setString(fileName);
				}
				else if (event.key.code == sf::Keyboard::Tab)
				{
					if (enteringFileName)
					{
						enteringFileName = false;
						enteringWidth = true;
					}
					else if (enteringWidth)
					{
						enteringWidth = false;
						enteringHeight = true;
					}
					else if (enteringHeight)
					{
						enteringHeight = false;
						enteringFileName = true;
					}
				}
				else if (event.key.code == sf::Keyboard::Return && atoi(width.c_str()) > 2 && atoi(height.c_str()) > 2 && fileName.length() > 0)
					valuesEntered = true;				

				break;

			case sf::Event::TextEntered:
				if (event.text.unicode >= 48 && event.text.unicode <= 57)
				{
					if (width.length() < 5 && enteringWidth)
						width += static_cast<char>(event.text.unicode);
					if (height.length() < 5 && enteringHeight)
						height += static_cast<char>(event.text.unicode);

					enteredW.setString(width);
					enteredH.setString(height);
				}

				if (fileName.length() < 15 && enteringFileName && event.text.unicode != 32 && event.text.unicode != 8) //Ensure spaces are not being added to the file name
					fileName += (char)event.text.unicode;

				enteredName.setString(fileName);

				break;

			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (textField.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
					{
						enteringWidth = true;
						enteringHeight = false;
						enteringFileName = false;
					}
					else if (textField2.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
					{
						enteringHeight = true;
						enteringWidth = false;
						enteringFileName = false;
					}
					else if (textField3.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
					{
						enteringFileName = true;
						enteringWidth = false;
						enteringHeight = false;
					}
				}

				break;
				}
			}

		if (enteringFileName)
			bar.setPosition((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 17 + (fileName.length() * 10), (window.getSize().y / 2) - 73);
		else if (enteringWidth)
			bar.setPosition((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 17 + (width.length() * 10), (window.getSize().y / 2) - 33);
		else if (enteringHeight)
			bar.setPosition((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 17 + (height.length() * 10), (window.getSize().y / 2) + 7);

		window.draw(Width);
		window.draw(Height);
		window.draw(FileName);
		window.draw(textField);
		window.draw(textField2);
		window.draw(textField3);
		window.draw(enteredW);
		window.draw(enteredH);
		window.draw(enteredName);
		window.draw(bar);

		window.display();
		}

	window.close();

	*str = fileName;
	return sf::Vector2i(atoi(width.c_str()) / Map::getTileSize(), atoi(height.c_str()) / Map::getTileSize());
}