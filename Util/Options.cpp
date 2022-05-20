/**
 Copyright (c) 2015-present, Facebook, Inc.
 All rights reserved.

 This source code is licensed under the BSD-style license found in the
 LICENSE file in the root directory of this source tree.
 */

#include "Options.h"

#include <cstdlib>

using libutil::Options;

namespace libutil {

template<>
std::pair<bool, std::string> Options::
Next<std::string>(std::optional<std::string> *result, std::vector<std::string> const &args, std::vector<std::string>::const_iterator *it, bool allowDuplicate)
{
    std::string const &arg = **it;
    if (++*it != args.end()) {
        std::string const &value = **it;

        if (!*result || allowDuplicate) {
            *result = value;
            return std::make_pair(true, std::string());
        } else {
            return std::make_pair(false, "duplicate argument " + arg);
        }
    } else {
        return std::make_pair(false, "missing argument value for argument " + arg);
    }
}

template<>
std::pair<bool, std::string> Options::
Next<int>(std::optional<int> *result, std::vector<std::string> const &args, std::vector<std::string>::const_iterator *it, bool allowDuplicate)
{
    std::string const &arg = **it;
    if (++*it != args.end()) {
        std::string const &value = **it;

        if (!*result || allowDuplicate) {
            *result = std::atoi(value.c_str());
            return std::make_pair(true, std::string());
        } else {
            return std::make_pair(false, "duplicate argument " + arg);
        }
    } else {
        return std::make_pair(false, "missing argument value for argument " + arg);
    }
}

template<>
std::pair<bool, std::string> Options::
Next<bool>(std::optional<bool> *result, std::vector<std::string> const &args, std::vector<std::string>::const_iterator *it, bool allowDuplicate)
{
    std::string const &arg = **it;
    if (++*it != args.end()) {
        std::string const &value = **it;

        if (!*result || allowDuplicate) {
            if (value == "YES") {
                *result = true;
                return std::make_pair(true, std::string());
            } else if (value == "NO") {
                *result = false;
                return std::make_pair(true, std::string());
            } else {
                return std::make_pair(false, "invalid value " + arg + " for boolean argument");
            }
        } else {
            return std::make_pair(false, "duplicate argument " + arg);
        }
    } else {
        return std::make_pair(false, "missing argument value for argument " + arg);
    }
}

template<>
std::pair<bool, std::string> Options::
AppendNext<std::string>(std::vector<std::string> *result, std::vector<std::string> const &args, std::vector<std::string>::const_iterator *it)
{
    std::optional<std::string> value;
    auto ret = Next<std::string>(&value, args, it);
    if (ret.first) {
        result->push_back(*value);
    }
    return ret;
}

template<>
std::pair<bool, std::string> Options::
Current<std::string>(std::optional<std::string> *result, std::string const &arg, bool allowDuplicate)
{
    if (!*result || allowDuplicate) {
        *result = arg;
        return std::make_pair(true, std::string());
    } else {
        return std::make_pair(false, "duplicate argument " + arg);
    }
}

template<>
std::pair<bool, std::string> Options::
Current<bool>(std::optional<bool> *result, std::string const &arg, bool allowDuplicate)
{
    if (!*result || allowDuplicate) {
        *result = true;
        return std::make_pair(true, std::string());
    } else {
        return std::make_pair(false, "duplicate argument " + arg);
    }
}

template<>
std::pair<bool, std::string> Options::
AppendCurrent<std::string>(std::vector<std::string> *result, std::string const &arg)
{
    result->push_back(arg);
    return std::make_pair(true, std::string());
}

}
