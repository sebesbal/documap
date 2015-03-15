#include "world_ui.h"
#include "AppMacros.h"
#include "ui\UICheckBox.h"
#include "ui\UILayout.h"
#include "ui\UIHBox.h"
#include "ui\UIVBox.h"
#include "ui\UIText.h"
#include "ui\UIButton.h"
//#include "CC
#include "base\ccTypes.h"
#include <algorithm>

namespace simciv
{

USING_NS_CC;
using namespace std;

std::string factory_strings[4] = {
	"factory_red.png", "factory_blue.png", "factory_green.png", "factory_yellow.png"
};

std::string mine_strings[4] = {
	"mine_red.png", "mine_blue.png", "mine_green.png", "mine_yellow.png"
};

Scene* WorldUI::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    WorldUI *layer = WorldUI::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

using namespace ui;

Layout* labelled_cb(std::string text, bool checked, CheckBox::ccCheckBoxCallback cb)
{
	auto l = HBox::create();
	auto p = LinearLayoutParameter::create();
	p->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);

	CheckBox* chb = CheckBox::create("cocosui/check_box_normal.png",
										"cocosui/check_box_normal_press.png",
										"cocosui/check_box_active.png",
										"cocosui/check_box_normal_disable.png",
										"cocosui/check_box_active_disable.png");
	chb->setSelectedState(checked);
	chb->addEventListener(cb);
	chb->setLayoutParameter(p);
	l->addChild(chb);
	
    auto label = Text::create();
	label->setString(text);
	label->setFontSize(18);
	label->setLayoutParameter(p);
	l->addChild(label);
	l->requestDoLayout();
	auto height = std::max(chb->getSize().height, label->getSize().height);
	l->setSize(Size(100, height));
	label->setTouchEnabled(true);
	label->addTouchEventListener([chb, cb](Ref*,Widget::TouchEventType type)
	{ 
		if (type == Widget::TouchEventType::ENDED)
		{
			chb->setSelectedState(!chb->getSelectedState());
			cb(NULL, chb->getSelectedState() ? CheckBox::EventType::SELECTED : CheckBox::EventType::UNSELECTED); 
		}
	});
	
	return l;
}




Layout* combobox(const std::string* labels)
{
	return NULL;
}

VBox* left_menu;


#define RBON "cocosui/Radio_button_on.png"
#define RBOFF "cocosui/Radio_button_off.png"

class RadioBox: public HBox
{
public:
	RadioBox (int* data, std::vector<std::string> labels, int hh, int marginy): data(data), hh(hh), marginy(marginy)
	{
		int k = 0;
		for (auto l: labels)
		{
			auto cb = [this, k](Ref* pSender, CheckBox::EventType type) { setSelected(k); };
			Widget* rb;
			if (l.substr(0, 1) == "_")
			{
				rb = image_radio(l.substr(1, l.length() - 1), cb);
			}
			else
			{
				rb = labelled_radio(l, cb);
			}
			++k;
			addChild(rb);
			items.push_back(rb);
		}
		setSelected(0);
	}
	void setSelected(int i)
	{
		int l = 0;
		for (Widget* item: items)
		{
			((CheckBox*)item->getChildren().at(0))->setSelectedState(i == l++);
		}
		*(this->data) = i;
	}

	static RadioBox* create(int* data, std::vector<std::string> labels, int hh, int marginy)
	{
	    RadioBox* widget = new RadioBox(data, labels, hh, marginy);
		if (widget && widget->init())
		{
			widget->autorelease();
			widget->setSize(Size(100, hh + marginy));
			return widget;
		}
		else
		{
			CC_SAFE_DELETE(widget);
			return nullptr;
		}
	}
	Layout* labelled_radio(std::string text, CheckBox::ccCheckBoxCallback cb)
	{
		auto l = HBox::create();
		auto p = LinearLayoutParameter::create();
		p->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);

		CheckBox* chb = CheckBox::create(RBOFF, RBOFF, RBON, RBOFF, RBON);
		chb->setSelectedState(false);
		chb->addEventListener(cb);
		chb->setLayoutParameter(p);
		l->addChild(chb);
	
		auto label = Text::create();
		label->setString(text);
		label->setFontSize(18);
		label->setLayoutParameter(p);
		label->setTouchEnabled(true);
		label->addTouchEventListener([chb, cb](Ref*,Widget::TouchEventType type)
		{ 
			if (type == Widget::TouchEventType::ENDED)
			{
				chb->setSelectedState(true);
				cb(NULL, CheckBox::EventType::SELECTED); 
			}
		});

		l->addChild(label);
		l->requestDoLayout();
		auto height = std::max(chb->getSize().height, label->getSize().height);
		l->setSize(Size(100, height));

		LinearLayoutParameter* pp = LinearLayoutParameter::create();
		pp->setGravity(LinearLayoutParameter::LinearGravity::TOP);
		pp->setMargin(Margin(2, marginy, 2, 2));

