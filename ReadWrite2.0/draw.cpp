#pragma once
#include "draw.hpp"

PUCHAR WIN32KBASE_BASE;
PUCHAR WIN32KFULL_BASE;
PUCHAR WIN32K_BASE;
ULONG64 WIN32KBASE_SIZE;
ULONG64 WIN32KFULL_SIZE;
ULONG64 WIN32K_SIZE;


//绘图
GetDC_t NtUserGetDC;
SelectBrush_t NtGdiSelectBrush;
PatBlt_t NtGdiPatBlt;
ReleaseDC_t NtUserReleaseDC;
CreateSolidBrush_t NtGdiCreateSolidBrush;
DeleteObjectApp_t NtGdiDeleteObjectApp;
gNtUserInvalidateRect NtUserInvalidateRect;
ExtTextOutW_t NtGdiExtTextOutW;
HfontCreate_t NtGdiHfontCreate;
SelectFont_t NtGdiSelectFont;
gGreSetBkMode GreSetBkMode;
gGreSetTextColor GreSetTextColor;
gGreSetBkColor GreSetBkColor;
gValidateHwnd ValidateHwnd;
gNtUserSetWindowPos NtUserSetWindowPos;
gNtUserSetWindowLongPtr NtUserSetWindowLongPtr;

//双缓冲
gGreCreateCompatibleDC GreCreateCompatibleDC;
gGreCreateCompatibleBitmapInternal GreCreateCompatibleBitmapInternal;
ghbmSelectBitmap hbmSelectBitmap;
gNtGdiBitBltInternal NtGdiBitBltInternal;
gbDeleteDCInternal bDeleteDCInternal;
ggDeleteObject gDeleteObject;



SKSTATUS DrGetAllWin32kModuleBase() {
	SKSTATUS ret = 0;
	if (!WIN32KBASE_BASE)
	{
		ret |= KrGetKernelModuleBase((PUCHAR)"win32kbase.sys", &WIN32KBASE_SIZE, (PULONG64)&WIN32KBASE_BASE);
	}
	if (!WIN32KFULL_BASE)
	{
		ret |= KrGetKernelModuleBase((PUCHAR)"win32kfull.sys", &WIN32KFULL_SIZE, (PULONG64)&WIN32KFULL_BASE);
	}
	if (!WIN32K_BASE)
	{
		ret |= KrGetKernelModuleBase((PUCHAR)"win32k.sys", &WIN32K_SIZE, (PULONG64)&WIN32K_BASE);
	}
	return ret;
}

