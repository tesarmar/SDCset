// SDCset.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "pwinreg.h"
#include "Wincrypt.h"


extern "C" {
#include "sdc_sdk.h"
}


//	141020.rbl - added SDK ver reporting, rebuilt with new SDK version (4.1.1)

char chProgData[] = "Prog: SDCset";
char chAuthData[] = "Auth: Reid Ligon TSS";
char chModDate[] = __DATE__;
char chModTime[] = __TIME__;
char chFileName[] = __FILE__;


SDCConfig SDCcfg;
#define MAX_ARGS 40


WCHAR szSerialNumber[64];

int (*getSN)(LPWSTR, DWORD);

void Bologna()
{
	HINSTANCE hMod = LoadLibrary(L"\\Windows\\dl_api.dll");
	getSN = (int(*)(LPWSTR, DWORD)) GetProcAddress(hMod, L"DLDEVICE_GetSerialNumber");
	getSN(szSerialNumber, 64);
	FreeLibrary(hMod);
}

void Eugene()
{
	HINSTANCE hMod = LoadLibrary(L"\\Windows\\unitapi.dll");
	getSN = (int(*)(LPWSTR, DWORD)) GetProcAddress(hMod, L"UnitGetSerialNumber");
	getSN(szSerialNumber, 64);
	FreeLibrary(hMod);
}



// utility macro to convert a hexa digit into its value
#define HEX(c)  ((c)<='9'?(c)-'0':(c)<='F'?(c)-'A'+0xA:(c)-'a'+0xA)

int gRetStatus = 0;


DWORD atoh(char *str, byte *bptr) 
{
	unsigned int i, j, k;
	unsigned char ch;
	DWORD dwRet = 0;

	i = 0;
	k = 0;
	for (j = 0; j < strlen(str); j++) {
		ch = str[j];
		ch -= 0x30;
		if ( ch > 9 ) {
			ch &= 0x07;
			ch += 9;
		}
		i = (i << 4) + ch;
		// in case they want a dword rather than a byte buffer
		dwRet = (dwRet << 4) + ch;
		if ((j%2) == 1) {
			bptr[k] = i;
			i = 0;
			k++;
		}
	}
	return(dwRet );
}

int WasOption
// look for argument like '-t' or '/t'.
// returns option index
// returns index of argv[] found, -1 if not found
(
    IN int argc,       // number of args
    IN char* argv[],  // arg array
    IN char* sOption // to find ('t')
)
{
    for(int i=0; i<argc; i++)
    {
        if( ((*argv[i] == '-') || (*argv[i] == '/')) &&
            !_stricmp(argv[i]+1, sOption))
            return i;
    }
    return -1;
}   // WasOption()



int
GetOption
// look for argument like '-t 100' or '/t 100'.
// returns index of '100' if option ('t') is found
// returns -1 if not found
(
    int argc,                // number of args
    IN char* argv[],        // arg array
    IN char* psOption,     // to find ('n')
    OUT char** ppsArgument // option value ('100')
)
{
    if(!ppsArgument)
        return -1;

    int i = WasOption(argc, argv, psOption);
    if((i < 0) || ((i+1) >= argc))
    {
        *ppsArgument = NULL;
        return -1;
    }

    *ppsArgument = argv[i+1];
    return i+1;
}   // GetOption()



int FindSerialNumber(char *SNbuff)
{
	int iSNlen = 0;
	// get the serial number
	// is this a Bologna unit?
	if (GetFileAttributes(L"\\Windows\\dl_api.dll") != 0xFFFFFFFF) {
		Bologna();
	}else{
		Eugene();
	}
	wcstombs(SNbuff, szSerialNumber, 64);
	iSNlen = wcslen(szSerialNumber);
	return iSNlen;
}


void DoRadio(IN int argc, IN char* argv[]){
	char *sRadOpt = NULL;
	BOOL bOn;
	SDCGlobalConfig SDCgcfg;

	if (GetOption(argc, argv, "radio", &sRadOpt) < 0) {
		// no options, just display the current status
		if (GetGlobalSettings(&SDCgcfg) != SDCERR_SUCCESS) {
			printf("Error getting global\n   configuration.\n");
			return;
		}
		bOn = SDCgcfg.radioState;
		if (bOn) {
			printf("Radio is on\n");
			gRetStatus = 0;
		}else{
			printf("Radio is off\n");
			gRetStatus = 1;
		}
	}else if (!_stricmp(sRadOpt, "on")) {
		// turn on the radio
		RadioEnable();
		gRetStatus = 0;
	}else if (!_stricmp(sRadOpt, "off")) {
		// turn off the radio
		RadioDisable();
		gRetStatus = 0;
	}else{
		printf("Unknown radio mode requested.\n");
		gRetStatus = 1;
	}
	return;
}

void QueryIP(IN int argc, IN char* argv[]){
	CF10G_STATUS status;

	if (GetCurrentStatus(&status) != SDCERR_SUCCESS) {
		printf("Error getting current\n   status.\n");
		return;
	}

	switch(status.cardState) {
		case CARDSTATE_NOT_INSERTED: printf("Card not inserted\n"); return; break;
		case CARDSTATE_NOT_ASSOCIATED: printf("Card not associated\n"); break;
		case CARDSTATE_ASSOCIATED: printf("Card associated\n"); break;
		case CARDSTATE_AUTHENTICATED: printf("Card authenticated\n"); break;
		case CARDSTATE_FCCTEST: printf("Card in FCC test\n"); return; break;
		case CARDSTATE_NOT_SDC: printf("Card not SDC\n"); return; break;
		default: printf("Unknown card state\n"); return; break;

	}

	printf("Client:\n");
	printf("   IP: %3d.%3d.%3d.%3d\n", 
		status.client_IP[0], status.client_IP[1],
		status.client_IP[1], status.client_IP[3]);
	printf("   MAC: %02.02X:%02.02X:%02.02X:%02.02X:%02.02X:%02.02X\n",
		status.client_MAC[0], status.client_MAC[1],
		status.client_MAC[2], status.client_MAC[3],
		status.client_MAC[4], status.client_MAC[5]);
	printf("   Name: %s\n", status.configName);

	if ((status.cardState == CARDSTATE_ASSOCIATED) || (
		status.cardState == CARDSTATE_AUTHENTICATED)) {
		printf("AP:\n");
		printf("   IP: %3d.%3d.%3d.%3d\n", 
			status.AP_IP[0], status.AP_IP[1],
			status.AP_IP[2], status.AP_IP[3]);
		printf("   MAC: %02.02X:%02.02X:%02.02X:%02.02X:%02.02X:%02.02X\n",
			status.AP_MAC[0], status.AP_MAC[1],
			status.AP_MAC[2], status.AP_MAC[3],
			status.AP_MAC[4], status.AP_MAC[5]);
		printf("   Name: %s\n", status.APName); 
	}

	return;
}


