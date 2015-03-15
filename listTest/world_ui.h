#pragma once

#include "cocos2d.h"

namespace simciv
{

USING_NS_CC;

enum ItemType;
class Item;

class WorldUI : public cocos2d::Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
	void init_menu();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // a selector callback
    void menuCloseCallback(Ref* sender);
    
	void onTouchMoved(Touch* touch, Event  *event);
	bool onTouchBegan(Touch* touch, Event  *event);
	void onTouchEnded(Touch* touch, Event  *event);
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	virtual void onEnter() override;
    // implement the "static node()" method manually
    CREATE_FUNC(WorldUI);

	Item* add_item(ItemType type, int x, int y);
protected:
	cocos2d::Node* _items;
	cocos2d::Node* _map;

	CustomCommand _customCommand;
    void onDraw(const Mat4 &transform, uint32_t flags);
	void draw_rect(int x, int y, double rate, double alpha);
	void draw_rect_green(int x, int y, double rate, double alpha);
	void draw_vec(Vec2 a, Vec2 v);
};

}