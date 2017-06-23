/*
Copyright (c) 2013, Laird
Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

// sdc_sdk.h  Header file for Summit Data SDK for the CF10G
#ifndef _SDC_SDK_H_
#define _SDC_SDK_H_ 1

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__
#include <stdint.h>
	#include "sdc_events.h"
    #define _vsnprintf  vsnprintf
    #define _snprintf   snprintf
    #define _strdup     strdup
    #define _ltoa       ltoa
    #define _strnicmp   strncasecmp

    #ifndef FALSE
        #define FALSE (0)
        #define TRUE (!(FALSE))
    #endif
#endif

#define CONFIG_NAME_SZ  33
#define SSID_SZ         33
#define CLIENT_NAME_SZ  17
#define USER_NAME_SZ    65
#define USER_PWD_SZ     65
#define PSK_SZ          65
#define MAX_CFGS        20
#define NUM_WEP_KEYS    4
#define PDELAY_LOW      0
#define PDELAY_HIGH     7200000
#define PTIME_LOW       0
#define PTIME_HIGH      30000
#define FRAG_LOW        256
#define FRAG_HIGH       2346
#define RTS_LOW         0
#define RTS_HIGH        2347
#define AUTH_LOW        3
#define AUTH_HIGH       60
#define SCANDFSTIME_LOW 20 //ms
#define SCANDFSTIME_HIGH 500 //ms
#define MAX_CERT_PATH   65
#define CRED_CA_POS     72 //in SDCConfig userName.buffer
#define CRED_UCA_POS    72 //in SDCConfig userPwd.buffer
#define CRED_PFILE_POS  34 //in SDCConfig userPwd.buffer
#define CRED_CERT_SZ    48 //for CRED_CA and CRED_UCA
#define CRED_PFILE_SZ   32
#define USER_CERT_PW_SZ 64 //in SDCCondif userPwd.buffer
#define LRS_MAX_CHAN	  32

typedef enum _SDCERR {
	SDCERR_SUCCESS = 0,
	SDCERR_FAIL,
	SDCERR_INVALID_NAME,
	SDCERR_INVALID_CONFIG,
	SDCERR_INVALID_DELETE,
	SDCERR_POWERCYCLE_REQUIRED,
	SDCERR_INVALID_PARAMETER,
	SDCERR_INVALID_EAP_TYPE,
	SDCERR_INVALID_WEP_TYPE,
	SDCERR_INVALID_FILE,
	SDCERR_INSUFFICIENT_MEMORY,
	SDCERR_NOT_IMPLEMENTED,
	SDCERR_NO_HARDWARE,
	SDCERR_INVALID_VALUE
} SDCERR;

typedef enum _AUTH {
	AUTH_OPEN = 0,
	AUTH_SHARED,
	AUTH_NETWORK_EAP,
} AUTH;

typedef enum _EAPTYPE {
	EAP_NONE = 0,
	EAP_LEAP,
	EAP_EAPFAST,
	EAP_PEAPMSCHAP,
	EAP_PEAPGTC,
	EAP_EAPTLS,
	EAP_EAPTTLS,
	EAP_PEAPTLS,
	EAP_WAPI_CERT
} EAPTYPE;

typedef enum _POWERSAVE {
	POWERSAVE_OFF = 0,
	POWERSAVE_MAX,
	POWERSAVE_FAST,
} POWERSAVE;

typedef enum _WEPTYPE {
	WEP_OFF = 0,
	WEP_ON,
	WEP_AUTO,
	WPA_PSK,
	WPA_TKIP,
	WPA2_PSK,
	WPA2_AES,
	CCKM_TKIP,
	WEP_CKIP,
	WEP_AUTO_CKIP,
	CCKM_AES,
	WPA_PSK_AES,
	WPA_AES,
	WPA2_PSK_TKIP,
	WPA2_TKIP,
	WAPI_PSK,
	WAPI_CERT
} WEPTYPE;

typedef enum _RADIOMODE {
	RADIOMODE_B_ONLY = 0,
	RADIOMODE_BG,
	RADIOMODE_G_ONLY,
	RADIOMODE_BG_LRS,
	RADIOMODE_A_ONLY,
	RADIOMODE_ABG,
	RADIOMODE_BGA,
	RADIOMODE_ADHOC,
	RADIOMODE_GN,
	RADIOMODE_AN,
	RADIOMODE_ABGN,
	RADIOMODE_BGAN,
	RADIOMODE_BGN
} RADIOMODE;

typedef enum _TXPOWER {
	TXPOWER_MAX=0,
	TXPOWER_1=1,
	TXPOWER_5=5,
	TXPOWER_10=10,
	TXPOWER_20=20,
	TXPOWER_30=30,
	TXPOWER_50=50,
} TXPOWER;

typedef enum _BITRATE {
	BITRATE_AUTO  = 0,
	BITRATE_1     = 2,
	BITRATE_2     = 4,
	BITRATE_5_5   = 11,
	BITRATE_6     = 12,
	BITRATE_9     = 18,
	BITRATE_11    = 22,
	BITRATE_12    = 24,
	BITRATE_18    = 36,
	BITRATE_24    = 48,
	BITRATE_36    = 72,
	BITRATE_48    = 96,
	BITRATE_54    = 108,
	BITRATE_6_5   = 13,
	BITRATE_13    = 26,
	BITRATE_19_5  = 39,
	BITRATE_26    = 52,
	BITRATE_39    = 78,
	BITRATE_52    = 104,
	BITRATE_58_5  = 117,
	BITRATE_65    = 130,
	BITRATE_72    = 144,
    BITRATE_7_2   = 14,
    BITRATE_14_4  = 28,
    BITRATE_21_7  = 42,
    BITRATE_28_9  = 56,
    BITRATE_43_3  = 86,
    BITRATE_57_8  = 114
} BITRATE;

typedef enum _PREAMBLE {
	PRE_AUTO = 0,
	PRE_SHORT,
	PRE_LONG,
} PREAMBLE;

typedef enum _GSHORTSLOT {
	GSHORT_AUTO = 0,
	GSHORT_OFF,
	GSHORT_ON,
} GSHORTSLOT;

typedef enum _BT_COEXIST {
	BT_OFF  = 0,
	BT_ON,
} BT_COEXIST;

typedef enum _REGDOMAIN {
	REG_FCC   = 0,	// North America, South America, Central America, Australia, New Zealand, various parts of Asia
	REG_ETSI  = 1,	// Europe, Middle East, Africa, various parts of Asia
	REG_TELEC = 2,	// Japan
	REG_WW    = 3,	// World Wide
	REG_KCC   = 4,	// Korea
	REG_CA    = 5,	// Canada
	REG_FR    = 6,	// France
	REG_GB    = 7,	// United Kingdom
	REG_AU    = 8,	// Australia
	REG_NZ    = 9,	// New Zealand
	REG_CN    = 10,	// China
} REG_DOMAIN;

typedef enum _PING_PAYLOAD {
	PP_32    = 32,
	PP_64    = 64,
	PP_128   = 128,
	PP_256   = 256,
	PP_512   = 512,
	PP_1024  = 1024
} PING_PAYLOAD;

typedef enum _RX_DIV {
	RXDIV_MAIN = 0,
	RXDIV_AUX,
	RXDIV_START_AUX,
	RXDIV_START_MAIN,
} RX_DIV;

typedef enum _TX_DIV {
	TXDIV_MAIN = 0,
	TXDIV_AUX,
	TXDIV_ON=3,
} TX_DIV;

typedef enum _ROAM_TRIG {
	RTRIG_50 = 50,
	RTRIG_55 = 55,
	RTRIG_60 = 60,
	RTRIG_65 = 65,
	RTRIG_70 = 70,
	RTRIG_75 = 75,
	RTRIG_80 = 80,
	RTRIG_85 = 85,
	RTRIG_90 = 90,
} ROAM_TRIG;

typedef enum _ROAM_DELTA {
	RDELTA_5  = 5,
	RDELTA_10 = 10,
	RDELTA_15 = 15,
	RDELTA_20 = 20,
	RDELTA_25 = 25,
	RDELTA_30 = 30,
	RDELTA_35 = 35,
} ROAM_DELTA;

typedef enum _ROAM_PERIOD {
	RPERIOD_5	  = 5,
	RPERIOD_10  = 10,
	RPERIOD_15  = 15,
	RPERIOD_20  = 20,
	RPERIOD_25  = 25,
	RPERIOD_30  = 30,
	RPERIOD_35  = 35,
	RPERIOD_40  = 40,
	RPERIOD_45  = 45,
	RPERIOD_50  = 50,
	RPERIOD_55  = 55,
	RPERIOD_60  = 60,
} ROAM_PERIOD;

#ifndef _IGNORE_CCX_FEATURES_
typedef enum _CCX_FEATURES {
	CCX_OPTIMIZED = 0,
	CCX_FULL = 1,
	CCX_OFF = 2,
} CCX_FEATURES;
#endif

typedef enum _WEPLEN {
	WEPLEN_NOT_SET = 0,
	WEPLEN_40BIT,
	WEPLEN_128BIT,
} WEPLEN;

typedef enum _FCCTEST {
	FCCTEST_OFF  = 0,
	FCCTEST_TX   = 1,
	FCCTEST_RX   = 3,
	FCCTEST_FREQ = 2,
} FCC_TEST;

typedef enum _CARDSTATE {
	CARDSTATE_NOT_INSERTED = 0,
	CARDSTATE_NOT_ASSOCIATED,
	CARDSTATE_ASSOCIATED,
	CARDSTATE_AUTHENTICATED,
	CARDSTATE_FCCTEST,
	CARDSTATE_NOT_SDC,
	CARDSTATE_DISABLED,
	CARDSTATE_ERROR
} CARDSTATE;

#define RADIOTYPE_BCM_OFFSET 0x00 //CF,PE,SD10 Radios
#define RADIOTYPE_AR_OFFSET 0x100 //SD30 Radio
typedef enum _RADIOTYPE {
	RADIOTYPE_BG        = 0,
	RADIOTYPE_ABG       = 1,
	RADIOTYPE_NBG       = 2,
	RADIOTYPE_NABG      = 3,
	RADIOTYPE_AR_BG     = (RADIOTYPE_AR_OFFSET + RADIOTYPE_BG),
	RADIOTYPE_AR_ABG    = (RADIOTYPE_AR_OFFSET + RADIOTYPE_ABG),
	RADIOTYPE_AR_NBG    = (RADIOTYPE_AR_OFFSET + RADIOTYPE_NBG),
	RADIOTYPE_AR_NABG   = (RADIOTYPE_AR_OFFSET + RADIOTYPE_NABG),
	RADIOTYPE_NOT_SDC   = 0x64,
	RADIOTYPE_NOT_SDC_1 = 0x65, //reserved
} RADIOTYPE;

typedef enum _RADIOCHIPSET {
	RADIOCHIPSET_NONE     = 0,
	RADIOCHIPSET_SDC10    = 1, //BCM4318
	RADIOCHIPSET_SDC15    = 2, //BCM4322,
	RADIOCHIPSET_SDC30    = 3, //AR6002,
	RADIOCHIPSET_SDC40L   = 4, //BCM4319,
	RADIOCHIPSET_SDC40NBT = 5, //BCM4329,
	RADIOCHIPSET_SDC45    = 6, //AR6003,
} RADIOCHIPSET;


typedef enum _CERTLOCATION {
	CERT_NONE = 0,	// don't validate the server
	CERT_FILE,		// specify the filename for caCert
	CERT_FULL_STORE,	// use the entire MS-store
	CERT_IN_STORE		// use one specific cert from the MS-store, specify the cert's hash
} CERTLOCATION;

typedef enum _INTERFERENCE {
	INTER_NONE = 0,	// OFF
	INTER_NONWLAN,    // reduces CCA Tx threshold
	INTER_WLAN,	    // reduces interchannel noise
	INTER_AUTO		// automatic
} INTERFERENCE;

typedef enum _TTLS_INNER_METHOD {
	TTLS_AUTO = 0,	// uses any available EAP method
	TTLS_MSCHAPV2,
	TTLS_MSCHAP,
	TTLS_PAP,
	TTLS_CHAP,
	TTLS_EAP_MSCHAPV2,
	//TTLS_EAP_MD5,
	//TTLS_EAP_GTC,
	//TTLS_EAP_OTP,
	//TTLS_EAP_TLS
} TTLS_INNER_METHOD;

typedef enum _DFS_CHANNELS {
	DFS_OFF = 0,
	DFS_FULL,
	DFS_OPTIMIZED
} DFS_CHANNELS;

typedef enum _UAPSD {   //Bitmask enums for UAPSD
	UAPSD_AC_VO = 1,    //Access Category Voice
	UAPSD_AC_VI = 2,    //Access Category Video
	UAPSD_AC_BK = 4,    //Access Category Background
	UAPSD_AC_BE = 8     //Access Category Best Effort
} UPASD;

// or this in the length to set the XMIT key
#define XMITBIT 0x100

#pragma pack(1)

typedef struct _monitorPacket {
	unsigned long length; // add this to get to the next packet (this is the last thing set)
	unsigned long dataOffset; // add this to the frame below to get at the data of the packet
	struct _monitorPacket *previous;
	unsigned long packetLength;  //actual packet size (with header)
	unsigned long channel;
	unsigned long speed; // in 500kPs increments (11 Mbs is 22)
	unsigned long RSSI;
	unsigned long macTime;
	unsigned long CRCflag;
	unsigned long frame;
} monitorPacket;

typedef struct _monitorHeader {
	unsigned long bufSize;  //(total, includes this field, minimum 16K)
	monitorPacket  *current;
	unsigned long halted;	 //	give sniffer the ability to 'pause'
	unsigned long numPackets;
	unsigned long data;    // this is were the monitor packets get stored and wrapped around to
} monitorHeader;

typedef struct _WEPKEY {
	unsigned long  length;  //40 or 128 or 0 for not set
	unsigned char data[16]; //enough to store 128 bits
} WEPKey;

#define CRYPT_BUFFER_SIZE 120
typedef struct _CRYPT {
	unsigned long size;
	char  buffer[CRYPT_BUFFER_SIZE];
	unsigned long offset;
} CRYPT;

typedef struct _SDCConfig {
	char        configName[CONFIG_NAME_SZ];
	char        SSID[SSID_SZ];
	char        clientName[CLIENT_NAME_SZ];
	int         txPower;
	AUTH        authType;
	EAPTYPE     eapType;
	POWERSAVE   powerSave;
	WEPTYPE     wepType;
	BITRATE     bitRate;
	RADIOMODE   radioMode;
	CRYPT       userName;
	CRYPT       userPwd;
	CRYPT       PSK;
	CRYPT       WEPKeys;
} SDCConfig;

typedef struct _SDCGlobalConfig {
	unsigned long fragThreshold;
	unsigned long RTSThreshold;
	RX_DIV        RxDiversity;
	TX_DIV        TxDiversity;
	ROAM_TRIG     roamTrigger;
	ROAM_DELTA    roamDelta;
	ROAM_PERIOD   roamPeriod;
	PREAMBLE      preamble;	//not used
	GSHORTSLOT    g_shortslot;//not used
	BT_COEXIST    BTcoexist;
	PING_PAYLOAD  pingPayload;
	unsigned long pingTimeout;   //in ms
	unsigned long pingDelay;     //in ms
	unsigned long radioState;  // enabled-1, disabled-0
	unsigned long displayPasswords;  //0 no, 1 yes
	unsigned long adminOverride;
	unsigned long txMax;	  // BG radio - to account for high gain antennae--(max power out desired/19dbm) * 100
	FCC_TEST      FCCtest;      // 0 - off, 1 tx 2 frequency 3 rx
	unsigned long testChannel;  // 1-14
	BITRATE       testRate;	  //
	TXPOWER       testPower;	  //	0-100 %
	unsigned long regDomain;	  // status purposes only
	unsigned long ledUsed;      //for minimodule GPIO 0, need resistor off board to make it work
	unsigned long txTestTimeout;//in seconds--60000 (decimal) means no timeout--
	unsigned long WMEenabled;   //enable 1
	unsigned long CCXfeatures;  // enable 1 or CCX RM and AP control of TX power
	char          certPath[MAX_CERT_PATH]; // to change the path of the certificate store
	CRYPT         adminPassword;
	unsigned long bLRS;         // bit 0 = chan 1, bit 1 =chan 2, etc.
	unsigned long avgWindow;    // 2-8 (rssi moving average window)
	unsigned long probeDelay;   // 2-120 (delay before sending out probes when AP's aren't located--not config for WZC)
	unsigned long polledIRQ;    // for units that can't share IRQs nicely
	unsigned long keepAlive;	  // in cam mode how often a null packet gets sent in seconds (0 means never, 9 by default)
	unsigned long trayIcon;     // enable 1
	unsigned long aggScanTimer; // enable 1
	unsigned long authTimeout;  // in seconds, for EAP credentials, default is 8 ms
	unsigned long autoProfile;  // not implemented
	unsigned long PMKcaching;   //0 standard, 1 opportunistic key caching enabled
	unsigned long defAdhocChannel; // when no beacons found this channel is used
	unsigned long silentRunning;	 //	enables silent running mode (no active scans unless connected)
	unsigned long scanDFSTime;	//20-500 ms, default of 160 ms. Maximum time spent scanning each DFS channel during a scan.
	unsigned long suppInfo;		
#define SUPPINFO_FIPS (1U<<0)                     //bit 0 is Summit FIPS enable 
                                                  //bit 1 is reserved
#define SUPPINFO_TLS_TIME_CHECK (1U<<2)           //bit 2 is CA cert date-check 
#define SUPPINFO_WPA1_ORIGINAL_OPERATION (1U<< 3) //bit 3 is pre 2014 WPA1 operation 
	unsigned long uAPSD;          // UAPSD bitmasks
	unsigned long txMaxA;	      // A radio - to account for high gain antennae-- %
	unsigned long adminFiles;   // allows import/export of settings to file
	unsigned long DFSchannels;  //enable 1, optimized 2
	unsigned long interferenceMode;  //0 off, 1 nonWLAN, 2 WLAN, 3 auto
	unsigned long authServerType; //0 ACS (type 1), 1 SBR (type 2)
	unsigned long TTLSInnerMethod;//0 auto-EAP
	unsigned long aLRS; //bit 0 = chan 36, bit 1 =chan 40, etc
	unsigned long Reserved1[2];   // future expansion of the global config......
} SDCGlobalConfig;


typedef struct _SDC3rdPartyConfig {
	char        clientName[CLIENT_NAME_SZ];
	POWERSAVE   powerSave;
	int         txPower;
	BITRATE     bitRate;
	RADIOMODE   radioMode;
} SDC3rdPartyConfig;

typedef struct _CF10G_STATUS {
	CARDSTATE     cardState;
	char          configName[CONFIG_NAME_SZ];
	unsigned char client_MAC[6];
	unsigned char client_IP[4];
	char          clientName[CLIENT_NAME_SZ];
	unsigned char AP_MAC[6];
	unsigned char AP_IP[4];
	char          APName[CLIENT_NAME_SZ];
	EAPTYPE       eapType;
	unsigned long channel;
	int           rssi;
	BITRATE       bitRate;
	int           txPower;
	unsigned long driverVersion;
	RADIOTYPE     radioType;
	unsigned long DTIM;
	unsigned long beaconPeriod;
	unsigned long beaconsReceived;
} CF10G_STATUS;

typedef CF10G_STATUS SDC_STATUS, *PSDC_STATUS;

typedef struct _CONFIG_FILE_INFO {
	unsigned long   numConfigs; //no more than MAX_CFGS
	unsigned char   globalConfigPresent;
	unsigned char   thirdPartyConfigPresent;
	unsigned long   sdkVersion;
} CONFIG_FILE_INFO;

typedef struct _SDC_ALL {
	unsigned long		numConfigs; //no more than MAX_CFGS
	SDCConfig			*configs;
	SDC3rdPartyConfig	*configThirdParty;
	SDCGlobalConfig		*configGlobal;
} SDC_ALL;

typedef struct _lrs_channels {
	unsigned long chan[LRS_MAX_CHAN];
} LRD_WF_LRSChannels;

#pragma pack()

#ifdef __linux__
/* see lrd_sdk_pil.h for details */
typedef struct _pil_info {
	uint32_t api_version;
	char * company_name;
	char * version_string;  //optional
	char * serial_number;  // optional
	char * product_id;   // optional
	void * data;  // optional - customer use
} LRD_WF_PilInfo;

