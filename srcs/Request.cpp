#include "Request.hpp"

Request::Request(void)
	: mPhase(READY)
	, mTransferType(GENERAL)
	, mURItype(DIRECTORY)
	, mSeek(0)
{
}

void								Request::addOrigin(const std::string& added_origin)
{
	m_origin.append(added_origin);
}

const std::string&					Request::get_m_origin(void) const
{
	return (m_origin);
}

void								Request::SetOrigin(const std::string& origin)
{
	m_origin = origin;
}

const std::string&					Request::GetMethod(void) const
{
	return (mMethod);
}

void								Request::SetMethod(const std::string& method)
{
	mMethod = method;
}

const Request::ePhase&				Request::GetPhase(void) const
{
	return (mPhase);
}

void								Request::SetPhase(const ePhase& phase)
{
	mPhase = phase;
}

bool								Request::isValidHeader(const std::string& header)
{
	std::size_t	found = header.find(": ");
	if (found == std::string::npos)
	{
		throw 400;
	}

	std::string	key = header.substr(0, found);

	std::locale loc;
	for(std::string::size_type i = 0; i < key.length(); i++)
	{
		key[i] = std::tolower(key[i], loc);
	}

	// FIXME request에 없는 헤더도 있는데, 그 부분을 수정해야할 듯 일단 다 넣음
	std::string			headerSet[18] = {"accept-charsets", "accept-language", "allow", "authorization", "content-language", "content-length", "content-location", "content-type", "date", "host", "last-modified", "location", "referer", "retry-after", "server", "transfer-encoding", "user-agent", "www-authenticate"};
	int					i;

	for (i = 0 ; i < 18; i++)
	{
		if ((key.compare(headerSet[i]) == 0))
		{
			return (true);
		}
	}
	return (false);
}

void								Request::addHeader(const std::string& header)
{
	std::size_t	found = header.find(": ");
	if (found == std::string::npos)
	{
		throw 400;
	}
	std::string	key = header.substr(0, found);
	std::locale loc;
	for(std::string::size_type i = 0; i < key.length(); i++)
	{
		key[i] = std::tolower(key[i], loc);
	}

	std::string	value = header.substr(found + 2);

	if (key.compare("transfer-encoding") == 0)
	{
		if (value.find("chunked") != std::string::npos)
		{
			SetTransferType(CHUNKED);
		}
	}

	if (key.compare("host") == 0)
	{
		std::size_t	found = value.find(":");
		if (found != std::string::npos)
		{
			value = value.substr(0, found);
		}
	}
	std::cout << "\t\t|" << key << "| |" << value << "|" << std::endl;
	mHeaders.insert(std::pair<std::string, std::string>(key, value)); // REVIEW pair 허용인지 확인해야함
}

const std::string&					Request::getBody(void) const
{
	return (mBody);
}

void								Request::SetBody(const std::string& body)
{
	mBody = body;
}

void								Request::addBody(const std::string& body)
{
	mBody.append(body);
}

const std::size_t&					Request::GetSeek(void) const
{
	return (mSeek);
}

void								Request::SetSeek(const std::size_t& seek)
{
	mSeek = seek;
}

std::map<std::string, std::string>&	Request::GetHeaders(void)
{
	return (mHeaders);
}

const std::string&					Request::GetVersion(void) const
{
	return (mVersion);
}
void								Request::SetVersion(const std::string& version)
{
	mVersion = version;
}

const Request::eTransferType&		Request::GetTransferType(void) const
{
	return (mTransferType);
}

void								Request::SetTransferType(const eTransferType& trasferType)
{
	mTransferType = trasferType;
}

void								Request::ParseURI(std::string& uri)
{
	//ANCHOR parseURI 작업중
	// http://localhost:8000/2019/08/index.html;page=1?isEdit=true&id=123#fragment

	std::size_t	found;
	
	mURI = uri;

	// query parsing
	found = uri.find_last_of("?");
	if (found != std::string::npos)
	{
		mQuery = uri.substr(found + 1);
		std::cout << "\t    mQuery : |" << mQuery << "|" << std::endl;
		uri.resize(found);
	}

	// parameter parsing
	found = uri.find_last_of(";");
	if (found != std::string::npos)
	{
		mParameter = uri.substr(found + 1);
		std::cout << "\tmParameter : |" << mParameter << "|" << std::endl;
		uri.resize(found);
	}

	// filename parsing
	found = uri.find_last_of("/");
	if (found != uri.length() - 1)
	{
		mFileName = uri.substr(found + 1);
		std::cout << "\t mFileName : |" << mFileName << "|" << std::endl;
		uri.resize(found);
	}

	// directory parsing
	mDirectory = uri;
	std::cout << "\tmDirectory : |" << mDirectory << "|" << std::endl;

	// uri 타입 설정
	if (mFileName.empty())
	{
		mURItype = Request::DIRECTORY;
	}
	else
	{
		// STUB CGI_PROGRAM 설정
		// if (mFileName.compare(GetCGIprogramName()) == 0)
		if (mFileName.compare("cgi_tester") == 0)
		{
			mURItype = Request::CGI_PROGRAM;
		}
		else
		{
			mURItype = Request::FILE;
		}
	}
	if (mMethod.compare("PUT") == 0) // STUB POST메소드도 추가해야할수도
	{
			mURItype = Request::FILE_TO_CREATE;
	}
}

void								Request::SetURI(const std::string& uri)
{
	mURI = uri;
}

const std::string&					Request::GetURI(void) const
{
	return (mURI);
}

const std::string&					Request::GetDirectory(void) const
{
	return (mDirectory);
}

void								Request::SetDirectory(const std::string& directory)
{
	mDirectory = directory;
}

const std::string&					Request::GetFileName(void) const
{
	return (mFileName);
}

void								Request::SetFileName(const std::string& fileName)
{
	mFileName = fileName;
}

const std::string&					Request::GetParameter(void) const
{
	return (mParameter);
}

void								Request::SetParameter(const std::string& parameter)
{
	mParameter = parameter;
}

const std::string&					Request::GetQuery(void) const
{
	return (mQuery);
}

void								Request::SetQuery(const std::string& query)
{
	mQuery = query;
}

const Request::eURIType&			Request::GetURItype(void) const
{
	return (mURItype);
}

void								Request::SetURItype(const eURIType& uriType)
{
	mURItype = uriType;
}