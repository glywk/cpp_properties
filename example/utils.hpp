//=============================================================================
// Copyright (c) 2015 glywk
// https://github.com/glywk
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#ifndef CPP_PROPERTIES_EXAMPLE_UTILS_HPP
#define CPP_PROPERTIES_EXAMPLE_UTILS_HPP

#include <boost/config/warning_disable.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/value_type.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <codecvt>
#include <functional>

/*!
 * helper function reading a file into a string
 */
inline std::string
read_from_file(char const* infile)
{
    std::ifstream instream(infile);
    if (!instream.is_open()) {
        std::cerr << "Couldn't open file: " << infile << std::endl;
        exit(-1);
    }
    instream.unsetf(std::ios::skipws);      // No white space skipping!
    return std::string(std::istreambuf_iterator<char>(instream.rdbuf()),
        std::istreambuf_iterator<char>());
}

/*!
 * convert escape char to its given value or remove '\' when escape sequence is
 * not specific.
 */
template <
	typename ForwardTraversalIterator,
	typename CharT = boost::iterator_value<ForwardTraversalIterator>::type
>
const CharT escape_sequence(const boost::iterator_range<ForwardTraversalIterator> & sequence) {
    const CharT c = sequence.back();
    switch (c) {
    case 't':  return '\t'; break;
    case 'r':  return '\r'; break;
    case 'n':  return '\n'; break;
    case 'f':  return '\f'; break;
    default:
        return c;
    }
}

/*!
 * convert escape unicode codepoint to its utf-8 character representation.
 */
template <
	typename ForwardTraversalIterator,
	typename CharT = boost::iterator_value<ForwardTraversalIterator>::type
>
std::basic_string<CharT> decode_to_utf8(const boost::iterator_range<ForwardTraversalIterator> & code_point)
{
    auto c = code_point.begin();
    // skip unicode escape sequence (\u)
    std::advance(c, 2);
    int value = 0;
    for (int i = 0; i < 4; i++) {
        char chr = *c++;
        switch (chr) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            value = (value << 4) + chr - '0';
            break;
        case 'a': case 'b': case 'c':
        case 'd': case 'e': case 'f':
            value = (value << 4) + 10 + chr - 'a';
            break;
        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
            value = (value << 4) + 10 + chr - 'A';
            break;
        }
    }

    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> utf8_converter;

    return utf8_converter.to_bytes(std::char_traits<wchar_t>::to_char_type(value));
}

/*!
 * convert iso 8859-1 string to its utf-8 character representation.
 */
template <typename CharT>
void encode_latin1_to_utf8(const CharT code_point, std::basic_string<CharT> & utf8)
{
    /* code points above 0xff are impossible since iso 8859-1 is 8-bit encoded */
    uint8_t ch = code_point;

    if(ch < 0x80) {
      utf8 += ch;
    } else {
      utf8 += 0xc0 | (ch & 0xc0) >> 6; // first byte
      utf8 += 0x80 | (ch & 0x3f);      // last byte
    }
}

/*!
 * convert iso 8859-1 string to its utf-8 character representation.
 */
template <typename CharT>
std::basic_string<CharT> encode_latin1_to_utf8(const CharT code_point)
{
    std::basic_string<CharT> utf8;
    encode_latin1_to_utf8(code_point, utf8);
    return utf8;
}

/*!
 * convert escape char to its given value or remove '\' when escape sequence is
 * not specific.
 */
template <
	typename ForwardTraversalIterator,
	typename CharT = boost::iterator_value<ForwardTraversalIterator>::type
>
std::basic_string<CharT> escape_sequence_to_utf8(const boost::iterator_range<ForwardTraversalIterator> & sequence) {
    return encode_latin1_to_utf8(escape_sequence(sequence));
}

/*!
 * convert iso 8859-1 string to its utf-8 character representation.
 */
template <
	typename ForwardTraversalIterator,
	typename CharT = boost::iterator_value<ForwardTraversalIterator>::type
>
std::basic_string<CharT> latin1_to_utf8(const boost::iterator_range<ForwardTraversalIterator> & latin1_string)
{
    std::basic_string<CharT> utf8;

    for (auto code_point : latin1_string) {
        encode_latin1_to_utf8(code_point, utf8);
    }

    return utf8;
}

#endif