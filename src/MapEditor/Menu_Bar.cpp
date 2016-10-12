#include "MapEditor\Menu_Bar.h"

Menu_Bar::Menu_Bar(const sf::Vector2f& size, const sf::Vector2f& pos, const sf::Color& color, const sf::Color& outlineColor, int outlineThickness)
{
	bar.setSize(size);
	bar.setFillColor(color);
	bar.setPosition(pos);
	bar.setOutlineColor(outlineColor);
	bar.setOutlineThickness((float)outlineThickness);

	pb::System::load(&font, "arial.ttf");
}

void Menu_Bar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(bar);

	for (unsigned int i = 0; i < menuBar.size(); i++)
		target.draw(menuBar[i]);
}

std::string Menu_Bar::itemClicked(const sf::Vector2i& mouseCoords)
{
	std::string s;

	for (unsigned int i = 0; i < menuBar.size(); i++)
	{
		s = menuBar[i].clicked(mouseCoords);

		if (s.compare("Nothing") != 0)
			return s;
	}

	return s;
}

void Menu_Bar::update(const sf::Vector2i& mouseCoords)
{
	for (unsigned int i = 0; i < menuBar.size(); i++)
	{
		if (i != 0 && menuBar[i - 1].isDisplaingDropDown())
		{
			menuBar[i].update(mouseCoords, true);

			if (menuBar[i].isDisplaingDropDown())
				menuBar[i - 1].hideContainer();
		}
		else if (i != menuBar.size() - 1 && menuBar[i + 1].isDisplaingDropDown())
		{
			menuBar[i].update(mouseCoords, true);

			if (menuBar[i].isDisplaingDropDown())
				menuBar[i + 1].hideContainer();
		}
		else
			menuBar[i].update(mouseCoords, menuBar[i].isDisplaingDropDown());
	}
}

void Menu_Bar::addMenuItem(const std::string& menuBarItem)
{
	sf::Vector2f pos = sf::Vector2f(bar.getPosition().x, bar.getPosition().y);

	for (unsigned int i = 0; i < menuBar.size(); i++)
		pos.x += menuBar[i].getBoxSize().x;

	Container_Item item(menuBarItem, font, pos, (int)bar.getSize().y, int(bar.getSize().y * 0.75f), bar.getFillColor());

	menuBar.push_back(item);
}

void Menu_Bar::addDropDownItem(const std::string& src, const std::string& menuItem)
{
	// Local variables
	bool srcFound = false;

	for (unsigned int i = 0; i < menuBar.size(); i++)
	{
		if (menuBar[i].isItem(src))
		{
			menuBar[i].addItem(menuItem, font);
			srcFound = true;
		}
	}

	if (!srcFound)
	{
		addMenuItem(src);
		menuBar.back().addItem(menuItem, font);
	}
}

void Menu_Bar::hideContainers()
{
	for (unsigned int i = 0; i < menuBar.size(); i++)
		menuBar[i].hideContainer();
}

bool Menu_Bar::isContainerShowing()
{
	for (unsigned int i = 0; i < menuBar.size(); i++)
	{
		if (menuBar[i].isDisplaingDropDown())
			return true;
	}

	return false;
}

const sf::Vector2f Menu_Bar::getSize()
{
	return bar.getSize();
}