typedef struct _DHCP_LEASE {
	char interface[20];
	char address[20];
	char subnet_mask[20];
	char routers[100];
	long lease_time;
	int message_type;
	char dns_servers[100];
	char dhcp_server[20];
	char domain_name[200];
	char renew[30];
	char rebind[30];
	char expire[30];
} DHCP_LEASE;
#endif //__linux__

#ifndef NDIS_MINIPORT_DRIVER

// iphlpapi.lib must be linked into any program using the SDK from user mode
// ws2.lib must also be linked into any program using the SDK from user mode

/*---------------------------------------------------------
SDCERR GetCurrentConfig(unsigned long *num, char *name)

Synopsis:  returns the number and name of the active config

input arguments:  unsigned long *num  If NULL, item is skipped
                              0 = ThirdPartyConfig is active
							  >0 number of active config

                  char *name  If NULL item is skipped
				              'ThirdPartyConfig' if the
							    ThirdPartyConfig is active
							   otherwise the name of the
							   active profile is stored
							   here--It is assumed that
							   at least CONFIG_NAME_SZ
							   bytes of storage are
							   allocated and passed in
							   with this argument

return values:  SDCERR_SUCCESS if successful
				SDCERR_FAIL  if unsuccessful
---------------------------------------------------------*/
SDCERR GetCurrentConfig(unsigned long *num, char *name);


