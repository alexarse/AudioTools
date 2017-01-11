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

#pragma once

#include <axlib/Xml.hpp>
#include <axlib/axlib.hpp>
#include <map>
#include <vector>

namespace at {
class WindowEventsComponent : public ax::util::Component {
public:
	static std::vector<std::pair<std::string, std::string>> ParseValuesFromWidgetNode(ax::Xml::Node& node);
	static void SetupWindowEventsWidget(ax::Window* win);

	/// Shared pointer.
	typedef std::shared_ptr<WindowEventsComponent> Ptr;

	WindowEventsComponent(ax::Window* win);

	virtual ~WindowEventsComponent();

	ax::Window* GetWindow();

	void SetFunctionValue(const std::pair<std::string, std::string>& fct_value);

	void SetFunctionsValue(const std::vector<std::pair<std::string, std::string>>& fcts_value);

	std::pair<std::string, std::string> GetFunctionValue(const std::string& fct_name) const;

	std::vector<std::pair<std::string, std::string>> GetFunctionsValue() const;

	std::vector<std::string> GetFunctionNameList() const;

	inline void SetOnPaint(const std::string& fct_name)
	{
		_on_paint = fct_name;
	}

	inline void SetOnMouseLeftDown(const std::string& fct_name)
	{
		_on_m_left_down = fct_name;
	}

	inline void SetOnMouseLeftUp(const std::string& fct_name)
	{
		_on_m_left_up = fct_name;
	}

	inline void SetOnMouseMotion(const std::string& fct_name)
	{
		_on_m_motion = fct_name;
	}

	inline void SetOnMouseEnter(const std::string& fct_name)
	{
		_on_m_enter = fct_name;
	}

	inline void SetOnMouseLeave(const std::string& fct_name)
	{
		_on_m_leave = fct_name;
	}

	inline const std::string& GetOnPaint() const
	{
		return _on_paint;
	}

	inline const std::string& GetOnMouseLeftDown() const
	{
		return _on_m_left_down;
	}

	inline const std::string& GetOnMouseLeftUp() const
	{
		return _on_m_left_up;
	}

	inline const std::string& GetOnMouseMotion() const
	{
		return _on_m_motion;
	}

	inline const std::string& GetOnMouseEnter() const
	{
		return _on_m_enter;
	}

	inline const std::string& GetOnMouseLeave() const
	{
		return _on_m_leave;
	}

protected:
	ax::Window* _win;
	std::string _on_paint;
	std::string _on_m_left_down;
	std::string _on_m_left_up;
	std::string _on_m_motion;
	std::string _on_m_enter;
	std::string _on_m_leave;

	std::map<std::string, std::string&> _fct_map;
};
}
