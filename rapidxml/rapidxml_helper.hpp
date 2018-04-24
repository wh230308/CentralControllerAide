#ifndef RAPID_XML_HELPER_HPP
#define RAPID_XML_HELPER_HPP

#include <cstdint>
#include <string>
#include "rapidxml.hpp"

namespace rapidxml_helper
{

inline bool read_xml_element(rapidxml::xml_node<char> *element, const char *node_name, int &data)
{
    auto child_element = element->first_node(node_name);
    if (child_element != nullptr) {
        child_element = child_element->first_node();
        if (child_element != nullptr && child_element->type() == rapidxml::node_data && child_element->value_size() > 0) {
            uint32_t value = std::stoul(std::string(child_element->value(), child_element->value_size()));
            data = static_cast<int>(value);
            return true;
        }
    }
    return false;
}

inline bool read_xml_element(rapidxml::xml_node<char> *element, const char *node_name, uint16_t &data)
{
    auto child_element = element->first_node(node_name);
    if (child_element != nullptr) {
        child_element = child_element->first_node();
        if (child_element != nullptr && child_element->type() == rapidxml::node_data && child_element->value_size() > 0) {
            uint32_t value = std::stoul(std::string(child_element->value(), child_element->value_size()));
            data = static_cast<uint16_t>(value);
            return true;
        }
    }
    return false;
}

inline bool read_xml_element(rapidxml::xml_node<char> *element, const char *node_name, uint32_t &data)
{
    auto child_element = element->first_node(node_name);
    if (child_element != nullptr) {
        child_element = child_element->first_node();
        if (child_element != nullptr && child_element->type() == rapidxml::node_data && child_element->value_size() > 0) {
            data = std::stoul(std::string(child_element->value(), child_element->value_size()));
            return true;
        }
    }
    return false;
}

inline bool read_xml_element(rapidxml::xml_node<char> *element, const char *node_name, uint64_t &data)
{
    auto child_element = element->first_node(node_name);
    if (child_element != nullptr) {
        child_element = child_element->first_node();
        if (child_element != nullptr && child_element->type() == rapidxml::node_data && child_element->value_size() > 0) {
            data = std::stoull(std::string(child_element->value(), child_element->value_size()));
            return true;
        }
    }
    return false;
}

inline bool read_xml_element(rapidxml::xml_node<char> *element, const char *node_name, std::string &data)
{
    auto child_element = element->first_node(node_name);
    if (child_element != nullptr) {
        child_element = child_element->first_node();
        if (child_element != nullptr && child_element->type() == rapidxml::node_data && child_element->value_size() > 0) {
            data.assign(child_element->value(), child_element->value_size());
            return true;
        }
    }
    return false;
}


inline char* string_to_xml_str_r(rapidxml::xml_document<>& doc, const std::string& str)
{
    return doc.allocate_string(str.c_str());
}

inline char* string_to_xml_str(rapidxml::xml_document<>& doc, std::string str)
{
    return string_to_xml_str_r(doc, str);
}

inline char* uint32_to_xml_str(rapidxml::xml_document<>& doc, uint32_t val)
{
    char tmp[16] = { 0 };
#ifdef WIN32
    sprintf_s(tmp, sizeof(tmp) / sizeof(tmp[0]), "%u", val);
#else
    snprintf(tmp, sizeof(tmp) / sizeof(tmp[0]), "%u", val);
#endif
    return doc.allocate_string(tmp);
}

inline char* uint64_to_xml_str(rapidxml::xml_document<>& doc, uint64_t val)
{
    char tmp[32] = { 0 };
#ifdef WIN32
    sprintf_s(tmp, sizeof(tmp) / sizeof(tmp[0]), "%llu", val);
#else
    snprintf(tmp, sizeof(tmp) / sizeof(tmp[0]), "%llu", val);
#endif
    return doc.allocate_string(tmp);
}

}  // namespace rapidxml_helper

#endif  // RAPID_XML_HELPER_HPP