using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Emotiv
{
    public class EmotivCloudClient
    {

        //! Default success value.
        public const Int32 EC_OK                        = 0x0000;

        //! An internal error occurred.
        public const Int32 EC_UNKNOWN_ERROR             = 0x0001;

        //! An internal error occurred.
        public const Int32 EC_COULDNT_RESOLVE_PROXY     = 0x0002;

        public const Int32 EC_COULDNT_RESOLVE_HOST      = 0x0003;

        public const Int32 EC_COULDNT_CONNECT           = 0x0004;

        //! Profile created by EC_SaveUserProfile() is existed in Emotiv Cloud.
        public const Int32 EC_PROFILE_CLOUD_EXISTED     = 0x0101;

        //! The buffer is not a valid, serialized EmoEngine profile.
        public const Int32 EC_INVALID_PROFILE_ARCHIVE   = 0x0102;

        //! One of the parameters supplied to the function is invalid
        public const Int32 EC_INVALID_PARAMETER         = 0x0300;

        //! A parameter supplied to the function is out of range.
        public const Int32 EC_OUT_OF_RANGE              = 0x0301;

        //! A filesystem error occurred.
        public const Int32 EC_FILESYSTEM_ERROR          = 0x0302;

        //! The buffer supplied to the function is not large enough.
        public const Int32 EC_BUFFER_TOO_SMALL          = 0x0303;

        //! The file uploaded to cloud is failed
        public const Int32 EC_UPLOAD_FAILED             = 0x0304;

        //! The cloud user ID supplied to the function is invalid.
        public const Int32 EC_INVALID_CLOUD_USER_ID     = 0x0400;

        //! The user ID supplied to the function is invalid
        public const Int32 EC_INVALID_ENGINE_USER_ID    = 0x0401;

        //! The user ID supplied to the function dont login, call EC_Login() first
        public const Int32 EC_CLOUD_USER_ID_DONT_LOGIN  = 0x0402;

        //! The Emotiv Cloud needs to be initialized via EC_Connect()
        public const Int32 EC_EMOTIVCLOUD_UNINITIALIZED = 0x0500;

        //! The Emotiv Engine needs to be initialized via IEE_EngineConnect() or IEE_EngineRemoteConnect() before call EC_Connect() or Emotiv Engine is disconnected
        public const Int32 EC_EMOENGINE_UNINITIALIZED   = 0x0501;

        public const Int32 EC_FILE_EXISTS               = 0x1001;

        //! Reserved return value.
        public const Int32 EC_RESERVED1                 = 0x0900;

        public enum profileFileType
	    {
		    TRAINING,
		    EMOKEY
	    };

        public struct profileVersionInfo
	    {
		    public int version;
		    public IntPtr last_modified;
	    };

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_Connect")]
        public static extern Int32 EC_Connect();

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_ReconnectEngine")]
        public static extern Int32 EC_ReconnectEngine();

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_DisconnectEngine")]
        public static extern Int32 EC_DisconnectEngine();

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_Disconnect")]
        public static extern Int32 EC_Disconnect();

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_Login")]
        public static extern Int32 EC_Login(String email, String password);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_Logout")]
        public static extern Int32 EC_Logout(int userCloudID);
        
        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_GetUserDetail")]
        public static extern Int32 EC_GetUserDetail(ref int userCloudID);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_SaveUserProfile")]
        public static extern Int32 EC_SaveUserProfile(int userCloudID, int engineUserID, String profileName, profileFileType ptype);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_UpdateUserProfile")]
        public static extern Int32 EC_UpdateUserProfile(int userCloudID, int engineUserID, int profileId);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_DeleteUserProfile")]
        public static extern Int32 EC_DeleteUserProfile(int userCloudID, int profileId);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_GetProfileId")]
        public static extern int EC_GetProfileId(int userCloudID, String profileName);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_LoadUserProfile")]
        public static extern Int32 EC_LoadUserProfile(int userCloudID, int engineUserID, int profileId, int version);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_GetAllProfileName")]
        public static extern Int32 EC_GetAllProfileName(int userCloudID);
        
        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_ProfileIDAtIndex")]
        public static extern int EC_ProfileIDAtIndex(int userCloudID, int index);
        
        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_ProfileNameAtIndex")]
        private static extern IntPtr _EC_ProfileNameAtIndex(int userCloudID, int index);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_ProfileLastModifiedAtIndex")]
        private static extern IntPtr _EC_ProfileLastModifiedAtIndex(int userCloudID, int index);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_ProfileTypeAtIndex")]
        public static extern profileFileType EC_ProfileTypeAtIndex(int userCloudID, int index);

        public static string EC_ProfileNameAtIndex(int userCloudId, int index)
        {
            IntPtr ptr = _EC_ProfileNameAtIndex(userCloudId, index);
            return Marshal.PtrToStringAnsi(ptr);
        }
        public static string Plugin_EC_ProfileLastModifiedAtIndex(int userCloudId, int index)
        {
            IntPtr ptr = _EC_ProfileLastModifiedAtIndex(userCloudId, index);
            return Marshal.PtrToStringAnsi(ptr);
        }
    }
}
