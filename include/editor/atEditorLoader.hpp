/*
 * Copyright (c) 2016 AudioTools - All Rights Reserved
 *
 * This Software may not be distributed in parts or its entirety
 * without prior written agreement by AudioTools.
 *
 * Neither the name of the AudioTools nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY AUDIOTOOLS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AUDIOTOOLS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Written by Alexandre Arsenault <alx.arsenault@gmail.com>
 */

#ifndef __AX_EDITOR_LOADER_H__
#define __AX_EDITOR_LOADER_H__

#include <axlib/Xml.hpp>
#include <axlib/axlib.hpp>

namespace at {
namespace editor {

	class Loader {
	public:
		Loader(ax::Window* win);

		std::string OpenLayout(const std::string& path, bool clear = false);
		std::string OpenLayoutContent(const std::string& content, bool clear = false);

		void SetupExistingWidget(ax::Window* widget, const std::string& builder_name,
			const std::string& pyo_fct = "", const std::string& unique_name = "");

	private:
		ax::Window* _win;

		std::string OpenLayoutFromXml(ax::Xml& xml);

		void SetupEditWidget(ax::Window* win);
		void SetupPyoComponent(ax::Window* win, const std::string& fct_name);
		void SetupUniqueNameComponent(ax::Window* win, const std::string& name);

		void SetupButtonPyoEvent(ax::Window* win);
		void SetupTogglePyoEvent(ax::Window* win);
		void SetupKnobPyoEvent(ax::Window* win);
		void SetupSliderPyoEvent(ax::Window* win);
		void SetupNumberBoxPyoEvent(ax::Window* win);
	};
}
}

#endif // __AX_EDITOR_LOADER_H__
