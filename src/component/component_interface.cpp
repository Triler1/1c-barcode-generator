#include "component/component_interface.h"

#include "common/barcode_options.h"
#include "common/barcode_types.h"

#include <cstring>
#include <stdexcept>
#include <string>
#include <atomic>

#if defined(__linux__) || defined(__APPLE__)
#include <iconv.h>
#endif

// ─────────────────────────────────────────────────────────────────────────────
// Вспомогательные функции конвертации wchar_t ↔ WCHAR_T (UTF-16 / UTF-32).
// На Windows wchar_t уже 2 байта, на Linux/macOS — 4 байта, поэтому нужна
// явная конвертация. Взяты из официального примера 1С AddInNative.
// ─────────────────────────────────────────────────────────────────────────────

static uint32_t getLen(const WCHAR_T* src) {
    uint32_t n = 0;
    const WCHAR_T* p = src;
    while (*p++) ++n;
    return n;
}

static uint32_t convToShortWchar(WCHAR_T** dest, const wchar_t* src, size_t len = 0) {
    if (!len)
        len = ::wcslen(src) + 1;
    if (!*dest)
        *dest = new WCHAR_T[len];

    WCHAR_T*  d = *dest;
    const wchar_t* s = src;
    ::memset(*dest, 0, len * sizeof(WCHAR_T));

#ifdef __linux__
    size_t f = len * sizeof(wchar_t), t = len * sizeof(WCHAR_T);
    const char* from = sizeof(wchar_t) == 2 ? "UTF-16" : "UTF-32";
    iconv_t cd = iconv_open("UTF-16LE", from);
    if (cd != (iconv_t)-1) {
        size_t ok = iconv(cd, (char**)&s, &f, (char**)&d, &t);
        iconv_close(cd);
        if (ok != (size_t)-1) return (uint32_t)ok;
    }
#endif
    uint32_t res = 0;
    for (; len; --len, ++res, ++s, ++d)
        *d = (WCHAR_T)*s;
    return res;
}

static uint32_t convFromShortWchar(wchar_t** dest, const WCHAR_T* src, uint32_t len = 0) {
    if (!len)
        len = getLen(src) + 1;
    if (!*dest)
        *dest = new wchar_t[len];

    wchar_t*  d = *dest;
    const WCHAR_T* s = src;
    ::memset(*dest, 0, len * sizeof(wchar_t));

#ifdef __linux__
    const char* from = sizeof(wchar_t) == 2 ? "UTF-16" : "UTF-32";
    size_t f = len * sizeof(WCHAR_T), t = len * sizeof(wchar_t);
    iconv_t cd = iconv_open("UTF-32LE", from);
    if (cd != (iconv_t)-1) {
        size_t ok = iconv(cd, (char**)&s, &f, (char**)&d, &t);
        iconv_close(cd);
        if (ok != (size_t)-1) return (uint32_t)ok;
    }
#endif
    uint32_t res = 0;
    for (; len; --len, ++res, ++s, ++d)
        *d = (wchar_t)*s;
    return res;
}

// ─────────────────────────────────────────────────────────────────────────────
// Имена методов на английском и русском.
// Порядок должен строго совпадать с enum Methods в заголовке.
// ─────────────────────────────────────────────────────────────────────────────

static const wchar_t* g_MethodNames[] = {
    L"GenerateQR",
    L"GenerateDataMatrix",
    L"GenerateAztec",
};

static const wchar_t* g_MethodNamesRu[] = {
    L"СформироватьQR",
    L"СформироватьDataMatrix",
    L"СформироватьAztec",
};

// Количество параметров у каждого метода: (data, moduleSize, margin)
static const long g_MethodParamCount[] = { 3, 3, 3 };

// Статический счётчик возможностей платформы (заполняется 1С при загрузке).
static std::atomic<AppCapabilities> g_capabilities{ eAppCapabilitiesInvalid };
static std::u16string s_classNames(g_kClassName);

// ─────────────────────────────────────────────────────────────────────────────
// Экспортируемые функции — точки входа, которые вызывает 1С при загрузке DLL.
// ─────────────────────────────────────────────────────────────────────────────

long GetClassObject(const WCHAR_T* wsName, IComponentBase** pInterface) {
    if (!*pInterface) {
        *pInterface = new ComponentInterface;
        return (long)*pInterface;
    }
    return 0;
}

AppCapabilities SetPlatformCapabilities(const AppCapabilities capabilities) {
    g_capabilities = capabilities;
    return eAppCapabilitiesLast;
}

AttachType GetAttachType() {
    return eCanAttachAny;
}

long DestroyObject(IComponentBase** pIntf) {
    if (!*pIntf)
        return -1;
    delete *pIntf;
    *pIntf = nullptr;
    return 0;
}

