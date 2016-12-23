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

#include "atk/AudioCore.hpp"
#include "python/m_pyo.h"
#include <axlib/axlib.hpp>

class PyoAudio : public atk::AudioCore {
public:
	static PyoAudio* GetInstance();

	~PyoAudio();

	enum Events : ax::event::Id { RMS_VALUE_CHANGE = 89831 };

	void ProcessString(const std::string& script);
	bool IsServerStarted();

	void StopServer();

	void ProcessMidi(int status, int byte1, int byte2)
	{
		pyo_add_midi_event(_pyo, status, byte1, byte2);
	}

	void ReloadScript(const std::string& path);

	void SetConnectedObject(ax::event::Object* obj)
	{
		_connected_obj = obj;
	}

	std::string GetClassBrief(const std::string& name);

protected:
	static PyoAudio* _global_audio;

	PyoAudio();

	virtual int CoreCallbackAudio(const float* input, float* output, unsigned long frameCount);

	void CreateServer(float sr, int bufsize, int chnls);

private:
	ax::event::Object* _connected_obj;
	PyThreadState* _pyo;
	int _rms_count;
	std::pair<double, double> _rms_values;
	float* _output;
	int _server_id;
	void (*_callback_fct)(int);
};
