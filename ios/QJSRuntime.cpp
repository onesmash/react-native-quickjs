//
//  QJSRuntime.cpp
//  react-native-quickjs
//
//  Created by 徐晖 on 2020/9/14.
//

#include "QJSRuntime.h"
#include <sstream>
#include <jsi/jsilib.h>
#include <quickjs-libc.h>
#include <cutils.h>
using namespace facebook;
using namespace jsi;

namespace qjs {

namespace {
std::string JSStringToSTLString(JSContext *ctx, JSValue str) {
    const char* val = JS_ToCString(ctx, str);
    auto res = std::string(val);
    JS_FreeCString(ctx, val);
    return res;
}

//JSStringRef getLengthString() {
//  static JSStringRef length = JSStringCreateWithUTF8CString("length");
//  return length;
//}
//
//JSStringRef getNameString() {
//  static JSStringRef name = JSStringCreateWithUTF8CString("name");
//  return name;
//}
//
//JSStringRef getFunctionString() {
//  static JSStringRef func = JSStringCreateWithUTF8CString("Function");
//  return func;
//}
//
//#if !defined(_JSC_FAST_IS_ARRAY)
//JSStringRef getArrayString() {
//  static JSStringRef array = JSStringCreateWithUTF8CString("Array");
//  return array;
//}
//
//JSStringRef getIsArrayString() {
//  static JSStringRef isArray = JSStringCreateWithUTF8CString("isArray");
//  return isArray;
//}
//#endif
}

// std::string utility
namespace {
std::string to_string(void* value) {
  std::ostringstream ss;
  ss << value;
  return ss.str();
}
} // namespace

//typedef struct JSValue *JSValueRef;
typedef JSContext* JSContextRef;

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
    
    JSValue valueRef(const jsi::Value& value);
    
protected:
    class QJSAtomValue final : public PointerValue {
    #ifndef NDEBUG
        QJSAtomValue(JSContext *ctx,
                     const std::atomic<bool>& ctxInvalid,
                     JSAtom atom, std::atomic<intptr_t>& counter);
    #else
        QJSAtomValue(JSContext *ctx,
                     const std::atomic<bool>& ctxInvalid,
                     JSAtom atom);
    #endif
        void invalidate() override;
        const std::atomic<bool>& ctxInvalid_;
        JSContext *ctx_;
        JSAtom atom_;
    #ifndef NDEBUG
        std::atomic<intptr_t>& counter_;
    #endif
       protected:
        friend class QJSRuntime;
      };
    
    class QJSSymbolValue final : public PointerValue {
    #ifndef NDEBUG
        QJSSymbolValue(JSContext *ctx_,
                       const std::atomic<bool>& ctxInvalid,
                       JSValue sym, std::atomic<intptr_t>& counter);
    #else
        QJSSymbolValue(JSContext *ctx_,
                       const std::atomic<bool>& ctxInvalid,
                       JSValue sym);
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
        QJSStringValue(JSContext *ctx_, const std::atomic<bool>& ctxInvalid, JSValue str, std::atomic<intptr_t>& counter);
    #else
        QJSStringValue(JSContext *ctx_, const std::atomic<bool>& ctxInvalid, JSValue str);
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
            JSValue obj
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
    static JSValue symbolRef(const jsi::Symbol& str);
    static JSValue stringRef(const jsi::String& str);
    static JSAtom atomRef(const jsi::PropNameID& sym);
    static JSValue objectRef(const jsi::Object& obj);
    
    #ifdef RN_FABRIC_ENABLED
      static JSValue objectRef(const jsi::WeakObject& obj);
    #endif

    jsi::Symbol createSymbol(JSValue symbol) const;
    jsi::String createString(JSValue str) const;
    jsi::PropNameID createPropNameID(JSAtom atom);
    jsi::Object createObject(JSValue object) const;
    
    jsi::Runtime::PointerValue* makeAtomValue(JSAtom atom) const;
    jsi::Runtime::PointerValue* makeSymbolValue(JSValue sym) const;
    jsi::Runtime::PointerValue* makeStringValue(JSValue str) const;
    jsi::Runtime::PointerValue* makeObjectValue(JSValue obj) const;
    
