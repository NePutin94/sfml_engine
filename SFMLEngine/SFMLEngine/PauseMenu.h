#pragma once
#include "GUI.h"
#include "Inventory.h"
#include "LogConsole.h"
#include <entityx/entityx.h>
#include "EngineEvents.h"
#include "GuiEditor.h"

namespace Engine
{
	class PauseMenu : public BaseGui
	{
	private:
	public:
		PauseMenu(std::string pathToTheme, sf::RenderWindow& w) : BaseGui(w, pathToTheme)
		{
			gui.setTarget(w);
			name = "PauseMenu";
			sf::Font font;
			font.loadFromFile("Data/Fonts/Bricks.otf");
			gui.setFont(font);
			gui.add(groupArray[0]);
		}
		void makeMenu()
		{
			groupArray.addWidget(makeCanvas("Data/images/Debug.png"), "Canvas");
			groupArray.get("Default")->get<tgui::Canvas>("Canvas")->setInheritedOpacity(0.5f);
			groupArray.addWidget(makeTextBox(" *PauseMenu*", sf::Vector2f(270, 80), sf::Vector2f(960, 80)), "PauseBox");
		}
		void update() override { }
		void draw() override {
			gui.draw();
		}
		void handleEvent(sf::Event& e) override { gui.handleEvent(e); }
	};

}