/*---------------------------------------------------------
SDCERR ModifyConfig(char *name, SDCConfig *cfg)

Synopsis:  updates the config matching 'name'  If this is
           the current config, then it will also restart
		   the driver with the new config

input arguments: char *name  name of config to update
				             'ThirdPartyConfig' is not
							 modifyable with this function
							 use 'Set3rdPartyConfig'

				 SDCConfig *cfg Configuration.

return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_NAME if cant match name
				SDCERR_INVALID_CONFIG if config data is
				                      invalid
---------------------------------------------------------*/
SDCERR ModifyConfig(char *name, SDCConfig *cfg);


/*---------------------------------------------------------
SDCERR GetConfig(char *name, SDCConfig *cfg)

Synopsis:  Retrieves the config matching 'name'

input arguments: char *name  name of config to get
				             'ThirdPartyConfig' is not
							 allowed with this function
							 use 'Get3rdPartyConfig'
							 NULL is not a valid name

				 SDCConfig *cfg Configuration.

return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_NAME if cant match name
				SDCERR_INVALID_CONFIG if *cfg isnt valid
---------------------------------------------------------*/
SDCERR GetConfig(char *name, SDCConfig *cfg);


/*---------------------------------------------------------
SDCERR DeleteConfig(char *name)

Synopsis:  Deletes the config matching 'name'.  You are
           not allowed to delete the active config.

input arguments: char *name  name of config to delete
				             'ThirdPartyConfig' is not
							 allowed with this function
							 NULL is not a valid name

return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_NAME if cant match name
				SDCERR_INVALID_DELETE if trying to delete
				   the active config
---------------------------------------------------------*/
SDCERR DeleteConfig(char *name);



/*---------------------------------------------------------
SDCERR SetDefaultConfigValues(SDCConfig *cfg)

Synopsis:  Sets default values for new configs

input arguments: SDCConfig *cfg  Configuration

return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_CONFIG if configuration has
				  bad parameters in it
				SDCERR_FAIL for internal err
---------------------------------------------------------*/
SDCERR SetDefaultConfigValues(SDCConfig *cfg);

/*---------------------------------------------------------
SDCERR CreateConfig(SDCConfig *cfg);

Synopsis:  Creates a config from the default values.
		You must allocate the config memory.
		You must still add the config after it is created.

input arguments: SDCConfig *cfg  Configuration

return values:  SDCERR_SUCCESS if successful
				SDCERR_FAIL for internal err
---------------------------------------------------------*/
SDCERR CreateConfig(SDCConfig *cfg);

/*---------------------------------------------------------
SDCERR GetNumConfigs(unsigned long *num)

Synopsis:  Retrieves the number of configurations present

input arguments: unsigned long *num  number of configurations
                             present, 'ThirdPartyConfig' is
							 not counted as one

return values:  SDCERR_SUCCESS if successful
				SDCERR_FAIL if internal error
---------------------------------------------------------*/
SDCERR GetNumConfigs(unsigned long *num);

/*---------------------------------------------------------
SDCERR ActivateConfig(char *name)

Synopsis:  Activates the configuration with the given name

input arguments: char *name  name of configuration to make
                             the active one.  This function
							 will still succeed if the card
							 isnt present, so when it is
							 inserted this will be the
							 active config.  To use a 3rd
							 party WLAN framework, then
							 pass in 'ThirdPartyConfig' for
							 the name.  Please note that
							 in order for this to work
							 (3rdParty) then a power cycle
							 is REQUIRED--going to or from
							 it.
                    **Linux  If name is NULL, the last active
                             profile is reactivated.

return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_NAME if cant match name
				SDCERR_POWERCYCLE_REQUIRED if a power
				  cycle is required for this to take
				  effect
---------------------------------------------------------*/
SDCERR ActivateConfig(char *name);


/*---------------------------------------------------------
SDCERR AddConfig(SDCConfig *cfg)

Synopsis:  Adds the configuration

input arguments: SDCConfig *cfg  Configuration

return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_NAME if name	already exists
				SDCERR_INVALID_CONFIG if configuration has
				  bad parameters in it
				SDCERR_FAIL for internal err, or have
				  exceeded the maximum number of configs
---------------------------------------------------------*/
SDCERR AddConfig(SDCConfig *cfg);


/*---------------------------------------------------------
SDCERR GetAllConfigs(SDCConfig *cfgs, unsigned long *num)

Synopsis:  Retrieves all the configurations (except
           ThirdPartyConfig)

input arguments: SDCConfig *cfgs  Space for at least
 								  MAX_CFGS configs
                 unsigned long *num Number of configs returned

return values:  SDCERR_SUCCESS if successful
				SDCERR_FAIL for internal err, or *cfgs
				  is NULL
---------------------------------------------------------*/
SDCERR GetAllConfigs(SDCConfig *cfgs, unsigned long *num);


/*---------------------------------------------------------
SDCERR SetAllConfigs(unsigned long num, SDCConfig *cfgs)

Synopsis:  Sets all the configurations (except
           ThirdPartyConfig) to the given list, all previous
		   configurations are lost.  And if the active
		   config isn't ThirdParty, then it will auto-
		   matically be reset to the first configuration

input arguments: unsigned long num  number of configs
                 SDCConfig *cfgs  Configurations

return values:  SDCERR_SUCCESS if successful
				SDCERR_FAIL for internal err, or *cfgs
				  is NULL or num is 0 or > MAX_CFGS
---------------------------------------------------------*/
SDCERR SetAllConfigs(unsigned long num, SDCConfig *cfgs);


/*---------------------------------------------------------
SDCERR LRD_WF_GetbLRSBitmask(unsigned long numChannels, unsigned long *channels, unsigned long *bitmask)
Synopsis:	Converts an array of channels to a bitmask for use in a SDCGlobalConfig structure

Input arguments: unsigned long numChannels number of channels in array
				 LRD_WF_LRSChannels *channels array of channels
				 unsigned long *bitmask pointer to bitmask to create

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_PARAMETER if an channel given is invalid or not supported
---------------------------------------------------------*/
SDCERR LRD_WF_GetbLRSBitmask(unsigned long numChannels, LRD_WF_LRSChannels channels, unsigned long *bitmask);

/*---------------------------------------------------------
SDCERR LRD_WF_GetaLRSBitmask(unsigned long numChannels, unsigned long *channels, unsigned long *bitmask)
Synopsis:	Converts an array of channels to a bitmask for use in a SDCGlobalConfig structure

Input arguments: unsigned long numChannels number of channels in array
				 LRD_WF_LRSChannels *channels array of channels
				 unsigned long *bitmask pointer to bitmask to create

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_PARAMETER if an channel given is invalid or not supported
---------------------------------------------------------*/
SDCERR LRD_WF_GetaLRSBitmask(unsigned long numChannels, LRD_WF_LRSChannels channels, unsigned long *bitmask);

/*---------------------------------------------------------
SDCERR LRD_WF_GetbLRSChannels(unsigned long *numChannels, unsigned long *channels, unsigned long bitmask)
Synopsis:	Converts a bitmask to an array of channels

Input arguments: unsigned long *numChannels pointer to number of channels found from bitmask conversion
				 LRD_WF_LRSChannels *channels array of channels to fill in
				 unsigned long bitmask bitmask to convert

Return values:  SDCERR_SUCCESS if successful
				SDCERR_FAIL internal err
---------------------------------------------------------*/
SDCERR LRD_WF_GetbLRSChannels(unsigned long *numChannels, LRD_WF_LRSChannels *channels, unsigned long bitmask);

