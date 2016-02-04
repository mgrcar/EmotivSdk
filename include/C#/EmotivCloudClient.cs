using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Emotiv
{
    public class EmotivCloudClient
    {
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
        public static extern bool EC_Connect();

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_ReconnectEngine")]
        public static extern bool EC_ReconnectEngine();

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_DisconnectEngine")]
        public static extern bool EC_DisconnectEngine();

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_Disconnect")]
        public static extern void EC_Disconnect();

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_Login")]
        public static extern bool EC_Login(String email, String password);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_Logout")]
        public static extern bool EC_Logout(int userCloudID);
        
        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_GetUserDetail")]
        public static extern bool EC_GetUserDetail(ref int userCloudID);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_SaveUserProfile")]
        public static extern bool EC_SaveUserProfile(int userCloudID, int engineUserID, String profileName, profileFileType ptype);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_UpdateUserProfile")]
        public static extern bool EC_UpdateUserProfile(int userCloudID, int engineUserID, int profileId);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_DeleteUserProfile")]
        public static extern bool EC_DeleteUserProfile(int userCloudID, int profileId);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_GetProfileId")]
        public static extern int EC_GetProfileId(int userCloudID, String profileName);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_LoadUserProfile")]
        public static extern bool EC_LoadUserProfile(int userCloudID, int engineUserID, int profileId, int version);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_GetAllProfileName")]
        public static extern int EC_GetAllProfileName(int userCloudID);
        
        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_ProfileIDAtIndex")]
        public static extern int EC_ProfileIDAtIndex(int userCloudID, int index);
        
        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_ProfileNameAtIndex")]
        public static extern String EC_ProfileNameAtIndex(int userCloudID, int index);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_ProfileLastModifiedAtIndex")]
        public static extern String EC_ProfileLastModifiedAtIndex(int userCloudID, int index);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_ProfileTypeAtIndex")]
        public static extern profileFileType EC_ProfileTypeAtIndex(int userCloudID, int index);
    }
}
