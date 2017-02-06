package com.emotiv.examples.ActivateLicense;

import java.text.SimpleDateFormat;

import com.emotiv.Iedk.Edk;
import com.emotiv.Iedk.Edk.IEE_LicenseType_t;
import com.emotiv.Iedk.Edk.LicenseInfos_t;
import com.emotiv.Iedk.EdkErrorCode;
import com.sun.jna.Pointer;

public class ActivateLicense {

	public static void main(String[] args) {
		
		System.out.println("Activate SDK License" );
		
		// Need only 1 time to call with the internet connection
		int result = Edk.INSTANCE.IEE_ActivateLicense(""); //input license here , activate for first time
		
		if(result == EdkErrorCode.EDK_LICENSE_NOT_FOUND.ToInt())
	    {
	    	System.out.println("EDK_LICENSE_NOT_FOUND" );
	        System.out.println();
	    }
	    else if (result == EdkErrorCode.EDK_LICENSE_ERROR.ToInt())
	    {
	    	System.out.println("EDK_LICENSE_ERROR" );
	        System.out.println();
	    }
	    else if (result == EdkErrorCode.EDK_LICENSE_EXPIRED.ToInt())
	    {
	    	System.out.println("EDK_LICENSE_EXPIRED" );
	        System.out.println();
	    }
	    else if (result == EdkErrorCode.EDK_LICENSE_REGISTERED.ToInt())
	    {
	    	System.out.println("EDK_LICENSE_REGISTERED" );
	        System.out.println();
	    }
	    else if (result == EdkErrorCode.EDK_LICENSE_DEVICE_LIMITED.ToInt())
	    {
	    	System.out.println("EDK_LICENSE_DEVICE_LIMITED" );
	        System.out.println();
	    }
	    else if (result == EdkErrorCode.EDK_UNKNOWN_ERROR.ToInt())
	    {
	    	System.out.println("EDK_UNKNOWN_ERROR" );
	        System.out.println();
	    }
	    else
	    {
	    	System.out.println();
	    }
		
		if (!(result == EdkErrorCode.EDK_OK.ToInt() || result == EdkErrorCode.EDK_LICENSE_REGISTERED.ToInt()))
	        return ;
		
		Edk.LicenseInfos_t.ByReference licenseInfos = new Edk.LicenseInfos_t.ByReference();
		
		//Get License Information
	    result = Edk.INSTANCE.IEE_LicenseInformation(licenseInfos);
	    
	    if(result == EdkErrorCode.EDK_OVER_QUOTA_IN_DAY.ToInt())
	    {
	    	System.out.println("EDK_OVER_QUOTA_IN_DAY" );
	        System.out.println();
	    }
	    else if (result == EdkErrorCode.EDK_OVER_QUOTA_IN_MONTH.ToInt())
	    {
	    	System.out.println("EDK_OVER_QUOTA_IN_MONTH" );
	        System.out.println();
	    }
	    else if (result == EdkErrorCode.EDK_LICENSE_EXPIRED.ToInt())
	    {
	    	System.out.println("EDK_LICENSE_EXPIRED" );
	        System.out.println();
	    }
	    else if (result == EdkErrorCode.EDK_OVER_QUOTA.ToInt())
	    {
	    	System.out.println("EDK_OVER_QUOTA" );
	        System.out.println();
	    }
	    else if (result == EdkErrorCode.EDK_ACCESS_DENIED.ToInt())
	    {
	    	System.out.println("EDK_ACCESS_DENIED" );
	        System.out.println();
	    }
	    else if (result == EdkErrorCode.EDK_LICENSE_ERROR.ToInt())
	    {
	    	System.out.println("EDK_LICENSE_ERROR" );
	        System.out.println();
	    }
	    else if (result == EdkErrorCode.EDK_NO_ACTIVE_LICENSE.ToInt())
	    {
	    	System.out.println("EDK_NO_ACTIVE_LICENSE" );
	        System.out.println();
	    }
	    else
	    {
	    	System.out.println();
	    }
	    //Convert to Epoc time to Date time UTC
	    String date_from = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new java.util.Date(licenseInfos.date_from*1000L)); //multiple 1000 because convert to milisecond
	    String date_to = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new java.util.Date(licenseInfos.date_to*1000L));

	    System.out.println("Date From: " + date_from );
	    System.out.println();
	    System.out.println("Date To: " + date_to );
	    System.out.println();

	    System.out.println("Seat number: " + licenseInfos.seat_count );
	    System.out.println();

	    System.out.println("Total Quota: " + licenseInfos.quota );
	    System.out.println("Total quota used    : " + licenseInfos.usedQuota );
	    System.out.println();

	    System.out.println("Quota limit in day  : " + licenseInfos.quotaDayLimit );
	    System.out.println("Quota used in day   : " + licenseInfos.usedQuotaDay );
	    System.out.println();

	    System.out.println("Quota limit in month: " + licenseInfos.quotaMonthLimit );
	    System.out.println("Quota used in month : " + licenseInfos.usedQuotaMonth );
	    System.out.println();

	    if(licenseInfos.scopes == 1)
	    {
	    	System.out.println("License type : " + "EEG" );
	        System.out.println();
	    }
	    else if (licenseInfos.scopes == 2)
	    {
	    	 System.out.println("License type : " + "PM" );
		        System.out.println();
	    }
	    else if (licenseInfos.scopes == 3)
	    {
	    	System.out.println("License type : " + "EEG + PM" );
	        System.out.println();
	    }
	    else
	    {
	    	System.out.println("License type : " + "No type" );
	        System.out.println();
	    }
	    
	}

}
