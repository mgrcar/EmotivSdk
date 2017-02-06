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
        public static extern int EC_GetProfileId(int userCloudID, String profileName, ref int profileID);

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


        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_DownloadProfileFile")]
        public static extern int EC_DownloadProfileFile(int userCloudID, int profileId, String destPath, int version);
        
    
        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_UploadProfileFile")]
        public static extern int EC_UploadProfileFile(int userCloudID, String profileName, String filePath, profileFileType ptype, bool overwrite_if_exists);
    
        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "EC_GetLastestProfileVersions")]
        public static extern int EC_GetLastestProfileVersions(int userCloudID, int profileID, ref int nVersion);
    
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
