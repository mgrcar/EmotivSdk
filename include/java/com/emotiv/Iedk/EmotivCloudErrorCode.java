package com.emotiv.Iedk;


public enum EmotivCloudErrorCode {
	// ! Default success value.
	EC_OK(0x0000),

	// ! An internal error occurred.
	EC_UNKNOWN_ERROR(0x0001),

	// ! An internal error occurred.
	EC_COULDNT_RESOLVE_PROXY(0x0002),

	EC_COULDNT_RESOLVE_HOST(0x0003),

	EC_COULDNT_CONNECT(0x0004),

	// ! Profile created by EC_SaveUserProfile() is existed in Emotiv Cloud.
	EC_PROFILE_CLOUD_EXISTED(0x0101),

	// ! The buffer is not a valid, serialized EmoEngine profile.
	EC_INVALID_PROFILE_ARCHIVE(0x0102),

	// ! One of the parameters supplied to the function is invalid
	EC_INVALID_PARAMETER(0x0300),

	// ! A parameter supplied to the function is out of range.
	EC_OUT_OF_RANGE(0x0301),

	// ! A filesystem error occurred.
	EC_FILESYSTEM_ERROR(0x0302),

	// ! The buffer supplied to the function is not large enough.
	EC_BUFFER_TOO_SMALL(0x0303), 

	// ! The file uploaded to cloud is failed
	EC_UPLOAD_FAILED(0x0304),

	// ! The cloud user ID supplied to the function is invalid.
	EC_INVALID_CLOUD_USER_ID(0x0400),

	// ! The user ID supplied to the function is invalid
	EC_INVALID_ENGINE_USER_ID(0x0401),

	// ! The user ID supplied to the function dont login, call EC_Login() first
	EC_CLOUD_USER_ID_DONT_LOGIN(0x0402),

	// ! The Emotiv Cloud needs to be initialized via EC_Connect()
	EC_EMOTIVCLOUD_UNINITIALIZED(0x0500), 

	// ! The Emotiv Engine needs to be initialized via IEE_EngineConnect() or IEE_EngineRemoteConnect() before call EC_Connect() or Emotiv Engine is disconnected
	EC_EMOENGINE_UNINITIALIZED(0x0501),
	
	EC_FILE_EXISTS(0x1001), 
	
	// ! Reserved return value.
	EC_RESERVED1(0x0900);
	
	private int bit;

	EmotivCloudErrorCode(int bitNumber) {
		bit = bitNumber;
	}

	public int ToInt() {
		return (bit);
	}
}
