package com.emotiv.examples.MentalCommand_Profile;

import com.emotiv.Iedk.*;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FilenameFilter;
import java.util.ArrayList;

import javax.swing.JFileChooser;

import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.NativeLongByReference;

class Profile {
	Pointer hProfile;
	
	
	public Profile()
	{
		hProfile = Edk.INSTANCE.IEE_ProfileEventCreate();
		Edk.INSTANCE.IEE_GetBaseProfile(hProfile);
		
	}
	
	protected void finalize ()
	{
		if (hProfile != null)
		{
			Edk.INSTANCE.IEE_EmoEngineEventFree(hProfile);
		}
	}
	public byte[] getBytes()
	{
		IntByReference size = new IntByReference(0);
		Edk.INSTANCE.IEE_GetUserProfileSize(hProfile, size);
		byte [] profileBytes = new byte[size.getValue()];
		Edk.INSTANCE.IEE_GetUserProfileBytes(hProfile, profileBytes, size.getValue());
		return profileBytes;
	}
	public Pointer getProfile()
	{
		return hProfile;
	}
}
///////////////////////////////////////////////////////
class UserProfile
{
	Profile profile;
	String profileName;
	public UserProfile()
    {
        profile = new Profile();
        profileName = "";
    }
}
//////////////////////////////////////////////////////
public class EmoProfileManagement {
	public static int currentIndex = 0;
	public static ArrayList<UserProfile> userProfiles = new ArrayList<UserProfile>();
	
	/// <summary>
	/// Function to save byte array to a file
	/// </summary>
	/// <param name="_FileName">File name to save byte array</param>
	/// <param name="_ByteArray">Byte array to save to external file</param>
	/// <returns>Return true if byte array save successfully, if not return false</returns>
	public static Boolean ByteArrayToFile(String fName,byte[]arr)
	{
		try
		{
			FileOutputStream mFile = new FileOutputStream(new File(fName));
			mFile.write(arr);
			mFile.close();
			return true;
		}
		catch(Exception e)
		{
			e.printStackTrace();
			return false;
		}
		
	}
	
	/// <summary>
    /// Save all profiles in arraylist to file
    /// </summary>
    public static void SaveProfilesToFile()
    {
    	System.out.println("Save Profile to File");
    	String currentPath = System.getProperty("user.dir");
    	File f = new File(currentPath + "\\EmotivUserProfile");
    	
    	if(!f.exists())
    	{
    		f.mkdir();
    	}
    	String path = currentPath + "\\EmotivUserProfile";
    	
    	for(int i = 0;i< userProfiles.size();i++)
    	{
    		UserProfile tmp =userProfiles.get(i);
    		ByteArrayToFile(path + "\\"+tmp.profileName+".emo",tmp.profile.getBytes());
    	}
    	
    }
    
    /// <summary>
    /// Get Profile List
    /// </summary>
    /// <returns></returns>
    public static String[] GetProfileList()
    {
    	String currentPath = System.getProperty("user.dir");
    	File f = new File(currentPath + "\\EmotivUserProfile");
    	if(!f.exists())
    	{
    		f.mkdir();
    	}
    	String mPath = currentPath + "\\EmotivUserProfile";
    	
    	File file = new File(mPath);
    	
    	FilenameFilter filter = new FilenameFilter() {
			public boolean accept(File dir, String name) {
				String lowercaseName = name.toLowerCase();
				if (lowercaseName.endsWith(".emo")) {
					return true;
				} else {
					return false;
				}
			}
		};

    	File [] listFiles = file.listFiles(filter);
    	String[] re = new String[listFiles.length];
    	for(int i = 0;i<listFiles.length;i++)
    	{
    		try
    		{
    			re[i]=listFiles[i].getName();
    		}
    		catch(Exception e)
    		{}
    	}
    	return re;
    }
    
    /// <summary>
    /// Load all profile file in EmotivUserProfile folder into arraylist
    /// </summary>
    public static void LoadProfilesFromFile()
    {
    	String currentPath = System.getProperty("user.dir");
    	File f = new File(currentPath + "\\EmotivUserProfile");
    	if(!f.exists())
    	{
    		f.mkdir();
    	}
    	String mPath = currentPath + "\\EmotivUserProfile";
    	File file = new File(mPath);
    	
    	FilenameFilter filter = new FilenameFilter() {
			public boolean accept(File dir, String name) {
				String lowercaseName = name.toLowerCase();
				if (lowercaseName.endsWith(".emo")) {
					return true;
				} else {
					return false;
				}
			}
		};

    	File [] listFiles = file.listFiles(filter);
    	
    	for(int i=0;i<listFiles.length;i++)
    	{
    		try
    		{
    			FileInputStream is = new FileInputStream(listFiles[i]);
    			byte[] buffer = new byte[(int) listFiles[i].length()];
    			is.read(buffer, 0, (int) listFiles[i].length());
    			is.close();
    			SetUserProfileTmp(0,buffer);
    			UserProfile tmp = new UserProfile();
    			String name = listFiles[i].getName();
    			name = name.substring(0,name.length()-4);
    			tmp.profileName = name;
    			Profile tmpProfile = new Profile();
    			tmp.profile = GetUserProfile();
    			userProfiles.add(tmp);
    		}
    		catch(Exception e)
    		{
    			e.printStackTrace();
    		}
    	}
    	System.out.println("Length khoa:"+userProfiles.size());
    	for(int k = 0;k<userProfiles.size();k++)
    	{
    		System.out.println("name:"+userProfiles.get(k).profileName);
    	}
    }
    public static Profile GetUserProfile()
    {
    	Profile tmpProfile = new Profile();
    	Edk.INSTANCE.IEE_GetUserProfile(0,tmpProfile.getProfile());
    	return tmpProfile;
    }
    
