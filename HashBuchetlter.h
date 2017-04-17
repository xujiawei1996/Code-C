#pragma once

#pragma once
#include "comm.h"
#include <vector>
#include <string>
#include <assert.h>
#include <iostream>
using namespace std;

template<class K, class V>
struct HashTableNode
{
	HashTableNode(const pair<K, V>& kv)
	: _kv(kv)
	, _next(NULL)
	{}

	HashTableNode<K, V>* _next;
	pair<K, V> _kv;
};

template<class K, class V, class _HashFun = HashFunDef<K>>
class HashTableBucket;

template<class K, class V, class Ref, class Ptr, class _HashFun>
struct _HashTableIterator
{
	typedef HashTableNode<K, V> Node;
	Node* _node;
	HashTableBucket<K, V, _HashFun>* _ht;

	typedef _HashTableIterator<K, V, Ref, Ptr, _HashFun> Self;

	_HashTableIterator(Node* pNode, HashTableBucket<K, V, _HashFun>* ht)
		: _node(pNode)
		, _ht(ht)
	{}

	Ref operator*()
	{
		return _node->_kv;
	}

	Ptr operator->()
	{
		return &(operator*());
	}

	Self& operator++()
	{
		_node = _Next(_node);
		return *this;
	}

	Self operator++(int)
	{
		Self temp(*this);
		++(*this);
		return temp;
	}

	bool operator==(const Self& self)
	{
		return _ht == self._ht && _node == self._node;
	}

	bool operator!=(const Self& self)
	{
		return !(*this == self);
	}

	Node* _Next(Node* node)
	{
		Node* next = node->_next;
		if (next)
			return next;
		else
		{
			size_t index = _ht->HashFun(node->_kv.first) + 1;
			for (; index < _ht->_table.size(); ++index)
			{
				next = _ht->_table[index];
				if (next)
					return next;
			}

			return NULL;
		}
	}
};

// 增容 STL
template<class K, class V, class _HashFun = HashFunDef<K>>
class HashTableBucket
{
public:
	friend _HashTableIterator<K, V, pair<K, V>&, pair<K, V>*, _HashFun>;
	typedef _HashTableIterator<K, V, pair<K, V>&, pair<K, V>*, _HashFun> Iterator;
	typedef _HashTableIterator<K, V, const pair<K, V>&, const pair<K, V>*, _HashFun> ConstIterator;

	HashTableBucket(size_t size = 10)
		: _size(0)
	{
		_table.resize(GetNextPrim(size));
	}

	pair<bool, Iterator> Insert(const pair<K, V>& kv)
	{
		CheckCapacity();
		size_t index = HashFun(kv.first);
		HashTableNode<K, V>* pCur = _table[index];
		while (NULL != pCur)
		{
			if (kv.first == pCur->_kv.first)
				return make_pair(false, Iterator(pCur, this));
			pCur = pCur->_next;
		}

		HashTableNode<K, V>* pNewNode = new HashTableNode<K, V>(kv);
		pNewNode->_next = _table[index];
		_table[index] = pNewNode;
		_size++;

		return make_pair(true, Iterator(pNewNode, this));
	}

	bool Find(const K& key)
	{
		size_t index = HashFun(key);
		HashTableNode<K, V>* pCur = _table[index];
		while (NULL != pCur)
		{
			if (key == pCur->_kv.first)
				return true;

			pCur = pCur->_next;
		}

		return false;
	}

	bool Remove(const K& key)
	{
		size_t index = HashFun(key);
		HashTableNode<K, V>* pCur = _table[index];
		HashTableNode<K, V>* pPre = pCur;
		while (NULL != pCur)
		{
			if (key == pCur->_kv.first)
			{
				if (pCur == _table[index])
				{
					_table[index] = pPre->_next;
				}
				else
				{
					pPre->_next = pCur->_next;
				}

				delete pCur;
				--_size;
				return true;
			}

			pPre = pCur;
			pCur = pCur->_next;

		}
		return false;
	}

	void Clear()
	{
		for (size_t idx = 0; idx < _table.size(); ++idx)
		{
			HashTableNode<K, V>* pCur = _table[idx];
			while (pCur)
			{
				HashTableNode<K, V>* pDel = pCur;
				pCur = pCur->_next;
				delete pDel;
			}

			_table[idx] = NULL;
		}

		_size = 0;
	}

	~HashTableBucket()
	{
		Clear();
	}

	//HashTableBucket(HashTableBucket<T>& ht);
	//HashTableBucket<T>& operator=(HashTableBucket<T>& ht);

	Iterator Begin()
	{
		for (size_t index = 0; index < _table.size(); ++index)
		{
			if (_table[index])
				return Iterator(_table[index], this);
		}

		return Iterator(NULL, this);
	}

	Iterator End()
	{
		return Iterator(NULL, this);
	}

	V& operator[](const K& key)
	{
		size_t index = HashFun(key);
		HashTableNode<K, V>* pCur = _table[index];
		while (pCur)
		{
			if (pCur->_kv.first == key)
				return pCur->_kv.second;
		}

		assert(false);
	}

private:
	//
	void CheckCapacity()
	{
		if (_size == _table.size())
		{
			HashTableBucket<K, V> temp(GetNextPrim(_size));
			// 增容
			// 原哈希桶中的结点插入到新哈希桶
			for (size_t idx = 0; idx < _size; ++idx)
			{
				HashTableNode<K, V>* pCur = _table[idx];
				HashTableNode<K, V>* pPre = NULL;
				while (pCur)
				{
					pPre = pCur;
					pCur = pCur->_next;

					// 定位在新哈希桶中的位置
					size_t index = temp.HashFun(pPre->_kv.first);
					pPre->_next = temp._table[index];
					temp._table[index] = pPre;
					temp._size++;
				}

				_table[idx] = NULL;
			}

			//Swap(temp);
			std::swap(_size, temp._size);
			std::swap(_table, temp._table);
		}
	}

private:
	size_t HashFun(const K& key)
	{
		return _HashFun()(key) % _table.capacity();
	}

private:
	vector<HashTableNode<K, V>*> _table;
	size_t _size;
};

void TestItreator()
{

	HashTableBucket<string, string> ht;
	ht.Insert(make_pair("1", "杨过"));
	ht.Insert(make_pair("2", "小龙女"));
	ht.Insert(make_pair("3", "靖哥哥"));
	ht.Insert(make_pair("4", "蓉儿"));
	ht.Insert(make_pair("5", "东邪"));
	cout << ht["1"] << endl;
	HashTableBucket<string, string>::Iterator it = ht.Begin();

	while (it != ht.End())
	{
		cout << (*it).second << endl;
		++it;
	}
}