    void checkException(JSValue exc);
    void checkException(JSValue res, JSValue exc);
    void checkException(JSValue exc, const char* msg);
    void checkException(JSValue res, JSValue exc, const char* msg);
       
    
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
    JS_SetContextOpaque(ctx_, this);
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
    JSValue symbol) const {
#ifndef NDEBUG
  return new QJSSymbolValue(ctx_, ctxInvalid_, symbol, symbolCounter_);
#else
  return new QJSSymbolValue(ctx_, ctxInvalid_, symbol);
#endif
}

jsi::Runtime::PointerValue* QJSRuntime::makeStringValue(
    JSValue str) const {
#ifndef NDEBUG
  return new QJSStringValue(ctx_, ctxInvalid_, str, stringCounter_);
#else
  return new QJSStringValue(ctx_, ctxInvalid_, str);
#endif
}

jsi::Runtime::PointerValue* QJSRuntime::makeObjectValue(
    JSValue object) const {
#ifndef NDEBUG
  return new QJSObjectValue(ctx_, ctxInvalid_, object, objectCounter_);
#else
  return new QJSObjectValue(ctx_, ctxInvalid_, object);
#endif
}

jsi::Runtime::PointerValue* QJSRuntime::makeAtomValue(
    JSAtom atom) const {
#ifndef NDEBUG
  return new QJSAtomValue(ctx_, ctxInvalid_, atom, objectCounter_);
#else
  return new QJSAtomValue(ctx_, ctxInvalid_, atom);
#endif
}


jsi::Symbol QJSRuntime::createSymbol(JSValue sym) const {
  return make<jsi::Symbol>(makeSymbolValue(sym));
}

jsi::String QJSRuntime::createString(JSValue str) const {
  return make<jsi::String>(makeStringValue(str));
}

jsi::PropNameID QJSRuntime::createPropNameID(JSAtom atom) {
  return make<jsi::PropNameID>(makeAtomValue(atom));
}

jsi::Object QJSRuntime::createObject(JSValue obj) const {
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
      return jsi::Value(createString(value));
  } else if (JS_IsObject(value)) {
    return jsi::Value(createObject(value));
  } else if (JS_IsSymbol(value)) {
    return jsi::Value(createSymbol(value));
  } else {
    // WHAT ARE YOU
    abort();
  }
}

JSValue QJSRuntime::valueRef(const jsi::Value& value) {
  // I would rather switch on value.kind_
  if (value.isUndefined()) {
    return JS_UNDEFINED;
  } else if (value.isNull()) {
    return JS_NULL;
  } else if (value.isBool()) {
      return JS_NewBool(ctx_, value.getBool());
  } else if (value.isNumber()) {
      return JS_NewFloat64(ctx_, value.getNumber());
  } else if (value.isSymbol()) {
    return symbolRef(value.getSymbol(*this));
  } else if (value.isString()) {
    return stringRef(value.getString(*this));
  } else if (value.isObject()) {
    return objectRef(value.getObject(*this));
  } else {
    // What are you?
    abort();
  }
}

JSValue QJSRuntime::symbolRef(const jsi::Symbol& sym) {
  return static_cast<const QJSSymbolValue*>(getPointerValue(sym))->sym_;
}

JSValue QJSRuntime::stringRef(const jsi::String& str) {
  return static_cast<const QJSStringValue*>(getPointerValue(str))->str_;
}

JSAtom QJSRuntime::atomRef(const jsi::PropNameID& prop) {
  return static_cast<const QJSAtomValue*>(getPointerValue(prop))->atom_;
}

JSValue QJSRuntime::objectRef(const jsi::Object& obj) {
  return static_cast<const QJSObjectValue*>(getPointerValue(obj))->obj_;
}

#ifdef RN_FABRIC_ENABLED
JSValue QJSRuntime::objectRef(const jsi::WeakObject& obj) {
  // TODO: revisit this implementation
  return static_cast<const QJSObjectValue*>(getPointerValue(obj))->obj_;
}
#endif

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
    JSValue obj, val;
    obj = JS_ReadObject(ctx_, (uint8_t*)tmp.c_str(), tmp.size(), JS_READ_OBJ_BYTECODE);
    checkException(obj);
    if (JS_VALUE_GET_TAG(obj) == JS_TAG_MODULE) {
        if (JS_ResolveModule(ctx_, obj) < 0) {
            JS_FreeValue(ctx_, obj);
            goto exception;
        }
        js_module_set_import_meta(ctx_, obj, 0, 1);
    }
    val = JS_EvalFunction(ctx_, obj);
    if (JS_IsException(val)) {
    exception:
        js_std_dump_error(ctx_);
        exit(1);
    }
    auto res = createValue(val);
    JS_FreeValue(ctx_, val);
    return res;
}

