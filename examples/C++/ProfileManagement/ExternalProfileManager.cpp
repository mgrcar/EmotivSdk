#include <fstream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/regex.hpp>
#pragma warning(push)

// "conversion from size_t to unsigned int"
#pragma warning(disable : 4267)
#pragma warning(disable : 4996)

#include <boost/archive/archive_exception.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#pragma warning(pop)

#include "ExternalProfileManager.h"

using namespace std;
namespace fs = boost::filesystem;

#ifdef _WIN32
// Illegal profile chars are determined by legal filename characters
// Note: this is a Windows-specific list at the moment.
const char ILLEGAL_PROFILE_CHARS_RE[] = "[\\/?:*<>\"|]";
// Profile file suffix might be platform-specific too
const char PROFILE_FILENAME_SUFFIX[] = ".eep"; // Emo Engine Profile
const char PROFILE_FILENAME_RE[] = "\\A(.+)\\.eep\\z";
#endif

ExternalProfileManager::ExternalProfileManager()
{
}

ExternalProfileManager::~ExternalProfileManager()
{
}

bool ExternalProfileManager::save(const string& location)
{
	ofstream ofs(location.c_str(), ios_base::binary);
	if ( !ofs.is_open() ) return false;
	try {
		boost::archive::binary_oarchive oa(ofs);
		oa << *this;
	}
	catch (boost::archive::archive_exception& )
	{
		return false;
	}
	return true;
}

bool ExternalProfileManager::load(const string& location)
{
	ifstream ifs(location.c_str(), ios_base::binary);
	if ( !ifs.is_open() ) return false;
	try { 
		boost::archive::binary_iarchive ia(ifs);
		ia >> *this;
	}
	catch (boost::archive::archive_exception& )
	{
		return false;
	}
	return true;
}

bool ExternalProfileManager::unload()
{
	_profiles.clear();
	return true;
}

bool ExternalProfileManager::addProfile(const string& name)
{
	if ( _profiles.find(name) != _profiles.end() ) {
		// A profile with this name already exists
		return false;
	}
	// A new profile consists of an empty string
	_profiles[name] = "";
	return true;
}

bool ExternalProfileManager::removeProfile(const string& name)
{
	profileItr_t itr = _profiles.find(name);
	if ( itr == _profiles.end() ) return false;
	_profiles.erase(itr);

	return true;
}

bool ExternalProfileManager::insertProfile(const string& name,
                                           const unsigned char* profileBuf,
                                           unsigned int bufSize)
{
	assert(profileBuf);
	// Replace our stored bytes with the contents of the buffer passed by the caller
	string bytesIn(profileBuf, profileBuf+bufSize);
	_profiles[name] = bytesIn;
	return true;
}

bool ExternalProfileManager::extractProfile(const string& name,
                                            unsigned char* profileBuf,
                                            unsigned int* pBufSize /* in/out */)
{
	assert(pBufSize);

	// First, check to see if the designated profile exists
	profileItr_t itr = _profiles.find(name);
	if ( itr == _profiles.end() ) {
		*pBufSize = 0;
		return false;
	}

	const string& refProfileBytes = itr->second;

	// Copy the contents of our binary profile string into the caller's buffer
	// (unless it's not big enough).
	if ( *pBufSize && (refProfileBytes.size() <= *pBufSize) && profileBuf ) {
		memcpy(profileBuf, refProfileBytes.data(), refProfileBytes.size());
		*pBufSize = (unsigned int) refProfileBytes.size();
	}
	else {
		*pBufSize = (unsigned int) refProfileBytes.size();
		return false;
	}

	return true;
}

bool ExternalProfileManager::listProfile(vector<string>& profiles)
{
	profiles.clear();
	for ( profileItr_t itr = _profiles.begin(); itr != _profiles.end(); ++itr ) {
		profiles.push_back(itr->first);
	}
	return true;
}

bool ExternalProfileManager::isExist(const std::string& name)
{
	for ( profileItr_t itr = _profiles.begin(); itr != _profiles.end(); ++itr ) {
		if (itr->first == name) {
			return true;
		}
	}
	return false;
}

unsigned int ExternalProfileManager::numProfile()
{
	return (unsigned int)_profiles.size();
}
