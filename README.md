# Creature-Runtimes

For more information on how to use the runtimes, please head over to this [site](http://www.kestrelmoon.com/creaturedocs/Game_Engine_Runtimes_And_Integration/Runtimes_Introduction.html)

## Overview
This document describes how to use the Cocos2d-x runtimes. The language of the runtimes is in C++. There are 2 types of formats to read in, the JSON/FlatBuffers fully featured format or the much more performant + compact CreaturePack format.

## JSON/FlatBuffers

### Libaries Needed to compile
Along with the core runtime files, the following libraries are also included:

- **gason** (Include the Header and also compile the single gason.cpp source)

- **glm** (Header only so make sure your include paths are setup for it)

Make sure to include these 2 libraries in source form in your project.


### Header Includes
The following headers need to be included:

	#include "MeshBone.h"
	#include "CreatureRenderer.h"

### Loading and Initialization
Let us assume we have an exported dragon animation file called **dragonTest.json**. We also have its corresponding texture atlas called **character-dragon.png**.
We start off by first loading the file assets:

    auto filename = CCFileUtils::getInstance()->fullPathForFilename("dragonTest.json");
    auto texture_filename = CCFileUtils::getInstance()->fullPathForFilename("character-dragon.png");
    
    CreatureModule::CreatureLoadDataPacket json_data;
    CreatureModule::LoadCreatureJSONData(filename, json_data);

The above will load the **JSON** data from disk and into memory. Next, let us create the actual objects that can make use of these loaded assets:

    auto cur_creature = std::shared_ptr<CreatureModule::Creature>(new CreatureModule::Creature(json_data));
    
    creature_manager = new CreatureModule::CreatureManager(cur_creature);
    creature_manager->CreateAnimation(json_data, "default");
    creature_manager->CreateAnimation(json_data, "second");    
In the example above, the **JSON** file has 2 animation clips: **default** and **second**. Hence, we will need to create 2 animations from the **creature_manager** object to make them available for playback.    

Now that we are done loading, we can set the active animation for playback:

	creature_manager->SetActiveAnimationName("default");

This sets **default** as the currently active animation.
We will now go ahead and create the object to render the character animation:

    auto creature_renderer = CreatureRenderer::Renderer::create(
    	creature_manager,CCTextureCache::getInstance()->addImage("character-dragon.png"));

This creates a creature renderer with our texture atlas image file. Once that is done, we can go ahead and set some playback and rendering properties:

    creature_manager->SetIsPlaying(true);
    creature_manager->SetShouldLoop(true);
    creature_manager->SetMirrorY(true);
    creature_renderer->setColor(cocos2d::Color3B(255, 255, 255));

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    creature_renderer->setScale(30.0);
    creature_renderer->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    

Finally, we add the object to the scene:

    this->addChild(creature_renderer, 1);

That concludes the basic instructions on how to integrate your character into Cocox2dX!

### Complete Code Sample
Here is the complete code layout. Most the the code is generated from the default Cocos2dX project starter template. Pay attention to the method **loadModel()** which does most of the work:

	#include "HelloWorldScene.h"
	#include "MeshBone.h"
	#include "CreatureRenderer.h"

	USING_NS_CC;

	Scene* HelloWorld::createScene()
	{
   		 // 'scene' is an autorelease object
	    auto scene = Scene::create();
    
	    // 'layer' is an autorelease object
	    auto layer = HelloWorld::create();

   		 // add layer as a child to scene
	    scene->addChild(layer);

	    // return the scene
   		 return scene;
	}

	void HelloWorld::loadModel()
	{
		// The actual code that loads the character animation data
	    auto filename = CCFileUtils::getInstance()->fullPathForFilename("dragonTest.json");
	    auto texture_filename = CCFileUtils::getInstance()->fullPathForFilename("character-dragon.png");
    
	    CreatureModule::CreatureLoadDataPacket json_data;
	    CreatureModule::LoadCreatureJSONData(filename, json_data);
    
	    auto cur_creature = std::shared_ptr<CreatureModule::Creature>(new CreatureModule::Creature(json_data));
    
	    creature_manager = new CreatureModule::CreatureManager(cur_creature);
	    creature_manager->CreateAnimation(json_data, "default");
    	creature_manager->CreateAnimation(json_data, "second");
    
	    creature_manager->SetActiveAnimationName("default");
    
	    auto creature_renderer = CreatureRenderer::Renderer::create(creature_manager,                                                             			 CCTextureCache::getInstance()->addImage("character-dragon.png"));
	    creature_manager->SetIsPlaying(true);
	    creature_manager->SetShouldLoop(true);
	    creature_manager->SetMirrorY(true);
	    creature_renderer->setColor(cocos2d::Color3B(255, 255, 255));
	    creature_renderer->SetDebugDraw(false);
    
    
	    Size visibleSize = Director::getInstance()->getVisibleSize();
	    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	    creature_renderer->setScale(30.0);
	    creature_renderer->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	    this->addChild(creature_renderer, 1);
		scheduleUpdate();
	}

		// on "init" you need to initialize your instance
		bool HelloWorld::init()
		{
		    //////////////////////////////
		    // 1. super init first
		    if ( !Layer::init() )
	   		 {
	   	   	  return false;
		    }
    
		    Size visibleSize = Director::getInstance()->getVisibleSize();
	   		 Vec2 origin = Director::getInstance()->getVisibleOrigin();

		    /////////////////////////////
			 // 2. add a menu item with "X" image, which is clicked to quit the program
	   		 //    you may modify it.

		    // add a "close" icon to exit the progress. it's an autorelease object
		    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
			closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem-	>getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

		    // create menu, it's an autorelease object
		    auto menu = Menu::create(closeItem, NULL);
		    menu->setPosition(Vec2::ZERO);
		    this->addChild(menu, 1);

		    /////////////////////////////
		    // 3. add your codes below...

		    // add a label shows "Hello World"
		    // create and initialize a label
    
		    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
		    // position the label on the center of the screen
		    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

		    // add the label as a child to this layer
		    this->addChild(label, 1);

		    // add "HelloWorld" splash screen"
		    auto sprite = Sprite::create("HelloWorld.png");

		    // position the sprite on the center of the screen
		    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

		    // add the sprite as a child to this layer
		    this->addChild(sprite, 0);
    
    		// Load your character
		    loadModel();
    
	   		return true;
		}

	void HelloWorld::menuCloseCallback(Ref* pSender)
	{
	    Director::getInstance()->end();
	}
	
### SkinSwapping

You can enable and use SkinSwapping in the runtimes by first loading the metadata into the **CreatureRenderer**, setting the SkinSwap you want and then activating it:

    auto meta_filename = CCFileUtils::getInstance()->fullPathForFilename("swapGirl.mdata");
    creature_renderer->loadMetaData(meta_filename);
    creature_renderer->setSkinSwap("cape");
    creature_renderer->setUseSkinSwap(true);
    
SkinSwapping requires the **metadata JSON**(.mdata) to be loaded and included in your project.    

### Custom Time/Frame Range

You can set custom time/frame ranges for the currently active animation. Say you wanted to limit the playback to the frame range of **10 to 20**, you would do the following:

    creature_manager->SetUseCustomTimeRange(true);
    creature_manager->SetCustomTimeRange(10, 20);

### Animation Blending
You can blend between 2 animation clips by doing the following:

    creature_manager->SetBlending(true);
    creature_manager->SetBlendingAnimations("default", "second");
    creature_manager->SetBlendingFactor(0.5); // 0 to 1 blends between the 2 clips
    
### Smooth Transitions with Auto Blending
If you want to smoothly transition from one animation state to the next, you can use the Auto Blending feature in the runtimes.

To enable Auto Blending, first do:

	creature_manager->SetAutoBlending(true);

after you have added all animations into your creature manager object.

To use Auto Blending, a single call to:

	creature_manager->AutoBlendTo(NEW_ANIMATION_NAME, 0.1f);

will result in the character smoothly transitioning to the target animation called NEW_ANIMATION_NAME. The second parameter determines the time delta of the transition, with a range of 0.0 to 1.0.

### Overriding/Modifying Bone Positions
Sometimes you need to modify the bone positions of the character directly. For example, you might want the positions of the bones to follow a number of rigid bodies connected with springs/joints for ragdoll physics. In the cases where you need to set bone positions for your own custom requirements, you should do the following. First, write your custom bone override method. Here is an example that displaces the bones in y by some amount:

	// This is an example of how you can use a callback function to modify the position of the bones
	// on your character. In this example, we will displace all the bones by a fixed amount in y.
	void
	HelloWorld::bonesModifyCallback(std::unordered_map<std::string, meshBone *>& bones_map)
	{
	    for(auto& bone_data : bones_map)
	    {
	        auto cur_bone = bone_data.second;
	        auto cur_start_pos = cur_bone->getWorldStartPt();
	        auto cur_end_pos = cur_bone->getWorldEndPt();
        
	        cur_start_pos.y -= 5;
	        cur_end_pos.y -= 5;
        
	        cur_bone->setWorldStartPt(cur_start_pos);
	        cur_bone->setWorldEndPt(cur_end_pos);
	    }
	}

You will also need to tell the **CreatureManager** to use your custom bone modify callback like this:

    // Example of how to register a callback function to modify the bones
    std::function<void (std::unordered_map<std::string, meshBone *>&) > cur_callback =
        std::bind(&HelloWorld::bonesModifyCallback, this, std::placeholders::_1);
    creature_manager_2->SetBonesOverrideCallback(cur_callback);

### Character Instancing and Memory
If you need to instance multiple copies of a character(for example 2 dragons), you should create your animations like this instead:

    // Create and load the animations
    auto new_animation_1 = std::shared_ptr<CreatureModule::CreatureAnimation>(
                                                                            new CreatureModule::CreatureAnimation(json_data,
                                                                                                  "default"));

    auto new_animation_2 = std::shared_ptr<CreatureModule::CreatureAnimation>(
                                                                              new CreatureModule::CreatureAnimation(json_data,
                                                                                                                    "pose2"));


You should then proceed to you create a new **Creature** object, a new **CreatureManager** object and a new **CreatureRenderer** object. You will add the created animations into your newly created **CreatureManager** object like this:

    // Second creature instancing example. This shows you how to load a second creature.
    // Because both the first and second creature share animation data, you end up
    // saving memory.
    auto creature_2 = std::shared_ptr<CreatureModule::Creature>(new CreatureModule::Creature(json_data));

    CreatureModule::CreatureManager * creature_manager_2 = new CreatureModule::CreatureManager(creature_2);
    creature_manager_2->AddAnimation(new_animation_1);
    creature_manager_2->AddAnimation(new_animation_2);
    creature_manager_2->SetActiveAnimationName("pose2");
    creature_manager_2->SetIsPlaying(true);
    creature_manager_2->SetUseCustomTimeRange(true);
    creature_manager_2->SetCustomTimeRange(10, 60);
    
    auto creature_renderer_2 = CreatureRenderer::Renderer::create(creature_manager_2,
                                                                 CCTextureCache::getInstance()->addImage("character-dragon.png"));
    creature_renderer_2->setColor(cocos2d::Color3B(255, 255, 255));
    creature_renderer_2->setScale(30.0);
    creature_renderer_2->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y - 200));
    
    this->addChild(creature_renderer_2, 1);


