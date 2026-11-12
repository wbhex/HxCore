/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_LINKED_LIST_OBJECT_H
#define HX_CORE_LINKED_LIST_OBJECT_H

#ifdef __cplusplus
#ifndef HX_LINKED_LIST_OBJECT
#define HX_LINKED_LIST_OBJECT

#include "Uncopyable.h"

namespace HxLibs
{
	//表示链表对象。
	template <typename TNode>
	class LinkedListObject : Uncopyable
	{
	private:
		//首个节点
		TNode* m_first{ nullptr };
		//末个节点
		TNode* m_last{ nullptr };

	protected:
		//移除一个节点。
		HX_INLINE void remove(TNode* node) HX_NOEXCEPT
		{
			if (node == m_first)
			{
				if (node == m_last)
				{
					m_first = nullptr;
					m_last = nullptr;
				}
				else
				{
					m_first = m_first->m_next;
					m_first->m_prev = nullptr;
				}
			}
			else if (node == m_last)
			{
				m_last = node->m_prev;
				m_last->m_next = nullptr;
			}
			else
			{
				node->m_prev->m_next = node->m_next;
				node->m_next->m_prev = node->m_prev;
			}
			node->m_prev = nullptr;
			node->m_next = nullptr;
		}

		//添加为首个节点。
		HX_INLINE void addFirst(TNode* node) HX_NOEXCEPT
		{
			if (m_first == nullptr)
			{
				m_first = node;
				m_last = node;
			}
			else
			{
				node->m_next = m_first;
				m_first->m_prev = node;
				m_first = node;
			}
		}
		//添加为末个节点。
		HX_INLINE void addLast(TNode* node) HX_NOEXCEPT
		{
			if (m_last == nullptr)
			{
				m_first = node;
				m_last = node;
			}
			else
			{
				node->m_prev = m_last;
				m_last->m_next = node;
				m_last = node;
			}
		}
		//添加为指定节点之前的节点。
		HX_INLINE void addBefore(TNode* where, TNode* node) HX_NOEXCEPT
		{
			if (where == m_first)
			{
				addFirst(node);
			}
			else
			{
				node->m_prev = where->m_prev;
				node->m_next = where;
				where->m_prev->m_next = node;
				where->m_prev = node;
			}
		}
		//添加为指定节点之后的节点。
		HX_INLINE void addAfter(TNode* where, TNode* node) HX_NOEXCEPT
		{
			if (where == m_last)
			{
				addLast(node);
			}
			else
			{
				node->m_prev = where;
				node->m_next = where->m_next;
				where->m_next->m_prev = node;
				where->m_next = node;
			}
		}

		//初始化当前对象。
		HX_INLINE LinkedListObject() HX_NOEXCEPT
		{
			//NOTHING
		}

	public:
		//首个节点
		HX_INLINE TNode* first() const HX_NOEXCEPT
		{
			return m_first;
		}
		//末个节点
		HX_INLINE TNode* last() const HX_NOEXCEPT
		{
			return m_last;
		}
	};

	//表示链表节点基类对象。
	template <typename TNode>
	class LinkedListNodeBase : Uncopyable
	{
		friend class LinkedListObject<TNode>;

	private:
		//上一个节点
		TNode* m_prev{ nullptr };
		//下一个节点
		TNode* m_next{ nullptr };

	protected:
		//初始化当前对象。
		HX_INLINE LinkedListNodeBase() HX_NOEXCEPT
		{
			//NOTHING
		}

	public:
		//上一个节点
		HX_INLINE TNode* prev() const HX_NOEXCEPT
		{
			return m_prev;
		}
		//下一个节点
		HX_INLINE TNode* next() const HX_NOEXCEPT
		{
			return m_next;
		}
	};
}
template <typename TNode>
using HxLinkedListObject = ::HxLibs::LinkedListObject<TNode>;
template <typename TNode>
using HxLinkedListNodeBase = ::HxLibs::LinkedListNodeBase<TNode>;

#endif //!HX_LINKED_LIST_OBJECT
#endif //__cplusplus

#endif //!HX_CORE_LINKED_LIST_OBJECT_H
