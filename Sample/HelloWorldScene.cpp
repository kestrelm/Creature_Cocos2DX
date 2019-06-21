#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "CreatureModule.h"
#include "CreatureRenderer.h"
#include "CreaturePackModule.hpp"
#include "CreaturePackRenderer.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }

	// Load Regular Creature character
	{
		// Load Creature character
		auto filename = CCFileUtils::getInstance()->fullPathForFilename("swapGirl.json");
		CreatureModule::CreatureLoadDataPacket json_data;
		CreatureModule::LoadCreatureJSONData(filename, json_data);

		// Create Creature
		auto cur_creature =
			std::shared_ptr<CreatureModule::Creature>(new CreatureModule::Creature(json_data));

		// Set Animation settings
		auto creature_manager = new CreatureModule::CreatureManager(cur_creature);
		creature_manager->CreateAnimation(json_data, "default");
		creature_manager->CreateAnimation(json_data, "dress");
		creature_manager->CreateAnimation(json_data, "cape");

		creature_manager->SetActiveAnimationName("cape");

		// Create Creature Renderer
		auto creature_renderer = CreatureRenderer::Renderer::create(creature_manager, CCTextureCache::getInstance()->addImage("swapGirl.png"));
		creature_manager->SetIsPlaying(true);
		creature_manager->SetShouldLoop(true);
		creature_manager->SetMirrorY(true);
		creature_renderer->setColor(cocos2d::Color3B(255, 255, 255));
		creature_renderer->SetDebugDraw(false);

		// Meta Data
		auto meta_filename = CCFileUtils::getInstance()->fullPathForFilename("swapGirl.mdata");
		creature_renderer->loadMetaData(meta_filename);
		creature_renderer->setSkinSwap("cape");
		creature_renderer->setUseSkinSwap(true);

		visibleSize = Director::getInstance()->getVisibleSize();
		origin = Director::getInstance()->getVisibleOrigin();
		creature_renderer->setScale(15.0);
		creature_renderer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

		this->addChild(creature_renderer, 1);
	}

	// Load CreaturePack Character
	{
		// First, create the CreaturePackLoader
		auto packFilename = CCFileUtils::getInstance()->fullPathForFilename("fox.creature_pack");
		// Create the CreaturePackLoader that holds the data
		std::shared_ptr<CreaturePackLoader> packLoader = CreaturePackRenderer::createPackLoader(packFilename);
		// Renderer
		auto packRenderer =
			CreaturePackRenderer::Renderer::create(
				packLoader,
				CCTextureCache::getInstance()->addImage("fox.png"));
		packRenderer->setScale(5.0f);
		packRenderer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 4 + origin.y));

		this->addChild(packRenderer, 1);

		// Set Animation settings
		auto playerObj = packRenderer->getPlayerObj();
		playerObj->blendToAnimation("run", 0.2f);
		playerObj->isLooping = true;
		playerObj->isPlaying = true;
		packRenderer->setPlaybackSpeed(60.0f);
	}

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
