﻿========================================================================
    动态链接库：DataTablesPainter 项目概述
========================================================================


本模块提供 Databases/Tables/Records 为概念对象的数据管理方式。


使用方可以创建多个Database，并创建n个数据表，并为其插入m条数据记录。 
并且保证table中的数据记录是连续的内存块。


注意：
	a) 数据表的主键是一个string，限定长度建议在10以内的可见字符。
	b) 对使用者来说，多线程安全。


单元测试：
	测试启动脚本 python3 TestMemoPlugin.py, 用于调用Dll的导出函数ExecuteUnitTest()进行测试。


---------------------- 调用接口 -------------------------------------


/**
 * @brief							取得工厂对象引用,用于创建/管理/收回内存数据库对象
 * @return							返回管理对象的引用
 * @date							2017/5/9
 */
__declspec(dllexport) IDBFactory&	GetFactoryObject();


/**
 * @classs							RecordBlock
 * @brief							记录数据块类
 * @author							barry
 * @date							2017/5/9
 */
class RecordBlock
{
public:
	RecordBlock();
	RecordBlock( const RecordBlock& record );
	RecordBlock( char* pRecord, unsigned int nRecordLen );

public:
	/**
	 * @brief						记录对象为无效
	 * @return						true							无效对象
	 */
	bool							IsNone() const;

	/**
	 * @brief						记录体比较函数
	 * @param[in]					refRecord						比较标的对象
	 * @return						true							相同
									false							不同
	 */
	bool							Compare( const RecordBlock& refRecord );

	/**
	 * @brief						记录体deeply copy
	 * @param[in]					refRecord						内存copy的源头
	 * @return						==1								影响的数量
									==0								不需要copy，目标和源头数据完全一样
									<0								出现错误
	 */
	int								CloneFrom( const RecordBlock& refRecord );

public:
	/**
	 * @brief						取得数据地址
	 */
	const char*						GetPtr() const;

	/**
	 * @brief						数据长度
	 */
	unsigned int					Length() const;

protected:
	char*							m_pRecordData;					///< 数据结构地址
	unsigned int					m_nRecordLen;					///< 数据结构长度
};


/**
 * @class							I_Table
 * @brief							数据表操作接口
 * @author							barry
 * @date							2017/5/9
 */
class I_Table
{
public:
	/**
	 * @brief						追加新数据
	 * @param[in]					pRecord					记录体地址
	 * @param[in]					nRecordLen				记录体长度
	 * @param[out]					nDbSerialNo				数据库新增，更新操作流水号
	 * @return						>0						增加成功
									=0						记录已经存在，不需要增加
									<0						失败
	 */
	virtual int						InsertRecord( char* pRecord, unsigned int nRecordLen, unsigned __int64& nDbSerialNo ) = 0;

	/**
	 * @brief						更新数据
	 * @param[in]					pRecord					记录体地址
	 * @param[in]					nRecordLen				记录体长度
	 * @param[out]					nDbSerialNo				数据库新增，更新操作流水号
	 * @return						>0						增加成功
									=0						记录已经存在，不需要增加
									<0						失败
	 */
	virtual int						UpdateRecord( char* pRecord, unsigned int nRecordLen, unsigned __int64& nDbSerialNo ) = 0; 

	/**
	 * @brief						索引出记录对象
	 * @param[in]					pKeyStr					主键地址
	 * @param[in]					nKeyLen					主键长度
	 * @return						返回记录对象
	 */
	virtual RecordBlock				SelectRecord( char* pKeyStr, unsigned int nKeyLen ) = 0;

	/**
	 * @brief						删除某条记录
	 * @param[in]					pKeyStr					主键地址
	 * @param[in]					nKeyLen					主键长度
	 * @param[in,out]				nDbSerialNo				取出>nDbSerialNo的数据(若为0,则全部取出) & 将回填最新的流水号
	 * @return						>0						返回被操作到的记录数
									==0						未有记录被删除
									<0						删除时出现错误
	 */
	virtual int						DeleteRecord( char* pKeyStr, unsigned int nKeyLen, unsigned __int64& nDbSerialNo ) = 0;

	/**
	 * @brief						将数据表的数据原样copy到缓存
	 * @param[in]					pBuffer					缓存地址
	 * @param[in]					nBufferSize				缓存长度
	 * @param[in,out]				nDbSerialNo				取出>nDbSerialNo的数据(若为0,则全部取出) & 将回填最新的流水号
	 * @return						>=0						返回数据长度
									<						出错
	 */
	virtual int						CopyToBuffer( char* pBuffer, unsigned int nBufferSize, unsigned __int64& nDbSerialNo ) = 0;
};


/**
 * @class							I_Database
 * @brief							数据库操作接口
 * @author							barry
 * @date							2017/5/9
 */
class I_Database
{
public:
	virtual ~I_Database(){};

	/**
	 * @brief						根据消息id和消息长度，进行合适的数据表配置（在预备表中配置对应的占用关系）
	 * @param[in]					nBindID				数据类形标识号
	 * @param[in]					nRecordWidth		数据长度
	 * @param[in]					nKeyStrLen			主键长度
	 * @return						=0					配置成功
									>0					忽略（成功）
									<0					配置出错
	 */
	virtual bool					CreateTable( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen ) = 0;

	/**
	 * @brief						根据MessageID取得已经存在的或者分配一个新的内存表的引用
	 * @detail						本函数对每个messageid维护一个唯一且对应的内存表，根据nBindID值返回已经存在的，或新建后返回
	 * @param[in]					nBindID				MessageID
	 * @return						返回已经存在的内存表或新建的内存表
	 */
	virtual I_Table*				QueryTable( unsigned int nBindID ) = 0;

	/**
	 * @brief						清理所有数据表
	 */
	virtual int						DeleteTables() = 0;

	/**
	 * @brief						从硬盘恢复所有数据
	 */
	virtual bool					LoadFromDisk( const char* pszDataFile ) = 0;

	/**
	 * @brief						将所有数据存盘
	 */
	virtual bool					SaveToDisk( const char* pszDataFile ) = 0;

	/**
	 * @brief						取得数据更新自增流水号
	 */
	virtual unsigned __int64		GetUpdateSequence() = 0;
};


/**
 * @class							IDBFactory
 * @brief							内存数据分配管理工厂类接口
 * @author							barry
 * @date							2017/5/9
 */
class IDBFactory
{
public:
	/**
	 * @brief						创建并返回数据库对象指针
	 * @return						返回数据库指针的地址
	 */
	virtual I_Database*				GrapDatabaseInterface() = 0;

	/**
	 * @brief						释放分配的所有数据库对象
	 */
	virtual bool					ReleaseAllDatabase() = 0;

};


/////////////////////////////////////////////////////////////////////////////















