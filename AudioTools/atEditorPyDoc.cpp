//
//  atEditorPyoDoc.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-19.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "PyoAudio.h"
#include "atEditorPyDoc.h"
#include "atEditorPyDocElement.h"
#include "atEditorPyDocSeparator.h"

namespace at {
namespace editor {
	ax::StringPairVector GetClassNameBriefs(const ax::StringVector& names)
	{
		ax::StringPairVector elems;
		elems.reserve(names.size());

		for (auto& n : names) {
			elems.push_back(ax::StringPair(n, PyoAudio::GetInstance()->GetClassBrief(n)));
		}

		return elems;
	}

	ax::Point PyDoc::AddSeparator(const ax::Point& pos, const std::string& name, const ax::StringVector& args)
	{
		auto sep = ax::shared<PyDocSeparator>(
			ax::Rect(pos, ax::Size(win->dimension.GetSize().x, 20)), name, GetClassNameBriefs(args));

		_scroll_panel->node.Add(sep);

		sep->GetWindow()->AddConnection(PyDocSeparator::NEED_RESIZE, GetOnNeedResize());
		_separators.push_back(sep.get());

		return sep->GetWindow()->dimension.GetRect().GetNextPosDown(0);
	}

	PyDoc::PyDoc(const ax::Rect& rect)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &PyDoc::OnPaint);

		_scroll_panel = ax::Window::Create(ax::Rect(ax::Point(0, 0), rect.size));
		win->node.Add(ax::Window::Ptr(_scroll_panel));

		ax::Point pos(0, 0);
		ax::Size size(rect.size.x, 40);

		pos = AddSeparator(
			pos, "Audio Signal Analysis", { "Follower", "Follower2", "ZCross", "Yin", "Centroid",
											  "AttackDetector", "Spectrum", "Scope", "PeakAmp" });

		pos = AddSeparator(pos, "Arithmetic", { "Sin", "Cos", "Tan", "Tanh", "Abs", "Sqrt", "Log", "Log2",
												  "Log10", "Atan2", "Floor", "Ceil", "Round", "Pow" });

		pos = AddSeparator(pos, "Control Signals", { "Fader", "Adsr", "Linseg", "Expseg", "Sig", "SigT" });

		pos = AddSeparator(pos, "Dynamic management",
			{ "Clip", "Degrade", "Mirror", "Compress", "Gate", "Balance", "Min", "Max", "Wrap" });

		pos = AddSeparator(pos, "Special Effects",
			{ "Disto", "Delay", "SDelay", "Delay1", "Waveguide", "AllpassWG", "Freeverb", "Convolve",
				"WGVerb", "Chorus", "Harmonizer", "FreqShift", "STRev", "SmoothDelay" });

		pos = AddSeparator(
			pos, "Filters", { "Biquad", "Biquadx", "Biquada", "EQ", "Tone", "Atone", "Port", "DCBlock",
								"BandSplit", "FourBand", "Hilbert", "Allpass", "Allpass2", "Phaser",
								"Vocoder", "IRWinSinc", "IRAverage", "IRPulse", "IRFM", "SVF", "Average",
								"Reson", "Resonx", "ButLP", "ButHP", "ButBP", "ButBR", "ComplexRes" });

		pos = AddSeparator(pos, "Fast Fourier Transform",
			{ "FFT", "IFFT", "PolToCar", "CarToPol", "FrameAccum", "FrameDelta", "CvlVerb", "Vectral" });

		pos = AddSeparator(pos, "Phase Vocoder",
			{ "PVAnal", "PVSynth", "PVAddSynth", "PVTranspose", "PVVerb", "PVGate", "PVCross", "PVMult",
				"PVMorph", "PVFilter", "PVDelay", "PVBuffer", "PVShift", "PVAmpMod", "PVFreqMod",
				"PVBufLoops", "PVBufTabLoops", "PVMix" });

		pos = AddSeparator(pos, "Signal Generators",
			{ "Blit", "BrownNoise", "CrossFM", "FM", "Input", "LFO", "Lorenz", "Noise", "Phasor", "PinkNoise",
				"RCOsc", "Rossler", "Sine", "SineLoop", "SumOsc", "SuperSaw" });

		_scroll_panel->property.AddProperty("BlockDrawing");
		_scroll_panel->dimension.SetSizeNoShowRect(ax::Size(rect.size.x, pos.y));

		ax::ScrollBar::Info sInfo;
		sInfo.normal = ax::Color(0.60);
		sInfo.hover = ax::Color(0.65);
		sInfo.clicking = ax::Color(0.62);
		sInfo.slider_contour = ax::Color(0.0, 0.2);
		sInfo.contour = ax::Color(0.8, 0.2);
		sInfo.bg_top = ax::Color(0.8, 0.2);
		sInfo.bg_bottom = ax::Color(0.82, 0.2);

		const ax::Rect sRect(rect.size.x - 9, 0, 10, rect.size.y);
		_scrollBar = ax::shared<ax::ScrollBar>(sRect, ax::ScrollBar::Events(), sInfo);
		win->node.Add(_scrollBar);

		_scrollBar->SetWindowHandle(_scroll_panel);
		_scrollBar->UpdateWindowSize(_scroll_panel->dimension.GetSize());
	}

	void PyDoc::OnNeedResize(const ax::Event::EmptyMsg& msg)
	{
		ax::Print("PyDoc need resize");
		ax::Point pos(0, 0);
		for (auto& n : _separators) {
			n->GetWindow()->dimension.SetPosition(ax::Point(pos));
			pos = n->GetWindow()->dimension.GetRect().GetNextPosDown(0);
		}

		const ax::Size size(_scroll_panel->dimension.GetSize().x, pos.y);
		_scroll_panel->dimension.SetSizeNoShowRect(size);
		_scrollBar->UpdateWindowSize(size);
	}

	void PyDoc::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(1.0));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(0.7));
		gc.DrawRectangleContour(rect);
	}
}
}