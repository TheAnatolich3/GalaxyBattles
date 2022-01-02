#include <imgui.h>
#include "MenuItem.hpp"

void Menu::Button::visit()
{
	if (ImGui::Button(_text.c_str()))
	{
		is_clicked = true;
	}
}

void Menu::BeginItem::visit()
{
	ImGui::Begin(_text.c_str());
}

void Menu::EndItem::visit()
{
	ImGui::End();
}

void Menu::Slider::visit()
{
	ImGui::SliderFloat(_text.c_str(), &_sliderValue, _min, _max, "%.0f");
}