// ----------------------------------------------------------------------------
// Copyright (C) 2015-2021 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_PROPERTIES_PARSER_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_PROPERTIES_PARSER_HPP_INCLUDED

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/detail/properties_parser_error.hpp>
//#include <boost/property_tree/detail/properties_parser_writer_settings.hpp>
#include <boost/property_tree/detail/properties_parser_read.hpp>
//#include <boost/property_tree/detail/properties_parser_write.hpp>
#include <istream>

namespace boost { namespace property_tree { namespace properties_parser
{

    /**
     * Read properties from a the given stream and translate it to a property tree.
     * @note Replaces the existing contents. Strong exception guarantee.
     * @throw properties_parser_error If the stream cannot be read, doesn't contain
     *                          valid properties, or a conversion fails.
     */
    template<class Ptree, class Ch>
    void read_properties(std::basic_istream<Ch> &stream, Ptree &pt)
    {
        Ptree local;
        read_properties_internal(stream, local, std::string(), 0);
        pt.swap(local);
    }

    /**
     * Read properties from a the given stream and translate it to a property tree.
     * @note Replaces the existing contents. Strong exception guarantee.
     * @param default_ptree If parsing fails, pt is set to a copy of this tree.
     */
    template<class Ptree, class Ch>
    void read_properties(std::basic_istream<Ch> &stream, Ptree &pt,
                   const Ptree &default_ptree)
    {
        try {
            read_properties(stream, pt);
        } catch(file_parser_error &) {
            pt = default_ptree;
        }
    }

    /**
     * Read properties from a the given file and translate it to a property tree. The
     * tree's key type must be a string type, i.e. it must have a nested
     * value_type typedef that is a valid parameter for basic_ifstream.
     * @note Replaces the existing contents. Strong exception guarantee.
     * @throw properties_parser_error If the file cannot be read, doesn't contain
     *                          valid properties, or a conversion fails.
     */
    template<class Ptree>
    void read_properties(const std::string &filename, Ptree &pt,
                   const std::locale &loc = std::locale())
    {
        std::basic_ifstream<typename Ptree::key_type::value_type>
            stream(filename.c_str());
        if (!stream) {
            BOOST_PROPERTY_TREE_THROW(properties_parser_error(
                "cannot open file for reading", filename, 0));
        }
        stream.imbue(loc);
        Ptree local;
        read_properties_internal(stream, local, filename, 0);
        pt.swap(local);
    }

    /**
     * Read properties from a the given file and translate it to a property tree. The
     * tree's key type must be a string type, i.e. it must have a nested
     * value_type typedef that is a valid parameter for basic_ifstream.
     * @note Replaces the existing contents. Strong exception guarantee.
     * @param default_ptree If parsing fails, pt is set to a copy of this tree.
     */
    template<class Ptree>
    void read_properties(const std::string &filename,
                   Ptree &pt,
                   const Ptree &default_ptree,
                   const std::locale &loc = std::locale())
    {
        try {
            read_properties(filename, pt, loc);
        } catch(file_parser_error &) {
            pt = default_ptree;
        }
    }

    /**
     * Writes a tree to the stream in properties format.
     * @throw properties_parser_error If the stream cannot be written to, or a
     *                          conversion fails.
     * @param settings The settings to use when writing the properties data.
     */
//    template<class Ptree, class Ch>
//    void write_properties(std::basic_ostream<Ch> &stream,
//                    const Ptree &pt,
//                    const info_writer_settings<Ch> &settings =
//                        info_writer_settings<Ch>())
//    {
//        write_properties_internal(stream, pt, std::string(), settings);
//    }

    /**
     * Writes a tree to the file in properties format. The tree's key type must be a
     * string type, i.e. it must have a nested value_type typedef that is a
     * valid parameter for basic_ofstream.
     * @throw properties_parser_error If the file cannot be written to, or a
     *                          conversion fails.
     * @param settings The settings to use when writing the properties data.
     */
//    template<class Ptree>
//    void write_properties(const std::string &filename,
//                    const Ptree &pt,
//                    const std::locale &loc = std::locale(),
//                    const info_writer_settings<
//                        typename Ptree::key_type::value_type
//                    > &settings =
//                        info_writer_make_settings<
//                            typename Ptree::key_type::value_type>())
//    {
//        std::basic_ofstream<typename Ptree::key_type::value_type>
//            stream(filename.c_str());
//        if (!stream) {
//            BOOST_PROPERTY_TREE_THROW(properties_parser_error(
//                "cannot open file for writing", filename, 0));
//        }
//        stream.imbue(loc);
//        write_properties_internal(stream, pt, filename, settings);
//    }

} } }

namespace boost { namespace property_tree
{
    using properties_parser::properties_parser_error;
    using properties_parser::read_properties;
    //using properties_parser::write_properties;
    //using properties_parser::info_writer_settings;
    //using properties_parser::info_writer_make_settings;
} }

#endif
