#include "Iedk.h"
#include "IedkErrorCode.h"
#include "ProfileControl.h"


void split(const std::string& input, std::vector<std::string>& tokens) {

	tokens.clear();
	std::stringstream ss(input);
	std::string oneToken;
	
	while (ss >> oneToken) {
		try {
			tokens.push_back(oneToken);
		}
		catch (const std::exception&) {}
	}
}


template <typename T>
bool convert(const std::string& str, T& value) {
	std::istringstream iss(str);
	return (iss >> value) ? true : false;
}


bool profileHandleToByteArray(EmoEngineEventHandle eProfile,
                              unsigned char** profileBuffer,
                              unsigned int* profileSize) {

	assert(eProfile);
	assert(profileBuffer);
	assert(profileSize);

	if (*profileBuffer) {
		delete [] *profileBuffer, *profileBuffer = 0;
	}
	
	// Query the size of the profile byte stream
	bool ok = (IEE_GetUserProfileSize(eProfile, profileSize) == EDK_OK);

	if (ok && *profileSize > 0) {

		// Copy the content of profile byte stream into local buffer
		*profileBuffer = new unsigned char[*profileSize];
        ok = (IEE_GetUserProfileBytes(eProfile, *profileBuffer,
                                      *profileSize) == EDK_OK );
		
		if (!ok) {
			delete [] *profileBuffer, *profileBuffer = 0;
		}
	}

	return ok;
}