jsi::Value QJSRuntime::evaluateJavaScript(
    const std::shared_ptr<const jsi::Buffer> &buffer,
    const std::string& sourceURL) {
    std::string tmp(
                    reinterpret_cast<const char*>(buffer->data()), buffer->size());
    JSValue val = JS_Eval(ctx_, tmp.c_str(), tmp.size(), sourceURL.c_str(), 0);
    checkException(val);
    auto res = createValue(val);
    JS_FreeValue(ctx_, val);
    return res;
}

jsi::Object QJSRuntime::global() {
    JSValue globalObj = JS_GetGlobalObject(ctx_);
    auto res = createObject(globalObj);
    JS_FreeValue(ctx_, globalObj);
    return res;
}

std::string QJSRuntime::description() {
  if (desc_.empty()) {
    desc_ = std::string("<QJSRuntime@") + to_string(this) + ">";
  }
  return desc_;
}

bool QJSRuntime::isInspectable() {
  return false;
}

QJSRuntime::QJSAtomValue::QJSAtomValue(
    JSContext *ctx,
    const std::atomic<bool>& ctxInvalid,
    JSAtom atom
#ifndef NDEBUG
    ,
    std::atomic<intptr_t>& counter
#endif
    )
    : ctx_(ctx), ctxInvalid_(ctxInvalid),
      atom_(JS_DupAtom(ctx, atom))
#ifndef NDEBUG
      ,
      counter_(counter)
#endif
{
#ifndef NDEBUG
  counter_ += 1;
#endif
}

void QJSRuntime::QJSAtomValue::invalidate() {
#ifndef NDEBUG
    counter_ -= 1;
#endif
    if (!ctxInvalid_) {
        JS_FreeAtom(ctx_, atom_);
    }
    delete this;
}

QJSRuntime::QJSSymbolValue::QJSSymbolValue(
    JSContext *ctx,
    const std::atomic<bool>& ctxInvalid,
    JSValue sym
#ifndef NDEBUG
    ,
    std::atomic<intptr_t>& counter
#endif
    )
    : ctx_(ctx),
      ctxInvalid_(ctxInvalid),
      sym_(JS_DupValue(ctx, sym))
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
    JSValue str,
    std::atomic<intptr_t>& counter)
    : ctx_(ctx), ctxInvalid_(ctxInvalid), str_(JS_DupValue(ctx_, str)), counter_(counter) {
  counter_ += 1;
}
#else
QJSRuntime::QJSStringValue::QJSStringValue(JSContext *ctx, const std::atomic<bool>& ctxInvalid, JSValue str)
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
    JSValue obj
#ifndef NDEBUG
    ,
    std::atomic<intptr_t>& counter
#endif
    )
    : ctx_(ctx),
      ctxInvalid_(ctxInvalid),
      obj_(JS_DupValue(ctx, obj))
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

jsi::Runtime::PointerValue* QJSRuntime::cloneSymbol(
    const jsi::Runtime::PointerValue* pv) {
    if (!pv) {
        return nullptr;
        
    }
    const QJSSymbolValue* symbol = static_cast<const QJSSymbolValue*>(pv);
    return makeSymbolValue(symbol->sym_);
}

jsi::Runtime::PointerValue* QJSRuntime::cloneString(
    const jsi::Runtime::PointerValue* pv) {
  if (!pv) {
    return nullptr;
  }
  const QJSStringValue* string = static_cast<const QJSStringValue*>(pv);
  return makeStringValue(string->str_);
}

jsi::Runtime::PointerValue* QJSRuntime::cloneObject(
    const jsi::Runtime::PointerValue* pv) {
  if (!pv) {
    return nullptr;
  }
  const QJSObjectValue* object = static_cast<const QJSObjectValue*>(pv);
  assert(
      object->ctx_ == ctx_ &&
      "Don't try to clone an object backed by a different Runtime");
  return makeObjectValue(object->obj_);
}