The difference between this example and the previous is that the animations are first created, then added into their respective **CreatureManagers**. This means the memory allocated for the animations(the most expensive) are stored in a standard location. Multiple **CreatureManager** objects will add animations from a common pool of **CreatureAnimation** objects.

### Speeding up Playback Performance with Point Caches
If you want to increase the speed of the playback animations and not pay the penalty for the posing engine, you can enable point caches on specific animations.

Point caching essentially stores away the vertices of the animation, allowing the playback to bypass the posing engine. Character rigs with very complex skeletons and deformations benefit greatly from point caching.

To enable point caching on a specific animation, do this:
	
	my_creature_manager->MakePointCache("myAnimationName");

That is all that is required to create the cache. Remember that the cache is specific to the animation, not character. This means that if you have instanced multiple characters, you only pay the cost of a single cache for that animation. This results in both **memory savings and playback performance speedups.**

## Creature Pack

There is a alternative, lightweight plugin called the **CreaturePack** runtime designed for high performance playback of lots of characters. If you require high performance playback together with small binary file sizes but do not require in game bone manipulation, you might want to consider using the **Creature Pack** file format.

### Required Files

Please add these files from the runtimes folder into your project:

- CreaturePackRenderer.cpp
- CreaturePackRenderer.h
- CreaturePackModule.hpp
- mp.h
- mp.cpp

