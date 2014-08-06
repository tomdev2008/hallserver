/*
 * media_mgt.h
 *
 *  Created on: 2012-1-11
 *      Author: jimm
 */

#ifndef MEDIA_MGT_H_
#define MEDIA_MGT_H_

#include "common/common_pool.h"
#include "common/common_hashlist.h"
#include "dal/dal_media.h"
#include "def/server_namespace.h"
#include "public_typedef.h"

FRAME_HALLSERVER_NAMESPACE_BEGIN

//���巿�����غ���������
typedef CPool<CMedia, MaxMediaCount>	MediaPool;
typedef CHashList<MediaIndex, MaxMediaCount, MaxMediaCount * 64>	MediaList;

class CMediaMgt : public CObject
{
protected:
	enum
	{
		enmAdditionalIndex_HashListIndex			= 0,
	};
public:
	CMediaMgt();
	virtual ~CMediaMgt();

	//��ʼ�����������
	virtual int32_t Initialize();
	//�ָ����������
	virtual int32_t Resume();
	//ע�����������
	virtual int32_t Uninitialize();

	int32_t CreateMedia(const ServerID nServerID, CMedia *&pMedia);

	int32_t GetMedia(const ServerID nServerID, CMedia *&pMedia);

	int32_t DestoryMedia(const ServerID nServerID);

	int32_t Clear();

	//��ȡ���������
	uint32_t GetCapacity() const;
	//��ȡ������ж�������
	uint32_t GetMediaCount() const;
	//������Ƿ�Ϊ��
	bool IsEmpty() const;
	//������Ƿ�����
	bool IsFull() const;

	int32_t GetAllMedia(CMedia *arrMediaObject[], const int32_t arrSize, int32_t &nMediaObjectCount);

	int32_t GetMediaServerPlayerCount(ServerID nServerID);

protected:
	int32_t CreateMedia(const ServerID nServerID, MediaPool::CIndex *&pIndex);

	int32_t GetMedia(const ServerID nServerID, MediaPool::CIndex*& pIndex);

protected:
	MediaList::Key MakeKey(const ServerID nServerID) const;
private:
	MediaPool					m_sMediaPool;
	MediaList					m_sMediaList;
};

FRAME_HALLSERVER_NAMESPACE_END



#endif /* MEDIA_MGT_H_ */
