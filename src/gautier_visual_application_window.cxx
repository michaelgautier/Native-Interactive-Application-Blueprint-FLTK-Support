#include <algorithm>
#include <iostream>
#include <set>

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/names.h>
#include <FL/fl_draw.H>

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>

#include "gautier_visual_application_window.hxx"

namespace ns_visual_application = gautier::visual_application;
namespace ns_visual_model = gautier::visual_application::visual_model;

//Module type aliases
struct unit_type_window_widget_activity
{
	ns_visual_application::application_window* window = nullptr;
	ns_visual_model::activity activity_now{};
};

//Module variables
Fl_Callback_p default_window_callback = nullptr;

//Module function prototypes
void window_callback(Fl_Widget* wd, void* caller);
void screen_visual_callback(Fl_Widget* wd, void* activity);
void configure_window(ns_visual_application::application_window::window_ptr_type window, const ns_visual_model::window_config_type& window_config);
void set_size_range(ns_visual_application::application_window::window_ptr_type window, const ns_visual_model::window_config_type& window_config, ns_visual_model::window_size_type screen_size);

//Class Interface
ns_visual_model::window_config_type 
ns_visual_application::application_window::create_config(ns_visual_model::window_size_type window_size, std::string title, ns_visual_model::visual_rgb_type background_color)
{
	_screen_top_y_offset = Fl::y();

	_window_config = make_tuple(window_size, title, background_color);

	return _window_config;
}

ns_visual_model::list_type_ptr_screen 
ns_visual_application::application_window::create_screen_list()
{
	_screen_list.reset(new ns_visual_model::list_type_screen);
	_screen_list->reserve(32);

	_screen_visuals.reserve(4096);

	_activity_past.screen_list = _screen_list;

	return _screen_list;
}

void ns_visual_application::application_window::register_activity_processor_callbacks(ns_visual_model::callback_activity_processor a, ns_visual_model::callback_activity_processor b)
{
	_activity_processor_callbacks = activity_processor_callback_pair(a, b);

	return;
}

void ns_visual_application::application_window::activate()
{
	Fl::visual(FL_DOUBLE|FL_INDEX);

	_window.reset(new Fl_Double_Window(0,0));

	default_window_callback = _window->callback();

	_window->callback(window_callback, this);

	configure_window(_window, _window_config);

	Fl_Group::current(nullptr);

	ns_visual_model::activity activity_now = create_activity();

	process_requests(_activity_past, activity_now);

	return;
}

int ns_visual_application::application_window::execute()
{
	_window->show();

	return Fl::run();
}

const ns_visual_model::activity
ns_visual_application::application_window::get_past_activity()
{
	return _activity_past;
}

void ns_visual_application::application_window::process_requests(const ns_visual_model::activity& activity_past, ns_visual_model::activity& activity_now)
{
	std::cout << "inner process requests\n";

	if(activity_now.window_opening)
	{
		std::cout << "window opening\n";
		//any additional initialization work.
		//nice to have but the close event in FLTK is not guaranteed for all code paths.
		//will simply acknowledge an opening window if the event surfaces.
	}
	else if(activity_now.window_closing)
	{
		std::cout << "window closing\n";
		//Cleanup work. Unregistering function callbacks, etc.
		_activity_processor_callbacks = {};
		_activity_past = {};
		_window_config = {};
		_screen_list->clear();
		_screen_visuals.clear();
		_window->clear();
	}
	else
	{
		auto process_requests_cb = std::get<0>(_activity_processor_callbacks);

		if(process_requests_cb)
		{
			process_requests_cb(activity_past, activity_now);

			update_state(activity_past, activity_now);
		}
	}

	return;
}

void ns_visual_application::application_window::update_state(const ns_visual_model::activity& activity_past, ns_visual_model::activity& activity_now)
{
	std::cout << "inner update state\n";

	update_visuals(activity_past, activity_now);

	_activity_past = activity_past;

	_window->redraw();

	return;
}