void ConfigActivate(char *sConfigName){
	SDCERR iRet;

	gRetStatus = 0;
	iRet = ActivateConfig(sConfigName);
	switch(iRet) {
		case SDCERR_SUCCESS: printf("Configuration activated\n"); break;
		case SDCERR_INVALID_NAME: printf("Invalid configuration name\n"); break;
		case SDCERR_POWERCYCLE_REQUIRED: printf("Configuration activated,\n   power cycle required\n"); break;
		default: printf("Error activating configuration\n"); break;
	}

	return;
}

void ConfigDelete(char* sConfigName){
	SDCERR iRet;

	gRetStatus = 0;
	iRet = DeleteConfig(sConfigName);
	switch(iRet) {
		case SDCERR_SUCCESS: printf("Configuration deleted\n"); break;
		case SDCERR_INVALID_NAME: printf("Invalid configuration name\n"); break;
		case SDCERR_INVALID_DELETE: printf("Attempt to delete the\n   active configuration rejected\n"); break;
		default: printf("Error deleting configuration\n"); break;
	}

	return;
}

// update the wep key structure.  process hex to ascii if needed.  return validity of the key
bool UpdateWEP(WEPKey *wKey, char *sWEPkey)
{
	int iWEPlen;
	bool bRet = TRUE;
	byte byBuff[32];

	iWEPlen = strlen(sWEPkey);
	switch(iWEPlen) {
		case 10:
			// 40 bit in hex, convert it down
			atoh(sWEPkey, byBuff);
			wKey->length = WEPLEN_40BIT;
			memmove(&wKey->data[0], byBuff, 5);
			break;
		case 5:
			wKey->length = WEPLEN_40BIT;
			memmove(&wKey->data[0], sWEPkey, 5);
			break;
		case 26:
			// 128 bit in hex, convert it down
			atoh(sWEPkey, byBuff);
			wKey->length = WEPLEN_128BIT;
			memmove(&wKey->data[0], byBuff, 13);
			break;
		case 13:
			wKey->length = WEPLEN_128BIT;
			memmove(&wKey->data[0], sWEPkey, 13);
			break;
		default:
			printf("Invalid WEP key\n   %s\n", sWEPkey);
			bRet = FALSE;
			break;
	}
	return bRet;
}

void GetCertHash(BOOL bUserCert, char *chCert, char *hash)
{
	HCERTSTORE hSysStore = NULL;
	PCCERT_CONTEXT pCertContext = NULL;
	WCHAR szFriendly[256];
	char  chFriendly[256];
	DWORD dwData;
	BYTE  hashToAdd[20];

	// did they specify a user cert?
	if ((chCert != NULL) && (chCert[0] != 0x00)) {
		// yes, look up the hash for it
		if (bUserCert) {
			hSysStore = CertOpenSystemStore( 0, L"MY" );
		}else{
			hSysStore = CertOpenSystemStore(0, L"Root");
		}
		if (hSysStore) {
			while (pCertContext = CertEnumCertificatesInStore(hSysStore, pCertContext)) {
				if (bUserCert) {
					CertGetNameString( pCertContext, CERT_NAME_FRIENDLY_DISPLAY_TYPE, 0, NULL, szFriendly, 256);
				}else{
					CertGetNameString( pCertContext, CERT_NAME_FRIENDLY_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, szFriendly, 256);
				}
				wcstombs(chFriendly, szFriendly, 256);
				// is this the cert we are looking for?
				if (strcmp(chFriendly, chCert) == 0) {
					CertGetCertificateContextProperty(pCertContext, CERT_HASH_PROP_ID, &hashToAdd, &dwData);
					memcpy((BYTE *)hash, hashToAdd, 20);
					break;
				}
			}
			CertFreeCertificateContext(pCertContext);
			CertCloseStore(hSysStore, 0);
		}
	}

	return;
}

void SetEAPCert(SDCConfig *SDCcfg, char *eapUser, char *eapPassword, char *eapCACert, char *eapUserCert, BOOL eapValidateServer, BOOL eapUseMSStore,
				char *eapPAC, char *eapPACPassword)
{
	char caCert[129];
	char userCert[129];
	SDCERR dwSDCerr;
	CERTLOCATION cLoc = CERT_NONE;
	// assume using the name as is
	if (eapCACert != NULL) {
		strcpy(caCert, eapCACert);
	}
	if (eapValidateServer) {
		if (eapUseMSStore) {
			if (eapCACert == NULL) {
				cLoc = CERT_FULL_STORE;
			}else{
				// go find the specified certificate and get it's hash
				GetCertHash(FALSE, eapCACert, caCert);
				cLoc = CERT_IN_STORE;
			}
		}else{
			cLoc = CERT_FILE;
		}
	}

	switch(SDCcfg->eapType) {
		case EAP_NONE:
			return;
			break;
		case EAP_LEAP:
			dwSDCerr = SetLEAPCred(SDCcfg, eapUser, eapPassword);
			break;
		case EAP_EAPFAST:
			dwSDCerr = SetEAPFASTCred(SDCcfg, eapUser, eapPassword, eapPAC, eapPACPassword);
			break;
		case EAP_PEAPMSCHAP:
			dwSDCerr = SetPEAPMSCHAPCred(SDCcfg, eapUser, eapPassword, cLoc, caCert);
			break;
		case EAP_PEAPGTC:	
			dwSDCerr = SetPEAPGTCCred(SDCcfg, eapUser, eapPassword, cLoc, caCert);
			break;
		case EAP_EAPTLS:
			if (eapUserCert != NULL) {
				GetCertHash(TRUE, eapUserCert, userCert);
				dwSDCerr = SetEAPTLSCred(SDCcfg, eapUser, userCert, cLoc, caCert);
			}else{
				dwSDCerr = SetEAPTLSCred(SDCcfg, eapUser, userCert, cLoc, caCert);
			}
			break;
		case EAP_EAPTTLS:
			dwSDCerr = SetEAPTTLSCred(SDCcfg, eapUser, eapPassword, cLoc, caCert);
			break;
/***
case EAP_PEAPTLS:
			if (eapUserCert != NULL) {
				GetCertHash(TRUE, eapUserCert, userCert);
				dwSDCerr = SetPEAPTLSCred(SDCcfg, eapUser, userCert, cLoc, caCert);
			}else{
				dwSDCerr = SetPEAPTLSCred(SDCcfg, eapUser, NULL, cLoc, caCert);
			}
			break;
***/
		default:
			return;
	}
	if (dwSDCerr != SDCERR_SUCCESS) {
		printf("set EAP fail %d\n", dwSDCerr);
	}

	return;
}

