import sys
import os
import platform
import time
import ctypes

from array import *
from ctypes import *

if sys.platform.startswith('win32'):
    import msvcrt
elif sys.platform.startswith('linux'):
    import atexit
    from select import select

from ctypes import *

try:
    if sys.platform.startswith('win32'):
        libEDK = cdll.LoadLibrary("../../bin/win32/edk.dll")
    elif sys.platform.startswith('linux'):
        srcDir = os.getcwd()
	if platform.machine().startswith('arm'):
            libPath = srcDir + "/../../bin/armhf/libedk.so"
	else:
            libPath = srcDir + "/../../bin/linux64/libedk.so"
        libEDK = CDLL(libPath)
    else:
        raise Exception('System not supported.')
except Exception as e:
    print 'Error: cannot load EDK lib:', e
    exit()

EDK_OK                     = int("0x0000", 16)
EDK_UNKNOWN_ERROR          = int("0x0001", 16)

EDK_LICENSE_ERROR          = int("0x2010", 16)
EDK_LICENSE_EXPIRED        = int("0x2011", 16)
EDK_LICENSE_NOT_FOUND      = int("0x2012", 16)

EDK_OVER_QUOTA             = int("0x2013", 16)
EDK_OVER_QUOTA_IN_DAY      = int("0x2014", 16)
EDK_OVER_QUOTA_IN_MONTH    = int("0x2015", 16)

EDK_LICENSE_DEVICE_LIMITED = int("0x2019", 16)
EDK_LICENSE_REGISTERED     = int("0x2020", 16)
EDK_NO_ACTIVE_LICENSE      = int("0x2021", 16)

EDK_ACCESS_DENIED          = int("0x2031", 16)


class LicenseInfos_t(Structure):
    _fields_ = [
        ("scopes",         c_int),
        ("date_from",      c_int),
        ("date_to",        c_int),
        ("seat_count",     c_int),
		("quotaDayLimit",  c_int),
		("usedQuotaDay",   c_int),
		("quotaMonthLimit", c_int),
		("usedQuotaMonth", c_int),
		("usedQuota",      c_int),
		("quota",          c_int)]

IEE_LicenseInformation = libEDK.IEE_LicenseInformation
IEE_LicenseInformation.restype  = c_int
IEE_LicenseInformation.argtypes = [POINTER(LicenseInfos_t)]

def activateLicense_err(error_code):
    if error_code == EDK_LICENSE_NOT_FOUND: 
        print "LICENSE NOT FOUND"
    elif error_code == EDK_LICENSE_ERROR: 
        print "LICENSE ERROR"
    elif error_code == EDK_LICENSE_EXPIRED: 
        print "LICENSE EXPIRED"
    elif error_code == EDK_LICENSE_REGISTERED: 
        print "LICENSE REGISTERED"
    elif error_code == EDK_LICENSE_DEVICE_LIMITED: 
        print "DEVICE LICENSE LIMITED"
    elif error_code == EDK_UNKNOWN_ERROR: 
        print "UNKNOWN ERROR"
    elif error_code == EDK_OK:
	    print "License Activated!"
		
def licenseInfos_err(error_code):
    if error_code == EDK_OVER_QUOTA_IN_DAY: 
        print "OVER QUOTA IN DAY"
    elif error_code == EDK_OVER_QUOTA_IN_MONTH: 
        print "OVER QUOTA IN MONTH"
    elif error_code == EDK_LICENSE_EXPIRED: 
        print "LICENSE EXPIRED"
    elif error_code == EDK_OVER_QUOTA: 
        print "OVER QUOTA"
    elif error_code == EDK_ACCESS_DENIED: 
        print "ACCESS DENIED"
    elif error_code == EDK_LICENSE_ERROR: 
        print "LICENSE ERROR"
    elif error_code == EDK_NO_ACTIVE_LICENSE: 
        print "NO ACTIVE LICENSE" 		

def	convertEpochToTime(epoch):
    return time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(epoch))
		
# -------------------------------------------------------------------------
print "==================================================================="
print "The example to activate a license key"
print "==================================================================="

# -------------------------------------------------------------------------

your_license_key = ""

result = 0

#Need only 1 time to call with the internet connection
result = libEDK.IEE_ActivateLicense(your_license_key);

activateLicense_err(result)

if (result != EDK_OK) or (result != EDK_LICENSE_REGISTERED):
    print "Activate License: ", result
    exit()

licenseInfos = LicenseInfos_t(0,0,0,0,0,0,0,0,0,0)

#We can call this API any time to check current License information
result = IEE_LicenseInformation(byref(licenseInfos))
licenseInfos_err(result)

print 'result          =', result
print 'scopes          =', licenseInfos.scopes, "\n"

date_from = convertEpochToTime(licenseInfos.date_from)
print 'date_from       =', date_from

date_to = convertEpochToTime(licenseInfos.date_to)
print 'date_to         =', date_to , "\n"

print 'seat_count      =', licenseInfos.seat_count, "\n"

print 'quotaDayLimit   =', licenseInfos.quotaDayLimit
print 'usedQuotaDay    =', licenseInfos.usedQuotaDay
print 'quotaMonthLimit =', licenseInfos.quotaMonthLimit
print 'usedQuotaMonth  =', licenseInfos.usedQuotaMonth, "\n"

print 'usedQuota       =', licenseInfos.usedQuota
print 'quota           =', licenseInfos.quota


	
