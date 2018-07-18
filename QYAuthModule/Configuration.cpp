#include <string>
#include <algorithm>
#include "Configuration.h"
#include "Infrastructure/IniFile.h"
#include "Infrastructure/DateTime.h"


///< ----------------------------------- 日志类 ---------------------------------------------------------------


AuthLog::AuthLog()
{
	std::string			sPath;
    char				pszTmp[1024*8] = { 0 };

    ::GetModuleFileName( g_oModule, pszTmp, sizeof(pszTmp) );
    sPath = pszTmp;
    sPath = sPath.substr( 0, sPath.find(".dll") ) + ".log";

	m_objLogFile.open( sPath.c_str(), std::ios_base::app|std::ios_base::out );
}

AuthLog& AuthLog::GetLogger()
{
	static	AuthLog		obj;

	return obj;
}

void AuthLog::WriteInfo( const char* szFormat, ... )
{
	va_list				valist;
	int					nLen = 0;
	char				szsrc[80] = { 0 };
	char				tempbuf[1024*2] = { 0 };
	char				pszInfo[1024*2] = { 0 };
	DateTime			objDateTime = DateTime::Now();

	va_start( valist, szFormat );
	_vsnprintf( tempbuf, sizeof(tempbuf), szFormat, valist );
	va_end( valist ); 
	nLen = ::sprintf( pszInfo, "%d.%d [INF]%s\n", objDateTime.DateToLong(), objDateTime.TimeToLong(), tempbuf );

	if( m_objLogFile.is_open() )
	{
		CriticalLock	lock( m_objLock );

		m_objLogFile.write( pszInfo, nLen );
		m_objLogFile.flush();
		::printf( "%s", pszInfo );
	}
	else
	{
		::printf( "|%s", pszInfo );
	}
}

void AuthLog::WriteWarning( const char* szFormat, ... )
{
	va_list				valist;
	int					nLen = 0;
	char				szsrc[80] = { 0 };
	char				tempbuf[1024*2] = { 0 };
	char				pszInfo[1024*2] = { 0 };
	DateTime			objDateTime = DateTime::Now();

	va_start( valist, szFormat );
	_vsnprintf( tempbuf, sizeof(tempbuf), szFormat, valist );
	va_end( valist ); 
	nLen = ::sprintf( pszInfo, "%d.%d [WARN]%s\n", objDateTime.DateToLong(), objDateTime.TimeToLong(), tempbuf );

	if( m_objLogFile.is_open() )
	{
		CriticalLock	lock( m_objLock );

		m_objLogFile.write( pszInfo, nLen );
		m_objLogFile.flush();
		::printf( "%s", pszInfo );
	}
	else
	{
		::printf( "|%s", pszInfo );
	}
}

void AuthLog::WriteError( const char* szFormat, ... )
{
	va_list				valist;
	int					nLen = 0;
	char				szsrc[80] = { 0 };
	char				tempbuf[1024*2] = { 0 };
	char				pszInfo[1024*2] = { 0 };
	DateTime			objDateTime = DateTime::Now();

	va_start( valist, szFormat );
	_vsnprintf( tempbuf, sizeof(tempbuf), szFormat, valist );
	va_end( valist ); 
	nLen = ::sprintf( pszInfo, "%d.%d [ERR]%s\n", objDateTime.DateToLong(), objDateTime.TimeToLong(), tempbuf );

	if( m_objLogFile.is_open() )
	{
		CriticalLock	lock( m_objLock );

		m_objLogFile.write( pszInfo, nLen );
		m_objLogFile.flush();
		::printf( "%s", pszInfo );
	}
	else
	{
		::printf( "|%s", pszInfo );
	}
}