void SetUserCert(SDCConfig *SDCcfg, char *sUser, char *uCert, char *sCert) {
	HCERTSTORE hSysStore = NULL;
	PCCERT_CONTEXT pCurrCertContext = NULL;
	PCCERT_CONTEXT pPrevCertContext = NULL;
	WCHAR szFriendly[256];
	char  chFriendly[256];
	DWORD dwData;
	DWORD dwPropId;
	BYTE  bKeyUsage[32];
	char *pbUserHash = NULL;

	// did they specify a user cert?
	if ((uCert != NULL) && (uCert[0] != 0x00)) {
		// yes, look up the hash for it
		if(hSysStore = CertOpenSystemStore( 0, L"MY" ))
		{
			do {
				pCurrCertContext = CertEnumCertificatesInStore(hSysStore, pPrevCertContext);
				pPrevCertContext = pCurrCertContext;
				if (pCurrCertContext != NULL) {
					CertGetNameString( pCurrCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, szFriendly, 256);
					wcstombs(chFriendly, szFriendly, 256);
					if (strcmp(chFriendly, uCert) == 0) {
						dwPropId = 0;
						while (dwPropId = CertEnumCertificateContextProperties( pCurrCertContext, dwPropId)) {
							switch(dwPropId) {
								case CERT_SHA1_HASH_PROP_ID:
									dwData = 32;
									CertGetCertificateContextProperty(pCurrCertContext, dwPropId, bKeyUsage, &dwData);
									pbUserHash = (char *) bKeyUsage;
									break;
								default:
									break;
							}
						}
					}
				}
			} while (pCurrCertContext != NULL);
			CertCloseStore(hSysStore, 0);
		}
	}

	// did they set the server certificate?
	SDCERR sdcError;

	if ((sCert != NULL) && (sCert[0] != 0x00)) {
		sdcError = SetEAPTLSCred(SDCcfg, sUser, pbUserHash, CERT_FILE, sCert);
	}else{
		sdcError = SetEAPTLSCred(SDCcfg, sUser, pbUserHash, CERT_NONE, NULL);
	}
	return;
}


