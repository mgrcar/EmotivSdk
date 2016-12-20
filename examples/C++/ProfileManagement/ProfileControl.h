#ifndef PROFILE_CONTROL_H
#define PROFILE_CONTROL_H

#define DEFAULT_PROFILE_NAME	"profile.bin"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "ExternalProfileManager.h"

bool profileHandleToByteArray(EmoEngineEventHandle eProfile,
                              unsigned char** profileBuffer,
                              unsigned int* profileSize);

bool parseCommand(const std::string& command, std::ostream& output,
				  ExternalProfileManager& profileManager,
				  std::map<unsigned int,std::string>& headsetProfileMap,
                  const unsigned char* baseProfile,
                  unsigned int baseProfileSize);

#endif // PROFILE_CONTROL_H