/*---------------------------------------------------------
SDCERR LRD_WF_GetaLRSChannels(unsigned long *numChannels, unsigned long *channels, unsigned long bitmask)
Synopsis:	Converts a bitmask to an array of channels

Input arguments: unsigned long *numChannels pointer to number of channels found from bitmask conversion
				 LRD_WF_LRSChannels *channels array of channels to fill in
				 unsigned long bitmask bitmask to convert

Return values:  SDCERR_SUCCESS if successful
				SDCERR_FAIL internal err
---------------------------------------------------------*/
SDCERR LRD_WF_GetaLRSChannels(unsigned long *numChannels, LRD_WF_LRSChannels *channels, unsigned long bitmask);


/*---------------------------------------------------------
SDCERR GetGlobalSettings(SDCGlobalConfig *gcfg)

Synopsis:  Retrieves the global config settings

input arguments: SDCGlobalConfig *gcfg global config

return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_CONFIG if *gcfg is NULL
				SDCERR_FAIL internal err
---------------------------------------------------------*/
SDCERR GetGlobalSettings(SDCGlobalConfig *gcfg);


/*---------------------------------------------------------
SDCERR SetGlobalSettings(SDCGlobalConfig *gcfg)

Synopsis:  Sets the global config settings and restarts
           the card.

input arguments: SDCGlobalConfig *gcfg global config

return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_CONFIG if *gcfg is NULL or
				  the data is invalid
				SDCERR_FAIL internal err
---------------------------------------------------------*/
SDCERR SetGlobalSettings(SDCGlobalConfig *gcfg);

/*---------------------------------------------------------
SDCERR Get3rdPartyConfig(SDC3rdPartyConfig *cfg3rd)

Synopsis:  Retrieves the 3rd party config settings

input arguments: SDC3rdPartyConfig *cfg3rd 3rd party config

return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_CONFIG if *gcfg is NULL
				SDCERR_FAIL internal err

NOTE: not implemented on linux platforms
---------------------------------------------------------*/
SDCERR Get3rdPartyConfig(SDC3rdPartyConfig *cfg3rd);


/*---------------------------------------------------------
SDCERR Set3rdPartyConfig(SDC3rdPartyConfig *cfg3rd)

Synopsis:  Stores the 3rd party config settings

input arguments: SDC3rdPartyConfig *cfg3rd 3rd party config

return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_CONFIG if *gcfg is NULL or
				  data is invalid
				SDCERR_FAIL internal err

NOTE: not implemented on linux platforms
---------------------------------------------------------*/
SDCERR Set3rdPartyConfig(SDC3rdPartyConfig *cfg3rd);

/*---------------------------------------------------------
SDCERR GetCurrentStatus(CF10G_STATUS *status)

Synopsis:  Retrieves status for the card, IP info, MAC info
           AP association info, etc....

input arguments: CF10G_STATUS *status  area to retrieve the
                                       card status into

return values:  SDCERR_SUCCESS if successful
				SDCERR_FAIL if internal err or *status is
				  NULL
---------------------------------------------------------*/
SDCERR GetCurrentStatus(CF10G_STATUS *status);

/*---------------------------------------------------------
SDCERR GetWifiInterfaceName(char *ifname)

Synopsis:  Looks through /sys/class/net to find a wlan[0-9]
           interface. Defaults to wlan0.

input arguments:  char *ifname  interface name retrieved

return values:  SDCERR_SUCCESS if successful
                SDCERR_FAIL if internal err or defaulted to
                    wlan0
---------------------------------------------------------*/
SDCERR GetWifiInterfaceName(char *ifname);

/*---------------------------------------------------------
SDCERR RadioDisable()

Synopsis:  Disables the Radio

return values:  SDCERR_SUCCESS if successful
				SDCERR_FAIL if internal err
---------------------------------------------------------*/
SDCERR RadioDisable();

/*---------------------------------------------------------
SDCERR RadioEnable()

Synopsis:  Enables the Radio

return values:  SDCERR_SUCCESS if successful
				SDCERR_FAIL if internal err
---------------------------------------------------------*/
SDCERR RadioEnable();

/*---------------------------------------------------------
SDCERR FirstFCCTest(FCC_TEST test, BITRATE rate, int channel,
                      TXPOWER testPower)

Synopsis:  Puts the radio into FCC testing mode on next power cycle

input arguments: FCC_TEST test--test type  (1-continuous transmit,
                                2-frequency accuracy, 3-continuous
                                receive


                 BITRATE  rate--rate for the test
                 int channel----channel for the test
                 TXPOWER testPower --power for the test

return values:  SDCERR_POWERCYCLE if successful
				SDCERR_FAIL if err
---------------------------------------------------------*/
SDCERR FirstFCCTest(FCC_TEST test, BITRATE rate, int channel,
					TXPOWER testPower, unsigned long timeout);

/*---------------------------------------------------------
SDCERR NextFCCTest(FCC_TEST test, BITRATE rate, int channel,
                      TXPOWER testPower)

Synopsis:  Changes the FCC test for a unit already running in
            FCC test mode.  (valid call to FirstFCCTest() followed
                             by a power cycle will put the unit into
                             FCC test mode)

input arguments: FCC_TEST test--test type  (1-continuous transmit,
                                2-frequency accuracry, 3-continuous
                                receive
                 BITRATE  rate--rate for the test
                 int channel----channel for the test
                 TXPOWER testPower --power fof the test

NOTE: when completed with FCC testing a Warm Reset is REQUIRED to
      bring the unit back to a normal state.

return values:  SDCERR_SUCCESS if successful
				SDCERR_FAIL if internal err
---------------------------------------------------------*/
SDCERR NextFCCTest(FCC_TEST test, BITRATE rate, int channel,
				   TXPOWER testPower, unsigned long timeout);

/*---------------------------------------------------------
REG_DOMAIN GetCurrentDomain()

Synopsis:  returns the current regulatory domain set in the
           SROM
input arguments: none

return values:  REG_FCC if FCC regulatory domain
                REG_ETSI if ETSI regulatory domain
				REG_TELEC if TELEC regulatory domain
				REG_KCC if Korean regulatory domain
				REG_WW if set into 'WorldWide' mode--this
				   means it should be safe for all 3 above
				   regulatory domains (but isn't optimized
				   for any particular domain)

NOTE:  This is a lengthy call, and should not and need not
       be called frequently.  The value is stored in SROM
	   and it takes significant time to access it.
---------------------------------------------------------*/
REG_DOMAIN GetCurrentDomain();

/*---------------------------------------------------------
SDCERR updateSROM()

Synopsis:  Sets the BT coexistance, Regulatory Domain, and
           the Tx max %.

input arguments: none

return values:  SDCERR_FAIL if failed to set it
                SDCERR_SUCCESS if successful
				SDCERR_INVALID_CONFIG if current global config
				  isnt valid

NOTE:  This is a lengthy call, and should not and need not
       be called frequently.  The value is stored in SROM
	   and it takes significant time to access it.

       The global configuration is read for all 3 inputs,
	   if there isnt one or they are invalid then
	   SDCERR_INVALID_CONFIG is returned

---------------------------------------------------------*/
SDCERR updateSROM();

/*---------------------------------------------------------
SDCERR testTxData(unsigned char start, char Pattern)


Synopsis: Starts/Stops blasting data in FCCTEST mode

input arguments: start:  TRUE starts, FALSE stops data
                 Pattern:  The data in the packet is filled
				           with this value.

return values:  SDCERR_FAIL if Not in FCC test mode or internal failure
                SDCERR_SUCCESS if successful

NOTE:  This creates a process that sends data out to IP
       address FF.FF.FF.FF as FAST as it can so it can slow
	   down operation of the system on slower machines.  It can
	   end up being a lengthy call.

---------------------------------------------------------*/
SDCERR testTxData(unsigned char start, char pattern);

/*---------------------------------------------------------
SDCERR setMonitorMode(unsigned long enable, unsigned long channel, unsigned long slice, void *buffer, unsigned long bufferSize)

Synopsis: puts driver into promiscuous mode

input arguments: char *command command to send

return values:  SDCERR_FAIL if invalid command
                SDCERR_SUCCESS if successful

NOTE: This is used internally and the commands are not
      documented.....

---------------------------------------------------------*/
SDCERR setMonitorMode(unsigned long enable, unsigned long channel, unsigned long slice, void *buffer, unsigned long bufferSize);


/*---------------------------------------------------------
SDCERR GetSDKVersion(unsigned long *version)

Synopsis: returns the version of the sdk

input arguments: unsigned long *version

return values:  SDCERR_FAIL if error
                SDCERR_SUCCESS if successful
---------------------------------------------------------*/
SDCERR GetSDKVersion(unsigned long *version);

/*---------------------------------------------------------
SDCERR FlushConfigKeys(int configNumber)

Synopsis:   Flushes the specified registry keys.
			Depending on the system, registry changes are flushed
			to disk after a system-specified interval of time and at shutdown.
			This function forces a flush, so the Summit parameters
			will be saved if a power-cycle occurs before the system
			flushes the registry.  This is an expensive operation.
    **linux - ConfigurationKeys are written to disc and a sync is
            performed immediately on all configuration changes leaving
            this call unncessary.

input arguments: int configNumber--the config to flush
					-1 flushes the Global Settings
					 0 flushes the ThirdPartyConfig
					 1 - MAX_CFGS flushes the specified config number

return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_PARAMETER if invalid configNumber
---------------------------------------------------------*/
SDCERR FlushConfigKeys(int configNumber);