void ConfigAdd(IN int argc, IN char* argv[], char* sConfigName){
	int iErrCode;

	gRetStatus = 0;
	SDCConfig SDCcfg;

	if (GetConfig(sConfigName, &SDCcfg) != SDCERR_SUCCESS) {
		// we didn't find it, set the values to defaults
		memset(&SDCcfg, 0x00, sizeof(SDCConfig));
		strcpy(SDCcfg.configName, sConfigName);
		strcpy(SDCcfg.SSID, "tsunami");
		strcpy(SDCcfg.clientName, "");
		SDCcfg.txPower = TXPOWER_MAX;
		SDCcfg.authType = AUTH_OPEN;
		SDCcfg.eapType = EAP_NONE;
		SDCcfg.powerSave = POWERSAVE_FAST;
		SDCcfg.wepType = WEP_OFF;
		SDCcfg.bitRate = BITRATE_AUTO;
		SDCcfg.radioMode = RADIOMODE_BG;
		SDCcfg.userName.size = 0;
		SDCcfg.userPwd.size = 0;
		SDCcfg.PSK.size = 0;
		SDCcfg.WEPKeys.size = 0;
		AddConfig(&SDCcfg);
	}

//	// allow adhoc mode - NOT IN SUMMIT?????
//	if (WasOption(argc, argv, L"adhoc") >= 0) {
//	}

	// selected power mode?
	char* sPwrMode = NULL;
	if (GetOption(argc, argv, "pwr", &sPwrMode) >= 0) {
        if(!_stricmp(sPwrMode, "cam")) {
			SDCcfg.powerSave = POWERSAVE_OFF;
		} else if (!_stricmp(sPwrMode, "fast")) {
			SDCcfg.powerSave = POWERSAVE_FAST;
		} else if (!_stricmp(sPwrMode, "max")) {
			SDCcfg.powerSave = POWERSAVE_MAX;
        } else{
            printf("unknown power mode,\nassuming 'fast-psp'\n");
			SDCcfg.powerSave = POWERSAVE_FAST;
		}
    }

	// auth mode?
	char* sAuthMode = NULL;
	if (GetOption(argc, argv, "auth", &sAuthMode) >= 0) {
        if(!_stricmp(sAuthMode, "open")) {
			SDCcfg.authType = AUTH_OPEN;
		} else if (!_stricmp(sAuthMode, "shared")) {
			SDCcfg.authType = AUTH_SHARED;
		} else if (!_stricmp(sAuthMode, "leap")) {
			SDCcfg.authType = AUTH_NETWORK_EAP;
        } else{
            printf("unknown auth mode,\nassuming 'open'\n");
			SDCcfg.authType = AUTH_OPEN;
		}
    }

	// security mode
	char* sSecMode = NULL;
	if (GetOption(argc, argv, "eap", &sSecMode) >= 0) {
        if(!_stricmp(sSecMode, "none")) {
			SDCcfg.eapType = EAP_NONE;
		} else if (!_stricmp(sSecMode, "leap")) {
			SDCcfg.eapType = EAP_LEAP;
		} else if (!_stricmp(sSecMode, "eapfast")) {
			SDCcfg.eapType = EAP_EAPFAST;
		} else if (!_stricmp(sSecMode, "peapmschap")) {
			SDCcfg.eapType = EAP_PEAPMSCHAP;
		} else if (!_stricmp(sSecMode, "peapgtc")) {
			SDCcfg.eapType = EAP_PEAPGTC;
		} else if (!_stricmp(sSecMode, "eaptls")) {
			SDCcfg.eapType = EAP_EAPTLS;
		} else if (!_stricmp(sSecMode, "eapttls")) {
			SDCcfg.eapType = EAP_EAPTTLS;
		} else if (!_stricmp(sSecMode, "peaptls")) {
			SDCcfg.eapType = EAP_PEAPTLS;
		} else{
            printf("unknown eap mode,\nassuming 'none'\n");
			SDCcfg.eapType = EAP_NONE;
		}
    }


	// security mode (wep/wpa/etc)?
	char* sWEPMode = NULL;
	if (GetOption(argc, argv, "encrypt", &sWEPMode) >= 0) {
        if(!_stricmp(sWEPMode, "none")) {
			SDCcfg.wepType = WEP_OFF;
		} else if (!_stricmp(sWEPMode, "dynamic")) {
			SDCcfg.wepType = WEP_AUTO;
		} else if (!_stricmp(sWEPMode, "static")) {
			//clear any existing keys
			memset(SDCcfg.WEPKeys.buffer, 0x00, 120);
			SDCcfg.wepType = WEP_ON;
		} else if (!_stricmp(sWEPMode, "wpapsk")) {
			SDCcfg.wepType = WPA_PSK;
		} else if (!_stricmp(sWEPMode, "wpatkip")) {
			SDCcfg.wepType = WPA_TKIP;
		} else if (!_stricmp(sWEPMode, "wpa2psk")) {
			SDCcfg.wepType = WPA2_PSK;
		} else if (!_stricmp(sWEPMode, "wpa2aes")) {
			SDCcfg.wepType = WPA2_AES;
		} else if (!_stricmp(sWEPMode, "cckmtkip")) {
			SDCcfg.wepType = CCKM_TKIP;
		} else if (!_stricmp(sWEPMode, "ckip")) {
			SDCcfg.wepType = WEP_CKIP;
		} else if (!_stricmp(sWEPMode, "autockip")) {
			SDCcfg.wepType = WEP_AUTO_CKIP;
		} else if (!_stricmp(sWEPMode, "cckmaes")) {
			SDCcfg.wepType = CCKM_AES;
        } else{
            printf("unknown WEP mode,\nassuming 'none'\n");
			SDCcfg.wepType = WEP_OFF;
		}
	}

	char* sWEPkey = NULL;
	WEPKey *wKey;
	int iKeyNum;
	if (SDCcfg.wepType != WEP_ON) {
		// not static, clear the keys
		SDCcfg.WEPKeys.size = 0;
		memset(SDCcfg.WEPKeys.buffer, 0x00, 120);
	}else{
		// static WEP, set some keys here
		SDCcfg.WEPKeys.size = 0;
		if (GetOption(argc, argv, "tx", &sWEPkey) >= 0) {
			iKeyNum = atoi(sWEPkey);
		}else{
			iKeyNum = 1;
		}
		if (GetOption(argc, argv, "key1", &sWEPkey) >= 0) {
			wKey = (WEPKey *) &SDCcfg.WEPKeys.buffer[SDCcfg.WEPKeys.size];
			UpdateWEP(wKey, sWEPkey);
			SDCcfg.WEPKeys.size += sizeof(WEPKey);
			if (iKeyNum == 1) {
				wKey->length |= XMITBIT;
			}
		}
		if (GetOption(argc, argv, "key2", &sWEPkey) >= 0) {
			wKey = (WEPKey *) &SDCcfg.WEPKeys.buffer[SDCcfg.WEPKeys.size];
			UpdateWEP(wKey, sWEPkey);
			SDCcfg.WEPKeys.size += sizeof(WEPKey);
			if (iKeyNum == 2) {
				wKey->length |= XMITBIT;
			}
		}
		if (GetOption(argc, argv, "key3", &sWEPkey) >= 0) {
			wKey = (WEPKey *) &SDCcfg.WEPKeys.buffer[SDCcfg.WEPKeys.size];
			UpdateWEP(wKey, sWEPkey);
			SDCcfg.WEPKeys.size += sizeof(WEPKey);
			if (iKeyNum == 3) {
				wKey->length |= XMITBIT;
			}
		}
		if (GetOption(argc, argv, "key4", &sWEPkey) >= 0) {
			wKey = (WEPKey *) &SDCcfg.WEPKeys.buffer[SDCcfg.WEPKeys.size];
			UpdateWEP(wKey, sWEPkey);
			SDCcfg.WEPKeys.size += sizeof(WEPKey);
			if (iKeyNum == 4) {
				wKey->length |= XMITBIT;
			}
		}
	}

	// hopefully we have a ssid
	char* sSSID = NULL;
	if (GetOption(argc, argv, "ssid", &sSSID) >= 0) {
		strcpy(SDCcfg.SSID, sSSID);
	}

	// did they define a client name?
	char* sCName = NULL;
	if (GetOption(argc, argv, "client", &sCName) >= 0) {
		strcpy(SDCcfg.clientName, sCName);
	}


	// user id?
	char* sUser = NULL;
	if (GetOption(argc, argv, "username", &sUser) >= 0) {
		SDCcfg.userName.size = strlen(sUser);
		strcpy(SDCcfg.userName.buffer, sUser);
	}

	// user password?
	char* sUPwd = NULL;
	if (GetOption(argc, argv, "userpwd", &sUPwd) >= 0) {
		SDCcfg.userPwd.size = strlen(sUPwd);
		strcpy(SDCcfg.userPwd.buffer, sUPwd);
	}


	// WPA-PSK mode?
	char* sPSK = NULL;
	if (GetOption(argc, argv, "psk", &sPSK) >= 0) {
		// OK, we got a PSK so we need to set it
		SDCcfg.PSK.size = strlen(sPSK);
		strcpy(SDCcfg.PSK.buffer, sPSK);
	}

/***
	char* sCert = NULL;
	if (GetOption(argc, argv, "cert", &sCert) >= 0) {
		strcpy(&SDCcfg.userName.buffer[72], sCert);
	}
***/

	char* sRadioMode = NULL;
	if (GetOption(argc, argv, "rfmode", &sRadioMode) >= 0) {
		if (!_stricmp("b_only", sRadioMode)) {
			SDCcfg.radioMode = RADIOMODE_B_ONLY;
		}else if (!_stricmp("bg", sRadioMode)) {
			SDCcfg.radioMode =   RADIOMODE_BG;
		}else if (!_stricmp("g_only", sRadioMode)) {
			SDCcfg.radioMode = RADIOMODE_G_ONLY;
		}else if (!_stricmp("bg_subset", sRadioMode)) {
			SDCcfg.radioMode = RADIOMODE_BG_LRS;
		}else if (!_stricmp("a_only", sRadioMode)) {
			SDCcfg.radioMode = RADIOMODE_A_ONLY;
		}else if (!_stricmp("abg", sRadioMode)) {
			SDCcfg.radioMode = RADIOMODE_ABG;
		}else if (!_stricmp("bga", sRadioMode)) {
			SDCcfg.radioMode = RADIOMODE_BGA;
		}else if (!_stricmp("adhoc", sRadioMode)) {
			SDCcfg.radioMode = RADIOMODE_ADHOC;
		}else if (!_stricmp("gn", sRadioMode)) {
			SDCcfg.radioMode = RADIOMODE_GN;
		}else if (!_stricmp("an", sRadioMode)) {
			SDCcfg.radioMode = RADIOMODE_AN;
		}else if (!_stricmp("abgn", sRadioMode)) {
			SDCcfg.radioMode = RADIOMODE_ABGN;
		}else if (!_stricmp("bgan", sRadioMode)) {
			SDCcfg.radioMode = RADIOMODE_BGAN;
		}else if (!_stricmp("bgn", sRadioMode)) {
			SDCcfg.radioMode = RADIOMODE_BGN;
		}else{
            printf("unknown radio mode,\nassuming 'bg'\n");
			SDCcfg.radioMode = RADIOMODE_BG;
		}
	}

	char* sTxPower = NULL;
	if (GetOption(argc, argv, "txpwr", &sTxPower) >= 0) {
		if (!_stricmp("max", sTxPower)) {
			SDCcfg.txPower = TXPOWER_MAX;
		} else if (!_stricmp("50", sTxPower)) {
			SDCcfg.txPower = TXPOWER_50;
		} else if (!_stricmp("30", sTxPower)) {
			SDCcfg.txPower = TXPOWER_30;
		} else if (!_stricmp("20", sTxPower)) {
			SDCcfg.txPower = TXPOWER_20;
		} else if (!_stricmp("10", sTxPower)) {
			SDCcfg.txPower = TXPOWER_10;
		} else if (!_stricmp("5", sTxPower)) {
			SDCcfg.txPower = TXPOWER_5;
		} else if (!_stricmp("1", sTxPower)) {
			SDCcfg.txPower = TXPOWER_1;
		} else {
			printf("unknown tx power.\nassuming max\n");
			SDCcfg.txPower = TXPOWER_MAX;
		}
	}

	char *sBitRate = NULL;
	if (GetOption(argc, argv, "brate", &sBitRate) >= 0) {
		if (!_stricmp("auto", sBitRate)) {
			SDCcfg.bitRate = BITRATE_AUTO;
		} else if (!_stricmp("1", sBitRate)) {
			SDCcfg.bitRate = BITRATE_1;
		} else if (!_stricmp("2", sBitRate)) {
			SDCcfg.bitRate = BITRATE_2;
		} else if (!_stricmp("5.5", sBitRate)) {
			SDCcfg.bitRate = BITRATE_5_5;
		} else if (!_stricmp("6", sBitRate)) {
			SDCcfg.bitRate = BITRATE_6;
		} else if (!_stricmp("9", sBitRate)) {
			SDCcfg.bitRate = BITRATE_9;
		} else if (!_stricmp("11", sBitRate)) {
			SDCcfg.bitRate = BITRATE_11;
		} else if (!_stricmp("12", sBitRate)) {
			SDCcfg.bitRate = BITRATE_12;
		} else if (!_stricmp("18", sBitRate)) {
			SDCcfg.bitRate = BITRATE_18;
		} else if (!_stricmp("24", sBitRate)) {
			SDCcfg.bitRate = BITRATE_24;
		} else if (!_stricmp("36", sBitRate)) {
			SDCcfg.bitRate = BITRATE_36;
		} else if (!_stricmp("48", sBitRate)) {
			SDCcfg.bitRate = BITRATE_48;
		} else if (!_stricmp("54", sBitRate)) {
			SDCcfg.bitRate = BITRATE_54;
		} else {
			printf("unknown bit rate.\nassuming auto\n");
			SDCcfg.bitRate = BITRATE_AUTO;
		}
	}



	SDCcfg.WEPKeys.size = 0;

	// OLD CODE, USE SetEAPCert BELOW GOING FORWARD
	// allow setting a user certificate and/or server certificate
	char *uCert = NULL;
	GetOption(argc, argv, "ucert", &uCert);
	char *sCert = NULL;
	GetOption(argc, argv, "cert", &sCert);

	if ((uCert != NULL) || (sCert != NULL)) {
		SetUserCert(&SDCcfg, sUser, uCert, sCert);
	}

	BOOL bSetEAP = FALSE;
	char *eapUser = NULL;
	if (GetOption(argc, argv, "eapuser", &eapUser) >= 0) bSetEAP = TRUE;
	char *eapPassword = NULL;
	if (GetOption(argc, argv, "eappassword", &eapPassword) >= 0) bSetEAP = TRUE;
	char *eapCACert = NULL;
	if (GetOption(argc, argv, "eapcacert", &eapCACert) >= 0) bSetEAP = TRUE;
	char *eapUserCert = NULL;
	if (GetOption(argc, argv, "eapusercert", &eapUserCert) >= 0) bSetEAP = TRUE;
	char *eapPAC = NULL;
	if (GetOption(argc, argv, "eappac", &eapPAC) >= 0) bSetEAP = TRUE;
	char *eapPACpassword = NULL;
	if (GetOption(argc, argv, "eappacpassword", &eapPACpassword) >= 0) bSetEAP = TRUE;
	BOOL eapValidateServer = FALSE;
	char *tmpBuff = NULL;
	if (GetOption(argc, argv, "eapvalidateserver", &tmpBuff) >= 0) {
		eapValidateServer = (strcmp(tmpBuff, "on") == 0);
	}
	BOOL eapUseMSStore = FALSE;
	if (GetOption(argc, argv, "eapusemsstore", &tmpBuff) >= 0) {
		eapUseMSStore = (strcmp(tmpBuff, "on") == 0);
	}

	if (bSetEAP) {
		SetEAPCert(&SDCcfg, eapUser, eapPassword, eapCACert, eapUserCert, eapValidateServer, eapUseMSStore, eapPAC, eapPACpassword);
	}

	iErrCode = ModifyConfig( SDCcfg.configName, &SDCcfg );
	switch(iErrCode) {
		case SDCERR_SUCCESS: printf("Config updated\n"); break;
		case SDCERR_INVALID_NAME: printf("Invalid Config name\n"); break;
		case SDCERR_INVALID_CONFIG: printf("Invalid Config data\n"); break;
		default: printf("Error updating Config\n"); break;
	}

	return;
}


