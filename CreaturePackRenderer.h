/******************************************************************************
 * Creature Runtimes License
 *
 * Copyright (c) 2017, Kestrel Moon Studios
 * All rights reserved.
 *
 * Preamble: This Agreement governs the relationship between Licensee and Kestrel Moon Studios(Hereinafter: Licensor).
 * This Agreement sets the terms, rights, restrictions and obligations on using [Creature Runtimes] (hereinafter: The Software) created and owned by Licensor,
 * as detailed herein:
 * License Grant: Licensor hereby grants Licensee a Sublicensable, Non-assignable & non-transferable, Commercial, Royalty free,
 * Including the rights to create but not distribute derivative works, Non-exclusive license, all with accordance with the terms set forth and
 * other legal restrictions set forth in 3rd party software used while running Software.
 * Limited: Licensee may use Software for the purpose of:
 * Running Software on Licensee’s Website[s] and Server[s];
 * Allowing 3rd Parties to run Software on Licensee’s Website[s] and Server[s];
 * Publishing Software’s output to Licensee and 3rd Parties;
 * Distribute verbatim copies of Software’s output (including compiled binaries);
 * Modify Software to suit Licensee’s needs and specifications.
 * Binary Restricted: Licensee may sublicense Software as a part of a larger work containing more than Software,
 * distributed solely in Object or Binary form under a personal, non-sublicensable, limited license. Such redistribution shall be limited to unlimited codebases.
 * Non Assignable & Non-Transferable: Licensee may not assign or transfer his rights and duties under this license.
 * Commercial, Royalty Free: Licensee may use Software for any purpose, including paid-services, without any royalties
 * Including the Right to Create Derivative Works: Licensee may create derivative works based on Software,
 * including amending Software’s source code, modifying it, integrating it into a larger work or removing portions of Software,
 * as long as no distribution of the derivative works is made
 *
 * THE RUNTIMES IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE RUNTIMES OR THE USE OR OTHER DEALINGS IN THE
 * RUNTIMES.
 *****************************************************************************/

#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include "cocos2d.h"
#include "CreaturePackModule.hpp"

namespace CreaturePackRenderer {
	std::shared_ptr<CreaturePackLoader> createPackLoader(const std::string& fullpath);

    class Renderer : public cocos2d::Node, public cocos2d::BlendProtocol {
    public:
        static Renderer * create(std::shared_ptr<CreaturePackLoader> pack_loader,
                                 cocos2d::Texture2D * texture_in);
        
        virtual void update(float delta);
        
        virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformFlags);
        
        virtual void doDraw(const cocos2d::Mat4& transform, uint32_t transformFlags);
        
        virtual void setBlendFunc (const cocos2d::BlendFunc& blendFunc);
        
        virtual const cocos2d::BlendFunc& getBlendFunc () const;
        
        void SetDebugDraw(bool flag_in);
        
        std::pair<cocos2d::Point, cocos2d::Point> GetCharacterBounds();

		CreaturePackPlayer * getPlayerObj()
		{
			return playerObj.get();
		}

		float getPlaybackSpeed() const { return playback_speed; }

		void setPlaybackSpeed(float value_in)
		{
			playback_speed = value_in;
		}
        
    protected:
        Renderer(std::shared_ptr<CreaturePackLoader> pack_loader,
                 cocos2d::Texture2D * texture_in);
        
        virtual ~Renderer();
        
		std::shared_ptr<CreaturePackLoader> packLoader;
        std::shared_ptr<CreaturePackPlayer> playerObj;
        cocos2d::CustomCommand _drawCommand;
        cocos2d::BlendFunc _blendFunc;
        cocos2d::Texture2D * texture;
        bool debug_draw;
        cocos2d::Rect character_bounds;
		float playback_speed;
    };
};
