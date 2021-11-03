#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#include <iomanip> 
#include <exception>
#include <typeinfo>
#include <string.h>
#include<iostream>
using namespace std;

template <typename T>
class MAT {
	T* const e;							//指向所有整型矩阵元素的指针
	const int r, c;							//矩阵的行r和列c大小
public:
	MAT(int r, int c);						//矩阵定义
	MAT(const MAT& a);				//深拷贝构造
	MAT(MAT&& a)noexcept;			//移动构造
	virtual ~MAT()noexcept;
	virtual T* const operator[ ](int r);//取矩阵r行的第一个元素地址，r越界抛异常
	virtual MAT operator+(const MAT& a)const;	//矩阵加法，不能加抛异常
	virtual MAT operator-(const MAT& a)const;	//矩阵减法，不能减抛异常
	virtual MAT operator*(const MAT& a)const;	//矩阵乘法，不能乘抛异常
	virtual MAT operator~()const;					//矩阵转置
	virtual MAT& operator=(const MAT& a);		//深拷贝赋值运算
	virtual MAT& operator=(MAT&& a)noexcept;	//移动赋值运算
	virtual MAT& operator+=(const MAT& a);		//“+=”运算
	virtual MAT& operator-=(const MAT& a);		//“-=”运算
	virtual MAT& operator*=(const MAT& a);		//“*=”运算
//print输出至s并返回s：列用空格隔开，行用回车结束
	virtual char* print(char* s)const noexcept;
};
template MAT<int>;
template MAT<long long>;



/// <summary>
/// 矩阵定义
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="r"></param>
/// <param name="c"></param>
template<typename T>
MAT<T>::MAT(int r, int c) :r(r), c(c), e(new T[r * c]) {}



/// <summary>
/// 深拷贝构造
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="a">源矩阵</param>
template<typename T>
MAT<T>::MAT(const MAT& a) : e(new T[a.r * a.c]), r(a.r), c(a.c)
{
	for (int i = 0; i < a.r * a.c; i++)
	{
		e[i] = a.e[i];
	}
}



/// <summary>
/// 移动构造
/// </summary>
/// <typeparam name="T">元素类型</typeparam>
/// <param name="a">源矩阵</param>
/// <returns></returns>
template<typename T>
MAT<T>::MAT(MAT&& a) noexcept :e(a.e), r(a.r), c(a.c)
{
	*(T*)&(a.c) = 0;
	*(T*)&(a.r) = 0;
	*(T**)&(a.e) = NULL;
}



/// <summary>
/// 析构
/// </summary>
/// <typeparam name="T"></typeparam>
/// <returns></returns>
template<typename T>
MAT<T>::~MAT() noexcept
{
	if (e != nullptr)
	{
		free(e);
	}
	*(T**)&e = nullptr;//将剩余变量赋值为0
	*(int*)&r = 0;
	*(int*)&c = 0;
}



/// <summary>
/// 取矩阵r行的第一个元素地址，r越界抛异常
/// </summary>
/// <typeparam name="T">元素类型</typeparam>
/// <param name="r"></param>
/// <returns></returns>
template<typename T>
T* const MAT<T>::operator[](int r)
{
	if (r < this->r && r >= 0)
	{
		return &e[r * c];
	}
	else
	{
		throw("Operator[] ERROR!");
	}
}



/// <summary>
/// 矩阵加法
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="a"></param>
/// <returns></returns>
template<typename T>
MAT<T> MAT<T>::operator+(const MAT& a) const
{
	if (r == a.r && c == a.c)
	{
		MAT R(r, c);
		int tmp = 0, i = 0;
		for (i = 0; i < r * c; i++)//循环相加
		{
			R.e[i] = e[i] + a.e[i];
		}
		return R;

	}
	throw("Operator+ ERROR!");//抛出异常
}



/// <summary>
/// 矩阵减法
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="a"></param>
/// <returns></returns>
template<typename T>
MAT<T> MAT<T>::operator-(const MAT& a) const
{
	if (r == a.r && c == a.c)
	{
		MAT R(r, c);
		int i = 0;
		for (i = 0; i < r * c; i++)
		{
			R.e[i] = e[i] - a.e[i];//相减
		}
		return R;
	}
	throw("Operator- ERROR!");//抛出异常
}



/// <summary>
/// 矩阵乘法
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="a"></param>
/// <returns></returns>
template<typename T>
MAT<T> MAT<T>::operator*(const MAT& a) const
{
	if (c == a.r)//当this的列数等于a的行数时
	{
		MAT R(r, a.c);
		int i = 0, j = 0, k = 0;
		for (i = 0; i < r; i++)
		{
			for (j = 0; j < a.c; j++)
			{
				R[i][j] = 0;
				for (k = 0; k < c; k++)
				{
					R[i][j] = R[i][j] + e[i * c + k] * a.e[k * c + j];
				}
			}
		}
		return R;
	}
	throw("Operator* ERROR!");
}