void DoConfig(IN int argc, IN char* argv[]){
	char *sConfigName = NULL;
	char CfgName[CONFIG_NAME_SZ];

	if (GetOption(argc, argv, "active", &sConfigName) >= 0) {
		// activate command
		ConfigActivate(sConfigName);
	}else if (GetOption(argc, argv, "delete", &sConfigName) >= 0) {
		ConfigDelete(sConfigName);
	}else if (GetOption(argc, argv, "add", &sConfigName) >= 0) {
		ConfigAdd(argc, argv, sConfigName);
	}else{
		// find the name of the current configuration
		if (GetCurrentConfig(NULL, CfgName) == SDCERR_SUCCESS) {
			printf("Current config is \n   %s\n", CfgName);
			gRetStatus = 1;
		}else{
			wprintf(L"Unknown config command\n");
			gRetStatus = 0;
		}
	}

	return;
}

void DoPowerMode(IN int argc, IN char* argv[])
{
	char *sPwrMode = NULL;
	WORD PwrMode = 0;
	char CfgName[CONFIG_NAME_SZ + 2];



	if (GetOption(argc, argv, "power", &sPwrMode) >= 0) {

		// find the name of the current configuration
		if (GetCurrentConfig(NULL, CfgName) != SDCERR_SUCCESS) {
			printf("Error finding current\n   configuration.\n");
			return;
		}
		// now fill the structure with the current settings
		if (GetConfig(CfgName, &SDCcfg) != SDCERR_SUCCESS) {
			printf("Error reading current\n   configuration.\n");
			return;
		}

        if(!_stricmp(sPwrMode, "cam")) {
			SDCcfg.powerSave = POWERSAVE_OFF;
		} else if (!_stricmp(sPwrMode, "fast")) {
			SDCcfg.powerSave = POWERSAVE_FAST;
		} else if (!_stricmp(sPwrMode, "max")) {
			SDCcfg.powerSave = POWERSAVE_MAX;
		} else {
			printf("Unknown power mode\n");
			return;
		}

		// update the power mode to the new setting
		if (ModifyConfig(CfgName, &SDCcfg) != SDCERR_SUCCESS) {
			printf("Error updating current\n   configuration.\n");
			return;
		}
    }

	return;
}

