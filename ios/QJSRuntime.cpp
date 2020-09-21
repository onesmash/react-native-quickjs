//
//  QJSRuntime.cpp
//  react-native-quickjs
//
//  Created by 徐晖 on 2020/9/14.
//

#include "QJSRuntime.h"
#include <jsi/jsilib.h>
#include <quickjs-libc.h>
using namespace facebook;
using namespace jsi;

namespace qjs {

typedef struct JSValue *JSValueRef;

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
    
    jsi::Value createValue(JSValue value) const;
    
protected:
    class QJSSymbolValue final : public PointerValue {
    #ifndef NDEBUG
        QJSSymbolValue(JSContext *ctx_,
                       const std::atomic<bool>& ctxInvalid,
                       JSValueRef sym, std::atomic<intptr_t>& counter);
    #else
        QJSSymbolValue(JSContext *ctx_,
                       const std::atomic<bool>& ctxInvalid,
                       JSValueRef sym);
    #endif
        void invalidate() override;

        JSContext *ctx_;
        const std::atomic<bool>& ctxInvalid_;
        // There is no C type in the JSC API to represent Symbol, so this stored
        // a JSValueRef which contains the Symbol.
        JSValue sym_;
    #ifndef NDEBUG
        std::atomic<intptr_t>& counter_;
    #endif
       protected:
        friend class QJSRuntime;
      };
    
    class QJSStringValue final : public PointerValue {
    #ifndef NDEBUG
        QJSStringValue(JSContext *ctx_, const std::atomic<bool>& ctxInvalid, JSValueRef str, std::atomic<intptr_t>& counter);
    #else
        QJSStringValue(JSContext *ctx_, const std::atomic<bool>& ctxInvalid, JSValueRef str);
    #endif
        void invalidate() override;

        const std::atomic<bool>& ctxInvalid_;
        JSContext *ctx_;
        JSValue str_;
    #ifndef NDEBUG
        std::atomic<intptr_t>& counter_;
    #endif
       protected:
        friend class QJSRuntime;
      };
    
    class QJSObjectValue final : public PointerValue {
        QJSObjectValue(
            JSContext *ctx_,
            const std::atomic<bool>& ctxInvalid,
            JSValueRef obj
    #ifndef NDEBUG
            ,
            std::atomic<intptr_t>& counter
    #endif
                       );

        void invalidate() override;

        const std::atomic<bool>& ctxInvalid_;
        JSContext *ctx_;
        JSValue obj_;
    #ifndef NDEBUG
        std::atomic<intptr_t>& counter_;
    #endif
       protected:
        friend class QJSRuntime;
      };
    
private:
    void checkException(JSValueRef exc);
    void checkException(JSValueRef res, JSValueRef exc);
    void checkException(JSValueRef exc, const char* msg);
    void checkException(JSValueRef res, JSValueRef exc, const char* msg);
    jsi::Symbol createSymbol(JSValueRef symbol) const;
    jsi::String createString(JSValueRef str) const;
    jsi::Object createObject(JSValueRef object) const;
    
    jsi::Runtime::PointerValue* makeSymbolValue(JSValueRef sym) const;
    jsi::Runtime::PointerValue* makeStringValue(JSValueRef str) const;
    jsi::Runtime::PointerValue* makeObjectValue(JSValueRef obj) const;
    
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

jsi::Runtime::PointerValue* QJSRuntime::makeSymbolValue(
    JSValueRef symbol) const {
#ifndef NDEBUG
  return new QJSSymbolValue(ctx_, ctxInvalid_, symbol, symbolCounter_);
#else
  return new QJSSymbolValue(ctx_, ctxInvalid_, symbol);
#endif
}

jsi::Runtime::PointerValue* QJSRuntime::makeStringValue(
    JSValueRef str) const {
#ifndef NDEBUG
  return new QJSStringValue(ctx_, ctxInvalid_, str, stringCounter_);
#else
  return new QJSStringValue(ctx_, ctxInvalid_, str);
#endif
}

jsi::Runtime::PointerValue* QJSRuntime::makeObjectValue(
    JSValueRef object) const {
#ifndef NDEBUG
  return new QJSObjectValue(ctx_, ctxInvalid_, object, objectCounter_);
#else
  return new QJSObjectValue(ctx_, ctxInvalid_, object);
#endif
}


jsi::Symbol QJSRuntime::createSymbol(JSValueRef sym) const {
  return make<jsi::Symbol>(makeSymbolValue(sym));
}

jsi::String QJSRuntime::createString(JSValueRef str) const {
  return make<jsi::String>(makeStringValue(str));
}

jsi::Object QJSRuntime::createObject(JSValueRef obj) const {
  return make<jsi::Object>(makeObjectValue(obj));
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
      return jsi::Value(createString(&value));
  } else if (JS_IsObject(value)) {
    return jsi::Value(createObject(&value));
  } else if (JS_IsSymbol(value)) {
    return jsi::Value(createSymbol(&value));
  } else {
    // WHAT ARE YOU
    abort();
  }
}

