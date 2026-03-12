#ifndef SYMPCONV_AUTO_MEM_H
#define SYMPCONV_AUTO_MEM_H

namespace SympConv {

template<typename TYPE, size_t SIZE>
class TAutoMem
{
private:
	typedef TAutoMem<TYPE, SIZE> self_type;

public:
	explicit TAutoMem(size_t len)
	{
		if (len > SIZE)
		{
			mMem = (TYPE*)malloc(sizeof(TYPE) * len);
			mSize = len;
		}
		else
		{
			mMem = mStack;
			mSize = SIZE;
		}
	}

	~TAutoMem()
	{
		if (mMem != mStack)
		{
			free(mMem);
		}
	}

	TYPE* GetBuffer()
	{
		return mMem;
	}

	size_t GetLength()
	{
		return mSize;
	}

private:
	TAutoMem();
	TAutoMem(const self_type&);
	self_type& operator=(const self_type&);

private:
	TYPE* mMem;
	size_t mSize;
	TYPE mStack[SIZE];
};

}
#endif // SYMPCONV_AUTO_MEM_H