void DoGlobal(IN int argc, IN char* argv[])
{
	SDCERR sdcRet;
	SDCGlobalConfig GCfg;

	if (GetGlobalSettings(&GCfg) != SDCERR_SUCCESS) {
		printf("Error retrieving global\n   configuration\n");
		return;
	}


	char* sTrig;
	if (GetOption(argc, argv, "roam_trig", &sTrig) >= 0) {
		if (!_stricmp("50", sTrig)) {
			GCfg.roamTrigger = RTRIG_50;
		}else if (!_stricmp("55", sTrig)) {
			GCfg.roamTrigger = RTRIG_55;
		}else if (!_stricmp("60", sTrig)) {
			GCfg.roamTrigger = RTRIG_60;
		}else if (!_stricmp("65", sTrig)) {
			GCfg.roamTrigger = RTRIG_65;
		}else if (!_stricmp("70", sTrig)) {
			GCfg.roamTrigger = RTRIG_70;
		}else if (!_stricmp("75", sTrig)) {
			GCfg.roamTrigger = RTRIG_75;
		}else if (!_stricmp("80", sTrig)) {
			GCfg.roamTrigger = RTRIG_80;
		}else if (!_stricmp("85", sTrig)) {
			GCfg.roamTrigger = RTRIG_85;
		}else if (!_stricmp("90", sTrig)) {
			GCfg.roamTrigger = RTRIG_90;
		}else{
			printf("Invalid trigger,\n   assuming -70dBm\n");
			GCfg.roamTrigger = RTRIG_70;
		}
	}

	char* sDelta;
	if (GetOption(argc, argv, "roam_delta", &sDelta) >= 0) {
		if (!_stricmp("5", sDelta)) {
			GCfg.roamDelta = RDELTA_5;
		}else if (!_stricmp("10", sDelta)) {
			GCfg.roamDelta = RDELTA_10;
		}else if (!_stricmp("15", sDelta)) {
			GCfg.roamDelta = RDELTA_15;
		}else if (!_stricmp("20", sDelta)) {
			GCfg.roamDelta = RDELTA_20;
		}else if (!_stricmp("25", sDelta)) {
			GCfg.roamDelta = RDELTA_25;
		}else if (!_stricmp("30", sDelta)) {
			GCfg.roamDelta = RDELTA_30;
		}else if (!_stricmp("35", sDelta)) {
			GCfg.roamDelta = RDELTA_35;
		}else{
			printf("Invalid delta,\n   assuming -15dBm\n");
			GCfg.roamDelta = RDELTA_15;
		}
	}

	char* sPeriod;
	if (GetOption(argc, argv, "roam_per", &sPeriod) >= 0) {
		if (!_stricmp("5", sPeriod)) {
			GCfg.roamPeriod = RPERIOD_5;
		}else if (!_stricmp("10", sPeriod)) {
			GCfg.roamPeriod = RPERIOD_10;
		}else if (!_stricmp("15", sPeriod)) {
			GCfg.roamPeriod = RPERIOD_15;
		}else if (!_stricmp("20", sPeriod)) {
			GCfg.roamPeriod = RPERIOD_20;
		}else if (!_stricmp("25", sPeriod)) {
			GCfg.roamPeriod = RPERIOD_25;
		}else if (!_stricmp("30", sPeriod)) {
			GCfg.roamPeriod = RPERIOD_30;
		}else if (!_stricmp("35", sPeriod)) {
			GCfg.roamPeriod = RPERIOD_35;
		}else if (!_stricmp("40", sPeriod)) {
			GCfg.roamPeriod = RPERIOD_40;
		}else if (!_stricmp("45", sPeriod)) {
			GCfg.roamPeriod = RPERIOD_45;
		}else if (!_stricmp("50", sPeriod)) {
			GCfg.roamPeriod = RPERIOD_50;
		}else if (!_stricmp("55", sPeriod)) {
			GCfg.roamPeriod = RPERIOD_55;
		}else if (!_stricmp("60", sPeriod)) {
			GCfg.roamPeriod = RPERIOD_60;
		}else{
			printf("Invalid period,\b   assuming 10 sec\n");
			GCfg.roamPeriod = RPERIOD_10;
		}
	}

	char* sDFSchan;
	if (GetOption(argc, argv, "dfs_chan", &sDFSchan) >= 0) {
		if (!_stricmp("on", sDFSchan)) {
			GCfg.DFSchannels = 1;
		} else if (!_stricmp("off", sDFSchan)) {
			GCfg.DFSchannels = 0;
		}else{
			printf("Unknown DFS option\nassuming off\n");
			GCfg.DFSchannels = 0;
		}

		GCfg.DFSchannels = atoi(sDFSchan);
	}

	char* sAggressive = NULL;
	if ((GetOption(argc, argv, "agressive", &sAggressive) >= 0) || (GetOption(argc, argv, "aggressive", &sAggressive) >= 0)) {
		if (!_stricmp("on", sAggressive)) {
			GCfg.aggScanTimer = 1;
		} else if (!_stricmp("off", sAggressive)) {
			GCfg.aggScanTimer = 0;
		}else{
			printf("Unknown aggressive option\nassuming off\n");
			GCfg.aggScanTimer = 0;
		}
	}

	char* sPMKcaching = NULL;
	if (GetOption(argc, argv, "pmk", &sPMKcaching) >= 0){
		if (!_stricmp("std", sPMKcaching)) {
			GCfg.PMKcaching = 0;
		} else if (!_stricmp("opp", sPMKcaching)) {
			GCfg.PMKcaching = 1;
		}else{
			printf("Unknown PMK caching option\nassuming std\n");
			GCfg.PMKcaching = 0;
		}
	}



	char* sCCX;
	if (GetOption(argc, argv, "ccx", &sCCX) >= 0) {
		if (!_stricmp("on", sCCX)) {
			GCfg.CCXfeatures = CCX_FULL;
		}else if (!_stricmp("off", sCCX)) {
			GCfg.CCXfeatures = CCX_OFF;
		}else if (!_stricmp("optimized", sCCX)) {
			GCfg.CCXfeatures = CCX_OPTIMIZED;
		}else{
			printf("Unknown CCX option\n   assuming 'off'\n");
			GCfg.CCXfeatures = 0;
		}
	}

	char* sWMM;
	if (GetOption(argc, argv, "wmm", &sWMM) >= 0) {
		if (!_stricmp("on", sWMM)) {
			GCfg.WMEenabled = 1;
		}else if (!_stricmp("off", sWMM)) {
			GCfg.WMEenabled = 0;
		}else{
			printf("Unknown WMM option\n   assuming 'off'\n");
			GCfg.WMEenabled = 0;
		}
	}

	char *sAuthSrvr;
	if (GetOption(argc, argv, "auth_srvr", &sAuthSrvr) >= 0) {
		if (!_stricmp("1", sAuthSrvr)) {
			GCfg.authServerType = 0;
		} else if (!_stricmp("2", sAuthSrvr)) {
			GCfg.authServerType = 1;
		} else {
			printf("Unknown Auth Srvr\nassuming type 1\n");
			GCfg.authServerType = 0;
		}
	}


	char* sTxDiver;
	if (GetOption(argc, argv, "txdiver", &sTxDiver) >= 0) {
		if (!_stricmp("main", sTxDiver)) {
			GCfg.TxDiversity = TXDIV_MAIN;
		} else if (!_stricmp("aux", sTxDiver)) {
			GCfg.TxDiversity = TXDIV_AUX;
		} else if (!_stricmp("on", sTxDiver)) {
			GCfg.TxDiversity = TXDIV_ON;
		} else {
			printf("Unknown TX diversity\nassuming on\n");
			GCfg.TxDiversity = TXDIV_ON;
		}
	}

	char* sRxDiver;
	if (GetOption(argc, argv, "rxdiver", &sRxDiver) >= 0) {
		if (!_stricmp("main", sRxDiver)) {
			GCfg.RxDiversity = RXDIV_MAIN;
		} else if (!_stricmp("aux", sRxDiver)) {
			GCfg.RxDiversity = RXDIV_AUX;
		} else if (!_stricmp("startmain", sRxDiver)) {
			GCfg.RxDiversity = RXDIV_START_MAIN;
		} else if (!_stricmp("startaux", sRxDiver)) {
			GCfg.RxDiversity = RXDIV_START_AUX;
		} else {
			printf("Unknown RX diversity\nassuming start main\n");
			GCfg.RxDiversity = RXDIV_START_MAIN;
		}
	}



	char* sFrag;
	DWORD dwFrag;
	if (GetOption(argc, argv, "frag", &sFrag) >= 0) {
		dwFrag = atoi(sFrag);
		if ((dwFrag < 256) || (dwFrag > 2346)) {
			printf("Frag threshold out of\n   bounds, set to 2346\n");
			dwFrag = 2346;
		}
		GCfg.fragThreshold = dwFrag;
	}

	char* sRTS;
	DWORD dwRTS;
	if (GetOption(argc, argv, "rts", &sRTS) >= 0) {
		dwRTS = atoi(sRTS);
		if ((dwRTS < 0) || (dwRTS > 2347)) {
			printf("RTS threshold out of\n   bounds, set to 2347\n");
			dwRTS = 2347;
		}
		GCfg.RTSThreshold = dwRTS;
	}

	char* sCertPath;
	if (GetOption(argc, argv, "cert_path", &sCertPath) >= 0) {
		if ((strlen(sCertPath) > 0) && (strlen(sCertPath) < (MAX_CERT_PATH - 1))) {
			strcpy(GCfg.certPath, sCertPath);
		}else{
			printf("Invalid certificate path\n   given, ignored.\n");
		}
	}

	char* sOchan;
	DWORD dwChan;
	byte byTmp[32];
	if (GetOption(argc, argv, "opt_chan", &sOchan) >= 0) {
		dwChan = atoh(sOchan, &byTmp[0]);
		if ((dwChan <= 0) || (dwChan >= 0x02000)) {
			printf("Invalid optimized channel\n   list, set to all\n");
			GCfg.bLRS = 0x0000FFFF;
		}else{
			GCfg.bLRS = dwChan;
		}
	}

	char* sAvg;
	DWORD dwAvg;
	if (GetOption(argc, argv, "avg_rssi", &sAvg) >= 0) {
		dwAvg = atoi(sAvg);
		if ((dwAvg < 2) || (dwAvg > 8)) {
			printf("RSSI Average out of\n   bounds, set to 8\n");
			dwAvg = 8;
		}
		GCfg.avgWindow = dwAvg;
	}

	char* sProbeDelay;
	DWORD dwProbeDelay;
	if (GetOption(argc, argv, "probe", &sProbeDelay) >= 0) {
		dwProbeDelay = atoi(sProbeDelay);
		if ((dwProbeDelay < 2) || (dwProbeDelay > 60)) {
			printf("Probe delay out of\n   bounds, set to 10\n");
			dwProbeDelay = 10;
		}
		GCfg.probeDelay = dwProbeDelay;
	}

	char* sTray = NULL;
	if (GetOption(argc, argv, "tray", &sTray) >= 0) {
		if (!_stricmp("on", sTray)) {
			GCfg.trayIcon = 1;
		} else if (!_stricmp("off", sTray)) {
			GCfg.trayIcon = 0;
		}else{
			printf("Unknown tray option\nassuming on\n");
			GCfg.trayIcon = 1;
		}
	}

	char* sDispPwd = NULL;
	if (GetOption(argc, argv, "disppwd", &sDispPwd) >= 0) {
		if (!_stricmp("on", sDispPwd)) {
			GCfg.displayPasswords = 1;
		} else if (!_stricmp("off", sDispPwd)) {
			GCfg.displayPasswords = 0;
		}else{
			printf("Unknown pwd display option\nassuming off\n");
			GCfg.displayPasswords = 0;
		}
	}

	char* sAdminReq = NULL;
	if (GetOption(argc, argv, "adminrequire", &sAdminReq) >= 0) {
		if (!_stricmp("on", sAdminReq)) {
			GCfg.adminOverride = 0;
		} else if (!_stricmp("off", sAdminReq)) {
			GCfg.adminOverride = 1;
		}else{
			printf("Unknown pwd require option\nassuming on\n");
			GCfg.adminOverride = 0;
		}
	}

	// did they define a password?
	char* sAdminPwd = NULL;
	if (GetOption(argc, argv, "adminpwd", &sAdminPwd) >= 0) {
		GCfg.adminPassword.size = strlen(sAdminPwd);
		strcpy(GCfg.adminPassword.buffer, sAdminPwd);
	}

	// now update the global settings with whatever we have
	sdcRet = SetGlobalSettings(&GCfg);
	switch(sdcRet) {
		case SDCERR_SUCCESS:
			printf("Global settings updated\n");
			break;
		case SDCERR_INVALID_CONFIG:
			printf("Invalid global configuration\n");
			break;
		default:
			printf("Error updating global\n   configuration\n");
			break;
	}

	return;
}

