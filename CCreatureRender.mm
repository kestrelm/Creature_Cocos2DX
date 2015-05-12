//
//  CCreatureRender.m
//  Untitled
//
//  Created by Jiayi Chong on 3/29/15.
//  Copyright (c) 2015 Apportable. All rights reserved.
//

#import "CCreatureRender.h"

@implementation CCreatureRender

@synthesize manager;

-(id)init
{
    if((self = [super init])){
        // Set up a texture for rendering.
        // If you want to mix several textures, you need to make a shader and use CCNode.shaderUniforms.
        self.texture = [CCTexture textureWithFile:@"Tiles/05.png"];
        
        // Set a builtin shader that draws the node with a texture.
        // The default shader only draws the color of a node, ignoring it's texture.
        self.shader = [CCShader positionTextureColorShader];
    }
    
    return self;
}

-(id)initWithData:(CreatureModule::CreatureManager *) managerIn andTexture:(CCTexture *) textureIn
{
    if((self = [super init])){
        self.manager = managerIn;
        self.texture = textureIn;
        
        // Set a builtin shader that draws the node with a texture.
        // The default shader only draws the color of a node, ignoring it's texture.
        self.shader = [CCShader positionTextureColorShader];
        
        // Set a builtin shader that draws the node with a texture.
        // The default shader only draws the color of a node, ignoring it's texture.
        self.shader = [CCShader positionTextureColorShader];
        self.color = [CCColor whiteColor];
    }
    
    return self;
}

-(void) update:(CCTime)delta
{
    manager->Update(delta);
}

-(void)draw:(CCRenderer *)renderer transform:(const GLKMatrix4 *)transform
{
    int numVertices = manager->GetCreature()->GetTotalNumPoints();
    int numTriangles = manager->GetCreature()->GetTotalNumIndices() / 3;
    auto readPts = manager->GetCreature()->GetRenderPts();
    auto readIndices = manager->GetCreature()->GetGlobalIndices();
    auto readUVs = manager->GetCreature()->GetGlobalUvs();
    
    CCRenderBuffer buffer = [renderer enqueueTriangles:numTriangles
                                           andVertexes:numVertices
                                             withState:self.renderState globalSortOrder:0];
    
    float curR = self.color.red;
    float curG = self.color.green;
    float curB = self.color.blue;
    float curA = self.color.alpha;
    
    // Set points, colors and uvs
    for(int i = 0; i < numVertices; i++)
    {
        CCVertex curVertex;
        curVertex.position = GLKVector4Make(readPts[0],readPts[1], 0, 1.0);
        curVertex.texCoord1 = GLKVector2Make(readUVs[0], 1.0f - readUVs[1]);
        curVertex.color = GLKVector4Make(curR, curG, curB, curA);

        CCRenderBufferSetVertex(buffer, i, CCVertexApplyTransform(curVertex, transform));
        
        readPts += 3;
        readUVs += 2;
    }
    
    // Set topology
    for(int i = 0; i < numTriangles; i++)
    {
        CCRenderBufferSetTriangle(buffer, i,
                                  readIndices[0],
                                  readIndices[1],
                                  readIndices[2]);
        
        readIndices += 3;
    }
}

@end