SKSTATUS DrGetDrawKrFuncAddr_Win1011() {
	SKSTATUS ret = 0;
	ret = DrGetAllWin32kModuleBase();
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	//绘图
	if (!NtUserGetDC) ret |= PeGetExportFuncAddr64(WIN32KBASE_BASE, (PUCHAR)"NtUserGetDC", (PULONG64)&NtUserGetDC);
	if (!NtUserGetDC) ret |= PeGetExportFuncAddr64(WIN32KFULL_BASE, (PUCHAR)"NtUserGetDC", (PULONG64)&NtUserGetDC);
	if (!NtUserGetDC) ret |= PeGetExportFuncAddr64(WIN32K_BASE, (PUCHAR)"NtUserGetDC", (PULONG64)&NtUserGetDC);
	if (!NtGdiPatBlt) ret |= PeGetExportFuncAddr64(WIN32KBASE_BASE, (PUCHAR)"NtGdiPatBlt", (PULONG64)&NtGdiPatBlt);
	if (!NtGdiPatBlt) ret |= PeGetExportFuncAddr64(WIN32KFULL_BASE, (PUCHAR)"NtGdiPatBlt", (PULONG64)&NtGdiPatBlt);
	if (!NtGdiPatBlt) ret |= PeGetExportFuncAddr64(WIN32K_BASE, (PUCHAR)"NtGdiPatBlt", (PULONG64)&NtGdiPatBlt);
	if (!NtGdiSelectBrush) ret |= PeGetExportFuncAddr64(WIN32KBASE_BASE, (PUCHAR)"GreSelectBrush", (PULONG64)&NtGdiSelectBrush);
	if (!NtGdiSelectBrush) ret |= PeGetExportFuncAddr64(WIN32KFULL_BASE, (PUCHAR)"GreSelectBrush", (PULONG64)&NtGdiSelectBrush);
	if (!NtGdiSelectBrush) ret |= PeGetExportFuncAddr64(WIN32K_BASE, (PUCHAR)"GreSelectBrush", (PULONG64)&NtGdiSelectBrush);
	if (!NtUserReleaseDC) ret |= PeGetExportFuncAddr64(WIN32KBASE_BASE, (PUCHAR)"NtUserReleaseDC", (PULONG64)&NtUserReleaseDC);
	if (!NtUserReleaseDC) ret |= PeGetExportFuncAddr64(WIN32KFULL_BASE, (PUCHAR)"NtUserReleaseDC", (PULONG64)&NtUserReleaseDC);
	if (!NtUserReleaseDC) ret |= PeGetExportFuncAddr64(WIN32K_BASE, (PUCHAR)"NtUserReleaseDC", (PULONG64)&NtUserReleaseDC);
	if (!NtGdiCreateSolidBrush) ret |= PeGetExportFuncAddr64(WIN32KBASE_BASE, (PUCHAR)"NtGdiCreateSolidBrush", (PULONG64)&NtGdiCreateSolidBrush);
	if (!NtGdiCreateSolidBrush) ret |= PeGetExportFuncAddr64(WIN32KFULL_BASE, (PUCHAR)"NtGdiCreateSolidBrush", (PULONG64)&NtGdiCreateSolidBrush);
	if (!NtGdiCreateSolidBrush) ret |= PeGetExportFuncAddr64(WIN32K_BASE, (PUCHAR)"NtGdiCreateSolidBrush", (PULONG64)&NtGdiCreateSolidBrush);
	if (!NtGdiDeleteObjectApp) ret |= PeGetExportFuncAddr64(WIN32KBASE_BASE, (PUCHAR)"NtGdiDeleteObjectApp", (PULONG64)&NtGdiDeleteObjectApp);
	if (!NtGdiDeleteObjectApp) ret |= PeGetExportFuncAddr64(WIN32KFULL_BASE, (PUCHAR)"NtGdiDeleteObjectApp", (PULONG64)&NtGdiDeleteObjectApp);
	if (!NtGdiDeleteObjectApp) ret |= PeGetExportFuncAddr64(WIN32K_BASE, (PUCHAR)"NtGdiDeleteObjectApp", (PULONG64)&NtGdiDeleteObjectApp);
	if (!NtGdiExtTextOutW) ret |= PeGetExportFuncAddr64(WIN32KBASE_BASE, (PUCHAR)"NtGdiExtTextOutW", (PULONG64)&NtGdiExtTextOutW);
	if (!NtGdiExtTextOutW) ret |= PeGetExportFuncAddr64(WIN32KFULL_BASE, (PUCHAR)"NtGdiExtTextOutW", (PULONG64)&NtGdiExtTextOutW);
	if (!NtGdiExtTextOutW) ret |= PeGetExportFuncAddr64(WIN32K_BASE, (PUCHAR)"NtGdiExtTextOutW", (PULONG64)&NtGdiExtTextOutW);
	if (!NtGdiHfontCreate) ret |= PeGetExportFuncAddr64(WIN32KBASE_BASE, (PUCHAR)"hfontCreate", (PULONG64)&NtGdiHfontCreate);
	if (!NtGdiHfontCreate) ret |= PeGetExportFuncAddr64(WIN32KFULL_BASE, (PUCHAR)"hfontCreate", (PULONG64)&NtGdiHfontCreate);
	if (!NtGdiHfontCreate) ret |= PeGetExportFuncAddr64(WIN32K_BASE, (PUCHAR)"hfontCreate", (PULONG64)&NtGdiHfontCreate);
	if (!NtGdiSelectFont) ret |= PeGetExportFuncAddr64(WIN32KBASE_BASE, (PUCHAR)"NtGdiSelectFont", (PULONG64)&NtGdiSelectFont);
	if (!NtGdiSelectFont) ret |= PeGetExportFuncAddr64(WIN32KFULL_BASE, (PUCHAR)"NtGdiSelectFont", (PULONG64)&NtGdiSelectFont);
	if (!NtGdiSelectFont) ret |= PeGetExportFuncAddr64(WIN32K_BASE, (PUCHAR)"NtGdiSelectFont", (PULONG64)&NtGdiSelectFont);

	if (!GreSetBkMode) ret |= MmFindAddrBySignCode(WIN32KFULL_BASE, (PUCHAR)"48895C24??574883EC??8BFA33DB488BD148895C24??488D4C24??E8????????488B4424??4885C0", WIN32KFULL_SIZE, (PULONG64)&GreSetBkMode);//已验证
	//if (!GreSetBkMode) ret |= MmFindAddrBySignCode(WIN32KBASE_BASE, (PUCHAR)"48895C24??574883EC??8BFA33DB488BD148895C24??488D4C24??E8????????488B4424??4885C0", WIN32KBASE_SIZE,(PULONG64)&GreSetBkMode);//已验证
	//if (!GreSetBkMode) ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"48895C24??574883EC??8BFA33DB488BD148895C24??488D4C24??E8????????488B4424??4885C0",WIN32K_SIZE, (PULONG64)&GreSetBkMode);//已验证
	if (!GreSetTextColor) ret |= MmFindAddrBySignCode(WIN32KFULL_BASE, (PUCHAR)"48895C24??48897C24??55488BEC4883EC??8365????8BDA8365????488BD1488D4D??83CF", WIN32KFULL_SIZE, (PULONG64)&GreSetTextColor);//已验证
	if (!GreSetTextColor) ret |= MmFindAddrBySignCode(WIN32KFULL_BASE, (PUCHAR)"488BC4488958??574883EC??8360????8BDA8360????488BD1488D48??83CF", WIN32KFULL_SIZE, (PULONG64)&GreSetTextColor);//已验证
	//if (!GreSetTextColor) ret |= MmFindAddrBySignCode(WIN32KBASE_BASE, (PUCHAR)"488BC4488958??574883EC??8360????8BDA8360????488BD1488D48??83CF??E8????????488B4424??4885C074??488B80????????8BB8BC??????8998BC??????81E3????????3BDF75??", WIN32KBASE_SIZE, (PULONG64)&GreSetTextColor);
	//if (!GreSetTextColor) ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"488BC4488958??574883EC??8360????8BDA8360????488BD1488D48??83CF??E8????????488B4424??4885C074??488B80????????8BB8BC??????8998BC??????81E3????????3BDF75??", WIN32K_SIZE, (PULONG64)&GreSetTextColor);
	
	if (!NtUserSetWindowLongPtr) ret |= PeGetExportFuncAddr64(WIN32KBASE_BASE, (PUCHAR)"NtUserSetWindowLongPtr", (PULONG64)&NtUserSetWindowLongPtr);
	if (!NtUserSetWindowLongPtr) ret |= PeGetExportFuncAddr64(WIN32KFULL_BASE, (PUCHAR)"NtUserSetWindowLongPtr", (PULONG64)&NtUserSetWindowLongPtr);
	if (!NtUserSetWindowLongPtr) ret |= PeGetExportFuncAddr64(WIN32K_BASE, (PUCHAR)"NtUserSetWindowLongPtr", (PULONG64)&NtUserSetWindowLongPtr);

	
	
	//双缓冲
	//if (!GreCreateCompatibleDC) ret |= MmFindAddrBySignCode(WIN32KFULL_BASE, (PUCHAR)"48895C24??48897424??5557415441564157488BEC4883EC??4533E4488BD9418BF44885C90F84????????8BD30FB7C3C1EA??458D4424??81E2????????4C8965??0BD04C8965??4533C9448965??488D4D??44896424??418BFCE8????????443965??0F84", WIN32KFULL_SIZE, (PULONG64)&GreCreateCompatibleDC);
	//if (!GreCreateCompatibleDC) ret |= MmFindAddrBySignCode(WIN32KBASE_BASE, (PUCHAR)"48895C24??48897424??5557415441564157488BEC4883EC??4533E4488BD9418BF44885C90F84????????8BD30FB7C3C1EA??458D4424??81E2????????4C8965??0BD04C8965??4533C9448965??488D4D??44896424??418BFCE8????????443965??0F84", WIN32KBASE_SIZE, (PULONG64)&GreCreateCompatibleDC);
	//if (!GreCreateCompatibleDC) ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"48895C24??48897424??5557415441564157488BEC4883EC??4533E4488BD9418BF44885C90F84????????8BD30FB7C3C1EA??458D4424??81E2????????4C8965??0BD04C8965??4533C9448965??488D4D??44896424??418BFCE8????????443965??0F84", WIN32K_SIZE, (PULONG64)&GreCreateCompatibleDC);
	//if (!GreCreateCompatibleBitmapInternal) ret |= MmFindAddrBySignCode(WIN32KFULL_BASE, (PUCHAR)"48895C24??5556574154415541564157488D6C24??4881EC????????33F64D63F04533FF4C63E2488BD944897D??8BCE448965??418BC1448975??25????????418BF90F94C1894D??410FBAE1??0F82????????8BC7448BEE25????????8945??410F95C5C1EF??83E7", WIN32KFULL_SIZE, (PULONG64)&GreCreateCompatibleBitmapInternal);
	//if (!GreCreateCompatibleBitmapInternal) ret |= MmFindAddrBySignCode(WIN32KBASE_BASE, (PUCHAR)"48895C24??5556574154415541564157488D6C24??4881EC????????33F64D63F04533FF4C63E2488BD944897D??8BCE448965??418BC1448975??25????????418BF90F94C1894D??410FBAE1??0F82????????8BC7448BEE25????????8945??410F95C5C1EF??83E7", WIN32KBASE_SIZE, (PULONG64)&GreCreateCompatibleBitmapInternal);
	//if (!GreCreateCompatibleBitmapInternal) ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"48895C24??5556574154415541564157488D6C24??4881EC????????33F64D63F04533FF4C63E2488BD944897D??8BCE448965??418BC1448975??25????????418BF90F94C1894D??410FBAE1??0F82????????8BC7448BEE25????????8945??410F95C5C1EF??83E7", WIN32K_SIZE, (PULONG64)&GreCreateCompatibleBitmapInternal);
	//if (!hbmSelectBitmap) ret |= PeGetExportFuncAddr64(WIN32KBASE_BASE, (PUCHAR)"hbmSelectBitmap", (PULONG64)&hbmSelectBitmap);
	//if (!hbmSelectBitmap) ret |= PeGetExportFuncAddr64(WIN32KFULL_BASE, (PUCHAR)"hbmSelectBitmap", (PULONG64)&hbmSelectBitmap);
	//if (!hbmSelectBitmap) ret |= PeGetExportFuncAddr64(WIN32K_BASE, (PUCHAR)"hbmSelectBitmap", (PULONG64)&hbmSelectBitmap);
	//if (!hbmSelectBitmap) ret |= MmFindAddrBySignCode(WIN32KFULL_BASE, (PUCHAR)"488BC4488958??488970??574883EC??488360????488BF28360????488BD18360????488D48??418BD9418BF8E8????????48837C24????75??33DBEB??448BCBC74424??????????448BC7488D4C24??488BD6", WIN32KFULL_SIZE, (PULONG64)&hbmSelectBitmap);
	//if (!hbmSelectBitmap) ret |= MmFindAddrBySignCode(WIN32KBASE_BASE, (PUCHAR)"488BC4488958??488970??574883EC??488360????488BF28360????488BD18360????488D48??418BD9418BF8E8????????48837C24????75??33DBEB??448BCBC74424??????????448BC7488D4C24??488BD6", WIN32KBASE_SIZE, (PULONG64)&hbmSelectBitmap);
	//if (!hbmSelectBitmap) ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"488BC4488958??488970??574883EC??488360????488BF28360????488BD18360????488D48??418BD9418BF8E8????????48837C24????75??33DBEB??448BCBC74424??????????448BC7488D4C24??488BD6", WIN32K_SIZE, (PULONG64)&hbmSelectBitmap);
	//if (!NtGdiBitBltInternal) ret |= MmFindAddrBySignCode(WIN32KFULL_BASE, (PUCHAR)"4C8BDC5553498DAB????????4881EC????????488B05????????4833C4488985????????F685??????????418BD9498973??4C8BD149897B??8BF2488B95????????8BBD????????4D8963??4D896B??448BAD????????4D8973??458BF048894D??8B8D????????4D897B??448BBD????????895D??894C24??488955??0F85", WIN32KFULL_SIZE, (PULONG64)&NtGdiBitBltInternal);
	//if (!NtGdiBitBltInternal) ret |= MmFindAddrBySignCode(WIN32KBASE_BASE, (PUCHAR)"4C8BDC5553498DAB????????4881EC????????488B05????????4833C4488985????????F685??????????418BD9498973??4C8BD149897B??8BF2488B95????????8BBD????????4D8963??4D896B??448BAD????????4D8973??458BF048894D??8B8D????????4D897B??448BBD????????895D??894C24??488955??0F85", WIN32KBASE_SIZE, (PULONG64)&NtGdiBitBltInternal);
	//if (!NtGdiBitBltInternal) ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"4C8BDC5553498DAB????????4881EC????????488B05????????4833C4488985????????F685??????????418BD9498973??4C8BD149897B??8BF2488B95????????8BBD????????4D8963??4D896B??448BAD????????4D8973??458BF048894D??8B8D????????4D897B??448BBD????????895D??894C24??488955??0F85", WIN32K_SIZE, (PULONG64)&NtGdiBitBltInternal);
	//if (!bDeleteDCInternal) ret |= MmFindAddrBySignCode(WIN32KFULL_BASE, (PUCHAR)"4883EC??41F7D9451BD24181E2????????41F7D81BC025????????440BD0F7DA488D5424??1BC025????????440BD044895424??E8????????4883C4??C3", WIN32KFULL_SIZE, (PULONG64)&bDeleteDCInternal);
	//if (!bDeleteDCInternal) ret |= MmFindAddrBySignCode(WIN32KBASE_BASE, (PUCHAR)"4883EC??41F7D9451BD24181E2????????41F7D81BC025????????440BD0F7DA488D5424??1BC025????????440BD044895424??E8????????4883C4??C3", WIN32KBASE_SIZE, (PULONG64)&bDeleteDCInternal);
	//if (!bDeleteDCInternal) ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"4883EC??41F7D9451BD24181E2????????41F7D81BC025????????440BD0F7DA488D5424??1BC025????????440BD044895424??E8????????4883C4??C3", WIN32K_SIZE, (PULONG64)&bDeleteDCInternal);
	//if (!gDeleteObject) ret |= MmFindAddrBySignCode(WIN32KFULL_BASE, (PUCHAR)"4C8BDC55534156488BEC4883EC??488BD90FBAE1??0F82????????8BC3498973??C1E8??83E0??83F8??0F82????????83F8??0F85????????49897B??488BD14D8963??488D4D??4D896B??4533C04D897B??E8????????488B7D??33F64885FF0F84????????3977??0F85????????4C8B37488D4D??448BFE488975??48FF15????????0F1F4400??488BD8E8????????85C0", WIN32KFULL_SIZE, (PULONG64)&gDeleteObject);
	//if (!gDeleteObject) ret |= MmFindAddrBySignCode(WIN32KBASE_BASE, (PUCHAR)"4C8BDC55534156488BEC4883EC??488BD90FBAE1??0F82????????8BC3498973??C1E8??83E0??83F8??0F82????????83F8??0F85????????49897B??488BD14D8963??488D4D??4D896B??4533C04D897B??E8????????488B7D??33F64885FF0F84????????3977??0F85????????4C8B37488D4D??448BFE488975??48FF15????????0F1F4400??488BD8E8????????85C0", WIN32KBASE_SIZE, (PULONG64)&gDeleteObject);
	//if (!gDeleteObject) ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"4C8BDC55534156488BEC4883EC??488BD90FBAE1??0F82????????8BC3498973??C1E8??83E0??83F8??0F82????????83F8??0F85????????49897B??488BD14D8963??488D4D??4D896B??4533C04D897B??E8????????488B7D??33F64885FF0F84????????3977??0F85????????4C8B37488D4D??448BFE488975??48FF15????????0F1F4400??488BD8E8????????85C0", WIN32K_SIZE, (PULONG64)&gDeleteObject);

	/*&(ULONG64)GreCreateCompatibleDC &(ULONG64)GreCreateCompatibleBitmapInternal
		&(ULONG64)hbmSelectBitmap &(ULONG64)NtGdiBitBltInternal &(ULONG64)bDeleteDCInternal
		&(ULONG64)gDeleteObject*/
	
	if (!((ULONG64)NtUserGetDC & (ULONG64)NtGdiPatBlt & (ULONG64)NtGdiSelectBrush & (ULONG64)NtUserReleaseDC
		&(ULONG64)NtGdiCreateSolidBrush & (ULONG64)NtGdiDeleteObjectApp & (ULONG64)NtGdiExtTextOutW
		&(ULONG64)NtGdiHfontCreate & (ULONG64)NtGdiSelectFont) &(ULONG64)GreSetBkMode &(ULONG64)GreSetTextColor
		
		)
	{
		return ret;
	}
	return SK_SUCCESS;
}