		l->setLayoutParameter(pp);

		return l;
	}
	Layout* image_radio(string img, CheckBox::ccCheckBoxCallback cb)
	{
		auto l = HBox::create();
		auto p = LinearLayoutParameter::create();
		p->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);

		CheckBox* chb = CheckBox::create(RBOFF, RBOFF, RBON, RBOFF, RBON);
		chb->setSelectedState(false);
		chb->addEventListener(cb);
		chb->setLayoutParameter(p);
		l->addChild(chb);
	
		auto image = Widget::create();
		auto s = Sprite::create(img);
		s->setScale(hh / s->getContentSize().height);
		s->setPosition(hh/2, hh/2);
		image->addChild(s);
		image->setLayoutParameter(p);
		image->setSize(Size(hh, hh));
		image->setTouchEnabled(true);
		image->addTouchEventListener([chb, cb](Ref*,Widget::TouchEventType type)
		{ 
			if (type == Widget::TouchEventType::ENDED)
			{
				chb->setSelectedState(true);
				cb(NULL, CheckBox::EventType::SELECTED); 
			}
		});
		l->addChild(image);
		//l->requestDoLayout();
		// auto height = std::max(chb->getSize().height, label->getSize().height);
		l->setSize(Size(100, hh));

		LinearLayoutParameter* pp = LinearLayoutParameter::create();
		pp->setGravity(LinearLayoutParameter::LinearGravity::TOP);
		pp->setMargin(Margin(2, marginy, 2, 2));

		l->setLayoutParameter(pp);

		return l;
	}
	int hh;
	int marginy;
	vector<Widget*> items;
	int* data;
};

