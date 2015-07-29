#ifndef MXML_USE_STL

#ifndef MXML_MSTRING_INCLUDED
#define MXML_MSTRING_INCLUDED

#include<string.h>
#include<assert.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1200) || defined(__GNUC__) && (__GNUC__ >= 3)
#define m_explicit explicit
#else
#define m_explicit
#endif

/**
*		此类是参照STL中std::string的一个子集实现，
*		针对的是不能使用STL或不能使用STL中string的场景，
*		只包含了在XML解析过程中所需的字符串功能。
*
*		Written By Miaojiuchen 2015/07/29
*/
class MString
{
public:
	//	size_type
	typedef size_t size_type;

	//	error tag for no-found
	static const size_type nopos;

	//	default constructor
	MString() :_rep(&_nullrep){}

	//	copy constructor
	MString(const MString &copy)
	{
		init(copy.length());
		memcpy(firstc(), copy.c_str(), length());
	}

	//	construct from a cstring
	m_explicit MString(const char *copy) :_rep(0)
	{
		init(static_cast<size_type>(strlen(copy)));
		memcpy(firstc(), copy, length());
	}

	//	construct from a cstring
	MString(const char *copy, size_type sz) :_rep(0)
	{
		init(sz);
		memcpy(firstc(), copy, sz);
	}

	//	destructor
	~MString(){ distory(); }

	//	assign from another MString
	MString &operator=(const MString &copy)
	{
		return assign(copy.firstc(), copy.length());
	}

	//	assign from a cstring
	MString &operator=(const char *copy)
	{
		return assign(copy, static_cast<size_type>(strlen(copy)));
	}

	//	append from another MString
	MString &operator+=(const MString &suffix)
	{
		return append(suffix.firstc(), suffix.length());
	}

	//	append from a cstring
	MString &operator+=(const char *suffix)
	{
		return append(suffix, static_cast<size_type>(strlen(suffix)));
	}

	//	append a character
	MString &operator+=(const char c)
	{
		return append(&c, 1);
	}

	//	assign member method
	MString &assign(const char *, size_type);

	void reserve(size_type newcap);

	//	append member method
	MString &append(const char *, size_type);

	//	c_str getter
	const char *c_str() const { return _rep->str; }

	//	length getter
	size_type length() const { return _rep->size; }

	//	isEmpty getter
	bool isEmpty() const { return _rep->size == 0; }

	//	capacity getter
	size_type capacity() const { return _rep->capacity; }

	//	character accessor
	const char &at(size_type index) const
	{
		assert(index < length());
		return _rep->str[index];
	}

	//	character access operator
	const char &operator[](size_type index) const
	{
		assert(index < length());
		return _rep->str[index];
	}

	//	character search from first position
	size_type find(const char tarc) const
	{
		return find(tarc, 0);
	}

	//	character search from an offset
	size_type find(const char tarc, size_type offset) const
	{
		if (offset >= length())	return nopos;
		for (const char *pc = firstc() + offset; *pc != '\0'; ++pc)
		{
			if (*pc == tarc)
				return static_cast<size_type>(pc - firstc());
		}
		return nopos;
	}

	//	scale to empty string
	void clear()
	{
		distory();
		init(0);
	}

	//	swap two MStrings
	void swap(MString &another)
	{
		Rep *tmp = another._rep;
		another._rep = _rep;
		_rep = tmp;
	}

private:
	//	initialize memory space
	void init(size_type sz){ init(sz, sz); }

	//	initialize memory space
	void init(size_type sz, size_type cap)
	{
		if (cap > 0)
		{
			const size_type
				bytewidth = sizeof(Rep) + cap,
				intwidth = (bytewidth + sizeof(int) - 1) / sizeof(int);
			{
				_rep = reinterpret_cast<Rep *>(new int[intwidth]);
				_rep->size = sz;
				_rep->str[sz] = '\0';
				_rep->capacity = cap;
			}
		}
		else
		{
			_rep = &_nullrep;
		}
	}

	//	set string length
	void set_size(size_type sz){ _rep->str[_rep->size = sz] = '\0'; }

	//	first character getter
	char *firstc() const { return _rep->str; }

	//	last character setter
	char *lastc() const { return _rep->str + _rep->size; }

	//	distory memory space
	void distory()
	{
		if (_rep != &_nullrep)
		{
			delete[] reinterpret_cast<int *>(_rep);
		}
	}

private:
	struct Rep
	{
		size_type			size;
		size_type			capacity;
		char				str[1];
	};

	//	data area
	Rep*					_rep;

	//	empty space
	static Rep				_nullrep;
};

//	equality operators
inline bool operator==(const MString &lhs, const MString &rhs)
{
	return
		(lhs.length() == rhs.length())
		&&
		(strcmp(lhs.c_str(), rhs.c_str()) == 0);
}

inline bool operator!=(const MString &lhs, const MString &rhs){ return !(lhs == rhs); }

inline bool operator==(const MString &rhs, const char *lhs){ return strcmp(rhs.c_str(), lhs) == 0; }

inline bool operator!=(const MString &rhs, const char *lhs){ return !(rhs == lhs); }

inline bool operator==(const char *lhs, const MString &rhs){ return rhs == lhs; }

inline bool operator!=(const char *lhs, const MString &rhs){ return !(lhs == rhs); }

//	comparability operators
inline bool operator>(const MString &lhs, const MString &rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) > 0;
}

inline bool operator<(const MString &lhs, const MString &rhs){ return rhs > lhs; }

inline bool operator>=(const MString &lhs, const MString &rhs){ return !(lhs < rhs); }

inline bool operator<=(const MString &lhs, const MString &rhs){ return !(lhs > rhs); }

//	add operators
MString operator+(const MString &_1, const MString &_2);

MString operator+(const MString &_1, const char *_2);

MString operator+(const char *_1, const MString &_2);

/**
*		此类是参照STL中std::ostringstream的一个子集实现
*
*		Written By Miaojiuchen 2015/07/29
*/
class MStringStream :public MString
{
public:
	//	MStringStream << operator
	MStringStream &operator<<(const MString &str)
	{
		*this += str;
		return *this;
	}

	//	MStringStream << operator
	MStringStream &operator<<(const char *str)
	{
		*this += str;
		return *this;
	}
};

#endif	//	MXML_MSTRING_INCLUDED
#endif	//	MXML_USE_STL