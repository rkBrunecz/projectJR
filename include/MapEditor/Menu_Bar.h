#ifndef Menu_Bar_H
#define Menu_Bar_H

#include <vector>
#include <deque>
#include "SFML\Graphics\RectangleShape.hpp"
#include "SFML\Graphics\Font.hpp"
#include "SFML\Graphics\Text.hpp"
#include "SFML\Graphics\Color.hpp"
#include "SFML\Graphics\Drawable.hpp"
#include "SFML\Window\Mouse.hpp"
#include "PBE\System\System.h"

namespace
{
	class Item : public sf::Drawable
	{
	public:
		// Constructor
		Item(const std::string& name) : NAME(name) { }

		// Pure virtual methods
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

		// Defined behavior for all child classes
		const sf::Vector2f getBoxSize() { return box.getSize(); }

		const std::string getName() { return NAME; }

		bool isItem(const std::string& name)
		{
			if (NAME.compare(name) == 0)
				return true;

			return false;
		}

	protected:
		const std::string NAME;

		sf::Text text;
		sf::RectangleShape box;
		sf::Color defaultColor, highlightedColor;
	};

	class Menu_Item : public Item
	{
	public:
		// Constructor
		Menu_Item(const std::string& itemName, const sf::Font& font, const sf::Vector2f& pos, int height, int characterSize, const sf::Color& color) : Item(itemName)
		{
			text.setFont(font);
			text.setString(itemName);
			text.setCharacterSize(characterSize);
			text.setPosition(pos.x, pos.y + (box.getSize().y * 0.25f));
			text.setFillColor(sf::Color::Black);

			defaultColor = color;
			highlightedColor = color;
			highlightedColor += sf::Color(80, 80, 80, 255);

			box.setSize(sf::Vector2f(text.getLocalBounds().width + (text.getLocalBounds().width * 0.25f), (float)height));
			box.setPosition(pos);
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(box);
			target.draw(text);
		}

		void update(const sf::Vector2i& mouseCoords)
		{
			if (mouseCoords.x > box.getPosition().x && mouseCoords.x < box.getPosition().x + box.getSize().x &&
				mouseCoords.y > box.getPosition().y && mouseCoords.y < box.getPosition().y + box.getSize().y)
				box.setFillColor(highlightedColor);
			else
				box.setFillColor(defaultColor);
		}

		void setBoxSize(const sf::Vector2f& size)
		{
			box.setSize(size);
		}
	};


	class Container_Item : public Item
	{
	public:
		// Constructor
		Container_Item(const std::string& itemName, const sf::Font& font, const sf::Vector2f& pos, int height, int characterSize, const sf::Color& color) : Item(itemName)
		{
			text.setFont(font);
			text.setString(itemName);
			text.setCharacterSize(characterSize);
			text.setFillColor(sf::Color::Black);

			box.setSize(sf::Vector2f(text.getLocalBounds().width + (text.getLocalBounds().width * 0.25f), (float)height));
			box.setPosition(pos);

			text.setPosition(pos.x + ((box.getSize().x - text.getLocalBounds().width) / 2), pos.y);

			defaultColor = color;
			highlightedColor = color;
			highlightedColor += sf::Color(80, 80, 80, 255);

			container.setPosition(box.getPosition().x, box.getPosition().y + box.getSize().y);
			container.setFillColor(sf::Color::White);
			container.setSize(sf::Vector2f(0, box.getSize().y));
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(box);
			target.draw(text);

			if (displayContainer)
			{
				target.draw(container);
				for (unsigned int i = 0; i < items.size(); i++)
					target.draw(items[i]);
			}
		}

		void hideContainer()
		{
			displayContainer = false;
		}

		void update(const sf::Vector2i& mouseCoords, bool showContainer)
		{
			if (mouseCoords.x > box.getPosition().x && mouseCoords.x < box.getPosition().x + box.getSize().x &&
				mouseCoords.y > box.getPosition().y && mouseCoords.y < box.getPosition().y + box.getSize().y)
			{
				box.setFillColor(highlightedColor);
				displayContainer = showContainer;
			}
			else if (displayContainer)
				box.setFillColor(highlightedColor);
			else
				box.setFillColor(defaultColor);

			for (unsigned int i = 0; i < items.size(); i++)
				items[i].update(mouseCoords);
		}

		void addItem(const std::string& menuItem, const sf::Font& font)
		{
			// Check to see if the menu item is in the vector already
			for (unsigned int i = 0; i < items.size(); i++)
			{
				if (items[i].isItem(menuItem))
					return;
			}

			// Local varaibles
			sf::Vector2f pos = sf::Vector2f(container.getPosition().x, container.getPosition().y + (box.getSize().y * items.size()));

			// Create a menu item
			Menu_Item item(menuItem, font, pos, (int)box.getSize().y, text.getCharacterSize(), defaultColor);

			// Add item to drop down
			items.push_back(item);

			// Determine the size of the box
			if (items.back().getBoxSize().x > container.getSize().x)
			{
				container.setSize(sf::Vector2f(items.back().getBoxSize().x, box.getSize().y * items.size()));
				for (unsigned int i = 0; i < items.size(); i++)
					items[i].setBoxSize(items.back().getBoxSize());
			}
			else
			{
				items.back().setBoxSize(sf::Vector2f(container.getSize().x, box.getSize().y));
				container.setSize(sf::Vector2f(container.getSize().x, box.getSize().y * items.size()));
			}
		}

		const std::string clicked(const sf::Vector2i& mouseCoords)
		{
			if (mouseCoords.x > box.getPosition().x && mouseCoords.x < box.getPosition().x + box.getSize().x &&
				mouseCoords.y > box.getPosition().y && mouseCoords.y < box.getPosition().y + box.getSize().y)
				displayContainer = !displayContainer;
			else if (displayContainer &&
				mouseCoords.x > container.getPosition().x && mouseCoords.x < container.getPosition().x + container.getSize().x &&
				mouseCoords.y > container.getPosition().y && mouseCoords.y < container.getPosition().y + container.getSize().y)
				return items[int(mouseCoords.y / box.getSize().y) - 1].getName();
			else
				displayContainer = false;

			return "Nothing";
		}

		bool isDisplaingDropDown()
		{
			return displayContainer;
		}

	private:
		std::vector<Menu_Item> items;
		sf::RectangleShape container;

		bool displayContainer = false;
	};

}

class Menu_Bar : public sf::Drawable
{
public:
	// Constructor
	Menu_Bar(const sf::Vector2f& size, const sf::Vector2f& pos, const sf::Color& color, const sf::Color& outlineColor, int outlineThickness);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::string itemClicked(const sf::Vector2i& mouseCoords);

	void update(const sf::Vector2i& mouseCoords);

	void addMenuItem(const std::string& menuBarItem);

	void addDropDownItem(const std::string& src, const std::string& menuItem);

	void hideContainers();

	bool isContainerShowing();

	const sf::Vector2f getSize();

private:
	std::vector<Container_Item> menuBar;
	sf::RectangleShape bar;
	sf::Font font;
};

#endif