///< ----------------------------------- 配置类 ---------------------------------------------------------------


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
	if( 0 != (nErrCode=oIniFile.load( sPath )) )	{
		AuthLog::GetLogger().WriteError( "Configuration::Initialize() : invalid configuration file path: %s", sPath.c_str() );
		return -1;
	}

	///< 客户端类型
	m_tagAuthConfig.m_nClientType = oIniFile.getIntValue( std::string("client"), std::string("type"), nErrCode );
	if( 0 != nErrCode )	{
		AuthLog::GetLogger().WriteError( "Configuration::Initialize() : invliad client type value" );
		return -2;
	}

	///< 客户端版本，主版本、子版本、Build号，32位各用8、12、12位
	m_tagAuthConfig.m_nClientVersion = oIniFile.getIntValue( std::string("client"), std::string("version"), nErrCode );
	if( 0 != nErrCode )	{
		AuthLog::GetLogger().WriteError( "Configuration::Initialize() : invliad client version" );
		return -3;
	}

	///< 券商ID
	m_tagAuthConfig.m_nCustomerID = oIniFile.getIntValue( std::string("client"), std::string("participantid"), nErrCode );
	if( 0 != nErrCode )	{
		AuthLog::GetLogger().WriteError( "Configuration::Initialize() : invliad participantid" );
		return -4;
	}

	///< 认证服务器数量
	m_tagAuthConfig.m_nServerCount = oIniFile.getIntValue( std::string("server"), std::string("count"), nErrCode );
	if( 0 != nErrCode )	{
		AuthLog::GetLogger().WriteError( "Configuration::Initialize() : invliad server count" );
		return -5;
	}

	///< 认证服务器配置，最多8组
	for( int n = 0; n < AUTHSERVER_MAX && n < m_tagAuthConfig.m_nServerCount; n++ ) {
		std::string		sIPTag = "ip_";
		std::string		sPortTag = "port_";
		char			pszTmp[32] = { 0 };

		sIPTag += ::itoa(n, pszTmp, 10);
		strcpy( m_tagAuthConfig.m_vctServers[n].Host, oIniFile.getStringValue( std::string("server"), sIPTag, nErrCode ).c_str() );
		if( 0 != nErrCode )	{
			AuthLog::GetLogger().WriteError( "Configuration::Initialize() : invliad ip tag(%d)", n );
			return -100;
		}

		sPortTag += ::itoa(n, pszTmp, 10);
		m_tagAuthConfig.m_vctServers[n].Port = oIniFile.getIntValue( std::string("server"), sPortTag, nErrCode );
		if( 0 != nErrCode )	{
			AuthLog::GetLogger().WriteError( "Configuration::Initialize() : invliad port tag(%d)", n );
			return -101;
		}
	}

	///< 网络通讯超时，单位：秒
	m_tagAuthConfig.m_nNetworkTimeOut = oIniFile.getIntValue( std::string("server"), std::string("timeout"), nErrCode );
	if( 0 != nErrCode )	{
		AuthLog::GetLogger().WriteError( "Configuration::Initialize() : invliad server timeout" );
		return -6;
	}

	///< 心跳间隔，单位：秒
	m_tagAuthConfig.m_nHeartbeatInterval = oIniFile.getIntValue( std::string("server"), std::string("heartbeat"), nErrCode );
	if( 0 != nErrCode )	{
		AuthLog::GetLogger().WriteError( "Configuration::Initialize() : invliad server heart beat" );
		return -7;
	}

	///< 调试日志
	m_tagAuthConfig.m_nbIsOutputDebugLog = true;

	///< 是否使用SSL
	int	nUseSSL = oIniFile.getIntValue( std::string("server"), std::string("ssl"), nErrCode );
	if( 0 != nErrCode )	{
		AuthLog::GetLogger().WriteError( "Configuration::Initialize() : invliad server heart beat" );
		return -7;
	}
	if( nUseSSL > 0 ) {
		m_tagAuthConfig.m_bIsUseSSL = true;
	} else {
		m_tagAuthConfig.m_bIsUseSSL = false;
	}

	///< Crt证书
	strcpy( m_tagAuthConfig.m_pszCrtFilename, oIniFile.getStringValue( std::string("server"), std::string("crtpath"), nErrCode ).c_str() );
	if( 0 != nErrCode )	{
		AuthLog::GetLogger().WriteError( "Configuration::Initialize() : invliad Crt certification path" );
	}

	///< Pfx证书
	strcpy( m_tagAuthConfig.m_pszPfxFilename, oIniFile.getStringValue( std::string("server"), std::string("pfxpath"), nErrCode ).c_str() );
	if( 0 != nErrCode )	{
		AuthLog::GetLogger().WriteError( "Configuration::Initialize() : invliad Crt certification path" );
	}

	///< Pfx证书密码
	strcpy( m_tagAuthConfig.m_pszPfxPassword, oIniFile.getStringValue( std::string("server"), std::string("pfxpassword"), nErrCode ).c_str() );
	if( 0 != nErrCode )	{
		AuthLog::GetLogger().WriteError( "Configuration::Initialize() : invliad Crt certification path" );
	}

	return 0;
}

void Configuration::FetchAuthConfig( CQAuthClientInput& objCfg )
{
	objCfg.Logger = &(AuthLog::GetLogger());
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