/// <summary>
/// 矩阵转置
/// </summary>
/// <typeparam name="T"></typeparam>
/// <returns></returns>
template<typename T>
MAT<T> MAT<T>::operator~() const
{
	MAT R(c, r);
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			R[j][i] = e[i * c + j];
		}
	}
	return R;
}



/// <summary>
/// 深拷贝赋值运算
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="a"></param>
/// <returns></returns>
template<typename T>
MAT<T>& MAT<T>::operator=(const MAT& a)
{
	if (this != &a)//若this和a不相等则进行操作
	{
		if (e != nullptr)//如果p中队列不为空
		{
			free(e);//则释放内存，避免泄露
		}
		*(T**)&e = (T*)malloc(sizeof(T) * r * c);//申请空间
		for (int i = 0; i < r * c; i++)//利用循环移动队列中元素
		{
			e[i] = a.e[i];
		}
		//进行相应赋值
		*(int*)&r = a.r;
		*(int*)&c = a.c;
	}
	return *this;
}



/// <summary>
/// 移动赋值运算
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="a"></param>
/// <returns></returns>
template<typename T>
MAT<T>& MAT<T>::operator=(MAT&& a) noexcept
{
	if (this != &a)//若p和q不相等则进行操作
	{
		if (e != nullptr)//如果p中队列不为空
		{
			free(e);//则释放内存，避免泄露
		}
		*(T**)&e = a.e;//进行相应浅拷贝赋值
		*(int*)&r = a.r;
		*(int*)&c = a.c;

		*(T**)&a.e = nullptr;//将原来结构体赋值为0
		*(int*)&a.r = 0;
		*(int*)&a.c = 0;
		return *this;
	}
}



/// <summary>
/// “+=”运算
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="a"></param>
/// <returns></returns>
template<typename T>
MAT<T>& MAT<T>::operator+=(const MAT& a)
{
	if (r == a.r && c == a.c)
	{
		int tmp = 0, i = 0;
		for (i = 0; i < r * c; i++)//循环相加
		{
			e[i] = e[i] + a.e[i];
		}
		if (i == r * c)//检查循环是否完整结束
		{
			return *this;
		}
	}
	throw("Operator+= ERROR!");//抛出异常
}



/// <summary>
/// “-=”运算
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="a"></param>
/// <returns></returns>
template<typename T>
MAT<T>& MAT<T>::operator-=(const MAT& a)
{
	if (r == a.r && c == a.c)
	{
		int i = 0;
		for (i = 0; i < r * c; i++)//循环相加
		{
			e[i] = e[i] - a.e[i];//相减
		}
		if (i == r * c)//检查循环是否完整结束
		{
			return *this;
		}
	}
	throw("Operator-= ERROR!");//抛出异常
}



/// <summary>
/// “*=”运算
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="a"></param>
/// <returns></returns>
template<typename T>
MAT<T>& MAT<T>::operator*=(const MAT& a)
{
	if (c == a.r)//当this的列数等于a的行数时
	{
		MAT R(r, a.c);
		int i = 0, j = 0, k = 0;
		for (i = 0; i < r; i++)
		{
			for (j = 0; j < a.c; j++)
			{
				R[i][j] = 0;
				for (k = 0; k < c; k++)
				{
					R[i][j] = R[i][j] + e[i * c + k] * a.e[k * c + j];
				}
			}
		}
		(*this) = (MAT&&)R;
		return *this;
	}
	throw("Operator*= ERROR!");
}



/// <summary>
/// print输出至s并返回s：列用空格隔开，行用回车结束
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="s"></param>
/// <returns></returns>
template<typename T>
char* MAT<T>::print(char* s) const noexcept
{
	int n = 0;
	if (typeid(e[0]) == typeid(int))
	{
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				if (j != 0)
				{
					n += sprintf(s + n, "%c", ' ');
				}
				n += sprintf(s + n, "%6ld", e[i * c + j]);
				if (j == c - 1)
				{
					n += sprintf(s + n, "%c", '\n');
				}
			}
		}
	}
	else if (typeid(e[0]) == typeid(long))
	{
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				if (j != 0)
				{
					n += sprintf(s + n, "%c", ' ');
				}
				n += sprintf(s + n, "%6lld", e[i * c + j]);
				if (j == c - 1)
				{
					n += sprintf(s + n, "%c", '\n');
				}
			}
		}
	}
	else if (typeid(e[0]) == typeid(float))
	{
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				if (j != 0)
				{
					n += sprintf(s + n, "%c", ' ');
				}
				n += sprintf(s + n, "%8f", e[i * c + j]);
				if (j == c - 1)
				{
					n += sprintf(s + n, "%c", '\n');
				}
			}
		}
	}
	else if (typeid(e[0]) == typeid(double))
	{
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				if (j != 0)
				{
					n += sprintf(s + n, "%c", ' ');
				}
				n += sprintf(s + n, "%8lf", e[i * c + j]);
				if (j == c - 1)
				{
					n += sprintf(s + n, "%c", '\n');
				}
			}
		}
	}
	s[n] = '\0';
	printf("%s", s);
	return s;
}