SKSTATUS DrGetDrawKrFuncAddr_Win7() {
	SKSTATUS ret = NULL;
	ret = KrGetKernelModuleBase((PUCHAR)"win32k.sys", &WIN32K_SIZE, (PULONG64)&WIN32K_BASE);
	if (ret != SK_SUCCESS) return ret;
	
	ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"48895C24??48897424??574883EC??488BD9488B0D????????BE????????FF15????????4885DB74??488BCBE8????????488BD84885C075??33FFE9", WIN32K_SIZE, (PULONG64)&NtUserGetDC);//已验证
	ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"488BC4488958??488968??488970??5741544155415641574881EC????????33F6448BF24C8BE1488BD1488D48??458BF9458BE88BDE8970??8970??E8", WIN32K_SIZE, (PULONG64)&NtGdiPatBlt);//已验证
	ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"488BC4488958??574883EC??488BFA488BD133DB488D48??8958??8958??E8", WIN32K_SIZE, (PULONG64)&NtGdiSelectBrush);//已验证
	ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"FFF34883EC??488BD9488B0D????????FF15????????33D2488BCBE8????????33DB83F8??0F95C3E8????????8BC34883C4??5BC3", WIN32K_SIZE, (PULONG64)&NtUserReleaseDC);//已验证
	ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"4C8BC241B9????????33D2E9", WIN32K_SIZE, (PULONG64)&NtGdiCreateSolidBrush);//已验证
	ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"48895C24??574883EC??480FBAE1??488BD973??B8????????E9????????4C8BD949C1EB??4183E3??418BD3E8", WIN32K_SIZE, (PULONG64)&NtGdiDeleteObjectApp);//已验证
	ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"FFF3565741544155415641574881EC????????488B05????????4833C448898424????????44894C24??4C8BD948898C24????????48898C24????????899424????????44898424????????44894C24??488B9C24????????48895C24??488B8424????????48898424????????48898424????????4C8B9424????????4C895424??8B8C24????????81F9", WIN32K_SIZE, (PULONG64)&NtGdiExtTextOutW);//已验证
	ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"44894424??535657", WIN32K_SIZE, (PULONG64)&NtGdiHfontCreate);//已验证
	ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"488BC4488958??488968??488970??574883EC??488BEA488BD133DB2158??2158??488D48??E8", WIN32K_SIZE, (PULONG64)&NtGdiSelectFont);//已验证
	ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"488BC4488958??574883EC??8BDA488BD133FF488D48??8978??8978??E8????????488B4C24??483BCF74??488B41??8B78??8958??83FB??74??B8????????3BD80F45D8", WIN32K_SIZE, (PULONG64)&GreSetBkMode);//已验证
	ret |= MmFindAddrBySignCode(WIN32K_BASE, (PUCHAR)"488BC4488958??574883EC??8360????8360????8BDA488BD1488D48??83CF??E8????????4C8B5C24??4D85DB74??498B43??8B78??89582C81E3????????3BDF74??498B4B??8959??498B4B??8349", WIN32K_SIZE, (PULONG64)&GreSetTextColor);//已验证
	

	if (!((ULONG64)NtUserGetDC & (ULONG64)NtGdiPatBlt & (ULONG64)NtGdiSelectBrush & (ULONG64)NtUserReleaseDC
		& (ULONG64)NtGdiCreateSolidBrush & (ULONG64)NtGdiDeleteObjectApp & (ULONG64)NtGdiExtTextOutW
		& (ULONG64)NtGdiHfontCreate & (ULONG64)NtGdiSelectFont &(ULONG64)GreSetBkMode &(ULONG64)GreSetTextColor
		))
	{
		return ret;
	}
	return SK_SUCCESS;
}

