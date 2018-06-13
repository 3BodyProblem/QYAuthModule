#ifndef __INFRASTRUCTURE__HASH_H__
#define __INFRASTRUCTURE__HASH_H__


#pragma warning(disable: 4996)
#include <stdexcept>
#include <exception>
#include <functional>
#include <algorithm>


/**
 * @class			T_ListNode
 * @brief			��ϣ�ڵ�
 * @author			barry
 * @date			2017/4/2
 */
struct T_ListNode
{
public:
	T_ListNode() : nDataPos( -1 ), pPrevNode(NULL), pNextNode( NULL ), nHashKey( 0 ), nUpdateSequence( 0 ) {};
	bool				IsNull()	{	return nDataPos < 0;	}		///< �ڵ��Ƿ�����Чֵ
	bool				HasNext()	{	return pNextNode != NULL;	}	///< �ڵ��Ƿ�����һ��ֵ
	bool				IsFirst()	{	return pPrevNode == pNextNode && NULL != pNextNode;	}
	void				Clear()		{	nDataPos = -1; pNextNode = NULL; pPrevNode = NULL, nHashKey = 0; nUpdateSequence = 0;	}
	void				Delete()
	{
		if( NULL != pPrevNode )	{
			((struct T_ListNode*)(pPrevNode))->pNextNode = pNextNode;
		}
		if( NULL != pNextNode )	{
			((struct T_ListNode*)(pNextNode))->pPrevNode = pPrevNode;
		}
	}
public:
	unsigned __int64	nHashKey;						///< ��ֵ����
	int					nDataPos;						///< �������ڵ�����λ��
	unsigned __int64	nUpdateSequence;				///< ���´������
	void*				pPrevNode;						///< ������ͬkey����һ��ֵ������λ��
	void*				pNextNode;						///< ������ͬkey����һ��ֵ������λ��
};


#if _DEBUG
//#define		MAX_BUCKET_SIZE			(17)
//#define		MAX_DATATABLE_NUM		(1024*8)
#define		MAX_BUCKET_SIZE			(1024*8)
#define		MAX_DATATABLE_NUM		(MAX_BUCKET_SIZE*5)
#else
#define		MAX_BUCKET_SIZE			(1024*8)
#define		MAX_DATATABLE_NUM		(MAX_BUCKET_SIZE*5)
#endif


/**
 * @class			UInt2UIntHash
 * @brief			����ֵ��λ��ֵ�Ĺ�ϣӳ���
 * @note			Ĭ��ֵ������£����ݱ����ǹ�ϣͰ���ȵ�3��(����)
 * @author			barry
 * @date			2017/4/2
 */
class CollisionHash
{
public:
	CollisionHash();

	/**
	 * @brief			����keyȡ�ö�Ӧ��ӳ��ֵ
	 * @param[in]		nKey		����ֵ
	 * @return			����ӳ��ֵ
	 * @note			���key�����ڣ����׳�runtime�쳣
	 */
	struct T_ListNode*	operator[]( unsigned __int64 nKey );

	/**
	 * @brief			���ü�ֵ��
	 * @param[in]		nKey		��ֵ
	 * @param[in]		nDataPos	����λ������
	 * @param[in]		nSeqNo		������ˮ(ȫ��)
	 * @return			==1			���óɹ�
						==0			�Ѿ����ڣ�����Ҫ�½�
						<0			ʧ��
	 */
	int					NewKey( unsigned __int64 nKey, int nDataPos, unsigned __int64 nSeqNo );

	/**
	 * @brief			ɾ����ֵ��
	 * @param[in]		nKey		��ֵ
	 * @return			==1			���óɹ�
						==0			�Ѿ������ڣ�����Ҫɾ��
						<0			ʧ��
	 */
	int					DeleteKey( unsigned __int64 nKey );

	/**
	 * @brief			�����������
	 */
	void				Clear();

public:///< ���߷���
	/**
	 * @brief			ͳһ������ϡͰ����ײͰ��ָ��ĵ�ַƫ��
	 * @param[in]		nOffset		ƫ����(>0���ƫ��, <0��ǰƫ��)
	 */
	void				CoordinateNodePtr( int nOffset );

protected:
	/**
	 * @brief			��һ���ڵ���б����Ƴ�
	 * @detail			�������/ɾ���ڵ�/�ؽӽڵ�ǰ�����ڵ� + ���������ĺ��ǰ�� + ���ݴ�ϴ���ַҪ��׼���¼�����ڵ��ǰ��ָ���ƫ��
	 * @param[in]		pNode2Erase				Ҫ�Ƴ��ڵ�ĵ�ַ
	 * return			true					�����ɹ�
	 */
	bool				CoordinateCollisionBucketPtr( struct T_ListNode* pNode2Erase );

	/**
	 * @brief			�����й�ϣͰ�ڣ�nextָ�������ײͰɾ���ڵ��ַ�ģ�������ǰƫ�Ƶ���
	 * @param[in]		pEraseNodeInCollisionBucket	��ײͰ�ڱ�ɾ���ڵ�ĵ�ַ
	 * @return			true					�����ɹ�
	 */
	bool				CoordinateHashBucketPtr( struct T_ListNode* pEraseNodeInCollisionBucket );

	/**
	 * @brief			����������ĳ�ڵ�ɾ��ʱ�������������������λ��ָ��
	 * @param[in]		pNodeArray				��Ҫ���������������ݵ�ַ
	 * @param[in]		nNodeCount				�������ݵ���Ч����
	 * @param[in]		nEraseIndex				��ɾ����ֵ�����������е�λ��
	 */
	void				CoordinateNodeIndex( struct T_ListNode* pNodeArray, unsigned int nNodeCount, int nEraseIndex );

private:
	T_ListNode			m_BucketOfHash[MAX_BUCKET_SIZE];			///< ��ϡͰ
	T_ListNode			m_CollisionBucket[MAX_DATATABLE_NUM];		///< ��ϡ��ײͰ
	unsigned int		m_nUsedNumOfCollisionBucket;				///< �Ѿ�ʹ�õĽڵ�����(��ײͰ)
};




#endif






