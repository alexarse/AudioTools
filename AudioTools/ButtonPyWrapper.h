//
//  ButtonPyWrapper.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-15.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef ButtonPyWrapper_hpp
#define ButtonPyWrapper_hpp

#include "WindowPyWrapper.h"
#include <OpenAX/OpenAX.h>
#include <OpenAX/Button.h>


namespace ax {
	namespace python {
		
		class Button : public ax::python::Window {
		public:
			Button(ax::Button* btn);
			
			void SetBackgroundColor(const ax::Color& color);
			
			void SetContourColor(const ax::Color& color);
			
		private:
			ax::Button* _btn;
		};
		
		void export_python_wrapper_button();
	}
}

#endif /* ButtonPyWrapper_hpp */
