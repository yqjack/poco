//
// DigestStream.cpp
//
// $Id: //poco/Main/Foundation/src/DigestStream.cpp#13 $
//
// Library: Foundation
// Package: Crypt
// Module:  DigestStream
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/DigestStream.h"


namespace Poco {


const int DigestBuf::BUFFER_SIZE = 256;


DigestBuf::DigestBuf(DigestEngine& eng): 
	BufferedStreamBuf(BUFFER_SIZE, std::ios::out), 
	_eng(eng), 
	_pIstr(0),
	_pOstr(0) 
{
}


DigestBuf::DigestBuf(DigestEngine& eng, std::istream& istr): 
	BufferedStreamBuf(BUFFER_SIZE, std::ios::in), 
	_eng(eng), 
	_pIstr(&istr), 
	_pOstr(0)
{
}


DigestBuf::DigestBuf(DigestEngine& eng, std::ostream& ostr): 
	BufferedStreamBuf(BUFFER_SIZE, std::ios::in), 
	_eng(eng), 
	_pIstr(0), 
	_pOstr(&ostr)
{
}


DigestBuf::~DigestBuf()
{
}


int DigestBuf::readFromDevice(char* buffer, std::streamsize length)
{
	if (_pIstr && _pIstr->good())
	{
		_pIstr->read(buffer, length);
		std::streamsize n = _pIstr->gcount();
		if (n > 0) _eng.update(buffer, static_cast<unsigned>(n));
		return static_cast<int>(n);
	}
	return -1;
}


int DigestBuf::writeToDevice(const char* buffer, std::streamsize length)
{
	_eng.update(buffer, (unsigned) length);
	if (_pOstr) _pOstr->write(buffer, length);
	return static_cast<int>(length);
}


void DigestBuf::close()
{
	sync();
	if (_pOstr) _pOstr->flush();
}


DigestIOS::DigestIOS(DigestEngine& eng): _buf(eng)
{
	poco_ios_init(&_buf);
}


DigestIOS::DigestIOS(DigestEngine& eng, std::istream& istr): _buf(eng, istr)
{
	poco_ios_init(&_buf);
}


DigestIOS::DigestIOS(DigestEngine& eng, std::ostream& ostr): _buf(eng, ostr)
{
	poco_ios_init(&_buf);
}


DigestIOS::~DigestIOS()
{
}


DigestBuf* DigestIOS::rdbuf()
{
	return &_buf;
}


DigestInputStream::DigestInputStream(DigestEngine& eng, std::istream& istr): 
	DigestIOS(eng, istr), 
	std::istream(&_buf)
{
}


DigestInputStream::~DigestInputStream()
{
}


DigestOutputStream::DigestOutputStream(DigestEngine& eng): 
	DigestIOS(eng), 
	std::ostream(&_buf)
{
}


DigestOutputStream::DigestOutputStream(DigestEngine& eng, std::ostream& ostr): 
	DigestIOS(eng, ostr), 
	std::ostream(&_buf)
{
}


DigestOutputStream::~DigestOutputStream()
{
}


void DigestOutputStream::close()
{
	_buf.close();
}


} // namespace Poco
