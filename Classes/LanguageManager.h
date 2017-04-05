#ifndef LanguageManager_h
#define LanguageManager_h

#include <string>
using std::string;

#include "cocos2d.h"
USING_NS_CC;

#include "../external/json/document.h" 
#include "../external/json/rapidjson.h"

using namespace rapidjson; // library that we use for json parsing

class LanguageManager
{
	Document document; // current document with language data
	LanguageManager(); // constructor is private
	static LanguageManager* _instance;
public:
	static LanguageManager* getInstance();
	string getStringForKey(string key);
	static string getString(string key);
};
#endif