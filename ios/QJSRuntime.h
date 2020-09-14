//
//  QJSRuntime.h
//  react-native-quickjs
//
//  Created by 徐晖 on 2020/9/14.
//

#ifndef QJSRuntime_h
#define QJSRuntime_h

#include <jsi/jsi.h>
#include <memory.h>
using namespace facebook;

namespace qjs {

std::unique_ptr<jsi::Runtime> makeJSCRuntime();

}

#endif /* QJSRuntime_h */
