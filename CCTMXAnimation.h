/****************************************************************************
Copyright (c) 2022 Skrylev Aleksey (useful@bk.ru)
http://spdstudio.ru

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/


#ifndef __CCTMX_ANIMATION_H__
#define __CCTMX_ANIMATION_H__


#include "cocos2d.h"


NS_CC_BEGIN

class experimental::TMXLayer;


/** @brief TMXAnimation.
 *
 * The class is designed to work with animated tiles created in the TiledMapEditor level editor
 */

class TMXAnimation : public Ref
{
public:

	TMXAnimation();
	static TMXAnimation *create();
	virtual ~TMXAnimation();

	/* Add the tile id with animation
	*
	* @param gid A id of the animated tile.
	*/
	void addAnamationInfo(uint32_t gid);

	/* Add a new frame to the current tile with animation
	*
	* @param tileId A tile ID for the new framee.
	* @param duration A delay time between frame changes.
	*/
	void addFrameInfo(uint32_t tileId, uint32_t duration);

	/* Layer tile animation initialization function
	*
	* @param layer A layer in which the animated tile is located.
	* @param size A layer size.
	*/
	void initAnimation(experimental::TMXLayer *layer, const Size &size);
	
protected:

	// animated tile frames update function
	void update(float dt);
	
	
	/** @brief TMXFrameInfo contains the information about the frame like:
	- Tile ID in tiset
	- Delay time between frame changes

	This is information about frame settings.
	*/
	struct TMXFrameInfo
	{
		TMXFrameInfo(uint32_t tileId, uint32_t duration)
		{
			this->tileId = tileId;
			this->duration = duration;
		}
		
		~TMXFrameInfo()
		{
			//CCLOG("~TMXFrameInfo()");
		}
		
		// id of the frame (image) of the tile animation
		uint32_t tileId;
		// delay time between changing frames (images)
		uint32_t duration;
	};


	/** @brief TMXFrameInfo contains the information about the animation like:
	- Tile ID in map
	- Tile position with animation
	- List of animation frames
	- Frame iterator
	- Time elapsed since the frame change

	This is information about animation settings.
	*/
	struct TMXAnimationInfo
	{
		TMXAnimationInfo(uint32_t gid) :
			pos(Vec2::ZERO),
			time(0),
			frameIt(framesInfo.begin())
		{
			this->gid = gid;
		}		
		
		~TMXAnimationInfo()
		{
		}
		
		// Tile ID in map
		uint32_t gid;
		// Tile position with animation
		Vec2 pos;
		// List of animation frames
		std::vector<TMXFrameInfo> framesInfo;
		// Frame iterator
		std::vector<TMXFrameInfo>::iterator frameIt;
		// Time elapsed since the frame change
		uint32_t time;

	};

	// list information about all animations
	std::vector<TMXAnimationInfo> _animationsInfo;
	// pointer to the layer TMXLayer
	experimental::TMXLayer *_layer;

};

NS_CC_END

#endif // __CCTMX_ANIMATION_H__
