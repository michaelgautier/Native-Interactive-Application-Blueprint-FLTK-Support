#ifndef __gautier_visual_application_application_window__
#define __gautier_visual_application_application_window__

#include <memory>
#include <tuple>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>

#include "gautier_visual_model.hxx"

namespace gautier
{
	namespace visual_application
	{
		namespace ns_visual_model = gautier::visual_application::visual_model;

		//An RAII handle to an FL Window
		class application_window
		{
			public:
				using window_ptr_type = std::shared_ptr<Fl_Double_Window>;

				application_window() : 

					_activity_processor_callbacks(),

					_window_config(
						ns_visual_model::window_size_type(0, 0), 
						_default_window_title, 
						ns_visual_model::visual_rgb_type(255, 255, 255)
					),

					_activity_past(),

					_screen_visuals{}

				{};

				//caller driven initialization
				ns_visual_model::window_config_type create_config(
					ns_visual_model::window_size_type window_size, 
					std::string title, 
					ns_visual_model::visual_rgb_type background_color
				);

				ns_visual_model::list_type_ptr_screen create_screen_list();

				void register_activity_processor_callbacks(
					ns_visual_model::callback_activity_processor a, 
					ns_visual_model::callback_activity_processor b
				);

				//Running the visual engine
				void activate();
				int execute();

				//Mapping application to visual
				const ns_visual_model::activity get_past_activity();

				void process_requests(const ns_visual_model::activity& a_past, ns_visual_model::activity& a_now);
				void update_state(const ns_visual_model::activity& a_past, ns_visual_model::activity& a_now);

				ns_visual_model::activity create_activity();

			private:
				unsigned int _screen_top_y_offset = 0;

				//for now, use a preset default of 24. That size tends to work well in some graphical environments.
				//It is a good size when the resolution is set to a standard setting.
				static constexpr unsigned short _default_visual_height = 24;

				const std::string _default_window_title = "gautier application window";

				using activity_processor_callback_pair = std::tuple<ns_visual_model::callback_activity_processor, ns_visual_model::callback_activity_processor>;
				activity_processor_callback_pair _activity_processor_callbacks;

				ns_visual_model::window_config_type _window_config;

				ns_visual_model::activity _activity_past;
				ns_visual_model::list_type_ptr_screen _screen_list = nullptr;

				window_ptr_type _window = nullptr;

				std::vector<Fl_Widget*> _screen_visuals;

				//Detailed implementation
				void update_visuals(const ns_visual_model::activity& activity_past, ns_visual_model::activity& activity_now);
				void build_screen_elements(ns_visual_model::unit_type_screen& screen);
				void add_all_visuals_to_screen();
				void trigger_all_visuals_to_redraw();
		};
	}
}
#endif
//Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 . Software distributed under the License is distributed on an "AS IS" BASIS, NO WARRANTIES OR CONDITIONS OF ANY KIND, explicit or implicit. See the License for details on permissions and limitations.

