#include <iostream>

#include "gautier_program.hxx"
#include "gautier_visual_application_window.hxx"
#include "gautier_visual_model.hxx"

namespace ns_visual_application = gautier::visual_application;
namespace ns_visual_model = gautier::visual_application::visual_model;

//Top-level Implementation Interface
void process_requests(const ns_visual_model::activity& activity_past, ns_visual_model::activity& activity_now);
void update_state(const ns_visual_model::activity& activity_past, ns_visual_model::activity& activity_now);

void fill_screen_list(ns_visual_model::list_type_ptr_screen);

const std::string get_screen_layout_data_login_screen();
const std::string get_screen_layout_data_profile_screen();

//Application Entry Point
//This is the general skeleton of the program.
//	Designed to work in all desktop operating environments.
int gautier::program::execute()
{
	using window_type = ns_visual_application::application_window;

	const std::string window_default_title = "Gautier Program";

	ns_visual_model::window_size_type 
	window_default_size(1024, 728);

	ns_visual_model::visual_rgb_type 
	window_background_color(0, 170, 172);

	window_type window;

	window.register_activity_processor_callbacks(process_requests, update_state);

	window.create_config(window_default_size, window_default_title, window_background_color);

	fill_screen_list(window.create_screen_list());

	window.activate();

	return window.execute();
}

//General, Top-level Implementation
void 
process_requests(const ns_visual_model::activity& activity_past, ns_visual_model::activity& activity_now)
{
	std::cout << "outer process requests\n";

	if(activity_now.window_closing)
	{
		std::cout << "window closing ... activity changed no further\n";
	}
	else if(activity_past.screen_list && !activity_past.screen_list->empty())
	{
		//default to first available
		if(activity_now.screen.name.empty())
		{
			std::cout << "setting activity screen\n";
			activity_now.screen = activity_now.screen_list->front();
		}
		//build fresh
		else if(activity_now.visual_clicked)
		{
			if(!activity_now.screen_list)
			{
				std::cout << "screen list nullptr in process_requests\n";
			}

			if(activity_now.screen_list && activity_now.screen.name == "loginscreen" 
			&& activity_now.screen_element.name == "loginbutton")
			{
				//find the next screen and set it.
				std::string want = "profilescreen";

				for(auto screen : *activity_now.screen_list)
				{
					if(screen.name == want)
					{
						activity_now.screen = screen;
						break;
					}
				}
			}
		}
	}

	update_state(activity_past, activity_now);

	return;
}

void 
update_state(const ns_visual_model::activity& activity_past, ns_visual_model::activity& activity_now)
{
	std::cout << "outer update state\n";

	if(activity_now.screen_list)
	{
		std::cout << "screen list size " << activity_now.screen_list->size() << " screen name " << activity_now.screen.name << " planar name " << activity_now.screen_element.name << "\n";

		//if(activity_now.screen_element.name.empty())
		//{
			//print_defined_screens(activity_now.screen_list);
		//}
	}
	else
	{
		std::cout << "screen list nullptr\n";
	}

	return;
}

//Screens in text format
//	The initial screen layouts.
//	They are embedded right in the program to create a self-contained layout definition.
const std::string get_screen_layout_data_login_screen()
{
	ns_visual_model::unit_type_screen_element_class_text 
	element_type;

	return 
	"[		\
		{	\
			\"screen_name\": \"loginscreen\", \
			\"screen_label\": \"Login Screen\", \
			 \"elements\": [\
				{ \
					\"screen_element_name\": \"username\", \
					\"screen_element_label\": \"User name\", \
					\"receives_input\": true, \
					\"input_limit\": 12, \
					\"x\": 100.00, \
					\"y\": 90.00, \
					\"w\": 180.00, \
					\"h\": 24.00, \
					\"screen_element_type\": " + element_type.text_field + " \
				},\
				{ \
					\"screen_element_name\": \"password\", \
					\"screen_element_label\": \"Password\", \
					\"receives_input\": true, \
					\"input_limit\": 8, \
					\"x\": 300.00, \
					\"y\": 90.00, \
					\"w\": 80.00, \
					\"h\": 24.00, \
					\"screen_element_type\": " + element_type.text_field + " \
				},\
				{ \
					\"screen_element_name\": \"loginbutton\", \
					\"screen_element_label\": \"Login\", \
					\"receives_input\": false, \
					\"input_limit\": 0, \
					\"x\": 0.00, \
					\"y\": 0.00, \
					\"w\": 0.00, \
					\"h\": 0.00, \
					\"screen_element_type\": " + element_type.button + " \
				}\
			]\
		}	\
	]		\
	";
}

const std::string get_screen_layout_data_profile_screen()
{
	ns_visual_model::unit_type_screen_element_class_text 
	element_type;

	return
	"[		\
		{	\
			\"screen_name\": \"profilescreen\", \
			\"screen_label\": \"Profile Screen\", \
			 \"elements\": [\
				{ \
					\"screen_element_name\": \"firstname\", \
					\"screen_element_label\": \"First name\", \
					\"receives_input\": true, \
					\"input_limit\": 24, \
					\"x\": 100.00, \
					\"y\": 90.00, \
					\"w\": 130.00, \
					\"h\": 24.00, \
					\"screen_element_type\": " + element_type.text_field + " \
				},\
				{ \
					\"screen_element_name\": \"lastname\", \
					\"screen_element_label\": \"Last name\", \
					\"receives_input\": true, \
					\"input_limit\": 24, \
					\"x\": 270.00, \
					\"y\": 90.00, \
					\"w\": 64.00, \
					\"h\": 24.00, \
					\"screen_element_type\": " + element_type.text_field + " \
				}\
			]\
		}	\
	]		\
	";
}

void fill_screen_list(ns_visual_model::list_type_ptr_screen screen_list)
{
		ns_visual_model::screen_build::list_type_screen_layout_data_getter 
		screen_descriptions = {
			get_screen_layout_data_login_screen,
			get_screen_layout_data_profile_screen
		};

		ns_visual_model::screen_build::get_all_screens(screen_list, screen_descriptions);

	return;
}
//Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 . Software distributed under the License is distributed on an "AS IS" BASIS, NO WARRANTIES OR CONDITIONS OF ANY KIND, explicit or implicit. See the License for details on permissions and limitations.