jsi::Runtime::PointerValue* QJSRuntime::clonePropNameID(
    const jsi::Runtime::PointerValue* pv) {
  if (!pv) {
    return nullptr;
  }
  const QJSAtomValue* atom = static_cast<const QJSAtomValue*>(pv);
  return makeAtomValue(atom->atom_);
}

jsi::PropNameID QJSRuntime::createPropNameIDFromAscii(
    const char* str,
    size_t length) {
    JSAtom atom = JS_NewAtomLen(ctx_, str, length);
    auto res = createPropNameID(atom);
    JS_FreeAtom(ctx_, atom);
    return res;
}

jsi::PropNameID QJSRuntime::createPropNameIDFromUtf8(
    const uint8_t* utf8,
    size_t length) {
    JSAtom atom = JS_NewAtomLen(ctx_, (const char*)utf8, length);
    auto res = createPropNameID(atom);
    JS_FreeAtom(ctx_, atom);
    return res;
}

jsi::PropNameID QJSRuntime::createPropNameIDFromString(const jsi::String& str) {
    std::string val = str.utf8(*this);
    JSAtom atom = JS_NewAtomLen(ctx_, (const char*)val.c_str(), val.size());
    auto res = createPropNameID(atom);
    JS_FreeAtom(ctx_, atom);
    return  res;
}

std::string QJSRuntime::utf8(const jsi::PropNameID& sym) {
    return std::string(JS_AtomToCString(ctx_, atomRef(sym)));
}

bool QJSRuntime::compare(const jsi::PropNameID& a, const jsi::PropNameID& b) {
    return atomRef(a) == atomRef(b);
}

std::string QJSRuntime::symbolToString(const jsi::Symbol& sym) {
  return jsi::Value(*this, sym)
    .toString(*this)
    .utf8(*this);
}

jsi::String QJSRuntime::createStringFromAscii(const char* str, size_t length) {
  // Yes we end up double casting for semantic reasons (UTF8 contains ASCII,
  // not the other way around)
  return this->createStringFromUtf8(
      reinterpret_cast<const uint8_t*>(str), length);
}

jsi::String QJSRuntime::createStringFromUtf8(
    const uint8_t* str,
    size_t length) {
    JSValue val = JS_NewStringLen(ctx_, (const char*)str, length);
    auto result = createString(val);
    JS_FreeValue(ctx_, val);
    return result;
}

std::string QJSRuntime::utf8(const jsi::String& str) {
  return JSStringToSTLString(ctx_, stringRef(str));
}

jsi::Object QJSRuntime::createObject() {
  return createObject(JS_NULL);
}

// HostObject details
namespace detail {
struct HostObjectProxyBase {
  HostObjectProxyBase(
      QJSRuntime& rt,
      const std::shared_ptr<jsi::HostObject>& sho)
      : runtime(rt), hostObject(sho) {}

  QJSRuntime& runtime;
  std::shared_ptr<jsi::HostObject> hostObject;
};
} // namespace detail

namespace {
std::once_flag hostObjectClassOnceFlag;
static JSClassID hostObjectClassID;
} // namespace