const WCHAR_T* GetClassNames() {
    return s_classNames.c_str();
}

// ─────────────────────────────────────────────────────────────────────────────
// ComponentInterface
// ─────────────────────────────────────────────────────────────────────────────

ComponentInterface::ComponentInterface() = default;

// ── IInitDoneBase ─────────────────────────────────────────────────────────────

bool ComponentInterface::Init(void* pConnection) {
    m_iConnect = static_cast<IAddInDefBase*>(pConnection);
    return m_iConnect != nullptr;
}

bool ComponentInterface::setMemManager(void* mem) {
    m_iMemory = static_cast<IMemoryManager*>(mem);
    return m_iMemory != nullptr;
}

long ComponentInterface::GetInfo() {
    return 2000;  // версия технологии внешних компонент: 2.0
}

void ComponentInterface::Done() {}

// ── ILanguageExtenderBase — регистрация ──────────────────────────────────────

bool ComponentInterface::RegisterExtensionAs(WCHAR_T** wsExtName) {
    const wchar_t* name = L"BarcodeGenerator";
    size_t len = ::wcslen(name) + 1;
    if (!m_iMemory)
        return false;
    if (m_iMemory->AllocMemory((void**)wsExtName, (unsigned)len * sizeof(WCHAR_T)))
        convToShortWchar(wsExtName, name, len);
    return true;
}

// ── Свойства (не используются) ────────────────────────────────────────────────

long ComponentInterface::GetNProps()                                   { return 0; }
long ComponentInterface::FindProp(const WCHAR_T*)                      { return -1; }
const WCHAR_T* ComponentInterface::GetPropName(long, long)             { return nullptr; }
bool ComponentInterface::GetPropVal(const long, tVariant*)             { return false; }
bool ComponentInterface::SetPropVal(const long, tVariant*)             { return false; }
bool ComponentInterface::IsPropReadable(const long)                    { return false; }
bool ComponentInterface::IsPropWritable(const long)                    { return false; }

// ── Методы ────────────────────────────────────────────────────────────────────

long ComponentInterface::GetNMethods() {
    return eMethLast;
}

long ComponentInterface::FindMethod(const WCHAR_T* wsMethodName) {
    wchar_t* name = nullptr;
    convFromShortWchar(&name, wsMethodName);
    long num = findName(g_MethodNames, name, eMethLast);
    if (num == -1)
        num = findName(g_MethodNamesRu, name, eMethLast);
    delete[] name;
    return num;
}

const WCHAR_T* ComponentInterface::GetMethodName(const long lMethodNum,
                                                   const long lMethodAlias) {
    if (lMethodNum >= eMethLast)
        return nullptr;

    const wchar_t* src = nullptr;
    switch (lMethodAlias) {
        case 0: src = g_MethodNames[lMethodNum];   break;
        case 1: src = g_MethodNamesRu[lMethodNum]; break;
        default: return nullptr;
    }

    WCHAR_T* result = nullptr;
    size_t len = ::wcslen(src) + 1;
    if (m_iMemory && m_iMemory->AllocMemory((void**)&result, (unsigned)len * sizeof(WCHAR_T)))
        convToShortWchar(&result, src, len);
    return result;
}

long ComponentInterface::GetNParams(const long lMethodNum) {
    if (lMethodNum >= eMethLast)
        return 0;
    return g_MethodParamCount[lMethodNum];
}

bool ComponentInterface::GetParamDefValue(const long, const long, tVariant* pvar) {
    // Все параметры обязательные — значений по умолчанию нет.
    TV_VT(pvar) = VTYPE_EMPTY;
    return false;
}

bool ComponentInterface::HasRetVal(const long lMethodNum) {
    // Все три метода возвращают строку SVG.
    return lMethodNum >= 0 && lMethodNum < eMethLast;
}

// CallAsProc — у наших методов есть возвращаемое значение, поэтому
// 1С вызывает CallAsFunc. Сюда управление не придёт.
bool ComponentInterface::CallAsProc(const long, tVariant*, const long) {
    return false;
}