int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    lpCmdLine,
					int       nCmdShow)
{
	int		i, j, k;

	int		argc;
	char	*argv[MAX_ARGS];
	char	argvs[MAX_ARGS][50];
	char	CmdLine[512];
	CF10G_STATUS CfgStat;
	union {
		DWORD version;
		BYTE  vers[4];
	} VerChk;

	strcpy(CfgStat.configName, "Default");

	// make sure we are on the right version before running
	if (GetCurrentStatus(&CfgStat) == SDCERR_SUCCESS) {
		VerChk.version = CfgStat.driverVersion;
		if (VerChk.vers[2] < 2) {
			printf("SDCset requires driver\nversion 2.0.0 or later\n-- cancelled --\n");
			Sleep(5000);
//			return 0;
		}
	}


	for (i = 0; i < MAX_ARGS; i++) {
		argv[i] = argvs[i];
		argvs[i][0] = 0x00;
	}
	
	strcpy(argvs[0], "SDCset.exe");
	argc = 1;
	wcstombs(CmdLine, lpCmdLine, 256);

	// break out the command line arguments
	bool bInQuotes = false;
	bool bPcntSign = false;
	for (i = 0, j = 0, k = 1; (k < MAX_ARGS) && (i < (int) strlen(CmdLine)); i++) {
		if (bInQuotes) {
			if (CmdLine[i] == '"') {
				//ending quote
				bInQuotes = false;
				argvs[k++][j] = 0x00;
				j = 0x00;
			}else{
				argvs[k][j++] = CmdLine[i];
			}
		}else if (bPcntSign) {
			switch(CmdLine[i]) {
				case 'S':						// %s says insert the serial num
				case 's':
					// lookup the serial number and insert it into the args
					j += FindSerialNumber(&argvs[k][j]);
					break;
				case 'M':						// %m says insert the MAC address
				case 'm':						
					sprintf(&argvs[k][j], "%02.02X%02.02X%02.02X%02.02X%02.02X%02.02X",
						CfgStat.client_MAC[0], CfgStat.client_MAC[1],
						CfgStat.client_MAC[2], CfgStat.client_MAC[3],
						CfgStat.client_MAC[4], CfgStat.client_MAC[5]);
					j += 12;
					break;
				case '%':
					argvs[k][j++] = '%';
					break;
				default: 
					break;
			}
			bPcntSign = false;
		}else if (CmdLine[i] == '%') {
			bPcntSign = true;
		}else if (CmdLine[i] == '"') {
			// starting quote
			bInQuotes = true;
		} else if (CmdLine[i] != 0x20) {
			// not in quotes, look for any non-space character
			argvs[k][j++] = CmdLine[i];
		}else if (j > 0) {
			// space while not in quoted strings, so then end of this arg
			argvs[k++][j] = 0x00;
			j = 0x00;
		}
	}
	if (j > 0) {
		argvs[k++][j] = 0x00;
	}
	argc = k;


    if(WasOption(argc, argv, "radio")>0)
	{
        DoRadio(argc, argv);
	}
	else if(WasOption(argc, argv, "ip")>0) 
	{
		QueryIP(argc, argv);
	}
    else if(WasOption(argc, argv, "config")>0) 
	{
		DoConfig(argc, argv);
	}
	else if(WasOption(argc, argv, "global") > 0) 
	{
		DoGlobal(argc, argv);
	}
	else if(WasOption(argc, argv, "power")>0)
	{
		DoPowerMode(argc, argv);
	}
    else 
	{
		unsigned long lVersion;
        printf("SDCset (c) 2008-14 Datalogic ADC, Inc.\n");
		printf("ver 2014.100, %s %s\n", __DATE__, __TIME__);
		if (GetCurrentStatus(&CfgStat) == SDCERR_SUCCESS) {
			VerChk.version = CfgStat.driverVersion;
			printf("SDC driver v%d.%d.%d\n", VerChk.vers[2], VerChk.vers[1], VerChk.vers[0]);
		}
		if (GetSDKVersion(&lVersion) == SDCERR_SUCCESS) {
			VerChk.version = lVersion;
			printf("SDC SDK v%d.%d.%d\n", VerChk.vers[2], VerChk.vers[1], VerChk.vers[0]);
		}
	}

	RegFlushKey( HKEY_LOCAL_MACHINE );

    return gRetStatus;
}   // wmain()