    /// <summary>
    /// Find index of profile in arraylist 
    /// </summary>
    /// <param name="prName">Profile Name</param>
    /// <returns></returns>
    static int  FindProfileName(String prName)
    {
        int i = 0;
        Boolean IsFound = false;
        while ((!IsFound)&&(i<userProfiles.size()))
        {
            UserProfile tmp = userProfiles.get(i);
            if (prName == tmp.profileName)
            {
                IsFound = true;            
                break;
            }else i++;
        }
		return i;
    }
    
    /// <summary>
    /// Set user profile , remember to save current profile before or lose it
    /// </summary>
    /// <param name="prName">Profile Name</param>
    /// <returns>Return false if no profile with this name</returns>
    public static Boolean SetUserProfile(String prName)
    {
        int i = FindProfileName(prName)-1;
     
        if (i != userProfiles.size())
        {
            UserProfile tmp = (UserProfile)userProfiles.get(i);
            SetUserProfileTmp(0,tmp.profile);
            currentIndex = i;
            return true;
        }
        else 
        {
            return false; 
        }// have no profile with this name
    }
    
    private static void SetUserProfileTmp(int userId, Profile profile)
	{
		if (profile == null)
		{
			try
			{
				throw new Exception();
			}
			catch(Exception e){}
		}
		byte[] profileBytes = profile.getBytes();
		Edk.INSTANCE.IEE_SetUserProfile(userId, profileBytes, (int)profileBytes.length);
		
	}
    private static void SetUserProfileTmp(int userId, byte[] profileBytes)
	{
		if (profileBytes == null)
		{
			try
			{
				throw new Exception();
			}
			catch(Exception e){}
		}
		
		Edk.INSTANCE.IEE_SetUserProfile(userId, profileBytes, (int)profileBytes.length);
		
	}
    /// <summary>
    /// Add new profile into arraylist , set this profile to current user
    /// </summary>
    /// <param name="prName"></param>
    /// <returns></returns>
    public static Boolean AddNewProfile(String prName)
    {
        if (FindProfileName(prName) == userProfiles.size() )
        {
            if(userProfiles.size() > 0 ) SaveCurrentProfile();
            UserProfile tmp = new UserProfile();
            tmp.profileName = prName;
            currentIndex = 0;
            userProfiles.add(currentIndex, tmp);
            SetUserProfileTmp(0,tmp.profile);
            SaveCurrentProfile();
            return true;
        }
        else
        {
            return false;//Already have this profile name
        }
    }
    
    /// <summary>
    /// Back up current profile of current user into arraylist
    /// </summary>
    public static void SaveCurrentProfile()
    {
        UserProfile tmp = (UserProfile)userProfiles.get(currentIndex);
        tmp.profile = GetUserProfile();
        userProfiles.remove(currentIndex);  
        userProfiles.add(currentIndex, tmp);
    }
    
    /// <summary>
    /// Delete a profile in arraylist
    /// </summary>
    /// <param name="prName"></param>
    /// <returns></returns>
    public static Boolean DeleteProfile(String prName)
    {
        int i = FindProfileName(prName);
        if (i != userProfiles.size() + 1)
        {
            userProfiles.remove(i);
            if ( i== currentIndex)
            {
                if (currentIndex > 0)
                {
                    currentIndex--;
                }
            }
            return true;
        }
        else return false;// have no profile with this name
    }
    /// <summary>
    /// Get a profile name in arraylist
    /// </summary>
    /// <param name="profileIndex">Index of a profile</param>
    /// <returns></returns>
    public static String GetProfileName(int profileIndex)
    {
        if (profileIndex < userProfiles.size())
        {
            UserProfile tmp = (UserProfile)userProfiles.get(profileIndex);
            return tmp.profileName;
        }
        else return null;        
    }
    /// <summary>
    /// Get number of profile in arraylist
    /// </summary>
    /// <returns>Arraylist size</returns>
    public static int GetProfilesArraySize()
    {
        return userProfiles.size();
    }
    public static void ClearProfileList()
    {
        userProfiles.clear();
        currentIndex = 0;
    }
    
    /// <summary>
    /// Check MentalCommand Actions That Enabled In The Profile
    /// </summary>
    /// <returns></returns>
	public static String CheckCurrentProfile()
    {
        NativeLongByReference temp = new NativeLongByReference();
        Edk.INSTANCE.IEE_MentalCommandGetActiveActions(0,temp);        
        String test = temp.getValue().toString();
        return test;        
    }
}

