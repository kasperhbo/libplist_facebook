/**
 Copyright (c) 2015-present, Facebook, Inc.
 All rights reserved.

 This source code is licensed under the BSD-style license found in the
 LICENSE file in the root directory of this source tree.
 */

#include <plist/Object.h>

using plist::Object;

std::unique_ptr<Object> Object::
Coerce(Object const *obj)
{
    return obj->copy();
}
