//
//  QJSRuntime.cpp
//  react-native-quickjs
//
//  Created by 徐晖 on 2020/9/14.
//

#include "QJSRuntime.h"
#include "quickjs.h"
using namespace jsi;

namespace qjs {

class QJSRuntime : public jsi::Runtime {
public:
    QJSRuntime();
    QJSRuntime(JSContext *ctx);
    
    ~QJSRuntime();
    
    facebook::jsi::Value evaluateJavaScript(const std::shared_ptr<const Buffer> &buffer, const std::string &sourceURL) override;
    
    std::shared_ptr<const PreparedJavaScript> prepareJavaScript(const std::shared_ptr<const Buffer> &buffer, std::string sourceURL) override;
    
    facebook::jsi::Value evaluatePreparedJavaScript(const std::shared_ptr<const PreparedJavaScript> &js) override;
    
    facebook::jsi::Object global() override;
    
    std::string description() override;
    
    bool isInspectable() override;
    
    facebook::jsi::Runtime::PointerValue *cloneSymbol(const Runtime::PointerValue *pv) override;
    
    facebook::jsi::Runtime::PointerValue *cloneString(const Runtime::PointerValue *pv) override;
    
    facebook::jsi::Runtime::PointerValue *cloneObject(const Runtime::PointerValue *pv) override;
    
    facebook::jsi::Runtime::PointerValue *clonePropNameID(const Runtime::PointerValue *pv) override;
    
    facebook::jsi::PropNameID createPropNameIDFromAscii(const char *str, size_t length) override;
    
    facebook::jsi::PropNameID createPropNameIDFromUtf8(const uint8_t *utf8, size_t length) override;
    
    facebook::jsi::PropNameID createPropNameIDFromString(const facebook::jsi::String &str) override;
    
    std::string utf8(const facebook::jsi::PropNameID &) override;
    
    bool compare(const facebook::jsi::PropNameID &, const facebook::jsi::PropNameID &) override;
    
    std::string symbolToString(const facebook::jsi::Symbol &) override;
    
    facebook::jsi::String createStringFromAscii(const char *str, size_t length) override;
    
    facebook::jsi::String createStringFromUtf8(const uint8_t *utf8, size_t length) override;
    
    std::string utf8(const facebook::jsi::String &) override;
    
    facebook::jsi::Object createObject() override;
    
    facebook::jsi::Object createObject(std::shared_ptr<HostObject> ho) override;
    
    std::shared_ptr<HostObject> getHostObject(const jsi::Object &) override;
    
    facebook::jsi::HostFunctionType &getHostFunction(const jsi::Function &) override;
    
    facebook::jsi::Value getProperty(const facebook::jsi::Object &, const facebook::jsi::PropNameID &name) override;
    
    facebook::jsi::Value getProperty(const facebook::jsi::Object &, const facebook::jsi::String &name) override;
    
    bool hasProperty(const facebook::jsi::Object &, const facebook::jsi::PropNameID &name) override;
    
    bool hasProperty(const facebook::jsi::Object &, const facebook::jsi::String &name) override;
    
    void setPropertyValue(facebook::jsi::Object &, const facebook::jsi::PropNameID &name, const facebook::jsi::Value &value) override;
    
    void setPropertyValue(facebook::jsi::Object &, const facebook::jsi::String &name, const facebook::jsi::Value &value) override;
    
    bool isArray(const facebook::jsi::Object &) const override;
    
    bool isArrayBuffer(const facebook::jsi::Object &) const override;
    
    bool isFunction(const facebook::jsi::Object &) const override;
    
    bool isHostObject(const jsi::Object &) const override;
    
    bool isHostFunction(const jsi::Function &) const override;
    
    facebook::jsi::Array getPropertyNames(const facebook::jsi::Object &) override;
    
    facebook::jsi::WeakObject createWeakObject(const facebook::jsi::Object &) override;
    
    facebook::jsi::Value lockWeakObject(const facebook::jsi::WeakObject &) override;
    
    facebook::jsi::Array createArray(size_t length) override;
    
    size_t size(const facebook::jsi::Array &) override;
    
    size_t size(const facebook::jsi::ArrayBuffer &) override;
    
    uint8_t *data(const facebook::jsi::ArrayBuffer &) override;
    
    facebook::jsi::Value getValueAtIndex(const facebook::jsi::Array &, size_t i) override;
    
    void setValueAtIndexImpl(facebook::jsi::Array &, size_t i, const facebook::jsi::Value &value) override;
    
    facebook::jsi::Function createFunctionFromHostFunction(const facebook::jsi::PropNameID &name, unsigned int paramCount, facebook::jsi::HostFunctionType func) override;
    
    facebook::jsi::Value call(const facebook::jsi::Function &, const facebook::jsi::Value &jsThis, const facebook::jsi::Value *args, size_t count) override;
    
    facebook::jsi::Value callAsConstructor(const facebook::jsi::Function &, const facebook::jsi::Value *args, size_t count) override;
    
