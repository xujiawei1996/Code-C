//#ifndef __HASH_HPP__
//#define __HASH_HPP__
//
//#include <stdio.h>
//#include <iostream>
//#include <string>
//#include "comm.h"
//using namespace std;
//
//enum State
//{
//	EMPTY,
//	EXIST,
//	DELETE
//};
//
////线性探测
///*
//template <typename T>
//class HashTable
//{
//public:
//	HashTable(size_t capacity = 10)
//		:_Capacity(capacity)
//		, _Size(0)
//	{
//		_hashTable = new T[_Capacity];
//		_state = new State[_Capacity];
//		memset(_state, EMPTY, sizeof(_state)*_Capacity);
//	}
//
//	bool Insert(const T& key)
//	{
//		//检查容量 容量不足就扩容
//		_CheckCapacity();
//
//		size_t index = HashFun(key);
//
//		//当次位置存在 且此位置的元素就为key就返回 否则就向后走 并且索引要对容量取余 因为如果第一个为空应该插入到第一个里
//		while (_state[index] == EXIST)
//		{
//			if (_hashTable[index] == key)
//				return true;
//			index++;
//			index %= _Capacity;
//		}
//
//		//找到为空的地方 把此地方改为EXIST 并且把此位置赋值为key
//		_state[index] = EXIST;
//		_hashTable[index] = key;
//		_Size++;
//		return true;
//	}
//
//	//查找 如果此时这里不为empty，并且值与key相同还exist 就返回此下标且返回
//	//否则就继续查找 如果走了一圈还没有找到就返回false
//	//startidx为起始值 如果走了一圈又到这个位置 就退出
//	bool Find(const T& key,size_t & hasidx)
//	{
//		size_t index = HashFun(key);
//		size_t startidx = index;
//		while (_state[index] != EMPTY)
//		{
//			if (_hashTable[index] == key && _state[index] == EXIST)
//			{
//				hasidx = index;
//				return true;
//			}
//			index++;
//			index %= _Capacity;
//			if (index == startidx)
//				break;
//		}
//		return false;
//	}
//
//	bool Remove(const T& key)
//	{
//		size_t index = 0;
//		if(Find(key,index))
//		{
//			_state[index] = DELETE;
//			_Size--;
//			return true;
//		}
//		return false;
//	}
//
//	~HashTable()
//	{
//		if (_hashTable)
//		{
//			delete[]_hashTable;
//			_hashTable = NULL;
//		}
//		if (_state)
//		{
//			delete[]_state;
//			_state = NULL;
//		}
//	}
//
//private:
//	int HashFun(const T& key)
//	{
//		return key%_Capacity;
//	}
//
//	void _CheckCapacity()
//	{
//		if (_Size >= _Capacity)
//		{
//			_Capacity *= 2;
//			T * temphashTable = new T[_Capacity];
//			State * tempState = new State[_Capacity];
//			memcpy(temphashTable, _hashTable, sizeof(_hashTable[0])*_Size);
//			memcpy(tempState,_state,sizeof(_state[0])*_Size);
//
//			delete[]_hashTable;
//			delete[] _state;
//			_hashTable = temphashTable;
//			_state = tempState;
//		}
//	}
//
//
//
//private:
//	T* _hashTable;
//	State* _state;
//	size_t _Capacity;
//	size_t _Size;
//};
//
//
//void test()
//{
//	HashTable<int> ht;
//	size_t index = 0;
//	ht.Insert(10);
//	ht.Insert(20);
//	ht.Insert(30);
//	ht.Insert(40);
//	ht.Insert(50);
//	ht.Insert(60);
//	ht.Insert(70);
//	ht.Insert(80);
//	ht.Insert(90);
//	ht.Insert(6);
//	ht.Insert(3);
//	ht.Insert(4);
//	ht.Remove(10);
//	ht.Insert(20);
//	cout<<ht.Find(13, index)<<endl;
//}
//*/
//
//template <typename K>
//struct HashFunDef
//{
//	size_t operator()(const K& key)
//	{
//		return key;
//	}
//};
//
//
////二次探测
//template <typename K ,typename V,class Fun = HashFunDef<K>>
//class HashTable
//{
//public:
//	//容量应该给成质数 
//	HashTable(size_t capacity = 10)
//		:_Capacity(capacity)
//		,_Size(0)
//	{
//		_Capacity = GetNextPrim(_Capacity);
//		_hashTable = new pair<K,V>[_Capacity];
//		_state = new State[_Capacity];
//		memset(_state, EMPTY, sizeof(_state)*_Capacity);
//	}
//
//	bool Insert(const K& key,const V& value)
//	{
//		CheckCapacity();
//
//		size_t index = HashFun1(key);
//		size_t i = 1;
//		//当次位置存在 且此位置的元素就为key就返回 否则就向后走 并且索引要对容量取余 因为如果第一个为空应该插入到第一个里
//		while (_state[index] == EXIST)
//		{
//			if (_hashTable[index].first == key)
//				return false;
//
//			index = HashFun2(index, i);
//			
//			//二次探测
//			index++;
//			index %= _Capacity;
//		}
//
//		//找到为空的地方 把此地方改为EXIST 并且把此位置赋值为key
//		_state[index] = EXIST;
//		_hashTable[index] = pair<K,V>(key,value);
//		_Size++;
//		return true;
//	}
//
//	//查找 如果此时这里不为empty，并且值与key相同还exist 就返回此下标且返回
//	//否则就继续查找 如果走了一圈还没有找到就返回false
//	//startidx为起始值 如果走了一圈又到这个位置 就退出
//	bool Find(const K& key, size_t & hasidx)
//	{
//		size_t index = HashFun1(key);
//		size_t startidx = index;
//		size_t i = 1;
//		//如果哈希表元素存满 并且找的元素不在哈希表里 就会发生死循环 所以不应该让他存满
//		while (_state[index] != EMPTY)
//		{
//			if (_hashTable[index].first == key && _state[index] == EXIST)
//			{
//				hasidx = index;
//				return true;
//			}
//			index = HashFun2(index, i++);
//		}
//		return false;
//	}
//
//	bool Remove(const K& key)
//	{
//		size_t index = 0;
//		if (Find(key, index))
//		{
//			_state[index] = DELETE;
//			_Size--;
//			return true;
//		}
//		return false;
//	}
//
//	~HashTable()
//	{
//		if (_hashTable)
//		{
//			delete[]_hashTable;
//			_hashTable = NULL;
//		}
//		if (_state)
//		{
//			delete[]_state;
//			_state = NULL;
//		}
//	}
//
//	HashTable(HashTable<K, V>& ht)
//		: _hashTable(NULL)
//		, _state(NULL)
//	{
//		HashTable<K, V>temp(ht._Capacity);
//		for (size_t index = 0; index < ht._Capacity; ++index)
//		{
//			if (EXIST == ht._state[index])
//				temp.Insert(ht._hashTable[index].first, ht._hashTable[index].second);
//		}
//		Swap(temp);
//	}
//
//	//这种不好 如果自己给自己 也要构建副本出来
//	//HashTable<K, V>& operator=(HashTable<K, V> ht)
//	//{
//	//	Swap(ht);
//	//	return *this;
//	//}
//
//	HashTable<K, V>& operator=(const HashTable<K,V>&ht)
//	{
//		if (this != ht)
//		{
//			HashTable<K, V>temp(ht);
//			Swap(ht);
//		}
//	}
//
//	void Print()
//	{
//		for (size_t idx = 0; idx < _Capacity; ++idx)
//		{
//			if (_state[idx] != EMPTY)
//				cout << "_tables[" << _hashTable[idx].first << "]-->_state[" << _state[idx] << "]" << endl;
//		}
//	}
//
//private:
//	void CheckCapacity()
//	{
//		if (_Size / _Capacity >= 0.7)
//		{
//			_Capacity = GetNextPrim(_Capacity);
//			HashTable<K, V> ht(_Capacity);
//
//			for (size_t index = 0; index < _Capacity; ++index)
//			{
//				if (EXIST == _state[index])
//					ht.Insert(ht._hashTable[index].first,ht._hashTable[index].second);
//			}
//			Swap(ht);
//		}		
//	}
//
//	size_t GetNextPrim(size_t prev)//prev = 10
//	{
//		const int _PrimeSize = 28;
//		static const unsigned long _PrimeList[_PrimeSize] =
//		{
//			53ul, 97ul, 193ul, 389ul, 769ul,
//			1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
//			49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
//			1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
//			50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
//			1610612741ul, 3221225473ul, 4294967291ul
//		};
//
//
//		for (size_t idx = 0; idx < _PrimeSize; ++idx)
//		{
//			if (prev < _PrimeList[idx])
//				return _PrimeList[idx];
//		}
//
//		return -1;
//	}
//
//	int HashFun1(const K& key)
//	{
//		return Fun()(key)%_Capacity;
//		//return key%_Capacity;
//	}
//
//	int HashFun2(size_t prevHashIdx,size_t i)
//	{
//		return (prevHashIdx + (i<<1) -1)%_Capacity;
//	}
//
//	void Swap(HashTable<K,V>& ht)
//	{
//		std::swap(_hashTable, ht._hashTable);
//		std::swap(_state, ht._state);
//		std::swap(_Capacity, ht._Capacity);
//		std::swap(_Size, ht._Size);
//
//	}
//
//private:
//	pair<K,V>* _hashTable;
//	State* _state;
//	size_t _Capacity;
//	size_t _Size;
//};
//
//void test()
//{
//	HashTable<int,int> ht;
//	size_t index = 0;
//	ht.Insert(10,1);
//	ht.Insert(20,2);
//	ht.Insert(30,31);
//	ht.Insert(8,8);
//	ht.Print();
//	cout << ht.Find(13, index) << endl;
//}
//
//void test2()
//{
//	HashTable<string, string, HashFunDef<string> > ht;
//	ht.Insert("hash", "哈希");
//	ht.Insert("哈希", "hash");
//	ht.Print();
//	ht.Remove("hash");
//	ht.Print();
//}
//#endif