jsi::Object QJSRuntime::createObject(std::shared_ptr<jsi::HostObject> ho) {
  struct HostObjectProxy : public detail::HostObjectProxyBase {
    static JSValue getProperty(JSContext *ctx, JSValueConst object, JSAtom atom,
                               JSValueConst receiver) {
        auto proxy = static_cast<HostObjectProxy*>(JS_GetOpaque(object, hostObjectClassID));
        auto& rt = proxy->runtime;
        jsi::PropNameID sym = rt.createPropNameID(atom);
        jsi::Value ret;
        try {
            ret = proxy->hostObject->get(rt, sym);
        } catch (const jsi::JSError& error) {
            return rt.valueRef(error.value());
        } catch (const std::exception& ex) {
            const std::string script = "Error(\"Exception in HostObject::get(propName:" + rt.utf8(sym) + "): " + ex.what() + "\")";
            auto excValue = JS_Eval(ctx, script.c_str(), script.size(), "<evalScript>", JS_EVAL_TYPE_GLOBAL);
            return rt.valueRef(excValue);
        } catch (...) {
            const std::string script = "Error(\"Exception in HostObject::get(propName:" + rt.utf8(sym) + "):  <unknown>\")";
            auto excValue = JS_Eval(ctx, script.c_str(), script.size(), "<evalScript>", JS_EVAL_TYPE_GLOBAL);
            return rt.valueRef(excValue);
        }
        return rt.valueRef(ret);
    }

    #define QJS_UNUSED(x) (void) (x);

    static int setProperty(JSContext *ctx, JSValueConst object, JSAtom atom,
                           JSValueConst value, JSValueConst receiver, int flags) {
        auto proxy = static_cast<HostObjectProxy*>(JS_GetOpaque(object, hostObjectClassID));
        auto& rt = proxy->runtime;
        jsi::PropNameID sym = rt.createPropNameID(atom);
        try {
            proxy->hostObject->set(rt, sym, rt.createValue(value));
        } catch (...) {
            return -1;
        }
        return TRUE;
    }

    // JSC does not provide means to communicate errors from this callback,
    // so the error handling strategy is very brutal - we'll just crash
    // due to noexcept.
    static int getPropertyNames(JSContext *ctx, JSPropertyEnum **ptab,
                                uint32_t *plen,
                                JSValueConst object) {
        auto proxy = static_cast<HostObjectProxy*>(JS_GetOpaque(object, hostObjectClassID));
        auto& rt = proxy->runtime;
        auto names = proxy->hostObject->getPropertyNames(rt);
        JSPropertyEnum *tab;
        size_t len = names.size();
        *ptab = NULL;
        *plen = 0;
        if (len > 0) {
            tab = (JSPropertyEnum *)js_malloc(ctx, sizeof(JSPropertyEnum) * len);
            if (!tab)
                return -1;
            for(int i = 0; i < len; i++) {
                tab[i].is_enumerable = TRUE;
                tab[i].atom = JS_DupAtom(ctx, atomRef(names[i]));
            }
            *plen = (uint32_t)len;
        }
        return 0;
    }

    #undef JSC_UNUSED

    static void finalize(JSRuntime *rt, JSValue val) {
        auto hostObject = static_cast<HostObjectProxy*>(JS_GetOpaque(val, hostObjectClassID));
        JS_SetOpaque(val, nullptr);
        delete hostObject;
    }

    using HostObjectProxyBase::HostObjectProxyBase;
  };

  std::call_once(hostObjectClassOnceFlag, [this]() {
      JS_NewClassID(&hostObjectClassID);
      JSClassExoticMethods exotic = {
          .get_property = HostObjectProxy::getProperty,
          .set_property = HostObjectProxy::setProperty,
          .get_own_property_names = HostObjectProxy::getPropertyNames,
      };
      JSClassDef hostObjectClassDef = {
          "HostObject",
          .finalizer = HostObjectProxy::finalize,
          .exotic = &exotic,
      };
      JS_NewClass(JS_GetRuntime(ctx_), hostObjectClassID, &hostObjectClassDef);
      JSValue proto = JS_NewObject(ctx_);
      JS_SetClassProto(ctx_, hostObjectClassID, proto);
  });

    JSValue obj = JS_NewObjectClass(ctx_, hostObjectClassID);
    JS_SetOpaque(obj, new HostObjectProxy(*this, ho));
    auto res = createObject(obj);
    JS_FreeValue(ctx_, obj);
    return res;
}

std::shared_ptr<jsi::HostObject> QJSRuntime::getHostObject(
    const jsi::Object& obj) {
  // We are guaranteed at this point to have isHostObject(obj) == true
  // so the private data should be HostObjectMetadata
  JSValue object = objectRef(obj);
  auto metadata =
      static_cast<detail::HostObjectProxyBase*>(JS_GetOpaque(object, hostObjectClassID));
  assert(metadata);
  return metadata->hostObject;
}

jsi::Value QJSRuntime::getProperty(
    const jsi::Object& obj,
    const jsi::String& name) {
    JSValue val = JS_GetPropertyStr(ctx_, objectRef(obj), name.utf8(*this).c_str());
    checkException(val);
    auto res = createValue(val);
    JS_FreeValue(ctx_, val);
    return res;
}

jsi::Value QJSRuntime::getProperty(
    const jsi::Object& obj,
    const jsi::PropNameID& name) {
    JSValue val = JS_GetProperty(ctx_, objectRef(obj), atomRef(name));
    checkException(val);
    auto res = createValue(val);
    JS_FreeValue(ctx_, val);
    return res;
}

