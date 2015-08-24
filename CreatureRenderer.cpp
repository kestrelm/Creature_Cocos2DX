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

#include "CreatureRenderer.h"

namespace CreatureRenderer {
    Renderer *
    Renderer::create(CreatureModule::CreatureManager * manager_in,
                     cocos2d::Texture2D * texture_in)
    {
        Renderer * new_node = new Renderer(manager_in, texture_in);
        new_node->autorelease();
        
        return new_node;
    }
    
    Renderer::Renderer(CreatureModule::CreatureManager * manager_in,
                       cocos2d::Texture2D * texture_in)
    : manager(manager_in), texture(texture_in), debug_draw(false)
    {
        setGLProgram(cocos2d::ShaderCache::getInstance()->getGLProgram(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
        scheduleUpdate();
    }
    
    Renderer::~Renderer()
    {
        
    }
    
    void
    Renderer::setBlendFunc (const cocos2d::BlendFunc& blendFunc)
    {
        _blendFunc = blendFunc;
    }
    
    const cocos2d::BlendFunc&
    Renderer::getBlendFunc () const
    {
        return _blendFunc;
    }

    void
    Renderer::update(float delta)
    {
        manager->Update(delta);
        auto compute_bounds = GetCharacterBounds();
        character_bounds = cocos2d::Rect(compute_bounds.first.x, compute_bounds.first.y, 
                                        compute_bounds.second.x - compute_bounds.first.x,
                                         compute_bounds.second.y - compute_bounds.first.y);
    }
    
    void
    Renderer::draw(cocos2d::Renderer* renderer,
                   const cocos2d::Mat4& transform,
                   uint32_t transformFlags)
    {
        _drawCommand.init(_globalZOrder);
        _drawCommand.func = CC_CALLBACK_0(Renderer::doDraw, this, transform, transformFlags);
        renderer->addCommand(&_drawCommand);
    }
    
    std::pair<glm::vec2, glm::vec2>
    Renderer::GetCharacterBounds()
    {
        auto cur_pts = manager->GetCreature()->GetRenderPts();
        
        glm::vec2 min_pt, max_pt;
        
        if( manager->GetCreature()->GetTotalNumPoints() > 0)
        {
            min_pt.x = cur_pts[0];
            min_pt.y = cur_pts[1];
            
            max_pt = min_pt;
        }
        
        for(int i = 0; i <  manager->GetCreature()->GetTotalNumPoints() * 3; i+= 3)
        {
            glm::float32 cur_x = cur_pts[i];
            glm::float32 cur_y = cur_pts[i + 1];
            
            if(cur_x < min_pt.x)
            {
                min_pt.x = cur_x;
            }
            if(cur_y < min_pt.y)
            {
                min_pt.y = cur_y;
            }
            
            if(cur_x > max_pt.x)
            {
                max_pt.x = cur_x;
            }
            if(cur_y > max_pt.y)
            {
                max_pt.y = cur_y;
            }

        }
        
        return std::make_pair(min_pt, max_pt);
    }
    
    void
    Renderer::SetDebugDraw(bool flag_in)
    {
        debug_draw = flag_in;
    }

    void
    Renderer::doDraw(const cocos2d::Mat4& transform, uint32_t transformFlags)
    {
        cocos2d::Director* director = cocos2d::Director::getInstance();
        cocos2d::Size frameSize = director->getOpenGLView()->getFrameSize();
        cocos2d::Rect curScreenRect(0, 0, frameSize.width, frameSize.height);
        
        cocos2d::Vec3 character_min_pt(character_bounds.getMinX(), character_bounds.getMinY(), 0);
        cocos2d::Vec3 character_max_pt(character_bounds.getMaxX(), character_bounds.getMaxY(), 0);
        //cocos2d::Vec3 character_min_world_pt, character_max_world_pt;
        transform.transformPoint(&character_min_pt);
        transform.transformPoint(&character_max_pt);
        
        cocos2d::Rect character_world_rect(character_min_pt.x, character_min_pt.y,
                                           character_max_pt.x - character_min_pt.x,
                                           character_max_pt.y - character_min_pt.y);
        
        if(curScreenRect.intersectsRect(character_world_rect) == false)
        {
            // not on screen, skip drawing
            return;
        }
        
        getGLProgramState()->apply(transform);
        cocos2d::GL::bindTexture2D(texture->getName());
        
        cocos2d::Color3B nodeColor = getColor();
        
        manager->GetCreature()->FillRenderColours(nodeColor.r,
                                                  nodeColor.g,
                                                  nodeColor.b,
                                                  getDisplayedOpacity());
        
        glEnableVertexAttribArray(cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
        glEnableVertexAttribArray(cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
        glEnableVertexAttribArray(cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        
        glVertexAttribPointer(cocos2d::GLProgram::VERTEX_ATTRIB_POSITION,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              manager->GetCreature()->GetRenderPts());
        
        glVertexAttribPointer(cocos2d::GLProgram::VERTEX_ATTRIB_COLOR,
                              4,
                              GL_UNSIGNED_BYTE,
                              GL_TRUE,
                              0,
                              manager->GetCreature()->GetRenderColours());
        
        glVertexAttribPointer(cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              manager->GetCreature()->GetGlobalUvs());
        
        glDrawElements(GL_TRIANGLES,
                       manager->GetCreature()->GetTotalNumIndices(),
                       GL_UNSIGNED_INT,
                       manager->GetCreature()->GetGlobalIndices());
        
        if(debug_draw)
        {
            director->pushMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
            director->loadMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
            
            cocos2d::DrawPrimitives::setPointSize(5.0);
            glLineWidth(5.0f);
            cocos2d::DrawPrimitives::setDrawColor4F(1, 1, 1, 1);
            
            drawDebugBones(manager->GetCreature()->GetRenderComposition()->getRootBone());
            
            director->popMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
        }
        
    }

    void
    Renderer::drawDebugBones(meshBone * draw_bone)
    {

        
        glm::vec4 pt1 = draw_bone->getWorldStartPt();
        glm::vec4 pt2 = draw_bone->getWorldEndPt();

        //glm::vec4 pt1 = draw_bone->getWorldRestStartPt();
        //glm::vec4 pt2 = draw_bone->getWorldRestEndPt();

        cocos2d::DrawPrimitives::drawLine(cocos2d::Vec2(pt1.x, pt1.y),
                                          cocos2d::Vec2(pt2.x, pt2.y));
        
        const std::vector<meshBone *>& bone_children = draw_bone->getChildren();
        for(auto& cur_child : bone_children)
        {
            drawDebugBones(cur_child);
        }
    }


}