//Private Class Interface
void ns_visual_application::application_window::update_visuals(const ns_visual_model::activity& activity_past, ns_visual_model::activity& activity_now)
{
	bool screen_changed = (activity_past.screen.name != activity_now.screen.name);

	//start a new screen fresh
	if(screen_changed)
	{
		std::cout << "starting a screen\n";

		std::string screen_title = activity_now.screen.label;

		if(!screen_title.empty())
		{
			std::string* window_title = new std::string(std::get<1>(_window_config) + " / " + screen_title);

			_window->label(window_title->data());
		}

		_screen_visuals.clear();
		_window->clear();

		build_screen_elements(activity_now.screen);

		add_all_visuals_to_screen();
	}
	//update existing screen
	else
	{
		std::cout << "screen update\n";
		//This is part of the next step. User interface updates.
	}

	return;
}

//Somewhat generic.
//Maps visual elements to a general data structure.
//A data structure with sufficient detail sent back out to the main application is the goal. 

//In this particular design, Fast and Light Toolkit's callback mechanism is used to 
//	link a specific visual element to a generic activity data structure.

//The overall process is a macro level loop over incremental changes to the activity data structure.
//As the activity data passes through functions, the program behavior and output changes.

//This function sets up a screen with visual elements. The initial screen setting for a given screen.
//An as yet to be defined function will deal with updates to the screen.
void ns_visual_application::application_window::build_screen_elements(ns_visual_model::unit_type_screen& screen)
{
	std::cout << "making " << screen.screen_elements.size() << " widgets for: " << screen.name << "\n";

	for(auto screen_element : screen.screen_elements)
	{
		std::cout << "making widget for: " << screen_element.label << "\n";

		int lastx=screen_element.x, lasty=screen_element.y, lastw=screen_element.w, lasth=screen_element.h;

		std::cout << lastx << "," << lasty << "," << lastw << "," << lasth << "\n";

		if(screen_element.element_type == ns_visual_model::unit_type_screen_element_class::text_field)
		{
			_screen_visuals.push_back(new Fl_Input(lastx, lasty, lastw, lasth, screen_element.label.data()));

			Fl_Input* screen_visual = static_cast<Fl_Input*>(_screen_visuals.back());
			//Need to specify the label alignment eventually.
			//Need to put the label alignment in the screen layout text in the main program.
			screen_visual->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
			screen_visual->maximum_size(screen_element.input_limit);
		}
		else if(screen_element.element_type == ns_visual_model::unit_type_screen_element_class::button)
		{
			_screen_visuals.push_back(new Fl_Button(lastx, lasty, lastw, lasth, screen_element.label.data()));

			auto screen_visual = _screen_visuals.back();

			//Activity setup for FLTK callback.
			unit_type_window_widget_activity* window_widget_activity_pair = new unit_type_window_widget_activity;
			window_widget_activity_pair->window = this;
			window_widget_activity_pair->activity_now = create_activity();
			window_widget_activity_pair->activity_now.screen_element = screen_element;
			window_widget_activity_pair->activity_now.screen = screen;
			window_widget_activity_pair->activity_now.visual_clicked = true;

			//All visuals with button behavior where follow action is expected
			//	will be default linked to a callback.
			screen_visual->callback(screen_visual_callback, window_widget_activity_pair);

			std::set<int> pos{lastx, lasty, lastw, lasth};
			auto min_result = std::min_element(pos.begin(), pos.end());
			auto max_result = std::max_element(pos.begin(), pos.end());

			//All zeros trigger automatic arrangement for the screen element.
			if((*min_result) < 1 && (*max_result) < 1)
			{
				std::cout << "a button position element is 0 \n";

				if(lastw == 0)
				{
					std::string* widget_text = new std::string(screen_visual->label());

					lastw = static_cast<int>(fl_width(widget_text->data()));
				}

				if(lasth == 0)
				{
					//A use case for auto deciding height is not yet apparent here.
					lasth = _default_visual_height;
				}

				if(lastx == 0)
				{
					lastx = _window->w() - lasth;
				}

				if(lasty == 0)
				{
					//screen offset treats the title bar. Should be zero in situations where there is no title bar.
					lasty = ((_window->h() - _screen_top_y_offset) - lasth);
				}

				std::cout << "button position rewrite xywh " << lastx << "/" << lasty << "/" << lastw << "/" << lasth << "\n";

				//forced in an offset, need to put this into the screen_element definition later on.
				screen_visual->resize(lastx-20, lasty-20, lastw, lasth);
			}
		}
	}

	return;
}

