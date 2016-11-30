#include "XmlResource.h"
#include "../AppFramework/BaseGameApp.h"

bool XmlResourceExtraData::ParseXml(const char* pRawBuffer, uint32_t rawSize)
{
	tinyxml2::XMLError error = m_xmlDocument.Parse(pRawBuffer, rawSize);
	return error == tinyxml2::XML_SUCCESS;
}

bool XmlResourceLoader::VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle)
{
	if (rawBuffer == nullptr || rawSize == 0)
		return false;

	shared_ptr<XmlResourceExtraData> pExtraData = shared_ptr<XmlResourceExtraData>(DEBUG_NEW XmlResourceExtraData());
	pExtraData->ParseXml(rawBuffer, rawSize);

	handle->SetExtraData(shared_ptr<XmlResourceExtraData>(pExtraData));

	return true;
}

shared_ptr<IResourceLoader> CreateXmlResourceLoader()
{
	return shared_ptr<IResourceLoader>(DEBUG_NEW XmlResourceLoader());
}

tinyxml2::XMLElement* XmlResourceLoader::LoadAndReturnRootXmlElement(const char* resourceString)
{
	Resource resource(resourceString);
	shared_ptr<ResHandle> pResourceHandle = g_pApp->GetResCache()->GetHandle(&resource);
	shared_ptr<XmlResourceExtraData> pExtraData = static_pointer_cast<XmlResourceExtraData>(pResourceHandle->GetExtraData());
	if (pExtraData != nullptr)
	{
		return pExtraData->GetRoot();
	}
	else
	{
		return nullptr;
	}
}


