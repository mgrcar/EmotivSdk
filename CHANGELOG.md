## Emotiv SDK Community Edition Changelog

#### Version 3.3.2
* Bug fixes:
  * Fixed unexpected failure in Mental Commands training
  * Fixed connection error for examples on Mac
* Improvement:
  * Support cmake on Ubuntu and Debian/armhf in examples
  * Add BlueAvatar and EmoCube for examples on Mac

#### Version 3.3.1
* New features and improvement:
  * Improved BTLE connection on all platforms
  * Improved noise tolerance for Mental Commands with Insight
  * Added `IEE_GetHeadsetSettings` and `IEE_SetHeadsetSettings` for EPOC+
  * Added `EC_ReconnectEngine` and `EC_DisconnectEngine` in EmotivCloudClient.h
  * Added `FE_LAUGH`, `FE_SMIRK_LEFT`, `FE_SMIRK_RIGHT` into detection list of Facial Expression
  * Added `IS_FacialExpressionIsLookingLeft`, `IS_FacialExpressionIsLookingRight` for EPOC/EPOC+.
  * Removed param `profileName` from `EC_UpdateUserProfile`
  * Added `IEE_GetInsightDeviceState` and `IEE_GetEpocPlusDeviceState` (Mac, iOS, Android)
  * Added multi-app support (Mac, iOS, Android)
  * Supports Ubuntu 15.10 64-bit
  * Supports Debian 8.3 Jessie (armhf) on Raspberry Pi 2
* Bug fixes:
  * Fixed bug when uploading/downloading profile on 64-bit platform
  * Fixed incorrect battery level display with Insight (require new firmware )
  * Fixed bug with Surprise and Frown detections
  * Fixed motion data values retrieved from EPOC+
  * Fixed bug that prevent Insight to work with Extender on Mac
  * Fixed bug that prevent connecting to headset via BTLE on Linux
  * Swapped Gyro Y and Z in motion data with Insight
* Known Issues:
  * EPOC+ motion data may have packet lost on Android
  * EPOC+ only works with dongle version `0x06ff`, not with `0x1fff` or `0x0565`
  * Changing the settings on EPOC+ with firmware `0x610` may fail and eventually turns the headset into a brick (!). Please contact hello@emotiv.com to arrange for a firmware upgrade

#### Version 3.3.0
* Supports saving/loading profile to Emotiv Cloud (see EmotivCloudClient.h)
* New API `IEE_GetAverageBandPowers` to get average band powers for:
  * theta (4-8 Hz)
  * alpha (8-12 Hz)
  * low-beta (12-16 Hz)
  * high-beta (16-25 Hz)
  * gamma (25-45 Hz)
* Supports Ubuntu 14.04 64-bit

#### Version 3.2.0
* Supports EPOC (over Emotiv USB dongle)
* Supports Insight and EPOC+ (over BTLE and Emotiv USB dongle)
