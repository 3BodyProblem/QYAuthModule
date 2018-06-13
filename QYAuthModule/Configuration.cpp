#include <string>
#include <algorithm>
#include "Configuration.h"
#include "Infrastructure/IniFile.h"


HMODULE						g_oModule;


Configuration::Configuration()
{
}

Configuration& Configuration::GetConfig()
{
	static Configuration	obj;

	return obj;
}

void Configuration::Release()
{
	::memset( &m_tagAuthConfig, 0, sizeof(m_tagAuthConfig) );
}

int Configuration::Initialize()
{
	std::string			sPath;
	inifile::IniFile	oIniFile;
	int					nErrCode = 0;
    char				pszTmp[1024*8] = { 0 };

	Release();
    ::GetModuleFileName( g_oModule, pszTmp, sizeof(pszTmp) );
    sPath = pszTmp;
    sPath = sPath.substr( 0, sPath.find(".dll") ) + ".ini";
	if( 0 != (nErrCode=oIniFile.load( sPath )) )
	{
//		QuoCollector::GetCollector()->OnLog( TLV_ERROR, "Configuration::Initialize() : configuration file isn\'t exist. [%s], errorcode=%d", sPath.c_str(), nErrCode );
		return -1;
	}

	///< 设置： 快照落盘目录(含文件名)
//	m_sDumpFileFolder = oIniFile.getStringValue( std::string("SRV"), std::string("DumpFolder"), nErrCode );
	if( 0 != nErrCode )	{
//		QuoCollector::GetCollector()->OnLog( TLV_WARN, "Configuration::Initialize() : shutdown dump function." );
	}
/*
	m_nBcBeginTime = oIniFile.getIntValue( std::string("SRV"), std::string("BroadcastBeginTime"), nErrCode );
	if( 0 != nErrCode )	{
		m_nBcBeginTime = 0xffffffff;
		QuoCollector::GetCollector()->OnLog( TLV_WARN, "Configuration::Initialize() : Topspeed Mode...!" );
	}
*/
	return 0;
}

void Configuration::FetchAuthConfig( CQAuthClientInput& objCfg )
{
	objCfg.ClientType = m_tagAuthConfig.m_nClientType;						///< 客户端类型
	objCfg.ClientVersion = m_tagAuthConfig.m_nClientVersion;				///< 客户端版本，主版本、子版本、Build号，32位各用8、12、12位
	objCfg.CustomerID = m_tagAuthConfig.m_nCustomerID;						///< 券商ID
	objCfg.ServerCount = m_tagAuthConfig.m_nServerCount;					///< 认证服务器数量
	objCfg.NetworkTimeOut = m_tagAuthConfig.m_nNetworkTimeOut;				///< 网络通讯超时，单位：秒
	objCfg.HeartbeatInterval = m_tagAuthConfig.m_nHeartbeatInterval;		///< 心跳间隔，单位：秒
	objCfg.isOutputDebugLog = m_tagAuthConfig.m_nbIsOutputDebugLog;			///< 调试日志
	objCfg.isUseSSL = m_tagAuthConfig.m_bIsUseSSL;							///< 是否使用SSL
	///< 认证服务器配置，最多8组
	::memcpy( objCfg.Servers, m_tagAuthConfig.m_vctServers, sizeof(objCfg.Servers) );
	///< Crt证书
	::memcpy( objCfg.CrtFilename, m_tagAuthConfig.m_pszCrtFilename, sizeof(objCfg.CrtFilename) );
	///< Pfx证书
	::memcpy( objCfg.PfxFilename, m_tagAuthConfig.m_pszPfxFilename, sizeof(objCfg.PfxFilename) );
	///< Pfx证书密码
	::memcpy( objCfg.PfxPassword, m_tagAuthConfig.m_pszPfxPassword, sizeof(objCfg.PfxPassword) );
}







