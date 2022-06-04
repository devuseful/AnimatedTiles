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


#include "CCTMXAnimation.h"
#include "CCFastTMXLayer.h"

constexpr auto Milliseconds = 1000;										// number of milliseconds
constexpr auto MinUpdateTime = 0.1f;									// minimum delay time for updating animated tiles

NS_CC_BEGIN


TMXAnimation::TMXAnimation()
	: _layer(nullptr)
	, _animationsInfo()
{

}


TMXAnimation *TMXAnimation::create()
{
	TMXAnimation *animation = new (std::nothrow) TMXAnimation();
	if (animation)
	{
		animation->autorelease();
		return animation;
	}
	CC_SAFE_DELETE(animation);
	return nullptr;
}


TMXAnimation::~TMXAnimation() 
{
	_layer = nullptr;
	Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
}


void TMXAnimation::addAnamationInfo(uint32_t gid)
{
	_animationsInfo.push_back(TMXAnimationInfo(gid));
}


void TMXAnimation::addFrameInfo(uint32_t tileId, uint32_t duration)
{
	TMXAnimationInfo &animationInfo = _animationsInfo.back();
	animationInfo.framesInfo.push_back(TMXFrameInfo(tileId, duration));
	animationInfo.frameIt = animationInfo.framesInfo.begin();
}


void TMXAnimation::initAnimation(experimental::TMXLayer *layer, const Size &size)
{
	if (_animationsInfo.empty() || !layer)
		return;

	int tilesAmount = size.width * size.height;
	for (auto it = _animationsInfo.begin(); it != _animationsInfo.end(); ++it)
	{
		for (int y = 0; y < size.height; ++y)
		{
			for (int x = 0; x < size.width; ++x)
			{
				int tileIndex = x + y * (int)size.width;
				if (layer->getTiles()[tileIndex] == (*it).gid)
				{
					(*it).pos = Vec2(x, y);
				}
			}
		}
	}

	_layer = layer;
	Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(TMXAnimation::update), this, MinUpdateTime, false);
}


void TMXAnimation::update(float dt)
{
	for (auto it = _animationsInfo.begin(); it != _animationsInfo.end(); ++it)
	{
		(*it).time += (uint32_t)(dt * Milliseconds);
		if ((*it).time >= (*(*it).frameIt).duration)
		{
			(*it).time = 0;
			++(*it).frameIt;
			if ((*it).frameIt == (*it).framesInfo.end())
				(*it).frameIt = (*it).framesInfo.begin();

			_layer->setTileGID((*(*it).frameIt).tileId, (*it).pos);
		}
	}
}


NS_CC_END