### Usage

Below is a sample of how to create a **CreaturePackLoader** and then instantiate a new **CreaturePackRenderer** object.

First, put in the the required include headers:

	#include "CreaturePackModule.hpp"
	#include "CreaturePackRenderer.h"

Next, fill in the code below to use the **CreaturePack** runtime:

	// CreaturePack Demo, shows you how to load a Creature Pack character in cocos2d-x

	// First, create the CreaturePackLoader
	auto packFilename = CCFileUtils::getInstance()->fullPathForFilename("testPack.creature_pack");
	packLoader = CreaturePackRenderer::createPackLoader(packFilename);

The important thing to note here is the **CreaturePackLoader** object MUST BE around for the lifetime of the **CreaturePackRenderer**. Make it a member of your game class ( or you can also make it static which ever way you choose ).

	// Now create the renderer for CreaturePack
	auto packRenderer =
		CreaturePackRenderer::Renderer::create(
			packLoader.get(),
			CCTextureCache::getInstance()->addImage("testPack.png"));
	packRenderer->setScale(120.0f);
	packRenderer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	this->addChild(packRenderer, 1);

	// Set Animation settings
	auto playerObj = packRenderer->getPlayerObj();
	playerObj->blendToAnimation("default", 0.2f);
	playerObj->isLooping = true;
	playerObj->isPlaying = true;
	packRenderer->setPlaybackSpeed(30.0f);
	
