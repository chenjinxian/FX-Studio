#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"
#include "ResCache.h"
#include <tinyxml.h>

extern shared_ptr<IResourceLoader> CreateXmlResourceLoader();

class XmlResourceExtraData : public IResourceExtraData
{
public:
	virtual std::string VToString() { return "XmlResourceExtraData"; }
	void ParseXml(char* pRawBuffer);
	TiXmlElement* GetRoot(void) { return m_xmlDocument.RootElement(); }

private:
	TiXmlDocument m_xmlDocument;
};


class XmlResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return true; }
	virtual uint32_t VGetLoadedResourceSize(char *rawBuffer, uint32_t rawSize) { return rawSize; }
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle);
	virtual std::string VGetPattern() { return "*.xml"; }

	static TiXmlElement* LoadAndReturnRootXmlElement(const char* resourceString);
};