bool parseCommand(const std::string& input, std::ostream& output,
				  ExternalProfileManager& profileManager,
				  std::map<unsigned int,std::string>& headsetProfileMap,
                  const unsigned char* baseProfile,
                  unsigned int baseProfileSize) {

	assert(baseProfile && baseProfileSize);

	bool result = true;
	std::ostringstream os;

	if (input.length()) {

		bool wrongArgument = true;
		std::vector<std::string> commands;
		split(input, commands);

		os << "==> ";

		// Quit command
		if (commands.at(0) == "exit") {
			os << "Bye!";
			result = false;
			wrongArgument = false;
		}

		// Print available commands
		else if (commands.at(0) == "help") {
			os << "Available commands:" << std::endl;
			
            os << "add_profile [profile name] \t\t add a new profile"
               << std::endl;
            os << "remove_profile [profile name] \t\t remove an existing profile"
               << std::endl;
            os << "set_profile [profile name] [userID] \t set an existing profile"
                  " to a certain user ID"
               << std::endl;
            os << "get_profile [profile name] [userID] \t get the binary profile"
                  " of a certain user ID"
               << std::endl;
			os << "list_profile \t\t\t\t list existing profiles" << std::endl;
			os << "save_profile \t\t\t\t save existing profiles" << std::endl;
			os << "list_mapping \t\t\t\t list mapping between headsets and profiles";
			os << "exit \t\t\t\t\t exit this program";

			wrongArgument = false;
		}

		// Add a new profile
		else if (commands.at(0) == "add_profile") {

			if (commands.size() == 2) {
				const std::string& profileName = commands.at(1);

				if (!profileManager.isExist(profileName)) {

					wrongArgument = !profileManager.insertProfile(
                                                    profileName, baseProfile,
                                                    baseProfileSize);
					if (!wrongArgument) {
						os << "Profile [" << profileName << "] added.";
					}
				}
				else {
					os << "Profile [" << profileName << "] already existed.";
					wrongArgument = false;
				}
			}
		}

		// Remove an existing profile
		else if (commands.at(0) == "remove_profile") {

			if (commands.size() == 2) {
				const std::string& profileName = commands.at(1);
				wrongArgument = !profileManager.removeProfile(profileName);

				if (!wrongArgument) {
					os << "Profile [" << profileName << "] removed.";
				}

                // Remove the profile from the mapping table
                // if it assoicated with a headset
                std::map<unsigned int,std::string>::iterator iter =
                        headsetProfileMap.begin();

				for ( ; iter != headsetProfileMap.end(); iter++) {
					if (iter->second == profileName) {
						headsetProfileMap[iter->first] = std::string();
						break;
					}
				}
			}
		}

		// Extract and use an existing profile for a particular headset
		else if (commands.at(0) == "set_profile") {

			if (commands.size() == 3) {

				unsigned int userID;
				const std::string& profileName = commands.at(1);

                if (convert(commands.at(2), userID) &&
                    profileManager.isExist(profileName)) {

					unsigned char tmp;
					unsigned int profileSize = 1;

					// first call: enquiry profile size
					profileManager.extractProfile(profileName, &tmp, &profileSize);

					unsigned char* profileBuffer = new unsigned char[profileSize];

					// second call: extract the actual profile content
                    if (profileManager.extractProfile(profileName, profileBuffer,
                                                      &profileSize)) {

						// Set the binary profile into EmoEngine
                        wrongArgument = IEE_SetUserProfile(userID, profileBuffer,
                                                           profileSize) != EDK_OK;

						if (!wrongArgument) {
							headsetProfileMap[userID] = profileName;
                            os << "Profile [" << profileName
                               << "] set with the headset " << userID << ".";
						}
					}

					if (profileBuffer) {
						delete [] profileBuffer, profileBuffer = 0;
					}
				}
			}
		}

		// Retrieve the profile associated with a particular headset
		else if (commands.at(0) == "get_profile") {

			if (commands.size() == 3) {

				unsigned int userID;
				const std::string& profileName = commands.at(1);
				
                if (convert(commands.at(2), userID) &&
                    profileManager.isExist(profileName)) {

					EmoEngineEventHandle eProfile = IEE_ProfileEventCreate();
					
					// Acquire the user profile
					if (IEE_GetUserProfile(userID, eProfile) == EDK_OK) {

						unsigned int   profileSize   = 0;
						unsigned char* profileBuffer = 0;
						
                        if (profileHandleToByteArray(eProfile,
                                                     &profileBuffer,
                                                     &profileSize)) {

							std::map<unsigned int, std::string>::iterator iter
								= headsetProfileMap.find(userID);

							if (iter != headsetProfileMap.end()) {
								const std::string& profileName = iter->second;
								
								if (!profileName.empty()) {
                                    profileManager.insertProfile(profileName,
                                                                 profileBuffer,
                                                                 profileSize);
                                    os << "Profile [" << profileName
                                       << "] acquired successfully.";
									wrongArgument = false;
								}
							}
						}

						if (profileBuffer) {
							delete [] profileBuffer, profileBuffer = 0;
						}
					}

					IEE_EmoEngineEventFree(eProfile);
				}
			}
		}

		// List all existing profiles
		else if (commands.at(0) == "list_profile") {

			if (commands.size() == 1) {
				std::vector<std::string> profileList;
				profileManager.listProfile(profileList);
				
				os << "Available profiles:" << std::endl;
				
				for (size_t i=0; i < profileList.size(); i++) {
					os << i+1 << ". " << profileList.at(i);
					if (i+1 < profileList.size()) {
						os << std::endl;
					}
				}
				
				wrongArgument = false;
			}
		}

		// List existing mapping of headsets and profiles
		else if (commands.at(0) == "list_mapping") {

			if (commands.size() == 1) {

				os << "Available headsets:" << std::endl;

                std::map<unsigned int, std::string>::const_iterator iter =
                                                    headsetProfileMap.begin();

				for ( ; iter != headsetProfileMap.end(); ) {
					const std::string& profileName = iter->second;
					os << "headset " << iter->first << " : ";
                    os << (profileName.empty() ? "[no profile associated]" :
                                                 profileName);
					iter++;
					if (iter != headsetProfileMap.end()) {
						os << std::endl;
					}
				}

				wrongArgument = false;
			}
		}

		// Save all existing profiles
		else if (commands.at(0) == "save_profile") {

			if (commands.size() == 1) {
				wrongArgument = !profileManager.save(DEFAULT_PROFILE_NAME);
				if (!wrongArgument) {
					os << "Profile saved.";
				}
			}
		}

		// Unknown command
		else {
            os << "Unknown command [" << commands.at(0)
               << "]. Type \"help\" to list available commands.";
			wrongArgument = false;
		}

		if (wrongArgument) {
            os << "Wrong argument(s) for command ["
               << commands.at(0) << "]";
		}
	}

	const std::string& outString = os.str();
	if (outString.length()) {
		output << outString << std::endl << std::endl;
	}

	return result;
}
