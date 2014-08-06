/*
 * dal_updatedatainfotable.h
 *
 *  Created on: 2012-5-15
 *      Author: fuz
 */

#ifndef DAL_UPDATEDATAINFOTABLE_H_
#define DAL_UPDATEDATAINFOTABLE_H_


//数据成员更新信息表的大小
#define UpdateDataInfoTableSize		128

class UpdateDataInfoTable
{
public:
	UpdateDataInfoTable()
	{
		nOffset = 0;
		nDataSize = 0;
	}

	uint32_t	nOffset;
	uint32_t	nDataSize;
};

//#define offsetof(type, member)		(uint32_t)((uint8_t *)(&((type*)0)->member))
//#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define GetMemberOffset(member)		((uint32_t)(/*GetStartPos() + */(((uint8_t *)&(member)) - ((uint8_t *)&m_nStartPos))))


#endif /* DAL_UPDATEDATAINFOTABLE_H_ */