/*---------------------------------------------------------
SDCERR FlushAllConfigKeys()

Synopsis:   Flushes all Summit config registry keys.
			Depending on the system, registry changes are flushed
			to disk after a system-specified interval of time and at shutdown.
			This function forces a flush, so the Summit parameters
			will be saved if a power-cycle occurs before the system
			flushes the registry.  This is an expensive operation.
    **linux - ConfigurationKeys are written to disc and a sync is
            performed immediately on all configuration changes leaving
            this call unncessary.


return values:  SDCERR_SUCCESS if successful
				SDCERR_FAIL if error
---------------------------------------------------------*/
SDCERR FlushAllConfigKeys();

#ifndef __linux__
/*---------------------------------------------------------
LONG QueryOID(ULONG ndis_oid, void *buffer, ULONG bufSize);

Synopsis:  Queries an NDIS OID with DeviceIOControl.

input arguments: ULONG ndis_oid  the NDIS OID to query
				 void *buffer    in/out
				 ULONG bufSize   in/out

return values:  0 if it fails. Call GetLastError for
					error information.
				non-zero for success
---------------------------------------------------------*/
LONG QueryOID(ULONG ndis_oid, void *buffer, ULONG bufSize);

/*---------------------------------------------------------
LONG SetOID(ULONG ndis_oid, void *buffer, ULONG bufSize);

Synopsis:  Sets an NDIS OID with DeviceIOControl.

input arguments: ULONG ndis_oid  the NDIS OID to query
				 void *buffer    in/out
				 ULONG bufSize   in/out

return values:  0 if it fails.  Call GetLastError for
					error information.
				non-zero for success
---------------------------------------------------------*/
LONG SetOID(ULONG ndis_oid, void *buffer, ULONG bufSize);
#endif //__linux__