    bool strictEquals(const facebook::jsi::Symbol &a, const facebook::jsi::Symbol &b) const override;
    
    bool strictEquals(const facebook::jsi::String &a, const facebook::jsi::String &b) const override;
    
    bool strictEquals(const facebook::jsi::Object &a, const facebook::jsi::Object &b) const override;
    
    bool instanceOf(const facebook::jsi::Object &o, const facebook::jsi::Function &f) override;
    
    jsi::Value createValue(JSValueRef value) const;
    
private:
    void checkException(JSValueRef exc);
    void checkException(JSValueRef res, JSValueRef exc);
    void checkException(JSValueRef exc, const char* msg);
    void checkException(JSValueRef res, JSValueRef exc, const char* msg);
    
    JSContext *ctx_;
    std::atomic<bool> ctxInvalid_;
    std::string desc_;
#ifndef NDEBUG
    mutable std::atomic<intptr_t> objectCounter_;
    mutable std::atomic<intptr_t> symbolCounter_;
    mutable std::atomic<intptr_t> stringCounter_;
#endif
    
};

QJSRuntime::QJSRuntime(): QJSRuntime(JS_NewContext(JS_NewRuntime())) {
    JS_FreeContext(ctx_);
}

QJSRuntime::QJSRuntime(JSContext *ctx)
    : ctx_(JS_DupContext(ctx)),
      ctxInvalid_(false)
#ifndef NDEBUG
      ,
      objectCounter_(0),
      stringCounter_(0)
#endif
{
}

QJSRuntime::~QJSRuntime() {
    ctxInvalid_ = true;
    JS_FreeContext(ctx_);
#ifndef NDEBUG
    assert(
      objectCounter_ == 0 && "QJSRuntime destroyed with a dangling API object");
    assert(
      stringCounter_ == 0 && "QJSRuntime destroyed with a dangling API string");
#endif
}

jsi::Value QJSRuntime::createValue(JSValue value) const {
  if (JS_IsNumber(value)) {
      double number;
      JS_ToFloat64(ctx_, &number, value);
      return jsi::Value(number);
  } else if (JS_IsBool(value)) {
      bool res = JS_ToBool(ctx_, value);
      return jsi::Value(res);
  } else if (JS_IsNull(value)) {
      return jsi::Value(nullptr);
  } else if (JS_IsUndefined(value)) {
    return jsi::Value();
  } else if (JS_IsString(value)) {
      JS_ToString(<#JSContext *ctx#>, <#JSValue val#>)
    JSStringRef str = JSValueToStringCopy(ctx_, value, nullptr);
    auto result = jsi::Value(createString(str));
    JSStringRelease(str);
    return result;
  } else if (JSValueIsObject(ctx_, value)) {
    JSObjectRef objRef = JSValueToObject(ctx_, value, nullptr);
    return jsi::Value(createObject(objRef));
  } else if (smellsLikeES6Symbol(ctx_, value)) {
    return jsi::Value(createSymbol(value));
  } else {
    // WHAT ARE YOU
    abort();
  }
}

jsi::Value QJSRuntime::evaluateJavaScript(
    const std::shared_ptr<const jsi::Buffer> &buffer,
    const std::string& sourceURL) {
    std::string tmp(
                    reinterpret_cast<const char*>(buffer->data()), buffer->size());
    JSValue val;
    int ret;

    val = JS_Eval(ctx_, tmp.c_str(), tmp.size(), sourceURL.c_str(), 0);
    if (JS_IsException(val)) {
        js_std_dump_error(ctx_);
        ret = -1;
    } else {
        ret = 0;
    }
    JS_FreeValue(ctx_, val);
    return createValue()
    
//  JSStringRef sourceRef = JSStringCreateWithUTF8CString(tmp.c_str());
//  JSStringRef sourceURLRef = nullptr;
//  if (!sourceURL.empty()) {
//    sourceURLRef = JSStringCreateWithUTF8CString(sourceURL.c_str());
//  }
//  JSValueRef exc = nullptr;
//  JSValueRef res =
//      JSEvaluateScript(ctx_, sourceRef, nullptr, sourceURLRef, 0, &exc);
//  JSStringRelease(sourceRef);
//  if (sourceURLRef) {
//    JSStringRelease(sourceURLRef);
//  }
//  checkException(res, exc);
//  return createValue(res);
}

std::unique_ptr<jsi::Runtime> makeJSCRuntime() {
  return std::make_unique<QJSRuntime>();
}

#if __has_builtin(__builtin_expect) || defined(__GNUC__)
#define JSC_LIKELY(EXPR) __builtin_expect((bool)(EXPR), true)
#define JSC_UNLIKELY(EXPR) __builtin_expect((bool)(EXPR), false)
#else
#define JSC_LIKELY(EXPR) (EXPR)
#define JSC_UNLIKELY(EXPR) (EXPR)
#endif

}
