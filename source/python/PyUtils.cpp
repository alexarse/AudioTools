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

#include "python/PyUtils.hpp"
#include "PyoAudio.h"

namespace ax {
namespace python {
	void CallFuncNoParam(const std::string& fct_name)
	{
		PyoAudio::GetInstance()->ProcessString(fct_name + "();\n");
	}

	void CallFuncStrParam(const std::string& fct_name, const std::string& msg)
	{
		PyoAudio::GetInstance()->ProcessString(fct_name + "('" + msg + "');\n");
	}

	void CallFuncIntParam(const std::string& fct_name, int value)
	{
		PyoAudio::GetInstance()->ProcessString(fct_name + "(" + std::to_string(value) + ");\n");
	}

	void CallFuncRealParam(const std::string& fct_name, double value)
	{
		PyoAudio::GetInstance()->ProcessString(fct_name + "(" + std::to_string(value) + ");\n");
	}

	void CallFuncPointParam(const std::string& fct_name, const ax::Point& pos)
	{
		PyoAudio::GetInstance()->ProcessString(fct_name + "(ax.Point(" + pos.ToString() + "));\n");
	}
}
}
