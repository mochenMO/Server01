#pragma once
#ifndef _JSON_H_
#define _JSON_H_

#include <string>


namespace myJson
{


class Json
{
private:
	std::string json;

public:
	Json():json("{}")
	{ }

	~Json() = default;

	std::string& getJson() {
		return json;
	}

	void addJsonValue(const std::string& attributeName, const std::string& strValue) {
		std::string value = "\"" + attributeName + "\"" + ":" + "\"" + strValue + "\"";
		json.insert(1, value);
	}
};




}

#endif // !_JSON_H_