SKSTATUS DrGetDrawKrFuncAddr() {
	SKSTATUS ret = NULL;
	ULONG buildNum = NULL;
	ret = GlbGetBuildNumber(&buildNum);
	if (ret != SK_SUCCESS)
	{
		return ret;
	}
	if (buildNum == 7600 || buildNum == 7601)
	{
		return DrGetDrawKrFuncAddr_Win7();
	}
	else {
		return DrGetDrawKrFuncAddr_Win1011();
	}
}




BOOLEAN alreadyInstall = FALSE;
SKSTATUS DrInstallDraw() {
	if (alreadyInstall)
	{
		return SK_SUCCESS;
	}
	SKSTATUS ret = 0;
	NTSTATUS ntRet = 0;
	ULONG64 dwmPid = NULL;
	PEPROCESS dwmProc = NULL;
	ret = PsGetPidByName("dwm.exe", &dwmPid);
	if (ret != SK_SUCCESS) return ret;
	ntRet = PsLookupProcessByProcessId((HANDLE)dwmPid,&dwmProc);
	if (!NT_SUCCESS(ntRet)) 
	{
		EcUpdateLastSysErrCode(ntRet);
		return SK_FAILED_DRAWINIT_BADDWMPROC;
	}
	ObDereferenceObject(dwmProc);

	KAPC_STATE apc = { 0 };
	KeStackAttachProcess(dwmProc,&apc);
	DrtInitWorkList();
	ret = DrGetDrawKrFuncAddr();
	if (ret != SK_SUCCESS) goto DR_RETN;
	ret = DrSetDrawHook();
	if (ret != SK_SUCCESS) goto DR_RETN;
	alreadyInstall = TRUE;
DR_RETN:
	KeUnstackDetachProcess(&apc);
	
	return ret;
}




SKSTATUS DrSetDrawHook() {
	SKSTATUS ret = NULL;
	ULONG buildNum = NULL;
	ret = GlbGetBuildNumber(&buildNum);
	if (ret!=SK_SUCCESS)
	{
		return ret;
	}
	if (buildNum == 7600 || buildNum == 7601)
	{
		return DrSetDrawHookWin7();
	}
	else if (buildNum == 22000) {
		return DrSetDrawHookWin10_DxgkRender() | DrSetDrawHookWin10_SubmitCommand()| DrSetDrawHookWin10_DxgkPresent() ;
	}
	else
	{
		return DrSetDrawHookWin10_DxgkRender() | DrSetDrawHookWin10_SubmitCommand()| DrSetDrawHookWin10_DxgkPresent() ;
	}
}