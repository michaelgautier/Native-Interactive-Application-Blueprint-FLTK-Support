#include <algorithm>
#include <iostream>

#include "gautier_visual_model.hxx"

namespace ns_visual_model = gautier::visual_application::visual_model;

//Module level implementation.

//These functions requires a reader for the JSON data format.
//Other functions can be added that support other formats.
//A top level function can be defined such that it takes a data format parameter.
//	It would wrap these functions such that any data format could be supported.
//		Such a function takes a unit_type_screen, input format flag and updates the unit_type_screen.
//	Since a perceived need for this does not exist in this program, it is set aside.
void link_screen_element_to_screen(ns_visual_model::unit_type_screen& screen, const int lvl, const Json::Value& json_structure);
void link_screen_element_to_screen(ns_visual_model::unit_type_screen& screen, bool immediate_descendant, const std::string name, const std::string value);

void 
ns_visual_model::screen_build::get_all_screens(list_type_ptr_screen dest_screens, list_type_screen_layout_data_getter screen_layout_calls)
{
	std::cout << "getting all screens\n";

	for(auto screen_layout_text_get : screen_layout_calls)
	{
		const unit_type_screen
		src_screen = create_screen(screen_layout_text_get);

		const std::string
		src_screen_name = src_screen.name;

		auto dest_screen = 
		std::find_if(	  dest_screens->begin()
				, dest_screens->end()

				,[src_screen_name] (const unit_type_screen active_screen)
				{
					const std::string active_screen_name = active_screen.name;

					return (active_screen_name == src_screen_name);
				}
		);

		if(dest_screen != dest_screens->end())
		{
			std::copy(src_screen.screen_elements.begin()
				, src_screen.screen_elements.end()
				, dest_screen->screen_elements.begin()
			);
		}
		else
		{
			dest_screens->push_back(src_screen);
		}
	};

	return;
}

ns_visual_model::unit_type_screen 
ns_visual_model::screen_build::create_screen(const std::function<const std::string()> layout_data_getter)
{
	//This will show the full text of the screen layout to the console.
	std::cout << layout_data_getter() << "\n";

	Json::Value screen_layout_data;
	{
		Json::Reader screen_layout_reader;

		screen_layout_reader.parse(layout_data_getter(), screen_layout_data, false);
	}

	unit_type_screen screen;

	constexpr int initial_screen_layout_level = 0;

	link_screen_element_to_screen(screen, initial_screen_layout_level, screen_layout_data);

	return screen;
}

void 
link_screen_element_to_screen(ns_visual_model::unit_type_screen& screen, const int lvl, const Json::Value& json_structure)
{
	for(Json::Value::const_iterator json_field = json_structure.begin(); json_field != json_structure.end(); std::advance(json_field, 1))
	{
		const auto node_key = json_field.key();
		const uint node_type = json_field->type();

		const bool is_container = (
		   node_type == Json::ValueType::objectValue 
		|| node_type == Json::ValueType::arrayValue
		);

		if(is_container)
		{
			if(lvl == 2)
			{
				screen.screen_elements.push_back(ns_visual_model::unit_type_screen_element());
			}

			link_screen_element_to_screen(screen, (lvl + 1), *json_field);
		}
		else
		{
			const std::string key_name = node_key.asString();
			const std::string value = json_field->asString();

			bool descends_from_screen = (lvl == 3);

			link_screen_element_to_screen(screen, descends_from_screen, key_name, value);
		}
	}

	return;
}

void 
link_screen_element_to_screen(ns_visual_model::unit_type_screen& screen, bool immediate_descendant, const std::string name, const std::string value)
{
	if(!immediate_descendant)
	{
		if(name == "screen_name")
		{
			screen.name = value;
		}
		else if(name == "screen_label")
		{
			screen.label = value;
		}
	}//this entire function can be moved to gautier_visual_model.hxx
	else if(name == "screen_element_name")
	{
		screen.screen_elements.back().name = value;
	}
	else if(name == "screen_element_label")
	{
		screen.screen_elements.back().label = value;
	}
	else if(name == "receives_input")
	{
		screen.screen_elements.back().receives_input = (value == "true");
	}
	else if(name == "input_limit")
	{
		screen.screen_elements.back().input_limit = std::stoi(value);
	}
	else if(name == "x")
	{
		screen.screen_elements.back().x = std::stoi(value);
	}
	else if(name == "y")
	{
		screen.screen_elements.back().y = std::stoi(value);
	}
	else if(name == "w")
	{
		screen.screen_elements.back().w = std::stoi(value);
	}
	else if(name == "h")
	{
		screen.screen_elements.back().h = std::stoi(value);
	}
	else if(name == "screen_element_type")
	{
		screen.screen_elements.back().element_type = ns_visual_model::unit_type_screen_element_class_text::create_type(value);
	}

	return;
}

//Debug output for the screen layout text.
//	Not used presently. Useful for quick, console based troubleshooting.
void 
ns_visual_model::screen_build::print_defined_screens(list_type_ptr_screen screen_list)
{
	std::cout << "checkup\n";

	for(auto screen : *screen_list)
	{
		std::cout << "screen " << screen.name << " " << screen.label << " \n";

		for(auto screen_element : screen.screen_elements)
		{
			std::cout << "screen_element " << screen_element.name << " " << screen_element.label << " \n";

			std::cout << " x/y/w/h input(y/n) limit " << screen_element.x << " " << screen_element.y << " " << screen_element.w << " " << screen_element.h << " " << screen_element.receives_input << " " << screen_element.input_limit << "\n";
		}
	}

	return;
}

