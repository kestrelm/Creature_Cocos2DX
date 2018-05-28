/******************************************************************************
 * Creature Runtimes License
 *
 * Copyright (c) 2015, Kestrel Moon Studios
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
#include <memory>
#include "CreatureModule.h"
#include "CreatureMetaData.h"
#include "cocos2d.h"

namespace CreatureRenderer {
    class Renderer : public cocos2d::Node, public cocos2d::BlendProtocol {
    public:
        static Renderer * create(CreatureModule::CreatureManager * manager_in,
                                 cocos2d::Texture2D * texture_in);
        
        virtual void update(float delta);
        
        virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformFlags);
        
        virtual void doDraw(const cocos2d::Mat4& transform, uint32_t transformFlags);
        
        virtual void setBlendFunc (const cocos2d::BlendFunc& blendFunc);
        
        virtual const cocos2d::BlendFunc& getBlendFunc () const;
        
        void setTexture(cocos2d::Texture2D * texture_in);
	    
        void SetDebugDraw(bool flag_in);
        
        std::pair<glm::vec2, glm::vec2> GetCharacterBounds();

		void loadMetaData(const std::string& filename);

		void setUseSkinSwap(bool flag_in)
		{
			use_skinswap = flag_in;
		}

		bool useSkinSwap() const {
			return use_skinswap;
		}

		void setUseLayerOrder(bool flag_in)
		{
			use_layerorder = flag_in;
		}

		bool useLayerOrder() const {
			return use_layerorder;
		}

		void setSkinSwap(const std::string& name_in)
		{
			skinswap_name = name_in;
		}
        
    protected:
        Renderer(CreatureModule::CreatureManager * manager_in,
                 cocos2d::Texture2D * texture_in);
        
        virtual ~Renderer();
        
#ifdef _CREATURE_DEBUG_DRAW
        void drawDebugBones(meshBone * draw_bone);
#endif
		void processSkinswap();

		void processLayerorder(int time_in);

        CreatureModule::CreatureManager * manager;
        cocos2d::CustomCommand _drawCommand;
        cocos2d::BlendFunc _blendFunc;
        cocos2d::Texture2D * texture;
        bool debug_draw;
        cocos2d::Rect character_bounds;
		std::vector<glm::uint32> meta_indices, real_meta_indices;
		std::unique_ptr<CreatureModule::CreatureMetaData> metadata;
		std::string skinswap_name;
		bool use_skinswap = false, use_layerorder = false;
    };
};