std::shared_ptr<const jsi::PreparedJavaScript> QJSRuntime::prepareJavaScript(
    const std::shared_ptr<const jsi::Buffer> &buffer,
    std::string sourceURL) {
  return std::make_shared<jsi::SourceJavaScriptPreparation>(
      buffer, std::move(sourceURL));
}

jsi::Value QJSRuntime::evaluatePreparedJavaScript(
const std::shared_ptr<const jsi::PreparedJavaScript>& js) {
    assert(
      dynamic_cast<const jsi::SourceJavaScriptPreparation*>(js.get()) &&
      "preparedJavaScript must be a SourceJavaScriptPreparation");
    auto sourceJs =
        std::static_pointer_cast<const jsi::SourceJavaScriptPreparation>(js);
    std::string tmp(
                    reinterpret_cast<const char*>(sourceJs->data()), sourceJs->size());
  return evaluateJavaScript(sourceJs, sourceJs->sourceURL());
}

jsi::Value QJSRuntime::evaluateJavaScript(
    const std::shared_ptr<const jsi::Buffer> &buffer,
    const std::string& sourceURL) {
    std::string tmp(
                    reinterpret_cast<const char*>(buffer->data()), buffer->size());
    JSValue val = JS_Eval(ctx_, tmp.c_str(), tmp.size(), sourceURL.c_str(), 0);
    checkException(&val);
    return createValue(val);
}

QJSRuntime::QJSSymbolValue::QJSSymbolValue(
    JSContext *ctx,
    const std::atomic<bool>& ctxInvalid,
    JSValueRef sym
#ifndef NDEBUG
    ,
    std::atomic<intptr_t>& counter
#endif
    )
    : ctx_(ctx),
      ctxInvalid_(ctxInvalid),
      sym_(JS_DupValue(ctx, *sym))
#ifndef NDEBUG
      ,
      counter_(counter)
#endif
{
#ifndef NDEBUG
  counter_ += 1;
#endif
}

void QJSRuntime::QJSSymbolValue::invalidate() {
#ifndef NDEBUG
  counter_ -= 1;
#endif

  if (!ctxInvalid_) {
      JS_FreeValue(ctx_, sym_);
  }
  delete this;
}

#ifndef NDEBUG
QJSRuntime::QJSStringValue::QJSStringValue(
    JSContext *ctx,
    const std::atomic<bool>& ctxInvalid,
    JSValueRef str,
    std::atomic<intptr_t>& counter)
    : ctx_(ctx), ctxInvalid_(ctxInvalid), str_(JS_DupValue(ctx_, *str)), counter_(counter) {
  counter_ += 1;
}
#else
QJSRuntime::QJSStringValue::QJSStringValue(JSContext *ctx, const std::atomic<bool>& ctxInvalid, JSValueRef str)
    : ctx_(ctx), ctxInvalid_(ctxInvalid), str_(JS_DupValue(ctx_, *str)) {
}
#endif

void QJSRuntime::QJSStringValue::invalidate() {
#ifndef NDEBUG
  counter_ -= 1;
#endif
    if (!ctxInvalid_) {
        JS_FreeValue(ctx_, str_);
    }
    delete this;
}

QJSRuntime::QJSObjectValue::QJSObjectValue(
    JSContext *ctx,
    const std::atomic<bool>& ctxInvalid,
    JSValueRef obj
#ifndef NDEBUG
    ,
    std::atomic<intptr_t>& counter
#endif
    )
    : ctx_(ctx),
      ctxInvalid_(ctxInvalid),
      obj_(JS_DupValue(ctx, *obj))
#ifndef NDEBUG
      ,
      counter_(counter)
#endif
{
#ifndef NDEBUG
    counter_ += 1;
#endif
}

void QJSRuntime::QJSObjectValue::invalidate() {
#ifndef NDEBUG
    counter_ -= 1;
#endif
    if (!ctxInvalid_) {
        JS_FreeValue(ctx_, obj_);
    }
    delete this;
}

void QJSRuntime::checkException(JSValueRef exc) {
  if (JS_IsException(*exc)) {
      js_std_dump_error(ctx_);
      throw jsi::JSError(*this, createValue(*exc));
  }
}

void QJSRuntime::checkException(JSValueRef res, JSValueRef exc) {
  if (JS_IsException(*res)) {
      js_std_dump_error(ctx_);
      throw jsi::JSError(*this, createValue(*exc));
  }
}

void QJSRuntime::checkException(JSValueRef exc, const char* msg) {
  if (JS_IsException(*exc)) {
      js_std_dump_error(ctx_);
      throw jsi::JSError(std::string(msg), *this, createValue(*exc));
  }
}

void QJSRuntime::checkException(
    JSValueRef res,
    JSValueRef exc,
    const char* msg) {
  if (JS_IsException(*res)) {
      js_std_dump_error(ctx_);
      throw jsi::JSError(std::string(msg), *this, createValue(*exc));
  }
}


std::unique_ptr<jsi::Runtime> makeJSCRuntime() {
  return std::make_unique<QJSRuntime>();
}
}
