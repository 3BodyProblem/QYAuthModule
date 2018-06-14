#include <string>
#include <algorithm>
#include "Configuration.h"
#include "Infrastructure/IniFile.h"
#include "Infrastructure/DateTime.h"


///< ----------------------------------- ��־�� ---------------------------------------------------------------


AuthLog::AuthLog()
{
	std::string			sPath;
	HMODULE				objModule = NULL;
    char				pszTmp[1024*8] = { 0 };

    ::GetModuleFileName( objModule, pszTmp, sizeof(pszTmp) );
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


///< ----------------------------------- ������ ---------------------------------------------------------------


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

	///< ���ã� ��������Ŀ¼(���ļ���)
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
	objCfg.ClientType = m_tagAuthConfig.m_nClientType;						///< �ͻ�������
	objCfg.ClientVersion = m_tagAuthConfig.m_nClientVersion;				///< �ͻ��˰汾�����汾���Ӱ汾��Build�ţ�32λ����8��12��12λ
	objCfg.CustomerID = m_tagAuthConfig.m_nCustomerID;						///< ȯ��ID
	objCfg.ServerCount = m_tagAuthConfig.m_nServerCount;					///< ��֤����������
	objCfg.NetworkTimeOut = m_tagAuthConfig.m_nNetworkTimeOut;				///< ����ͨѶ��ʱ����λ����
	objCfg.HeartbeatInterval = m_tagAuthConfig.m_nHeartbeatInterval;		///< �����������λ����
	objCfg.isOutputDebugLog = m_tagAuthConfig.m_nbIsOutputDebugLog;			///< ������־
	objCfg.isUseSSL = m_tagAuthConfig.m_bIsUseSSL;							///< �Ƿ�ʹ��SSL
	///< ��֤���������ã����8��
	::memcpy( objCfg.Servers, m_tagAuthConfig.m_vctServers, sizeof(objCfg.Servers) );
	///< Crt֤��
	::memcpy( objCfg.CrtFilename, m_tagAuthConfig.m_pszCrtFilename, sizeof(objCfg.CrtFilename) );
	///< Pfx֤��
	::memcpy( objCfg.PfxFilename, m_tagAuthConfig.m_pszPfxFilename, sizeof(objCfg.PfxFilename) );
	///< Pfx֤������
	::memcpy( objCfg.PfxPassword, m_tagAuthConfig.m_pszPfxPassword, sizeof(objCfg.PfxPassword) );
}





