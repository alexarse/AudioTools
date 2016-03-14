/*
 * Copyright (c) 2015 Alexandre Arsenault.
 *
 * This file is part of axFrameworks.
 *
 * axFrameworks is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * License or use a commercial axFrameworks License.
 *
 * axFrameworks is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axFrameworks. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axFrameworks, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 */

#ifndef __AX_EDITOR_LOADER_H__
#define __AX_EDITOR_LOADER_H__

#include "axLib.h"
#include "axXml.h"

namespace ax {
namespace editor {
	
	class Loader {
	public:
		Loader(ax::Window* win);
		
		std::string OpenLayout(const std::string& path, bool clear = false);

		void SetupExistingWidget(ax::Window* widget, const std::string& builder_name, const std::string& pyo_fct = "");
		
	private:
		ax::Window* _win;
		
		void SetupEditWidget(ax::Window* win);
		void SetupPyoComponent(ax::Window* win, const std::string& fct_name);

		void SetupButtonPyoEvent(ax::Window* win);
		void SetupTogglePyoEvent(ax::Window* win);
		void SetupKnobPyoEvent(ax::Window* win);

	};
}
}

#endif // __AX_EDITOR_LOADER_H__