bool QJSRuntime::hasProperty(const jsi::Object& obj, const jsi::String& name) {
    JSAtom atom = JS_NewAtom(ctx_, name.utf8(*this).c_str());
    auto res = JS_HasProperty(ctx_, objectRef(obj), atom) == TRUE;
    JS_FreeAtom(ctx_, atom);
    return res;
}

bool QJSRuntime::hasProperty(
    const jsi::Object& obj,
    const jsi::PropNameID& name) {
    return JS_HasProperty(ctx_, objectRef(obj), atomRef(name)) == TRUE;
}

void QJSRuntime::setPropertyValue(
    jsi::Object& object,
    const jsi::PropNameID& name,
    const jsi::Value& value) {
    int res = JS_SetProperty(ctx_, objectRef(object), atomRef(name), valueRef(value));
    if (res == -1) {
        JSValue exception = JS_GetException(ctx_);
        checkException(exception);
    }
}

void QJSRuntime::setPropertyValue(
    jsi::Object& object,
    const jsi::String& name,
    const jsi::Value& value) {
    JSAtom atom = JS_NewAtom(ctx_, name.utf8(*this).c_str());
    int res = JS_SetProperty(ctx_, objectRef(object), atom, valueRef(value));
    JS_FreeAtom(ctx_, atom);
    if (res == -1) {
        JSValue exception = JS_GetException(ctx_);
        checkException(exception);
    }
}

bool QJSRuntime::isArray(const jsi::Object& obj) const {
    return JS_IsArray(ctx_, objectRef(obj)) == TRUE;
}

bool QJSRuntime::isArrayBuffer(const jsi::Object& /*obj*/) const {
  // TODO: T23270523 - This would fail on builds that use our custom JSC
  // auto typedArrayType = JSValueGetTypedArrayType(ctx_, objectRef(obj),
  // nullptr);  return typedArrayType == kJSTypedArrayTypeArrayBuffer;
  throw std::runtime_error("Unsupported");
}

uint8_t* QJSRuntime::data(const jsi::ArrayBuffer& /*obj*/) {
  // TODO: T23270523 - This would fail on builds that use our custom JSC
  // return static_cast<uint8_t*>(
  //    JSObjectGetArrayBufferBytesPtr(ctx_, objectRef(obj), nullptr));
  throw std::runtime_error("Unsupported");
}

size_t QJSRuntime::size(const jsi::ArrayBuffer& /*obj*/) {
  // TODO: T23270523 - This would fail on builds that use our custom JSC
  // return JSObjectGetArrayBufferByteLength(ctx_, objectRef(obj), nullptr);
  throw std::runtime_error("Unsupported");
}

bool QJSRuntime::isFunction(const jsi::Object& obj) const {
  return JS_IsFunction(ctx_, objectRef(obj));
}

bool QJSRuntime::isHostObject(const jsi::Object& obj) const {
  auto cls = JS_GetClassProto(ctx_, hostObjectClassID);
  return JS_IsInstanceOf(ctx_, cls, objectRef(obj)) == TRUE;
}

// Very expensive
jsi::Array QJSRuntime::getPropertyNames(const jsi::Object& obj) {
    uint32_t len;
    JSPropertyEnum *tab;
    JS_GetOwnPropertyNames(ctx_, &tab, &len, objectRef(obj),
                           JS_GPN_STRING_MASK | JS_GPN_ENUM_ONLY);
    auto result = createArray(len);
    for (size_t i = 0; i < len; i++) {
        JSValue name = JS_AtomToString(ctx_, tab[i].atom);
        result.setValueAtIndex(*this, i, createString(name));
        JS_FreeValue(ctx_, name);
    }
    js_free(ctx_, tab);
    return result;
}

jsi::WeakObject QJSRuntime::createWeakObject(const jsi::Object& obj) {
#ifdef RN_FABRIC_ENABLED
  // TODO: revisit this implementation
  return make<jsi::WeakObject>(makeObjectValue(objectRef(obj)));
#else
  throw std::logic_error("Not implemented");
#endif
}

jsi::Value QJSRuntime::lockWeakObject(const jsi::WeakObject& obj) {
#ifdef RN_FABRIC_ENABLED
  // TODO: revisit this implementation
  return jsi::Value(createObject(objectRef(obj)));
#else
  throw std::logic_error("Not implemented");
#endif
}

