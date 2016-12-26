## SDK License

#### SDK LICENSES
* Allow a user / developer can get EEG data and PM from Emotiv Advance Library.
* Allow a application which is linked to Advance Edk library can work with EEG and PM from emotiv headset.
* Information of SDK license key:
    * Date_from and Date_to : period of a license.
    * Scopes : type of license: 2 type:
        * EEG      : Enable EEG data  -  BASIC
        * EEG - PM : Enable EEG data and Performance Metric -  PRIME
    * Seat_count : number of seat
    * Seat_info  : information of a seat

* If user dont have a license. or license expried,  they can use Advance library as a Community library.
* Need connect to the Internet once time to activate a SDK license
* 1 session = 30 min. Counting start from connect to EmoEngine.  Stop when disconnect to EmoEngine

#### HOW TO ACTIVE LICENSE
* Call  IEE_ActivateLicense(licenseID); to activate a license. You just call this API one time for activation.
