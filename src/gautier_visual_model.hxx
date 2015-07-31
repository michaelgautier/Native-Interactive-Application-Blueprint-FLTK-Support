#ifndef __gautier_visual_application_visual_model__
#define __gautier_visual_application_visual_model__

#include <functional>
#include <memory>
#include <tuple>
#include <vector>

#include <json/json.h>

namespace gautier
{
	namespace visual_application
	{
		namespace visual_model
		{
			using window_size_type = std::tuple<unsigned, unsigned>;
			using visual_rgb_type = std::tuple<unsigned char, unsigned char, unsigned char>;
			using window_config_type = std::tuple<window_size_type, std::string, visual_rgb_type>;

			using unit_type_screen_element_class_size_type = unsigned int;

			enum unit_type_screen_element_class : unit_type_screen_element_class_size_type
			{
				text_field,
				button
			};

			struct unit_type_screen_element_class_text
			{
				std::string 
					text_field = std::to_string(static_cast<unit_type_screen_element_class_size_type>(unit_type_screen_element_class::text_field)),
					button = 	 std::to_string(static_cast<unit_type_screen_element_class_size_type>(unit_type_screen_element_class::button))
				;

				static 
				unit_type_screen_element_class 
				create_type(const std::string& text_value)
				{
					auto nvalue = std::stoi(text_value);
					auto tvalue = static_cast<unit_type_screen_element_class>(nvalue);

					return tvalue;
				}
			};

			struct unit_type_screen_element
			{
				bool 
					//A value of true is a hint to consider text and 
					//other inputs as affiliated with this planar.
					receives_input = false,
					label_visible = true
				;

				unsigned int 
					//Specifies how much of an input quantity 
					//(characters, selection options, etc) will be accepted.
					input_limit = 16
				;

				unit_type_screen_element_class element_type = unit_type_screen_element_class::text_field;

				int 
					x = -1,
					y = -1,
					w = -1,
					h = -1
				;

				std::string name = "",
					label = "",
					text_value = ""
				;

			};

			using list_type_screen_element = std::vector<unit_type_screen_element>;

			struct unit_type_screen
			{
				list_type_screen_element
					screen_elements = {};

				std::string name = "",
					label = ""
				;
			};

			using list_type_screen = std::vector<unit_type_screen>;
			using list_type_ptr_screen = std::shared_ptr<list_type_screen>;

			struct activity
			{
				bool
					items_processed = false,
					window_closing = false,
					window_opening = false,
					window_esc_key_pressed = false,
					visual_clicked = false
				;

				list_type_ptr_screen screen_list = nullptr;
				unit_type_screen_element screen_element{};
				unit_type_screen screen{};

				activity()
				{
					return;
				}

				activity(const activity& in)
				{
					screen_list = in.screen_list;
					screen_element = in.screen_element;
					screen = in.screen;

					return;
				}
			};

			using callback_activity_processor = std::function<void(const activity&, activity&)>;

			namespace screen_build
			{
				using list_type_screen_layout_data_getter = std::vector<std::function<const std::string()>>;

				//Main function for translating visuals defined in text into defined data structures.
				//Collects all screens into a top level data structure.
				void get_all_screens(list_type_ptr_screen screens, list_type_screen_layout_data_getter layout_data_getters);

				//Creates a screen
				unit_type_screen create_screen(std::function<const std::string()> screen_layout_calls);

				//Diagnostic purposes.
				void print_defined_screens(list_type_ptr_screen dest_screens);

			}
		}
	}
}
#endif
//Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 . Software distributed under the License is distributed on an "AS IS" BASIS, NO WARRANTIES OR CONDITIONS OF ANY KIND, explicit or implicit. See the License for details on permissions and limitations.