void
ns_visual_application::application_window::add_all_visuals_to_screen()
{
	for(auto screen_visual : _screen_visuals)
	{
		if(screen_visual)
		{
			screen_visual->redraw();
			_window->add(screen_visual);
		}
	}

	return;
}

void
ns_visual_application::application_window::trigger_all_visuals_to_redraw()
{
	for(auto screen_visual : _screen_visuals)
	{
		if(screen_visual)
		{
			screen_visual->redraw();
		}
	}

	return;
}

ns_visual_model::activity 
ns_visual_application::application_window::create_activity()
{
	ns_visual_model::activity activity_value(_activity_past);

	return activity_value;
}

//Module Implementation
void screen_visual_callback(Fl_Widget* wd, void* data)
{
	unit_type_window_widget_activity* window_widget_activity = static_cast<unit_type_window_widget_activity*>(data);

	if(wd && window_widget_activity)
	{
		ns_visual_application::application_window* window = window_widget_activity->window;

		if(window)
		{
			std::cout << "clicked ";
			ns_visual_model::activity activity = window_widget_activity->activity_now;

			{
				std::string name = activity.screen.name;
				std::cout << name;
			}

			std::cout << " ";

			{
				std::string name = activity.screen_element.name;
				std::cout << name;
			}

			std::cout << "\n";

			window->process_requests(window->get_past_activity(), window_widget_activity->activity_now);
		}
	}

	return;
}

void window_callback(Fl_Widget* wd, void* data)
{
	ns_visual_application::application_window* window = static_cast<ns_visual_application::application_window*>(data);

	int event = Fl::event();

	std::cout << "evt " << fl_eventnames[event] << " evt id " << event << "\n";

	ns_visual_model::activity activity_past = window->get_past_activity();

	ns_visual_model::activity 
	activity_now = window->create_activity();

	activity_now.window_opening = (event == FL_SHOW);
	activity_now.window_closing = (event == FL_CLOSE);
	activity_now.window_esc_key_pressed = (event == FL_SHORTCUT && Fl::event_key() == FL_Escape);

	window->process_requests(activity_past, activity_now);

	if(default_window_callback && !activity_now.window_esc_key_pressed)
	{
		std::cout << "default window callback\n";

		default_window_callback(wd, nullptr);
	}

	return;
}

void configure_window(ns_visual_application::application_window::window_ptr_type window, const ns_visual_model::window_config_type& window_config)
{
	if(window->w() == 0 && window->h() == 0)
	{
		auto screen_width = Fl::w();
		auto screen_height = Fl::h();

		window->size(screen_width, screen_height);
		window->label(std::get<1>(window_config).data());

		set_size_range(window, window_config, ns_visual_model::window_size_type(screen_width, screen_height));
	}

	ns_visual_model::visual_rgb_type config_color = std::get<2>(window_config);

	auto r = std::get<0>(config_color);
	auto g = std::get<1>(config_color);
	auto b = std::get<2>(config_color);

	Fl_Color window_color = fl_rgb_color(r, g, b);

	window->color(window_color);

	return;
}

void set_size_range(ns_visual_application::application_window::window_ptr_type window, const ns_visual_model::window_config_type& window_config, ns_visual_model::window_size_type screen_size)
{
	fl_font(FL_SCREEN, 10);

	auto preferred_width = std::get<0>(std::get<0>(window_config));
	auto preferred_height = std::get<1>(std::get<0>(window_config));

	auto screen_width = std::get<0>(screen_size); 
	auto screen_height = std::get<1>(screen_size);

	auto min_width = std::min(preferred_width, screen_width);
	auto min_height = std::min(preferred_height, screen_height);

	if(min_width == screen_width)
	{
		min_width = screen_width/2;
	}

	if(min_height == screen_height)
	{
		min_height = screen_height/2;
	}

	std::cout << "screen w/h  " << screen_width << " " << screen_height << "\n";
	std::cout << "minimum w/h " << min_width << " " << min_height << "\n";

	window->size_range(min_width, min_height, screen_width, screen_height);

	return;
}
//Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 . Software distributed under the License is distributed on an "AS IS" BASIS, NO WARRANTIES OR CONDITIONS OF ANY KIND, explicit or implicit. See the License for details on permissions and limitations.