/*---------------------------------------------------------
SDCERR GetConfigFileInfo(char *filename, CONFIG_FILE_INFO *info)

Synopsis:	Retrieves file details from a Summit config file.

Input arguments: char *filename--a valid filename, required
                 CONFIG_FILE_INFO *info--pass in a pointer to an allocated CONFIG_FILE_INFO structure

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_PARAMETER if an invalid filename or info
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR GetConfigFileInfo(char *filename, CONFIG_FILE_INFO *info);

/*---------------------------------------------------------
SDCERR exportSettings(char *filename, SDC_ALL *all)

Synopsis:	Exports configs, global settings, and third party config to the specified file.

Input arguments: char *filename--a valid filename, required
                 SDC_ALL *all--specifies which information to export
					configGlobal--either NULL to skip global config export or a valid pointer
					configThirdParty--either NULL to skip 3rd party config export or a valid pointer
					configs--either NULL to skip configs export or a valid pointer to one or more SDCConfig structures
					numConfigs--0 to skip all SDCConfigs, or the number of configs (SDCConfig) to export
						don't include configGlobal or configThirdParty in this count

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_PARAMETER if an invalid filename or all structure
				SDCERR_INVALID_CONFIG if an invalid config (global, third party, or config)
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR exportSettings(char *filename, SDC_ALL *all);

/*---------------------------------------------------------
SDCERR importSettings(char *filename, SDC_ALL *all)

Synopsis:	Imports Summit settings from the file and writes to the SDC_ALL structure.
			You must allocate the memory in the SDC_ALL structure-configGlobal, configThirdParty, and up to MAX_CFGS configs.

Input arguments: char *filename--a valid filename, required
                 SDC_ALL *all--specifies which information to import and imported info will be saved here
					configGlobal--either NULL to skip global config import or a valid pointer to an allocated structure
					configThirdParty--either NULL to skip 3rd party config import or a valid pointer to an allocated structure
					configs--either NULL to skip configs import or a valid pointer to one or more allocated SDCConfig structures
					numConfigs--this will be set to the number of configs (SDCConfig) exported
						doesn't include configGlobal or configThirdParty in this count

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_PARAMETER if an invalid filename or all structure
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR importSettings(char *filename, SDC_ALL *all);

/*---------------------------------------------------------
SDCERR SetWEPKey(SDCConfig * cfg,  int nWepKey,
				WEPLEN keyLength, unsigned char* key, unsigned char txKey)

Synopsis:	Sets the WEP key information.
			It will validate the config's wepType and eapType members

Input arguments: SDCConfig* cfg--a valid configuration, required
				 int nWepKey--which of the 4 stored WEP keys to modify
						must be 1,2,3, or 4
				 WEPLEN keyLength--must be one of 3 values:
						WEPLEN_NOT_SET-clear this key
						WEPLEN_40BIT-key must be 10 hex chars
						WEPLEN_128BIT-key must be 26 hex chars
				 unsigned char* key--the WEP key in hexadecimal
						if NULL, this field will be cleared
						must be either 0, 10, or 26 hex chars
				 bool txKey--set if this is the active, transmit key
						only one of the 4 keys can be the txKey

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_WEP_TYPE if wepType is not WEP_ON or WEP_CKIP
				SDCERR_INVALID_EAP_TYPE if eapType is not EAP_NONE
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR SetWEPKey(SDCConfig* cfg, int nWepKey,  WEPLEN keyLength, unsigned char* key, unsigned char txKey);

/*---------------------------------------------------------
SDCERR GetWEPKey(SDCConfig* cfg, int nWepKey,  WEPLEN* keyLength,
					unsigned char* key, bool* txKey)

Synopsis:	Retrieves a WEP key.

Input arguments: SDCConfig* cfg--a valid configuration, required
				 int nWepKey--which of the 4 stored WEP keys to retrieve
						must be 1,2,3, or 4
				 WEPLEN keyLength--WEP key length
						if NULL, this field will be ignored
				 unsigned char* key--pass in an allocated buffer of at least 26 chars
						key will be returned as hex chars
						if NULL, this field will be ignored
				 bool txKey--returns if this is the active, transmit key
						if NULL, this field will be ignored

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_WEP_TYPE if wepType is not WEP_ON or WEP_CKIP
				SDCERR_INVALID_EAP_TYPE if eapType is not EAP_NONE
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR GetWEPKey(SDCConfig* cfg, int nWepKey,  WEPLEN* keyLength, unsigned char* key, unsigned char* txKey);

/*---------------------------------------------------------
SDCERR SetMultipleWEPKeys(SDCConfig *cfg,  int nTxKey,  WEPLEN key1Length, unsigned char * key1,
														WEPLEN key2Length, unsigned char * key2,
														WEPLEN key3Length, unsigned char * key3,
														WEPLEN key4Length, unsigned char * key4)

Synopsis:	Sets the WEP key information.
			It will validate the config's wepType and eapType members

Input arguments: SDCConfig* cfg--a valid configuration, required
				 int nTxKey-specifies transmit key
						must be 1,2,3, or 4
				 WEPLEN key1Length--must be one of 3 values:
						WEPLEN_NOT_SET-clear this key
						WEPLEN_40BIT-key must be 10 hex chars
						WEPLEN_128BIT-key must be 26 hex chars
				 unsigned char* key1--the WEP key in hexadecimal
						must be either 0, 10, or 26 hex chars

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_WEP_TYPE if wepType is not WEP_ON or WEP_CKIP
				SDCERR_INVALID_EAP_TYPE if eapType is not EAP_NONE
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR SetMultipleWEPKeys(SDCConfig *cfg,  int nTxKey,  WEPLEN key1Length, unsigned char * key1,
						  WEPLEN key2Length, unsigned char * key2,
						  WEPLEN key3Length, unsigned char * key3,
						  WEPLEN key4Length, unsigned char * key4);

/*---------------------------------------------------------
SDCERR GetMultipleWEPKeys(SDCConfig *cfg,  int *nTxKey, WEPLEN *key1Length, unsigned char * key1,
														WEPLEN *key2Length, unsigned char * key2,
														WEPLEN *key3Length, unsigned char * key3,
														WEPLEN *key4Length, unsigned char * key4)
Synopsis:	Retrieves all 4 WEP keys.

Input arguments: SDCConfig* cfg--a valid configuration, required
				 int *nTxKey--returns which key is currently the transmit key
						will be 1,2,3, or 4
				 WEPLEN *key1Length--returns the length of key1
				 unsigned char * key1--pass in an allocated buffer of at least 26 chars
						key will be returned as hex chars

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_WEP_TYPE if wepType is not WEP_ON or WEP_CKIP
				SDCERR_INVALID_EAP_TYPE if eapType is not EAP_NONE
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR GetMultipleWEPKeys(SDCConfig *cfg,  int *nTxKey, WEPLEN *key1Length, unsigned char * key1,
						  WEPLEN *key2Length, unsigned char * key2,
						  WEPLEN *key3Length, unsigned char * key3,
						  WEPLEN *key4Length, unsigned char * key4);


/*---------------------------------------------------------
SDCERR SetPSK(SDCConfig * cfg,  char * psk)

Synopsis:	Sets the PSK.
			Validates the config's wepType and eapType.

Input arguments: SDCConfig *cfg--a valid configuration, required
                 char* psk--null-terminated psk up to PSK_SZ characters
								for psk, must be 64 hex chars
								for passphrase, must be 8-63 chars (printable ASCII)
								if NULL, then the psk field will be cleared

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_WEP_TYPE if wepType is not WPA PSK or WPA2 PSK
				SDCERR_INVALID_EAP_TYPE if eapType is not EAP_NONE
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR SetPSK(SDCConfig * cfg,  char * psk);

/*---------------------------------------------------------
SDCERR GetPSK(SDCConfig * cfg,  char * psk)

Synopsis:	Retrieves the PSK.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* psk--pass in an allocated buffer of at least PSK_SZ

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_WEP_TYPE if wepType is not WPA PSK or WPA2 PSK
				SDCERR_INVALID_EAP_TYPE if eapType is not EAP_NONE
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR GetPSK(SDCConfig * cfg,  char * psk);

/*---------------------------------------------------------
SDCERR SetLEAPCred(SDCConfig * cfg, char * username, char * password)

Synopsis:	Sets the LEAP credentials.
			Validates the config's wepType and eapType.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--null-terminated username up to USER_NAME_SZ characters
								if NULL, then the username field will be cleared
                 char* password--null-terminated password up to USER_PWD_SZ characters
								if NULL, then the password field will be cleared

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_WEP_TYPE if wepType is not WEP_AUTO,WPA_TKIP,WPA2_AES,CCKM_TKIP, or WEP_AUTO_CKIP
				SDCERR_INVALID_EAP_TYPE if eapType is not EAP_LEAP
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR SetLEAPCred(SDCConfig * cfg,  char * username, char * password);

/*---------------------------------------------------------
SDCERR GetLEAPCred(SDCConfig * cfg,  char * username, char * password)

Synopsis:	Retrieves the LEAP credentials.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--pass in a valid pointer with an allocated buffer of at least USER_NAME_SZ characters
								if NULL, this parameter is ignored
                 char* password--pass in a valid pointer with an allocated buffer of at least USER_PWD_SZ characters
								if NULL, this parameter is ignored

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALIDPARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR GetLEAPCred(SDCConfig * cfg,  char * username, char * password);

/*---------------------------------------------------------
SDCERR SetEAPFASTCred(SDCConfig * cfg,  char * username, char * password,
						char* pacfilename, char* pacpassword);

Synopsis:	Sets the EAP-FAST credentials.
			Validates the config's wepType and eapType.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--null-terminated username up to USER_NAME_SZ characters
								if NULL, then the username field will be cleared
                 char* password--null-terminated password up to USER_PWD_SZ characters
								if NULL, then the password field will be cleared
				 char* pacfilename--null-terminated password up to CRED_PFILE_SZ characters
								if NULL, then the password field will be cleared
				 char* pacpassword--null-terminated password up to CRED_PFILE_SZ characters
								if NULL, then the password field will be cleared

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_WEP_TYPE if wepType is not WEP_AUTO,WPA_TKIP,WPA2_AES,CCKM_TKIP, or WEP_AUTO_CKIP
				SDCERR_INVALID_EAP_TYPE if eapType is not EAP_EAPFAST
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR SetEAPFASTCred(SDCConfig * cfg,  char * username, char * password, char* pacfilename, char* pacpassword);

/*---------------------------------------------------------
SDCERR GetEAPFASTCred(SDCConfig * cfg,  char * username, char * password,
				char* pacfilename, char* pacpassword);

Synopsis:	Retrieves the EAP-FAST credentials.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--pass in a valid pointer with an allocated buffer of at least USER_NAME_SZ characters
								if NULL, this parameter is ignored
                 char* password--pass in a valid pointer with an allocated buffer of at least USER_PWD_SZ characters
								if NULL, this parameter is ignored
				 char* pacfilename--pass in a valid pointer with an allocated buffer of at least CRED_PFILE_SZ characters
								if NULL, this parameter is ignored
                 char* pacpassword--pass in a valid pointer with an allocated buffer of at least CRED_PFILE_SZ characters
								if NULL, this parameter is ignored

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALIDPARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR GetEAPFASTCred(SDCConfig * cfg,  char * username, char * password, char* pacfilename, char* pacpassword);

/*---------------------------------------------------------
SDCERR SetPEAPGTCCred(SDCConfig* cfg,  char* username,
						char* password, CERTLOCATION CAcertLocation, char* caCert );

Synopsis:	Sets the PEAP-GTC credentials.
			Validates the config's wepType and eapType members.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--null-terminated username up to USER_NAME_SZ characters
								if NULL, then the username field will be cleared
                 char* password--null-terminated password up to USER_PWD_SZ characters
								if NULL, then the password field will be cleared
                 CERTLOCATION CAcertLocation--specifies where CA cert is stored
								it determines the value of the caCert parameter
                 char* caCert-- if NULL, this parameter is ignored
								depending on the caCertLocation field, caCert will contain:
								if CERT_NONE, caCert should be NULL - don't validate the server
								if CERT_FILE, caCert specifies the cert filename, up to CRED_CERT_SZ chars
								if CERT_FULL_STORE, caCert is NULL - the full MS cert store will be searched for a valid cert
								if CERT_IN_STORE, caCert is a 20 byte hash representing one specific cert from the MS-store

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_WEP_TYPE if wepType is not WEP_AUTO,WPA_TKIP,WPA2_AES,CCKM_TKIP, or WEP_AUTO_CKIP
				SDCERR_INVALID_EAP_TYPE if eapType is not EAP_PEAPGTC
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR SetPEAPGTCCred(SDCConfig* cfg,  char* username, char* password, CERTLOCATION CAcertLocation, char* caCert );

/*---------------------------------------------------------
SDCERR GetPEAPGTCCred(SDCConfig* cfg,  char* username,
					char * password, CERTLOCATION* CAcertLocation, char* caCert );

Synopsis:	Retrieves the PEAP-GTC credentials.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--pass in a valid pointer with an allocated buffer of at least USER_NAME_SZ characters
								if NULL, this parameter is ignored
                 char* password--pass in a valid pointer with an allocated buffer of at least USER_PWD_SZ characters
								if NULL, this parameter is ignored
				 CERTLOCATION* CAcertLocation--pass in a valid pointer
								if NULL, this parameter is ignored
                 char* caCert--pass in a valid pointer with an allocated buffer of at least CRED_CERT_SZ characters
								if NULL, this parameter is ignored
								depending on the caCertLocation field, caCert will contain:
								if CERT_NONE, caCert is NULL - don't validate the server
								if CERT_FILE, caCert will specify the cert filename, up to CRED_CERT_SZ chars
								CERT_FULL_STORE, caCert is NULL - the full MS cert store will be searched for a valid cert
								CERT_IN_STORE, caCert is a 20 byte hash representing one specific cert from the MS-store

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR GetPEAPGTCCred(SDCConfig* cfg,  char* username, char * password, CERTLOCATION* CAcertLocation, char* caCert );

/*---------------------------------------------------------
SDCERR SetPEAPMSCHAPCred(SDCConfig* cfg,  char* username, char* password,
						CERTLOCATION CAcertLocation, char* caCert );

Synopsis:	Sets the PEAP-MSCHAP credentials.
			Validates the config's wepType and eapType members.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--null-terminated username up to USER_NAME_SZ characters
								if NULL, then the username field will be cleared
                 char* password--null-terminated password up to USER_PWD_SZ characters
								if NULL, then the password field will be cleared
                 CERTLOCATION CAcertLocation--specifies where CA cert is stored
								it determines the value of the caCert parameter
                 char* caCert-- if NULL, this parameter is ignored
								depending on the caCertLocation field, caCert will contain:
								if CERT_NONE, caCert should be NULL - don't validate the server
								if CERT_FILE, caCert specifies the cert filename, up to CRED_CERT_SZ chars
								if CERT_FULL_STORE, caCert is NULL - the full MS cert store will be searched for a valid cert
								if CERT_IN_STORE, caCert is a 20 byte hash representing one specific cert from the MS-store

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_WEP_TYPE if wepType is not WEP_AUTO,WPA_TKIP,WPA2_AES,CCKM_TKIP, or WEP_AUTO_CKIP
				SDCERR_INVALID_EAP_TYPE if eapType is not EAP_PEAPMSCHAP
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR SetPEAPMSCHAPCred(SDCConfig* cfg,  char* username, char* password, CERTLOCATION CAcertLocation, char* caCert );

/*---------------------------------------------------------
SDCERR GetPEAPMSCHAPCred(SDCConfig* cfg,  char* username,
					char * password, CERTLOCATION* CAcertLocation, char* caCert );

Synopsis:	Retrieves the PEAP-MSCHAP credentials.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--pass in a valid pointer with an allocated buffer of at least USER_NAME_SZ characters
								if NULL, this parameter is ignored
                 char* password--pass in a valid pointer with an allocated buffer of at least USER_PWD_SZ characters
								if NULL, this parameter is ignored
				 CERTLOCATION* CAcertLocation--pass in a valid pointer
								if NULL, this parameter is ignored
                 char* caCert--pass in a valid pointer with an allocated buffer of at least CRED_CERT_SZ characters
								if NULL, this parameter is ignored
								depending on the caCertLocation field, caCert will contain:
								if CERT_NONE, caCert is NULL - don't validate the server
								if CERT_FILE, caCert will specify the cert filename, up to CRED_CERT_SZ chars
								CERT_FULL_STORE, caCert is NULL - the full MS cert store will be searched for a valid cert
								CERT_IN_STORE, caCert is a 20 byte hash representing one specific cert from the MS-store

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALIDPARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR GetPEAPMSCHAPCred(SDCConfig* cfg,  char* username, char * password, CERTLOCATION* CAcertLocation, char* caCert );

/*---------------------------------------------------------
SDCERR SetEAPTLSCred(SDCConfig * cfg,  char * username, char* userCert,
						CERTLOCATION certLocation, char* caCert);

Synopsis:	Sets the EAP_TLS credentials.
			Validates the config's wepType and eapType members.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--null-terminated username up to USER_NAME_SZ characters
								if NULL, then the username field will be cleared
                 char* userCert--a 20 byte hash representing one specific user cert from the MS-store
								if NULL, then the password field will be cleared
                 CERTLOCATION CAcertLocation--specifies where CA cert is stored
								it determines the value of the caCert parameter
                 char* caCert-- if NULL, this parameter is ignored
								depending on the caCertLocation field, caCert will contain:
								if CERT_NONE, caCert should be NULL - don't validate the server
								if CERT_FILE, caCert specifies the cert filename, up to CRED_CERT_SZ chars
								if CERT_FULL_STORE, caCert is NULL - the full MS cert store will be searched for a valid cert
								if CERT_IN_STORE, caCert is a 20 byte hash representing one specific cert from the MS-store

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_WEP_TYPE if wepType is not WEP_AUTO,WPA_TKIP,WPA2_AES,CCKM_TKIP, or WEP_AUTO_CKIP
				SDCERR_INVALID_EAP_TYPE if eapType is not EAP_EAPTLS
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR SetEAPTLSCred(SDCConfig * cfg,  char * username, char* userCert, CERTLOCATION certLocation, char* caCert);

/*---------------------------------------------------------
SDCERR GetEAPTLSCred(SDCConfig * cfg,  char * username, char* userCert,
						CERTLOCATION* certLocation, char* caCert);

Synopsis:	Retrieves the EAP-TLS credentials.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--pass in a valid pointer with an allocated buffer of at least USER_NAME_SZ characters
								if NULL, this parameter is ignored
                 char* userCert--pass in a valid pointer with an allocated buffer of at least CRED_CERT_SZ characters
								if NULL, this parameter is ignored
				 CERTLOCATION* CAcertLocation--pass in a valid pointer
								if NULL, this parameter is ignored
                 char* caCert--pass in a valid pointer with an allocated buffer of at least CRED_CERT_SZ characters
								if NULL, this parameter is ignored
								depending on the caCertLocation field, caCert will contain:
								if CERT_NONE, caCert is NULL - don't validate the server
								if CERT_FILE, caCert will specify the cert filename, up to CRED_CERT_SZ chars
								CERT_FULL_STORE, caCert is NULL - the full MS cert store will be searched for a valid cert
								CERT_IN_STORE, caCert is a 20 byte hash representing one specific cert from the MS-store

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALIDPARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR GetEAPTLSCred(SDCConfig * cfg,  char * username, char* userCert, CERTLOCATION* certLocation, char* caCert);

/*---------------------------------------------------------
SDCERR SetEAPTTLSCred(SDCConfig* cfg,  char* username,
						char* password, CERTLOCATION CAcertLocation, char* caCert );

Synopsis:	Sets the EAP-TTLS credentials.
			Validates the config's wepType and eapType members.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--null-terminated username up to USER_NAME_SZ characters
								if NULL, then the username field will be cleared
                 char* password--null-terminated password up to USER_PWD_SZ characters
								if NULL, then the password field will be cleared
                 CERTLOCATION CAcertLocation--specifies where CA cert is stored
								it determines the value of the caCert parameter
                 char* caCert-- if NULL, this parameter is ignored
								depending on the caCertLocation field, caCert will contain:
								if CERT_NONE, caCert should be NULL - don't validate the server
								if CERT_FILE, caCert specifies the cert filename, up to CRED_CERT_SZ chars
								if CERT_FULL_STORE, caCert is NULL - the full MS cert store will be searched for a valid cert
								if CERT_IN_STORE, caCert is a 20 byte hash representing one specific cert from the MS-store

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_WEP_TYPE if wepType is not WEP_AUTO,WPA_TKIP,WPA2_AES,CCKM_TKIP, or WEP_AUTO_CKIP
				SDCERR_INVALID_EAP_TYPE if eapType is not EAP_EAPTTLS
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR SetEAPTTLSCred(SDCConfig * cfg,  char * username, char* password, CERTLOCATION certLocation, char* caCert);

/*---------------------------------------------------------
SDCERR GetEAPTTLSCred(SDCConfig* cfg,  char* username,
					char * password, CERTLOCATION* CAcertLocation, char* caCert );

Synopsis:	Retrieves the EAP-TTLS credentials.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--pass in a valid pointer with an allocated buffer of at least USER_NAME_SZ characters
								if NULL, this parameter is ignored
                 char* password--pass in a valid pointer with an allocated buffer of at least USER_PWD_SZ characters
								if NULL, this parameter is ignored
				 CERTLOCATION* CAcertLocation--pass in a valid pointer
								if NULL, this parameter is ignored
                 char* caCert--pass in a valid pointer with an allocated buffer of at least CRED_CERT_SZ characters
								if NULL, this parameter is ignored
								depending on the caCertLocation field, caCert will contain:
								if CERT_NONE, caCert is NULL - don't validate the server
								if CERT_FILE, caCert will specify the cert filename, up to CRED_CERT_SZ chars
								CERT_FULL_STORE, caCert is NULL - the full MS cert store will be searched for a valid cert
								CERT_IN_STORE, caCert is a 20 byte hash representing one specific cert from the MS-store

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR GetEAPTTLSCred(SDCConfig * cfg,  char * username, char* password, CERTLOCATION* certLocation, char* caCert);

/*---------------------------------------------------------
SDCERR SetPEAPTLSCred(SDCConfig * cfg,  char * username, char* userCert,
						CERTLOCATION certLocation, char* caCert);

Synopsis:	Sets the PEAP_TLS credentials.
			Validates the config's wepType and eapType members.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--null-terminated username up to USER_NAME_SZ characters
								if NULL, then the username field will be cleared
                 char* userCert--under linux - a CRED_CERT_SZ file name string
                 char* userCert--under Windows - a 20 byte hash representing one specific user cert from the MS-store
								if NULL, then the password field will be cleared
                 CERTLOCATION CAcertLocation--specifies where CA cert is stored
								it determines the value of the caCert parameter
                 char* caCert-- if NULL, this parameter is ignored
								depending on the caCertLocation field, caCert will contain:
								if CERT_NONE, caCert should be NULL - don't validate the server
								if CERT_FILE, caCert specifies the cert filename, up to CRED_CERT_SZ chars
								if CERT_FULL_STORE, caCert is NULL - the full MS cert store will be searched for a valid cert
								if CERT_IN_STORE, caCert is a 20 byte hash representing one specific cert from the MS-store

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_WEP_TYPE if wepType is not WEP_AUTO,WPA_TKIP,WPA2_AES,CCKM_TKIP, or WEP_AUTO_CKIP
				SDCERR_INVALID_EAP_TYPE if eapType is not PEAP_EAPTLS
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR SetPEAPTLSCred(SDCConfig * cfg,  char * username, char* userCert, CERTLOCATION certLocation, char* caCert);
/*---------------------------------------------------------
SDCERR GetPEAPTLSCred(SDCConfig * cfg,  char * username, char* userCert,
						CERTLOCATION* certLocation, char* caCert);

Synopsis:	Retrieves the PEAP-TLS credentials.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--pass in a valid pointer with an allocated buffer of at least USER_NAME_SZ characters
								if NULL, this parameter is ignored
                 char* userCert--pass in a valid pointer with an allocated buffer of at least CRED_CERT_SZ characters
								if NULL, this parameter is ignored
				 CERTLOCATION* CAcertLocation--pass in a valid pointer
								if NULL, this parameter is ignored
                 char* caCert--pass in a valid pointer with an allocated buffer of at least CRED_CERT_SZ characters
								if NULL, this parameter is ignored
								depending on the caCertLocation field, caCert will contain:
								if CERT_NONE, caCert is NULL - don't validate the server
								if CERT_FILE, caCert will specify the cert filename, up to CRED_CERT_SZ chars
								CERT_FULL_STORE, caCert is NULL - the full MS cert store will be searched for a valid cert
								CERT_IN_STORE, caCert is a 20 byte hash representing one specific cert from the MS-store

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALIDPARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR GetPEAPTLSCred(SDCConfig * cfg,  char * username, char* userCert, CERTLOCATION* certLocation, char* caCert);

typedef struct _scanItemInfo {
       char            ssid[40];
       char            bssid[20];
       int             freq;
       int             channel;
       int             rssi;
       int             security;  // 0 open, 1 not open
       EAPTYPE         eapType;   // not yet supported
       WEPTYPE         wepType;
       int             adHocMode; //1 enable, default 0
} SCAN_ITEM_INFO;

typedef struct _SDC_802_11_BSSID_LIST_EX {
	unsigned long  NumberOfItems;
	SCAN_ITEM_INFO Bssid[1];
} SDC_802_11_BSSID_LIST_EX;

/*---------------------------------------------------------
SDCERR GetBSSIDList(SDC_802_11_BSSID_LIST_EX *list, int *numBufEntries)

Synopsis:  Get a list of BSSIDs from a scan

Input arguments: pointer to an 802_11_BSSID_LIST_EX structure,
                pointer to int with number of SCAN_ITEM_INFO elements in the
                structure

return values:  SDCERR_SUCCESS if successful
                SDCERR_INVALID_PARAMETER if invalid parameter,
				SDCERR_INSUFFICIENT_MEMORY if list structure is not large enough
                                           (all data that will fit will be copied)
				SDCERR_FAIL if internal err
				number of elements returned is indicated in NumberOfItems
				if insufficient memory error occurs, total number of needed elements
				  is returned in numBufEntries; -1 indicates a failure to allocate memory
---------------------------------------------------------*/
SDCERR GetBSSIDList(SDC_802_11_BSSID_LIST_EX *list, int *numBufEntries);

