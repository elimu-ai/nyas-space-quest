#include "LanguageManager.h"

LanguageManager* LanguageManager::_instance = 0;

LanguageManager::LanguageManager()
{
	string fileName;
	// detect current language
	switch (CCApplication::getInstance()->getCurrentLanguage())
	{
	case LanguageType::ENGLISH:
		fileName = "en.json";
		break;
	case LanguageType::RUSSIAN:
		fileName = "ru.json";
		break;
	case  LanguageType::CHINESE:
		fileName = "cn.json";
		break;
	case  LanguageType::FRENCH:
		fileName = "fr.json";
		break;
	case  LanguageType::ITALIAN:
		fileName = "it.json";
		break;
	case  LanguageType::GERMAN:
		fileName = "ge.json";
		break;
	case  LanguageType::SPANISH:
		fileName = "sp.json";
		break;
	case LanguageType::DUTCH:
		fileName = "du.json";
		break;
	case  LanguageType::KOREAN:
		fileName = "ko.json";
		break;
	case  LanguageType::JAPANESE:
		fileName = "jp.json";
		break;
	case  LanguageType::HUNGARIAN:
		fileName = "hu.json";
		break;
	case  LanguageType::PORTUGUESE:
		fileName = "pt.json";
		break;
	case  LanguageType::ARABIC:
		fileName = "ar.json";
		break;
	case  LanguageType::NORWEGIAN:
		fileName = "nw.json";
		break;
	case  LanguageType::POLISH:
		fileName = "po.json";
		break;
	case LanguageType::SWAHILI:
		fileName = "sw.json";
		break;
	default:
		CCLOG("Unknown language. Use english");
		fileName = "en.json";
		break;
	};

	// below we open, read and parse language data file with rapidjson library
	string clearContent = FileUtils::getInstance()->getStringFromFile("i18n/" + fileName);

	document.Parse<0>(clearContent.c_str());
	if (document.HasParseError())
	{
		CCLOG("Language file parsing error!");
		return;
	}
}

LanguageManager* LanguageManager::getInstance()
{
	if (!_instance)
		_instance = new LanguageManager();
	return _instance;
}

string LanguageManager::getStringForKey(string key)
{
	return document[key.c_str()].GetString();
}

string  LanguageManager::getString(string key)
{
	return getInstance()->getStringForKey(key);
}