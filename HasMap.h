#pragma  once

#include <vector>
#include <iostream>
using namespace std;

class BitMap
{
public:
	BitMap()
	{}

	BitMap(size_t size)
	{
	  //因为是int 4个字节 32比特位 
		_table.resize((size >> 5) + 1);
	}

	void Set(int data)
	{
		//计算第几个字节（data/32），第几位(data%32)
		size_t byteNo = data >> 5;
		size_t bitNo = data % 32;
		_table[byteNo] |= (1 << bitNo);
	}

	//恢复为原来的
	void ReSet(int data)
	{
		size_t byteNo = data >> 5;
		size_t bitNo = data % 32;
		//其他位都为1 这一位为0 与1 若原为1则此时变为0
		_table[byteNo] &= ~(1<<bitNo);
	}

	//检查数字是否在其中
	bool Check(int data)
	{
		size_t byteNo = data >> 5;
		size_t bitNo = data % 32;

		//如果这一位左移 按位与 如果此时不为0 则此数字存在
		if ( (1 << bitNo)&_table[byteNo] )
			return true;
		return false;
	}

private:
	vector<int> _table;
};

void Test()
{
	BitMap bp(1);
	bp.Set(0);
	bp.Set(1);
	bp.Set(2);
	bp.Set(3);
	bp.Set(4);
	bp.Set(5);
	bp.Set(6);

	cout << bp.Check(3);
	cout << bp.Check(7);
}