jsi::Array QJSRuntime::createArray(size_t length) {
    JSValue obj = JS_NewArray(ctx_);
    checkException(obj);
    JSValue len = JS_NewInt64(ctx_, length);
    JS_SetPropertyStr(ctx_, obj, "length", len);
    JS_FreeValue(ctx_, len);
    auto res = createObject(obj).getArray(*this);
    JS_FreeValue(ctx_, obj);
    return res;
}

size_t QJSRuntime::size(const jsi::Array& arr) {
    JSAtom len = JS_NewAtom(ctx_, "length");
    auto res = static_cast<size_t>(
      getProperty(arr, createPropNameID(len)).getNumber());
    JS_FreeAtom(ctx_, len);
    return res;
}

jsi::Value QJSRuntime::getValueAtIndex(const jsi::Array& arr, size_t i) {
    JSValue val = JS_GetPropertyUint32(ctx_, objectRef(arr), (uint32_t)i);
    checkException(val);
    auto res = createValue(val);
    JS_FreeValue(ctx_, val);
    return res;
}

void QJSRuntime::setValueAtIndexImpl(
    jsi::Array& arr,
    size_t i,
    const jsi::Value& value) {
    if (JS_SetPropertyUint32(ctx_, objectRef(arr), (unsigned)i, valueRef(value)) < 0) {
        checkException(JS_EXCEPTION);
    }
}

namespace {
std::once_flag hostFunctionClassOnceFlag;
JSClassID hostFunctionClassID;

class HostFunctionProxy {
 public:
  HostFunctionProxy(jsi::HostFunctionType hostFunction)
      : hostFunction_(hostFunction) {}

  jsi::HostFunctionType& getHostFunction() {
    return hostFunction_;
  }

 protected:
  jsi::HostFunctionType hostFunction_;
};
} // namespace

jsi::Function QJSRuntime::createFunctionFromHostFunction(
    const jsi::PropNameID& name,
    unsigned int paramCount,
    jsi::HostFunctionType func) {
  class HostFunctionMetadata : public HostFunctionProxy {
   public:
    static void initialize(JSContextRef ctx, JSValue object) {
      // We need to set up the prototype chain properly here. In theory we
      // could set func.prototype.prototype = Function.prototype to get the
      // same result. Not sure which approach is better.
    }

    static JSValue call(JSContext *ctx, JSValueConst funcObj, JSValueConst thisObject, int argumentCount, JSValueConst *arguments, int flags) {
        HostFunctionMetadata* metadata =
            static_cast<HostFunctionMetadata*>(JS_GetOpaque(funcObj, hostFunctionClassID));
        QJSRuntime& rt = *(QJSRuntime *)JS_GetContextOpaque(ctx);
        const unsigned maxStackArgCount = 8;
        jsi::Value stackArgs[maxStackArgCount];
        std::unique_ptr<jsi::Value[]> heapArgs;
        jsi::Value* args;
        if (argumentCount > maxStackArgCount) {
            heapArgs = std::make_unique<jsi::Value[]>(argumentCount);
            for (size_t i = 0; i < argumentCount; i++) {
                heapArgs[i] = rt.createValue(arguments[i]);
            }
            args = heapArgs.get();
        } else {
            for (size_t i = 0; i < argumentCount; i++) {
                stackArgs[i] = rt.createValue(arguments[i]);
            }
            args = stackArgs;
        }
        JSValue res;
        jsi::Value thisVal(rt.createObject(thisObject));
        try {
            res = rt.valueRef(metadata->hostFunction_(rt, thisVal, args, argumentCount));
        } catch (const jsi::JSError& error) {
            res = JS_UNDEFINED;
        } catch (const std::exception& ex) {
            res = JS_UNDEFINED;
        } catch (...) {
            res = JS_UNDEFINED;
        }
        return res;
    }
      
      static void finalize(JSRuntime *rt, JSValue val) {
          HostFunctionMetadata* metadata =
              static_cast<HostFunctionMetadata*>(JS_GetOpaque(val, hostFunctionClassID));
          JS_SetOpaque(val, nullptr);
          delete metadata;
      }

    HostFunctionMetadata(
        QJSRuntime* rt,
        jsi::HostFunctionType hf,
        unsigned ac,
        const std::string& n)
        : HostFunctionProxy(hf),
          runtime(rt),
          argCount(ac),
          name(n) {}

    QJSRuntime* runtime;
    unsigned argCount;
    std::string name;
  };

    
    std::call_once(hostFunctionClassOnceFlag, [this]() {
        JS_NewClassID(&hostFunctionClassID);
        JSClassDef hostFunctionObjectClassDef = {
            "HostFunctionObject",
            .call = HostFunctionMetadata::call,
            .finalizer = HostFunctionMetadata::finalize,
        };
        JS_NewClass(JS_GetRuntime(ctx_), hostFunctionClassID, &hostFunctionObjectClassDef);
        JSValue emptyFunc = JS_NewCFunction(ctx_, (JSCFunction *)nullptr, "", 0);
        JS_SetClassProto(ctx_, hostFunctionClassID, JS_GetPrototype(ctx_, emptyFunc));
    });
    JSValue functionObj = JS_NewObjectClass(ctx_, hostFunctionClassID);
    JS_SetOpaque(functionObj, new HostFunctionMetadata(this, func, paramCount, utf8(name)));
    auto res = createObject(functionObj).getFunction(*this);
    JS_FreeValue(ctx_, functionObj);
    return res;
}

