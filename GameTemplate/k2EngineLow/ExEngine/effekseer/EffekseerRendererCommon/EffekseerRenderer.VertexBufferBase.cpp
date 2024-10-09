﻿
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#include "EffekseerRenderer.VertexBufferBase.h"

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
VertexBufferBase::VertexBufferBase(int size, bool isDynamic)
	: m_isDynamic(isDynamic)
	, m_size(size)
	, m_offset(0)
	, m_resource(NULL)
	, m_isLock(false)
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
VertexBufferBase::~VertexBufferBase()
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexBufferBase::Push(const void* buffer, int size)
{
	assert(m_isLock);
	memcpy(GetBufferDirect(size), buffer, size);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
int VertexBufferBase::GetMaxSize() const
{
	return m_size;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
int VertexBufferBase::GetSize() const
{
	return m_offset;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void* VertexBufferBase::GetBufferDirect(int size)
{
	assert(m_isLock);
	if (m_offset + size > m_size)
		return NULL;

	void* pBuffer = NULL;

	// バッファに追記
	pBuffer = m_resource + m_offset;
	m_offset += size;

	return pBuffer;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace EffekseerRenderer
  //-----------------------------------------------------------------------------------
  //
  //-----------------------------------------------------------------------------------