Remember, you should **share CreaturePackLoader** objects between multiple **Renderer** objects if they are all sourced from the same asset. This saves load time and memory.
	
### Important Functions to know about

Most of the core functionality of **CreaturePack** lives on the **CreaturePackPlayer** object. You can get an instance of this from the **CreaturePackRenderer::Renderer** object via the **getPlayerObj()** method.

Once you have a reference to it, you can manipulate playback on it by calling the following methods or adjusting certain attributes:

- **blendToAnimation(nameIn, blendDelta)** Smoothly blends between 2 animation clips

- **blendToAnimation(nameIn)** Sets the active animation without blending

- **getRunTime()** Returns the current time/frame of the animation

- **isLoopin** This is a property you can directly set. Determines whether to loop the animation or not

- **isPlaying** A boolean property you can directly set. Determines whether the animation is playing or not.


## License
The **Creature Runtimes** operate under 2 License types depending on whether you own a Licensed copy of [Creature](https://creature.kestrelmoon.com) or not.
- **People who own a licensed copy of Creature:** You use the standard **Creature License** included with the runtime code. **TLDR:** You are free to publish/modify/sell your product with the Creature runtimes without needing to state you are using the runtimes/put the copyright notice in your code/app. If you already have been using the Creature runtimes as a licensed owner of Creature, nothing changes :)

- **Everyone else:** The runtimes are released under the very permissive [**Apache License**](https://choosealicense.com/licenses/apache-2.0/) :)

***Both Licenses allow for private use and do not require any disclosure of your source code.***
