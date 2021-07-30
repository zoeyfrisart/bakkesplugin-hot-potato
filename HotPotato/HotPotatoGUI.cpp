/**
 * HotPotato, a Bakkesmod plugin to help practice bounce dribbling.
 * Copyright (c) 2021 Yannick Frisart
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "pch.h"
#include "HotPotato.h"

// Do ImGui rendering here
void HotPotato::Render()
{
	ImGui::SetNextWindowBgAlpha(0.6f);
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_NoDecoration))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}


	std::string bounceCount = "0";
	if (HotPotato::getBounceCount() > 2) {
		bounceCount = "2+";
	}
	else {
		bounceCount = std::to_string(HotPotato::getBounceCount());
	}

	std::string bounceLabel = "ball bounced: " + bounceCount;
	std::string streakLabel = "streak: " + std::to_string(HotPotato::getBallStreak());
	std::string highscoreLabel = "highscore: " + std::to_string(HotPotato::getBallStreakRecord());

	ImGui::Text(bounceLabel.c_str());
	ImGui::Text(streakLabel.c_str());
	ImGui::Text(highscoreLabel.c_str());

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string HotPotato::GetMenuName()
{
	return "HotPotato";
}

// Title to give the menu
std::string HotPotato::GetMenuTitle()
{
	return menuTitle_;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void HotPotato::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool HotPotato::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool HotPotato::IsActiveOverlay()
{
	return false;
}

// Called when window is opened
void HotPotato::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void HotPotato::OnClose()
{
	isWindowOpen_ = false;
}

std::string HotPotato::GetPluginName() {
	return "Hot Potato";
}

void HotPotato::RenderSettings() {
	if (ImGui::Button("Toggle Hot Potato overlay")) {
		gameWrapper->Execute([this](GameWrapper* gw) {
			cvarManager->executeCommand("togglemenu HotPotato");
		});
	}

	ImGui::TextUnformatted("You can create a keybind for this menu by adding a binding for 'togglemenu HotPotato'");
}