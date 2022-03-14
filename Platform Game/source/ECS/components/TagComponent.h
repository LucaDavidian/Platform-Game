#ifndef TAG_COMPONENT_H
#define TAG_COMPONENT_H

#include "../Component.h"
#include <string>

class TagComponent : public Component
{
public:
	TagComponent(const std::string &tag) : mTag(tag) {}

	std::string GetTag() const { return mTag; }

	bool operator==(const std::string &tag) const
	{
		return mTag == tag;
	}

	bool operator!=(const std::string &tag) const
	{
		return !(*this == tag);
	}

private:
	const std::string mTag;

	static const bool mRegistered;
};

#endif // TAG_COMPONENT_H