typedef enum _LRD_WF_BSSTYPE {
    INFRASTRUCTURE = 0,
    ADHOC
} LRD_WF_BSSTYPE;

#define LRD_WF_MAX_SSID_LEN    32
#define LRD_WF_MAC_ADDR_LEN    6

typedef struct _LRD_WF_SSID{
	unsigned char len;
	unsigned char val[LRD_WF_MAX_SSID_LEN];
	                                  // Note that the val is not a string 
	                                  // and can have embedded NULL and non-
	                                  // printable characters.  Also note
	                                  // that val does not have a null
	                                  // termination character.
} LRD_WF_SSID;

typedef struct _LRD_WF_SCAN_INFO_ITEM{
	int             channel;
	int             rssi;
	unsigned int    securityMask; // bit mask of WEPTYPE enums indicating 
	                              // supported types
	LRD_WF_BSSTYPE  bssType;
	unsigned int    reserved;
	unsigned char   bssidMac[LRD_WF_MAC_ADDR_LEN];
	LRD_WF_SSID     ssid;
} LRD_WF_SCAN_ITEM_INFO ;

typedef struct _LRD_WF_BSSID_LIST{
	unsigned long  NumberOfItems;
	LRD_WF_SCAN_ITEM_INFO Bssid[1];
} LRD_WF_BSSID_LIST;

