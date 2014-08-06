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

//定义房间对象池和索引类型
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

	//初始化房间管理器
	virtual int32_t Initialize();
	//恢复房间管理器
	virtual int32_t Resume();
	//注销房间管理器
	virtual int32_t Uninitialize();

	int32_t CreateMedia(const ServerID nServerID, CMedia *&pMedia);

	int32_t GetMedia(const ServerID nServerID, CMedia *&pMedia);

	int32_t DestoryMedia(const ServerID nServerID);

	int32_t Clear();

	//获取对象池容量
	uint32_t GetCapacity() const;
	//获取对象池中对象数量
	uint32_t GetMediaCount() const;
	//对象池是否为空
	bool IsEmpty() const;
	//对象池是否已满
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
