// ----------------------------------------------------------------------------
// Copyright (C) 2015-2021 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_DETAIL_PROPERTIES_PARSER_ERROR_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_DETAIL_PROPERTIES_PARSER_ERROR_HPP_INCLUDED

#include <boost/property_tree/detail/file_parser_error.hpp>
#include <string>

namespace boost { namespace property_tree { namespace properties_parser
{

    class properties_parser_error: public file_parser_error
    {
    public:
        properties_parser_error(const std::string &message,
                          const std::string &filename,
                          unsigned long line) :
            file_parser_error(message, filename, line)
        {
        }
    };

} } }

#endif
