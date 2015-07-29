/**
*	This file was created by Miaojiuchen
*
*							2015/07/29
*
*/

#ifndef MXML_USE_STL

#include<mstring.h>

const MString::size_type MString::nopos = static_cast<MString::size_type>(-1);

MString::Rep MString::_nullrep = { 0, 0, '\0' };

MString &MString::assign(const char *s, size_type len)
{
	/**
	*	���ȿ���capacity�Ƿ�̫���̫С
	*	������������������µ��ַ����������·���ռ�
	*	����ʺϣ���ֱ��ת���ڴ浽��MString�ڲ�����_rep->size
	*/
	size_type cap = capacity();
	if (cap < len || cap > 3 * (len + 8))
	{
		MString newstr;
		newstr.init(len);
		memcpy(newstr.firstc(), s, len);
		swap(newstr);
	}
	else
	{
		memmove(firstc(), s, len);
		set_size(len);
	}

	return *this;
}

void MString::reserve(size_type newcap)
{
	if (newcap > capacity())
	{
		MString newstr;
		newstr.init(length(),newcap);//	attention: reserve won't change this string's length
		memcpy(newstr.firstc(), firstc(), length());
		swap(newstr);
	}
}

MString &MString::append(const char *s, size_type len)
{
	/**
	*	���ȿ���ʣ�������Ƿ����ɵ����´�
	*	��������������·���ռ�
	*	���������ֱ�������ڴ沢����_rep->size
	*/
	size_type newlen = length() + len;
	if (newlen > capacity())
	{
		reserve(newlen);
	}
	memmove(lastc(), s, len);
	set_size(newlen);
	return *this;
}

MString operator+(const MString &_1, const MString &_2)
{
	MString ret;
	ret.reserve(_1.length() + _2.length());
	ret += _1;
	ret += _2;
	return ret;
}

MString operator+(const MString &_1, const char *_2)
{
	MString ret;
	MString::size_type len2 = static_cast<MString::size_type>(strlen(_2));
	ret.reserve(_1.length() + len2);
	ret += _1;
	ret.append(_2, len2);
	return ret;
}

MString operator+(const char *_1, const MString &_2)
{
	/**
	*	���ﲻ������������д����ȡ��
	*	return _2 + _1;
	*	��Ϊ�ַ�������൱��ƴ�ӣ�
	*	���ʹ�����ַ������ᵼ��s1 + s2 == s2 + s1
	*	��ʵ�������ǲ�����ȣ�����Ӧ�õ���ƴ��
	*/
	MString ret;
	MString::size_type len1 = static_cast<MString::size_type>(strlen(_1));
	ret.reserve(len1 + _2.length());
	ret.append(_1, len1);
	ret += _2;
	return ret;
}

#endif