/*---------------------------------------------------------
SDCERR LRD_WF_GetBSSIDList(LRD_WF_BSSID_LIST *list, int *numBufEntries)

Synopsis:  Get a list of BSSIDs from a scan

Input arguments: pointer to an LRD_WF_BSSID_LIST structure,
                 pointer to int with number of SCAN_ITEM_INFO elements 
                 in the structure

return values:  SDCERR_SUCCESS if successful
                SDCERR_INVALID_PARAMETER if invalid parameter,
                SDCERR_INSUFFICIENT_MEMORY if list structure is not large 
                       enough (all data that will fit will be copied)
                SDCERR_NOT_IMPLEMENTED
                SDCERR_FAIL if internal err
notes
        number of elements returned is indicated in NumberOfItems
        if insufficient memory error occurs, total number of needed
        elements is returned in the numBufEntries entry of the
        LRD_WF_BSSID_LIST structure.
---------------------------------------------------------*/
SDCERR LRD_WF_GetBSSIDList(LRD_WF_BSSID_LIST *list, int *numBufEntries);

#ifdef __linux__
typedef SDCERR (*SDC_EVENT_HANDLER) (unsigned long event_type, SDC_EVENT *event);
// SDC_EVENT_HANDLER - user defined function for handling all desired events.
// event_type     - event struct pointer
//
// SDC_EVENT      - event structure containing status/reason/flags etc.
//             note that buffer will only be valid until SDC_EVENT_HANDLER function returns

/*-------------------------------------------------------
SDCERR SDCRegisterForEvents(unsigned long long eventMask, SDC_EVENT_HANDLER ehandler);

Synopsis:   Register event mask and event handler for selected events and start SDK events
Note:       If a LOST_COM status from a SDC_E_INTERNAL event is received and the program wants
            to attempt to recover, SDCDeRegisterEvents() must be called before attempting to
            call SDCRegisterForEvents() again.

Input arguments: eventMask - 64 bit bitmask of events to signal
                 SDC_EVENT_HANDLER - user defined function to be called on each event

return values:  SDCERR_SUCCESS if successful
                SDCERR_INVALID_PARAMETER if invalid parameter
                SDCERR_INVALID_CONFIG if attempting to call again without calling SDCRegisterForEvents()
                SDCERR_FAIL if internal err
---------------------------------------------------------*/
SDCERR SDCRegisterForEvents( unsigned long long eventMask, SDC_EVENT_HANDLER ehandler);

/*-------------------------------------------------------
SDCERR SDCREgisterdEventsList( unsigned long long *currentMask);

Synopsis:   Get current event mask

Input argument: unsigned long pointer for currentMask

return values:  SDCERR_SUCCESS if successful
                SDCERR_INVALID_PARAMETER if invalid parameter
				SDCERR_FAIL if internal err
---------------------------------------------------------*/
SDCERR SDCRegisteredEventsList( unsigned long long *currentMask);

/*-------------------------------------------------------
SDCERR SDCDeRegisterEvents();

Synopsis:   Deregister events handler and stop SDK events.

return values:  SDCERR_SUCCESS if successful
				SDCERR_FAIL if internal err
---------------------------------------------------------*/
SDCERR SDCDeregisterEvents();
#endif

/*-------------------------------------------------------
SDCERR Validate_WEP_EAP_Combo();

Synopsis:       Verify combination of WEP_TYPE and EAP_TYPE is valid/

Input Arguments: WEPTYPE wepType
                 EAPTYPE eapType

Return Values:   SDCERR_SUCCESS if valid
                 SDCERR_INVALID_WEP_TYPE if wepType is invalid
                 SDCERR_INVALID_EAP_TYPE if eapType is invalid
                 SDCERR_INVALID_PARAMETER if invalid combination
---------------------------------------------------------*/
SDCERR Validate_WEP_EAP_Combo(WEPTYPE wt, EAPTYPE et);

/*---------------------------------------------------------
SDCERR SetUserCertPassword(SDCConfig * cfg,  char * userPswd );
Synopsis:	Sets the User Certificate Password

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* userPswd--null-terminated password up to USER_PWD_SZ characters

Return values:  SDCERR_INVALID_WEP_TYPE if wepType is not WEP_AUTO,WPA_TKIP,WPA2_AES,CCKM_TKIP, or WEP_AUTO_CKIP
				SDCERR_INVALID_EAP_TYPE if eapType is not EAP-PEAPTLS or EAP_EAPTTLS
				SDCERR_INVALID_PARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
*/
SDCERR SetUserCertPassword(SDCConfig * cfg,  char * userPswd );

/*---------------------------------------------------------
SDCERR GetUserCertPassword(SDCConfig * cfg,  char * userPswd );

Synopsis:	Retrieves the User Certificate Password.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* userPswd--pass in a valid pointer with an allocated buffer of at least USER_PWD_SZ characters

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALIDPARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR GetUserCertPassword(SDCConfig * cfg,  char * userPswd );

/*---------------------------------------------------------
SDCERR GetWAPICertCred(SDCConfig * cfg,  char * username, char* userCert,
						CERTLOCATION* certLocation, char* caCert);

Synopsis:	Retrieves the WAPI EAP credentials.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--pass in a valid pointer with an allocated buffer of at least USER_NAME_SZ characters
								if NULL, this parameter is ignored
                 char* userCert--pass in a valid pointer with an allocated buffer of at least 20 characters
								if NULL, this parameter is ignored
				 CERTLOCATION* CAcertLocation--pass in a valid pointer
								if NULL, this parameter is ignored
                 char* caCert--pass in a valid pointer with an allocated buffer of at least CRED_CERT_SZ characters
								if NULL, this parameter is ignored
								depending on the caCertLocation field, caCert will contain:
								if CERT_NONE, caCert is NULL - don't validate the server
								if CERT_FILE, caCert will specify the cert filename, up to CRED_CERT_SZ chars

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALIDPARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR GetWAPICertCred(SDCConfig * cfg,  char * username, char* userCert, CERTLOCATION* certLocation, char* caCert);

/*---------------------------------------------------------
SDCERR SetWAPICertCred(SDCConfig * cfg,  char * username, char* userCert,
						CERTLOCATION* certLocation, char* caCert);

Synopsis:	Sets the WAPI credentials.

Input arguments: SDCConfig* cfg--a valid configuration, required
                 char* username--pass in a valid pointer with an allocated buffer of at least USER_NAME_SZ characters
								if NULL, this parameter is ignored
                 char* userCert--pass in a valid pointer with an allocated buffer of at least CRED_CERT_SZ characters
								if NULL, this parameter is ignored
				 CERTLOCATION* CAcertLocation--pass in a valid pointer
								if NULL, this parameter is ignored
                 char* caCert--pass in a valid pointer with an allocated buffer of at least CRED_CERT_SZ characters
								if NULL, this parameter is ignored
								depending on the caCertLocation field, caCert will contain:
								if CERT_NONE, caCert is NULL - don't validate the server
								if CERT_FILE, caCert will specify the cert filename, up to CRED_CERT_SZ chars

Return values:  SDCERR_SUCCESS if successful
				SDCERR_INVALIDPARAMETER if an invalid parameter
				SDCERR_INVALID_CONFIG if an invalid config
				SDCERR_FAIL if other err
---------------------------------------------------------*/
SDCERR SetWAPICertCred(SDCConfig * cfg,  char * username, char* userCert, CERTLOCATION caCertLocation, char* caCert);

#ifdef __linux__
/*-------------------------------------------------------
SDCERR LRD_WF_GetPilInfo(LRD_WF_PilInfo *pil_info);

Synopsis: Returns the PIL library pil_info struct pointer

Input arguments - LRD_WF_pilInfo * pil_info - pointer to a LRD_WF_pilInfo
                                              structure.

Return Values:  SDCERR_SUCCESS if successful
				SDCERR_INVALID_PARAMETER if pil_info is NULL
				SDCERR_FAIL is no PIL is registered.
---------------------------------------------------------*/
SDCERR LRD_WF_GetPilInfo(LRD_WF_PilInfo *pil_info);

/*------------------------------------------------------------
SDCERR LRD_WF_GetDHCPLease(DHCP_LEASE *dhcpLease)

Synopsis: returns the current dhcp lease information for the wifi interface

input arguments: DHCP_LEASE *dhcpLease - will contain the structure filled with the
                                        current DHCP lease.
return values: SDCERR_SUCCESS - *dhcpLease has the current lease info
		       SDCERR_FAIL - unable to find current lease in file
		       SDCERR_INVALID_FILE - error opening leases file
		       SDCERR_INSUFFICIENT_MEMORY - error allocating memory
*/
SDCERR LRD_WF_GetDHCPLease(DHCP_LEASE *dhcpLease);

/*
	LRD_WF_GetFipsStatus() will return the status of fips based on the 
	current state the supplicant was started, as well as the state that is 
	set for the next invocation of wireless startup.

	Return values are SDCERR_SUCCESS, SDCERR_INVALID_PARAMETER, and
	SDCERR_FAIL

	*current (if not NULL) will have the current state of fips where 
	1 is enabled and 0 is disabled. (-1 is error)
	*nextStart (if not NULL) will have the current state of the next startup
	of the supplicant where 1 is enabled and 0 is disabled.

*/
SDCERR LRD_WF_GetFipsStatus(char * current, char * nextStart);
#endif //__linux__

#endif//NDIS_MINIPORT_DRIVER

#ifdef __cplusplus
}
#endif

#endif//_SDC_SDK_H_;