namespace detail {

class ArgsConverter {
 public:
  ArgsConverter(QJSRuntime& rt, const jsi::Value* args, size_t count) {
    JSValue* destination = inline_;
    if (count > maxStackArgs) {
      outOfLine_ = std::make_unique<JSValue[]>(count);
      destination = outOfLine_.get();
    }

    for (size_t i = 0; i < count; ++i) {
      destination[i] = rt.valueRef(args[i]);
    }
  }

  operator JSValue*() {
    return outOfLine_ ? outOfLine_.get() : inline_;
  }

 private:
  constexpr static unsigned maxStackArgs = 8;
  JSValue inline_[maxStackArgs];
  std::unique_ptr<JSValue[]> outOfLine_;
};
} // namespace detail

bool QJSRuntime::isHostFunction(const jsi::Function& obj) const {
    auto cls = JS_GetClassProto(ctx_, hostFunctionClassID);
    return JS_IsInstanceOf(ctx_, cls, objectRef(obj)) == TRUE;
}

jsi::HostFunctionType& JSCRuntime::getHostFunction(const jsi::Function& obj) {
// We know that isHostFunction(obj) is true here, so its safe to proceed
auto proxy =
    static_cast<HostFunctionProxy*>(JSObjectGetPrivate(objectRef(obj)));
return proxy->getHostFunction();
}

jsi::Value JSCRuntime::call(
  const jsi::Function& f,
  const jsi::Value& jsThis,
  const jsi::Value* args,
  size_t count) {
JSValueRef exc = nullptr;
auto res = JSObjectCallAsFunction(
    ctx_,
    objectRef(f),
    jsThis.isUndefined() ? nullptr : objectRef(jsThis.getObject(*this)),
    count,
    detail::ArgsConverter(*this, args, count),
    &exc);
checkException(exc);
return createValue(res);
}

jsi::Value JSCRuntime::callAsConstructor(
  const jsi::Function& f,
  const jsi::Value* args,
  size_t count) {
JSValueRef exc = nullptr;
auto res = JSObjectCallAsConstructor(
    ctx_,
    objectRef(f),
    count,
    detail::ArgsConverter(*this, args, count),
    &exc);
checkException(exc);
return createValue(res);
}

void QJSRuntime::checkException(JSValue exc) {
  if (JS_IsException(exc)) {
      js_std_dump_error(ctx_);
      throw jsi::JSError(*this, createValue(exc));
  }
}

void QJSRuntime::checkException(JSValue res, JSValue exc) {
  if (JS_IsException(res)) {
      js_std_dump_error(ctx_);
      throw jsi::JSError(*this, createValue(exc));
  }
}

void QJSRuntime::checkException(JSValue exc, const char* msg) {
  if (JS_IsException(exc)) {
      js_std_dump_error(ctx_);
      throw jsi::JSError(std::string(msg), *this, createValue(exc));
  }
}

void QJSRuntime::checkException(
    JSValue res,
    JSValue exc,
    const char* msg) {
  if (JS_IsException(res)) {
      js_std_dump_error(ctx_);
      throw jsi::JSError(std::string(msg), *this, createValue(exc));
  }
}

std::unique_ptr<jsi::Runtime> makeJSCRuntime() {
  return std::make_unique<QJSRuntime>();
}
}
