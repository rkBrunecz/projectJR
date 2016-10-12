/*
UI.cpp
UI handles several user interface components. Primarily used to create additional windows and open file selectors.

@author Randall Brunecz
@version 1.0 10/15/2015
*/

#include "MapEditor/UI.h"
#include "PBE\System\System.h"

sf::RenderWindow* UI::mainWindow = NULL;

void UI::intializeMainWindow(sf::RenderWindow* window)
{
	mainWindow = window;
}

sf::Vector2i UI::getNewMapParams(std::string* str)
{
	sf::RenderWindow window(sf::VideoMode(400, 225, 0), "New Map", sf::Style::Close);
	sf::Font font;
	pb::System::load(&font, "arial.ttf");

	//LOCAL VARIABLES
	sf::Text Rows("Rows:", font, 20), Columns("Columns:", font, 20), FileName("File Name:", font, 20);
	sf::Text enteredW, enteredH, enteredName;
	sf::RectangleShape textField(sf::Vector2f(window.getSize().x * 0.5f, 25)), textField2(sf::Vector2f(window.getSize().x * 0.5f, 25)), textField3(sf::Vector2f(window.getSize().x * 0.5f, 25));
	sf::RectangleShape bar(sf::Vector2f(3, 20));
	sf::Clock barBlink;
	std::string rows, columns, fileName;
	bool valuesEntered = false, enteringRows = false, enteringColumns = false, enteringFileName = true;

	//Set the position for the Rows text. Set it to the center of the screen. Additionally, set the color of the text to white.
	FileName.setPosition(float((window.getSize().x / 2) - (Rows.getLocalBounds().width / 2) - 160), float((window.getSize().y / 2) - 75));
	FileName.setFillColor(sf::Color::White);
	
	Rows.setPosition(float((window.getSize().x / 2) - (Rows.getLocalBounds().width / 2) - 123), float((window.getSize().y / 2) - 35));
	Rows.setFillColor(sf::Color::White);

	Columns.setPosition(float((window.getSize().x / 2) - (Columns.getLocalBounds().width / 2) - 134), float((window.getSize().y / 2)));
	Columns.setFillColor(sf::Color::White);

	//Set up properties of the enteredName text field
	enteredName.setFont(font);
	enteredName.setFillColor(sf::Color(0, 155, 0, 255));
	enteredName.setCharacterSize(20);
	enteredName.setPosition(float((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15), float((window.getSize().y / 2) - 73));
	enteredName.setFillColor(sf::Color::Black);

	bar.setPosition(float((window.getSize().x / 2) - (textField3.getLocalBounds().width / 2) + 17), float((window.getSize().y / 2) - 73));

	//Set up properties of the enteredW text field
	enteredW.setFont(font);
	enteredW.setFillColor(sf::Color(0, 155, 0, 255));
	enteredW.setCharacterSize(20);
	enteredW.setPosition(float((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15), float((window.getSize().y / 2) - 33));
	enteredW.setFillColor(sf::Color::Black);

	//Set up properties of the enteredH text field
	enteredH.setFont(font);
	enteredH.setFillColor(sf::Color(0, 0, 0, 255));
	enteredH.setCharacterSize(20);
	enteredH.setPosition(float((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15), float((window.getSize().y / 2) + 7));
	enteredH.setFillColor(sf::Color::Black);

	//Set up properties of the text field
	textField.setFillColor(sf::Color::White);
	textField.setPosition(float((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15), float((window.getSize().y / 2) - 35));

	textField2.setFillColor(sf::Color::White);
	textField2.setPosition(float((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15), float((window.getSize().y / 2) + 5));

	textField3.setFillColor(sf::Color::White);
	textField3.setPosition(float((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15), float((window.getSize().y / 2) - 75));

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
					if (rows.length() > 0 && enteringRows)
						rows.erase(rows.length() - 1);
					if (columns.length() > 0 && enteringColumns)
						columns.erase(columns.length() - 1);
					if (fileName.length() > 0 && enteringFileName)
						fileName.erase(fileName.length() - 1, 1);

					enteredW.setString(rows);
					enteredH.setString(columns);
					enteredName.setString(fileName);
				}
				else if (event.key.code == sf::Keyboard::Tab)
				{
					if (enteringFileName)
					{
						enteringFileName = false;
						enteringRows = true;
					}
					else if (enteringRows)
					{
						enteringRows = false;
						enteringColumns = true;
					}
					else if (enteringColumns)
					{
						enteringColumns = false;
						enteringFileName = true;
					}
				}
				else if (event.key.code == sf::Keyboard::Return && atoi(rows.c_str()) > 2 && atoi(columns.c_str()) > 2 && fileName.length() > 0)
					valuesEntered = true;	
				else if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
					return sf::Vector2i(-1, -1);
				}

				break;

			case sf::Event::TextEntered:
				if (event.text.unicode >= 48 && event.text.unicode <= 57)
				{
					if (rows.length() < 5 && enteringRows)
						rows += static_cast<char>(event.text.unicode);
					if (columns.length() < 5 && enteringColumns)
						columns += static_cast<char>(event.text.unicode);

					enteredW.setString(rows);
					enteredH.setString(columns);
				}

				if (fileName.length() < 15 && enteringFileName && event.text.unicode > 40 && event.text.unicode != 127) //Ensure spaces are not being added to the file name
					fileName += (char)event.text.unicode;

				enteredName.setString(fileName);

				break;

			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (textField.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
					{
						enteringRows = true;
						enteringColumns = false;
						enteringFileName = false;
					}
					else if (textField2.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
					{
						enteringColumns = true;
						enteringRows = false;
						enteringFileName = false;
					}
					else if (textField3.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
					{
						enteringFileName = true;
						enteringRows = false;
						enteringColumns = false;
					}
				}

				break;
				}
			}

		if (enteringFileName)
			bar.setPosition(textField3.getPosition().x + enteredName.getLocalBounds().width, textField3.getPosition().y + (bar.getSize().y * 0.125f));
		else if (enteringRows)
			bar.setPosition(textField.getPosition().x + enteredW.getLocalBounds().width, textField.getPosition().y + (bar.getSize().y * 0.125f));
		else if (enteringColumns)
			bar.setPosition(textField2.getPosition().x + enteredH.getLocalBounds().width, textField2.getPosition().y + (bar.getSize().y * 0.125f));

		window.draw(Rows);
		window.draw(Columns);
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
	return sf::Vector2i(atoi(columns.c_str()), atoi(rows.c_str()));
}

std::string UI::getMap(std::string filter)
{
	OPENFILENAME ofn;

	char currentDirectory[MAX_PATH];
	GetModuleFileName(NULL, currentDirectory, MAX_PATH);
	std::string::size_type pos = std::string(currentDirectory).find_last_of("\\/");

	// a another memory buffer to contain the file name	
	char szFile[100];

	// open a file name
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = mainWindow->getSystemHandle();
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);

	if (filter.compare("JRM") == 0)
		ofn.lpstrFilter = "JRM\0*.jrm\0";
	else if (filter.compare("JRS") == 0)
		ofn.lpstrFilter = "JRS\0*.jrs\0";

	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	GetOpenFileName(&ofn);

	std::string s = ofn.lpstrFile;

	SetCurrentDirectory(std::string(currentDirectory).substr(0, pos).c_str());

	return s;
}

sf::Vector2i UI::getCoordinates(std::string windowName)
{
	sf::RenderWindow window(sf::VideoMode(400, 200, 0), windowName, sf::Style::Close);
	sf::Font font;
	pb::System::load(&font, "arial.ttf");

	//LOCAL VARIABLES
	sf::Text Row("Row :", font, 20), Column("Column :", font, 20);
	sf::Text enteredRow, enteredColumn;
	sf::RectangleShape textField(sf::Vector2f(window.getSize().x * 0.5f, 25)), textField2(sf::Vector2f(window.getSize().x * 0.5f, 25));
	sf::RectangleShape bar(sf::Vector2f(3, 20));
	sf::Clock barBlink;
	std::string row, column;
	bool valuesEntered = false, enteringRow = true, enteringColumn = false;

	Row.setPosition(float( (window.getSize().x / 2) - (Row.getLocalBounds().width / 2) - 120), float((window.getSize().y / 2) - 35));
	Row.setFillColor(sf::Color::White);

	Column.setPosition(float((window.getSize().x / 2) - (Column.getLocalBounds().width / 2) - 134), float((window.getSize().y / 2)));
	Column.setFillColor(sf::Color::White);


	//Set up properties of the enteredW text field
	enteredRow.setFont(font);
	enteredRow.setFillColor(sf::Color(0, 155, 0, 255));
	enteredRow.setCharacterSize(20);
	enteredRow.setPosition(float((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15), float((window.getSize().y / 2) - 33));
	enteredRow.setFillColor(sf::Color::Black);

	bar.setPosition(float((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 17), float((window.getSize().y / 2) - 33));

	//Set up properties of the enteredH text field
	enteredColumn.setFont(font);
	enteredColumn.setFillColor(sf::Color(0, 0, 0, 255));
	enteredColumn.setCharacterSize(20);
	enteredColumn.setPosition(float((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15), float((window.getSize().y / 2) + 7));
	enteredColumn.setFillColor(sf::Color::Black);

	//Set up properties of the text field
	textField.setFillColor(sf::Color::White);
	textField.setPosition(float((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15), float((window.getSize().y / 2) - 35));

	textField2.setFillColor(sf::Color::White);
	textField2.setPosition(float((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 15), float((window.getSize().y / 2) + 5));

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
					if (row.length() > 0 && enteringRow)
						row.erase(row.length() - 1);
					if (column.length() > 0 && enteringColumn)
						column.erase(column.length() - 1);

					enteredRow.setString(row);
					enteredColumn.setString(column);
				}
				else if (event.key.code == sf::Keyboard::Tab)
				{
					if (enteringRow)
					{
						enteringRow = false;
						enteringColumn = true;
					}
					else if (enteringColumn)
					{
						enteringColumn = false;
						enteringRow = true;
					}
				}
				else if (event.key.code == sf::Keyboard::Return && row.size() > 0 && column.size() > 0)
					valuesEntered = true;
				else if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
					return sf::Vector2i(-1, -1);
				}

				break;

			case sf::Event::TextEntered:
				if (event.text.unicode >= 48 && event.text.unicode <= 57)
				{
					if (row.length() < 5 && enteringRow)
						row += static_cast<char>(event.text.unicode);
					if (column.length() < 5 && enteringColumn)
						column += static_cast<char>(event.text.unicode);

					enteredRow.setString(row);
					enteredColumn.setString(column);
				}

				break;

			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (textField.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
					{
						enteringRow = true;
						enteringColumn = false;
					}
					else if (textField2.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
					{
						enteringColumn = true;
						enteringRow = false;
					}
				}

				break;
			}
		}

		if (enteringRow)
			bar.setPosition(float((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 17 + (row.length() * 10)), float((window.getSize().y / 2) - 33));
		else if (enteringColumn)
			bar.setPosition(float((window.getSize().x / 2) - (textField.getLocalBounds().width / 2) + 17 + (column.length() * 10)), float((window.getSize().y / 2) + 7));

		window.draw(Row);
		window.draw(Column);
		window.draw(textField);
		window.draw(textField2);
		window.draw(enteredRow);
		window.draw(enteredColumn);
		window.draw(bar);

		window.display();
	}

	window.close();

	return sf::Vector2i(atoi(column.c_str()), atoi(row.c_str()));
}