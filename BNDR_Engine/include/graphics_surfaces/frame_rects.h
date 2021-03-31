/*MIT License

Copyright (c) 2021 Caleb Christopher Bender

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#pragma once
#include <pch.h>
#include "include/graphics_surfaces/primitives/graphical_bedrocks.h";

namespace bndr {

	// interface for frame instances
	class Frame {

	public:
		// should be called in update function
		virtual void processEvents(const Window& window) = 0;
		// updates the current state of the frame, and takes in the delta time since its last update call
		virtual void update(float deltaTime) = 0;
		// rasterizes the frame to the screen
		// its only purpose is to draw the frame and return
		virtual void render() = 0;
		// redefines the translation of the frame
		virtual void translate(float x, float y) = 0;
		// adds to the current translation
		virtual void addTranslation(float xChange, float yChange) = 0;
		// rotate the frame by an angle in radians (default is counter-clockwise)
		virtual void rotate(float theta) = 0;
		// add to the current rotation
		virtual void addRotation(float thetaChange) = 0;
		// redefines the scale of the frame
		// the scale is the factor you want the frame's dimensions to increase by (i.e. 0.5 makes it half the size and 2.0 makes it double)
		// scale both x and y separately
		virtual void scale(float xScale, float yScale) = 0;
		// scale both x and y together
		virtual void scale(float _scale) = 0;
		// add to the current scale
		// separately
		virtual void addScale(float xScaleChange, float yScaleChange) = 0;
		// together
		virtual void addScale(float scaleChange) = 0;
		// get the rectangle of the frame in this format: {topLeftX, topLeftY, width, height}
		virtual std::vector<float> getRect() = 0;
		// get the center of the rectangle


	};


	// style flags to apply to the ui rectangle
	// ANCHOR_TO_CENTER: sets the x and y coordinate to be at the center of rectangle as opposed to the top left corner
	// X_COORD_IS_PERCENT: if flag is set the x coordinate is treated as a percentage value where 100.0f would be 100% of the screen width
	// Y_COORD_IS_PERCENT: if flag is set the y coordinate is treated as a percentage value where 100.0f would be 100% of the screen height
	// WIDTH_IS_PERCENT: if flag is set the width is treated as a percentage value where 100.0f would be 100% of the screen width
	// HEIGHT_IS_PERCENT: if flag is set the height is treated as a percentage value where 100.0f would be 100% of the screen height
	enum BNDR_API FrameRectFlags {

		FRAME_ANCHOR_TO_CENTER = 1,
		FRAME_X_COORD_IS_PERCENT = 2,
		FRAME_Y_COORD_IS_PERCENT = 4,
		FRAME_WIDTH_IS_PERCENT = 8,
		FRAME_HEIGHT_IS_PERCENT = 16,
		// this flag prevents memory leaks or double frees
		FRAME_IS_FRAME_RECT = 256
	};


	// an optimal class for a graphics component with only a single graphics frame
	class BNDR_API FrameRect {

	protected:
		// this TexturedRect will manage all the lower level drawing stuff for us
		TexturedRect* texRect = nullptr;
		// the flags of the frame rect
		uint flags;
	public:
		FrameRect(float x, float y, float width, float height, std::vector<RGBAData>&& colors = { bndr::WHITE }, Texture* newTex = nullptr, uint styleFlags = 0);
		FrameRect(const FrameRect& frameRect) : texRect(new TexturedRect(*frameRect.texRect)), flags(frameRect.flags) {}
		// move constructor and assignment operator are not allowed
		FrameRect(FrameRect&&) = delete;
		FrameRect& operator=(const FrameRect&) = delete;
		inline void render() { texRect->render(); }
		~FrameRect();
	};

	// container for animation information
	// fps: frames per second
	// frames: the textures that make up the animation
	// loop: if true the animation will loop indefinitely otherwise it will run once and stop
	struct AnimationInfo {

		uint fps;
		std::vector<Texture> frames;
		bool loop;
	};


	// AnimationRect is a FrameRect with different animation cycles
	class BNDR_API AnimationRect : public FrameRect {

	protected:
		// the animation texture vector is a vector of AnimationInfos
		using AnimationTexList = typename std::vector<AnimationInfo>;
		using AnimationInitializerList = typename std::initializer_list<AnimationInfo>;

		AnimationTexList* animationCycles = nullptr;
		// selects the animation to run
		int animationIndex;
		// selects the frame within the animation
		int animationFrame;
		// the time elapsed since the last frame change
		float elapsedTime;
		// should the animation animate?
		bool shouldAnimate;
	public:

		AnimationRect(float x, float y, float width, float height,
			AnimationInitializerList&& animationCycleList, std::vector<RGBAData>&& colors = { bndr::WHITE },
			uint styleFlags = 0);
		// copy constructor
		AnimationRect(const AnimationRect& animationRect) : FrameRect(animationRect),
			animationCycles(new AnimationTexList(*animationRect.animationCycles)),
			animationIndex(animationRect.animationIndex),
			animationFrame(animationRect.animationFrame),
			elapsedTime(animationRect.elapsedTime),
			shouldAnimate(animationRect.shouldAnimate) {}
		// move constructor and assignment operator are not allowed
		AnimationRect(AnimationRect&&) = delete;
		AnimationRect& operator=(const AnimationRect&) = delete;
		// when we change the animation cycle the frame is at 0 and the elapsed time is also 0
		// also we need to set shouldAnimate to true
		inline void changeAnimationCycle(int index) {
			animationIndex = index % animationCycles->size();
			animationFrame = 0;
			elapsedTime = 0.0f;
			shouldAnimate = true;
		}
		void update(float deltaTime);

		~AnimationRect() {

			delete animationCycles;
		}

	};
}

