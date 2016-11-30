#pragma once
#include "ResCache.h"

class XmlResourceExtraData : public IResourceExtraData
{
public:
	virtual std::string VToString() { return "XmlResourceExtraData"; }
	bool ParseXml(const char* pRawBuffer, uint32_t rawSize);
	tinyxml2::XMLElement* GetRoot(void) { return m_xmlDocument.RootElement(); }

private:
	tinyxml2::XMLDocument m_xmlDocument;
};


class XmlResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return true; }
	virtual uint32_t VGetLoadedResourceSize(char *rawBuffer, uint32_t rawSize) { return rawSize; }
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle);
	virtual std::string VGetPattern() { return "*.xml"; }

	static tinyxml2::XMLElement* LoadAndReturnRootXmlElement(const char* resourceString);
};