bool ComponentInterface::CallAsFunc(const long lMethodNum,
                                     tVariant*  pvarRetValue,
                                     tVariant*  paParams,
                                     const long lSizeArray) {
    // Ожидаем ровно 3 параметра: data (строка), moduleSize (int), margin (int).
    if (lSizeArray != 3 || !paParams || !pvarRetValue)
        return false;

    // Параметр 0: data
    std::string data = extractString(&paParams[0]);
    if (data.empty()) {
        addError(ADDIN_E_VERY_IMPORTANT, L"BarcodeGenerator",
                 L"Параметр data не может быть пустым", -1);
        return false;
    }

    // Параметры 1 и 2: moduleSize, margin
    int moduleSize = 10, margin = 4;  // разумные значения по умолчанию
    if (!extractInt(&paParams[1], moduleSize) || moduleSize <= 0) {
        addError(ADDIN_E_VERY_IMPORTANT, L"BarcodeGenerator",
                 L"moduleSize должен быть положительным целым числом", -1);
        return false;
    }
    if (!extractInt(&paParams[2], margin) || margin < 0) {
        addError(ADDIN_E_VERY_IMPORTANT, L"BarcodeGenerator",
                 L"margin должен быть неотрицательным целым числом", -1);
        return false;
    }

    BarcodeOptions opts;
    opts.ModuleSize = static_cast<std::size_t>(moduleSize);
    opts.Margin     = static_cast<std::size_t>(margin);

    // Определяем тип баркода по номеру метода.
    BarcodeType type{};
    switch (lMethodNum) {
        case eMethGenerateQR:         type = BarcodeType::QR;         break;
        case eMethGenerateDataMatrix: type = BarcodeType::DataMatrix;  break;
        case eMethGenerateAztec:      type = BarcodeType::Aztec;       break;
        default:
            return false;
    }

    try {
        std::string svg = m_service.Generate(data, type, opts);
        return returnWString(pvarRetValue, svg);
    }
    catch (const std::exception& e) {
        // Конвертируем сообщение об ошибке в wchar_t для передачи в 1С.
        std::string msg(e.what());
        std::wstring wmsg(msg.begin(), msg.end());
        addError(ADDIN_E_VERY_IMPORTANT, L"BarcodeGenerator", wmsg.c_str(), -1);
        return false;
    }
}

// ── LocaleBase / UserLanguageBase ─────────────────────────────────────────────

void ComponentInterface::SetLocale(const WCHAR_T*) {}

void ComponentInterface::SetUserInterfaceLanguageCode(const WCHAR_T*) {}

// ─────────────────────────────────────────────────────────────────────────────
// Приватные вспомогательные методы
// ─────────────────────────────────────────────────────────────────────────────

long ComponentInterface::findName(const wchar_t* names[],
                                   const wchar_t* name,
                                   uint32_t size) const {
    for (uint32_t i = 0; i < size; ++i)
        if (!::wcscmp(names[i], name))
            return (long)i;
    return -1;
}

void ComponentInterface::addError(uint32_t wcode, const wchar_t* source,
                                   const wchar_t* descriptor, long code) {
    if (!m_iConnect)
        return;
    WCHAR_T* wsSource = nullptr;
    WCHAR_T* wsDescr  = nullptr;
    convToShortWchar(&wsSource, source);
    convToShortWchar(&wsDescr,  descriptor);
    m_iConnect->AddError(wcode, wsSource, wsDescr, code);
    delete[] wsSource;
    delete[] wsDescr;
}

bool ComponentInterface::returnWString(tVariant* pvarRetValue, const std::string& utf8str) {
    if (!m_iMemory)
        return false;

    // SVG — ASCII-совместим, поэтому прямое расширение до UTF-16 корректно.
    std::u16string u16(utf8str.begin(), utf8str.end());
    size_t len = u16.size();

    if (!m_iMemory->AllocMemory((void**)&pvarRetValue->pwstrVal,
                                 (unsigned)(len + 1) * sizeof(WCHAR_T)))
        return false;

    ::memcpy(pvarRetValue->pwstrVal, u16.data(), len * sizeof(WCHAR_T));
    pvarRetValue->wstrLen = (unsigned)len;
    TV_VT(pvarRetValue) = VTYPE_PWSTR;
    return true;
}

std::string ComponentInterface::extractString(tVariant* param) const {
    if (TV_VT(param) != VTYPE_PWSTR || !param->pwstrVal)
        return {};

    wchar_t* wstr = nullptr;
    convFromShortWchar(&wstr, param->pwstrVal);
    std::wstring ws(wstr);
    delete[] wstr;

    // Конвертируем wstring → string (UTF-8 для многобайтных данных).
    // Для данных штрихкодов (ASCII, цифры, латиница) этого достаточно.
    // Для полноценного UTF-8 замените на wcstombs_s или std::codecvt.
    return std::string(ws.begin(), ws.end());
}

bool ComponentInterface::extractInt(tVariant* param, int& out) const {
    switch (TV_VT(param)) {
        case VTYPE_I4:  out = TV_I4(param);          return true;
        case VTYPE_UI4: out = (int)TV_UI4(param);    return true;
        case VTYPE_I2:  out = TV_I2(param);           return true;
        case VTYPE_R8:  out = (int)TV_R8(param);      return true;
        default:        return false;
    }
}