#include "AnimationManager.h"
#include <boost/lexical_cast.hpp>

using namespace Engine;
using std::string;
using sf::IntRect;
using sf::Vector2f;
using sf::Texture;
using std::vector;

Engine::AnimationManager::~AnimationManager()
{
	for (auto& iter : animationList)
	{
		delete iter;
	}
	animationList.clear();
}

void AnimationManager::loadAnimation_j(std::string_view path)
{
	AnimationJson* anim = new AnimationJson;
	json j;
	j.clear();
	std::ifstream i(path.data());
	i >> j;
	i.clear();
	i.close();
	IntRect rect;
	Vector2f center;

	if (j.at("IntRect").is_array())
		rect = IntRect(j.at("IntRect")[0].get<int>(), j.at("IntRect")[1].get<int>(), j.at("IntRect")[2].get<int>(), j.at("IntRect")[3].get<int>());
	if (j.at("Center").is_array())
		center = Vector2f(j.at("Center")[0].get<int>(), j.at("Center")[1].get<int>());

	anim->name = j.at("Name").get<string>();
	anim->rect = rect;
	anim->frame = j.at("Frame").get<int>();
	anim->speed = j.at("Speed").get<float>();
	anim->scale = j.at("Scale").get<float>();
	anim->Center = center;
	if (animationList.empty())
	{
		animationList.push_back(anim);
		currAnim = animationList.begin();
	}
	else
		animationList.push_back(anim);
}

IntRect& AnimationManager::animUpdate(float t)
{
	return (*currAnim)->tick(t);
}

void AnimationManager::loadAnimation_x(std::string_view fileName, sf::RenderWindow& w)
{
	path = fileName;
	using boost::lexical_cast;
	using boost::bad_lexical_cast;
	TiXmlDocument animFile(fileName.data());

	animFile.LoadFile();

	TiXmlElement* head;
	head = animFile.FirstChildElement("sprites");
	string img = head->Attribute("image");

	TiXmlElement* settings;
	settings = head->FirstChildElement("settings");

	TiXmlElement* animElement;
	animElement = head->FirstChildElement("animation");

	bool isXMLAnimation = true;

	auto xml_rect = settings->FirstChildElement("rect");
	auto xml_loop = settings->FirstChildElement("loop");
	auto xml_scale = settings->FirstChildElement("scale");

	scale = atof(xml_scale->Attribute("value"));
	texture.loadFromFile("Data\\OSprite\\" + img);
	texture.setSmooth(true);

	while (animElement)
	{
		AnimationXml* anim = new AnimationXml;
		vector<IntRect> frames;

		string Name = animElement->Attribute("title");
		float delay = atof(animElement->Attribute("delay"));
		anim->name = Name;
		anim->speed = delay / 20000;

		auto loop_ = animElement->Attribute("loop");
		if (loop_)
			anim->looped = boost::lexical_cast<bool>(loop_);
		else
			anim->looped = boost::lexical_cast<bool>(xml_loop->Attribute("value"));
		anim->scale = atof(xml_scale->Attribute("value"));

		TiXmlElement* cut;
		cut = animElement->FirstChildElement("cut");
		while (cut)
		{
			int x = atoi(cut->Attribute("x"));
			int y = atoi(cut->Attribute("y"));
			int w = atoi(cut->Attribute("w"));
			int h = atoi(cut->Attribute("h"));

			frames.emplace_back(x, y, w, h);
			cut = cut->NextSiblingElement("cut");
		}
		anim->frameCount++;
		frames.shrink_to_fit();
		anim->frameCount = frames.size();
		anim->frames = std::move(frames);
		if (animationList.empty())
		{
			animationList.push_back(anim);
			currAnim = animationList.begin();
			(*currAnim)->rect = (*currAnim)->frames[0];
			(*currAnim)->origin.x = anim->rect.width / 2;
			(*currAnim)->origin.y = anim->rect.height;
		}
		else
			animationList.push_back(anim);
		animElement = animElement->NextSiblingElement("animation");
	}
	animFile.Clear();
}

void AnimationManager::setCurrAnimation(std::list <Animation*>::iterator iter)
{
	(*currAnim)->frame = 0;
	currAnim = iter;
}

std::list <Animation*>::iterator AnimationManager::getAnimationByName(std::string_view Name)
{
	return
		std::find_if(animationList.begin(), animationList.end(),
			[=](Animation* anim)
			{
				return (anim->name == Name);
			});
}

std::list<Engine::Animation*>* const Engine::AnimationManager::getAnimationList()
{
	return &animationList;
}

IntRect& AnimationJson::tick(float time)
{
	int widht = rect.width;
	int height = rect.height;
	int top = rect.top;
	int left = rect.left;
	frame += speed * time;
	if (frame > 2) frame = 0;
	if (int(frame) >= 1)
		int z = 5;
	IntRect x(widht * int(frame), top, widht, height);
	return x;
}
#include "LogConsole.h"
sf::IntRect& Engine::AnimationXml::tick(const float time)
{
	if (frames.size() > 0 && state != AEnd)
	{
		state = APlay;
		frame += speed;
		if (frame > frameCount)
		{
			frame = 0;
			if (!looped)
				state = AEnd;
		}
		origin.x = frames[static_cast<int>(frame)].width / 2;
		origin.y = frames[static_cast<int>(frame)].height;
		return frames[static_cast<int>(frame)];
	}
	else
		return frames[0];
}