#define defvec(vec, ...) \
	static const string arr ## vec[] = { __VA_ARGS__ }; \
	vector<string> vec (arr ## vec, arr ## vec + sizeof(arr ## vec) / sizeof(arr ## vec[0]) );

void WorldUI::init_menu()
{

}

struct Label: Text
{
public:
	static Label* create()
	{
		Label* widget = new Label();
		if (widget && widget->init())
		{
			widget->autorelease();
			widget->setSize(Size(100, 20));
			return widget;
		}
		else
		{
			CC_SAFE_DELETE(widget);
			return nullptr;
		}
	}

	// int strip_pos; // position in the stripe
	std::vector<Label*> items;
	Label* parent;
	int get_depth()
	{
		int depth = 0;
		for (auto label: items)
		{
			depth = max(depth, label->get_depth());
		}
		return depth + 1;
	}
	void fill_dummies(int dummy_count)
	{
		if (dummy_count > 0)
		{
			if (items.size() == 0)
			{
				items.push_back(Label::create());
			}
			for (Label* label: items)
			{
				label->fill_dummies(dummy_count - 1);
			}
		}
	}
	float position()
	{
		return -getPositionY();
	}
	float set_position(float pos)
	{
		setPositionY(-pos);
	}
};

struct PosData
{
	float position;
	float left_stripe_position;
	float right_stripe_position;
};

struct Strip: Node
{
	Strip()
	{
		height = 0;
		label_height = 20;
		space = 10;
	}

	std::vector<PosData> positions;
	// std::vector<Stripe*> labels;
	float label_height;
	float space;
	float height;
	void add_label(Label* label)
	{
		label->setPosition(Vec2(0, -height));
		height += label_height;
		this->addChild(label);
	}
	void add_space()
	{
		height += space;
	}
	//void add_dummy()
	//{
	//	auto label = new Label();
	//	add_label(label);
	//}

	PosData set_position(float position)
	{
		//setPositionY(position);
		for (PosData& pos: positions)
		{
			if (position <= pos.position)
			{
				setPositionY(pos.position);
				return pos;
			}
		}
	}
};

struct TreeMap: Node
{
	std::vector<Strip*> stripes;
	Label* root;
	int depth() { return stripes.size(); }
	void fill(float x, Label* tree, int strip_index)
	{
		Strip* strip;
		if (strip_index >= stripes.size())
		{
			strip = new Strip();
			strip->setPositionX(x);
			stripes.push_back(strip);
			addChild(strip);
		}
		else
		{
			strip = stripes[strip_index];
		}

		x += 100;

		if (tree == NULL)
		{
			// add space if needed
			if (strip->getChildrenCount() > 0)
			{
				strip->add_space();
			}
		}
		else
		{
			strip->add_label(tree);
			fill(x, NULL, strip_index + 1); // add space to children strip
			for (auto label: tree->items)
			{
				fill(x, label, strip_index + 1);
			}
		}
	}

	// calculate stripe positions
	void measure()
	{
		root->fill_dummies(depth() - 1);
		measure(0, root, 0);
	}

	void measure(int strip_index, Label* tree, float left_stripe_position)
	{
		Strip* strip = stripes[strip_index];
		auto& positions = strip->positions;
		PosData pos;
		pos.left_stripe_position = left_stripe_position;
		pos.position = tree->position();
		if (strip_index < depth() - 1)
		{
			float first_child_pos = (*tree->items.begin())->position();
			// float last_child_pos = (*tree->items.begin())->position();
			pos.right_stripe_position = first_child_pos;
		}
		else
		{
			// ignore pos.right_stripe_position
		}

		positions.push_back(pos);

		for (Label* label: tree->items)
		{
			measure(strip_index + 1, label, pos.position);
		}
	}

	int get_strip_index(Strip* s)
	{
		auto it = find(stripes.begin(), stripes.end(), s);
		return it - stripes.begin();
	}

	void set_label_to_horizont(Label* label)
	{
		Strip* s = (Strip*)label->getParent();
		int s_index = get_strip_index(s);
		set_posiotion(s_index, label->position());
	}

	void set_posiotion(int strip_index, int position)
	{
		int n = depth();
		Strip* s = stripes[strip_index];
		PosData spos = s->set_position(position);

		// set_strip_position(strip_index, position);
		
		position = spos.right_stripe_position;
		for (int i = strip_index + 1; i < n; ++i)
		{
			Strip* f = stripes[i];
			PosData d = f->set_position(position);
			position = d.right_stripe_position;
		}

		position = spos.left_stripe_position;
		for (int i = strip_index - 1; i >= 0; --i)
		{
			Strip* f = stripes[i];
			PosData d = f->set_position(position);
			position = d.left_stripe_position;
		}
	}

	void test()
	{
		root = Label::create();
		test_fill(root, "label", 0, 3);
		fill(100, root, 0);
		measure();
	}

	void test_fill(Label* label, string parent_name, int s_ind, int s_n)
	{
		if (s_ind < s_n)
		{
			int n = 3;
			for (int i = 0; i < n; ++i)
			{
				Label* l = Label::create();
				string name = parent_name + "." + to_string(i);
				l->setText(name);
				//l->setText("alma");
				label->items.push_back(l);
				test_fill(l, name, s_ind + 1, s_n);
			}
		}
	}
};

TreeMap tree;

// on "init" you need to initialize your instance
bool WorldUI::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	init_menu();

	// init model
	
    auto visibleSize = Director::getInstance()->getVisibleSize();

    _map = Sprite::create("map.png");
    _map->setPosition(Vec2(visibleSize / 2));
	_map->setScale(1);
    this->addChild(_map, 0, 0);
	_map->setLocalZOrder(-1);
    
	//auto label = Text::create();
	//label->setText("fazsa");
	//label->setFontSize(18);
	//label->setPosition(Vec2(visibleSize / 2));
	//_map->addChild(label);
	_map->setVisible(false);

	this->addChild(&tree);
	tree.setPosition(Vec2(100, visibleSize.height / 2));
	tree.test();
	tree.set_label_to_horizont(tree.root->items[1]->items[1]);
	//tree.set_label_to_horizont(tree.root->items[1]->items[1]->items[1]);
	//tree.set_label_to_horizont(tree.root->items[0]);
	//for (Strip* s: tree.stripes)
	//{
	//	this->addChild(s);
	//}
	

    //auto listener = EventListenerTouchAllAtOnce::create();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchMoved = CC_CALLBACK_2(WorldUI::onTouchMoved, this);
	listener->onTouchBegan = CC_CALLBACK_2(WorldUI::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(WorldUI::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void WorldUI::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(WorldUI::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}
int lofusz = 0;
void WorldUI::onDraw(const Mat4 &transform, uint32_t flags)
{
    Director* director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    //draw
    CHECK_GL_ERROR_DEBUG();
    
    // draw a simple line
    // The default state is:
    // Line Width: 1
    // color: 255,255,255,255 (white, non-transparent)
    // Anti-Aliased
    //  glEnable(GL_LINE_SMOOTH);
    //DrawPrimitives::drawLine( VisibleRect::leftBottom(), VisibleRect::rightTop() );
	auto b = _map->getBoundingBox();
	//DrawPrimitives::drawLine( Vec2(b.getMinX(), b.getMinY()), Vec2(b.getMaxX(), b.getMaxY()));
	//DrawPrimitives::drawLine( Vec2(b.getMinX(), b.getMaxY()), Vec2(b.getMaxX(), b.getMinY()));
}

void WorldUI::onEnter()
{
	Layer::onEnter();
}

void WorldUI::menuCloseCallback(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void WorldUI::onTouchMoved(Touch* touch, Event  *event)
{
	auto diff = touch->getDelta();
	_map->setPosition(_map->getPosition() + diff);
	tree.setPosition(tree.getPosition() + diff);
	//_items->setPosition(_items->getPosition() + diff);
}

bool WorldUI::onTouchBegan(Touch* touch, Event  *event)
{
    return true;
}

void WorldUI::onTouchEnded(Touch* touch, Event  *event)
{
	auto s = touch->getStartLocation();
	auto p = touch->getLocation();
}
}