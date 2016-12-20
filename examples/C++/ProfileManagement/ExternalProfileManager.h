#ifndef EXTERNAL_PROFILE_MANAGER_H
#define EXTERNAL_PROFILE_MANAGER_H

#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/tracking.hpp>
#include <boost/serialization/base_object.hpp>

class ExternalProfileManager
{
	ExternalProfileManager(const ExternalProfileManager&) {};
	ExternalProfileManager& operator = (const ExternalProfileManager&) {};

protected:
	std::map<std::string, std::string > _profiles;
	typedef std::map<std::string, std::string >::iterator profileItr_t;

	// Boost serialization support
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive& ar, const unsigned int /*file version */)
	{
		ar & _profiles;
	}

public:
	ExternalProfileManager();
	virtual ~ExternalProfileManager();

	virtual bool        save(const std::string& location);
	virtual bool        load(const std::string& location);
	virtual bool        unload();
	virtual bool        addProfile(const std::string& name);
	virtual bool        removeProfile(const std::string& name);
    virtual bool        insertProfile(const std::string& name,
                                      const unsigned char* profileBuf,
                                      unsigned int bufSize);
    virtual bool        extractProfile(const std::string& name,
                                       unsigned char* profileBuf,
                                       unsigned int* pBufSize /* in/out */);
	virtual bool		listProfile(std::vector<std::string>& profiles);
	virtual bool		isExist(const std::string& name);
	virtual unsigned int       numProfile();
};

//BOOST_CLASS_FEORT(ExternalProfileManager);
//BOOST_CLASS_TRACKING(ExternalProfileManager, boost::serialization::track_never);

#endif // EXTERNAL_PROFILE_MANAGER_H
