/*
 * Copyright (c) 2017 AudioTools - All Rights Reserved
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

#include "editor/GridSnapProxy.hpp"
#include "editor/atEditorGridWindow.hpp"

namespace at {
namespace editor {
	GridSnapProxy::GridSnapProxy(GridWindow* gwin)
		: _gwin(gwin)
	{
	}

	int GridSnapProxy::GetGridSpace() const
	{
		return _gwin->_grid_space;
	}

	bool GridSnapProxy::IsSnapActive() const
	{
		return _gwin->_is_snap_active;
	}

	void GridSnapProxy::SetSnap(bool active)
	{
		_gwin->_is_snap_active = active;
	}

	ax::Point GridSnapProxy::FindClosestPosition(const ax::Point& pos) const
	{
		const int grid_space = _gwin->_grid_space;
		int x = -1;
		int y = -1;

		// Find x position.
		if (pos.x < grid_space) {
			x = 0;
		}
		else {
			const std::vector<ax::FPoint>& vertical_lines = _gwin->_vertical_lines_array;
			for (int i = 0; i < vertical_lines.size() - 2; i += 2) {
				if (pos.x >= vertical_lines[i].x && pos.x < vertical_lines[i + 2].x) {
					x = vertical_lines[i].x;
					break;
				}
			}

			if (x == -1) {
				x = vertical_lines[vertical_lines.size() - 1].x;
			}
		}

		// Find y position.
		if (pos.y < grid_space) {
			y = 0;
		}
		else {
			const std::vector<ax::FPoint>& horizontal_lines = _gwin->_horizontal_lines_array;
			for (int i = 0; i < horizontal_lines.size() - 2; i += 2) {
				if (pos.y >= horizontal_lines[i].y && pos.y < horizontal_lines[i + 2].y) {
					y = horizontal_lines[i].y;
					break;
				}
			}

			if (y == -1) {
				y = horizontal_lines[horizontal_lines.size() - 1].y;
			}
		}

		return ax::Point(x, y);
	}

	void GridSnapProxy::SetGridSpace(int space)
	{
		_gwin->SetGridSpace(space);
	}
}
}
