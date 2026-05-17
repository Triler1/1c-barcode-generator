#pragma once

#include "ComponentBase.h"
#include "AddInDefBase.h"
#include "IMemoryManager.h"

#include "services/barcode_service.h"

#include <string>
#include <cstdint>

// Имя компоненты, под которым компонента регистрируется в 1С:
//   ПодключитьВнешнююКомпоненту(...);
//   НовыйОбъект("AddIn.BarcodeGenerator.BarcodeGenerator");
static const WCHAR_T g_kClassName[] = u"BarcodeGenerator";

class ComponentInterface : public IComponentBase {
public:
    // Порядок элементов должен совпадать с g_MethodNames / g_MethodNamesRu.
    enum Methods {
        eMethGenerateQR = 0,
        eMethGenerateDataMatrix,
        eMethGenerateAztec,
        eMethLast  // всегда последний — используется как счётчик
    };

    ComponentInterface();
    virtual ~ComponentInterface() = default;

    // ── IInitDoneBase ────────────────────────────────────────────────────────
    virtual bool ADDIN_API Init(void* pConnection) override;
    virtual bool ADDIN_API setMemManager(void* mem) override;
    virtual long ADDIN_API GetInfo() override;
    virtual void ADDIN_API Done() override;

    // ── ILanguageExtenderBase ────────────────────────────────────────────────
    virtual bool        ADDIN_API RegisterExtensionAs(WCHAR_T** wsExtName) override;

    // Свойства (не используются — возвращаем 0 / nullptr / false)
    virtual long        ADDIN_API GetNProps() override;
    virtual long        ADDIN_API FindProp(const WCHAR_T* wsPropName) override;
    virtual const WCHAR_T* ADDIN_API GetPropName(long lPropNum, long lPropAlias) override;
    virtual bool        ADDIN_API GetPropVal(const long lPropNum, tVariant* pvarPropVal) override;
    virtual bool        ADDIN_API SetPropVal(const long lPropNum, tVariant* varPropVal) override;
    virtual bool        ADDIN_API IsPropReadable(const long lPropNum) override;
    virtual bool        ADDIN_API IsPropWritable(const long lPropNum) override;

    // Методы
    virtual long        ADDIN_API GetNMethods() override;
    virtual long        ADDIN_API FindMethod(const WCHAR_T* wsMethodName) override;
    virtual const WCHAR_T* ADDIN_API GetMethodName(const long lMethodNum,
                                                     const long lMethodAlias) override;
    virtual long        ADDIN_API GetNParams(const long lMethodNum) override;
    virtual bool        ADDIN_API GetParamDefValue(const long lMethodNum,
                                                    const long lParamNum,
                                                    tVariant* pvarParamDefValue) override;
    virtual bool        ADDIN_API HasRetVal(const long lMethodNum) override;
    virtual bool        ADDIN_API CallAsProc(const long lMethodNum,
                                              tVariant* paParams,
                                              const long lSizeArray) override;
    virtual bool        ADDIN_API CallAsFunc(const long lMethodNum,
                                              tVariant* pvarRetValue,
                                              tVariant* paParams,
                                              const long lSizeArray) override;

    // ── LocaleBase / UserLanguageBase ────────────────────────────────────────
    virtual void ADDIN_API SetLocale(const WCHAR_T* loc) override;
    virtual void ADDIN_API SetUserInterfaceLanguageCode(const WCHAR_T* lang) override;

private:
    // Поиск имени в массиве wchar_t*-строк.
    long findName(const wchar_t* names[], const wchar_t* name, uint32_t size) const;

    // Передать ошибку в 1С.
    void addError(uint32_t wcode, const wchar_t* source,
                  const wchar_t* descriptor, long code);

    // Выделить память через IMemoryManager и вернуть UTF-16 строку в tVariant.
    // Используется для возврата SVG из CallAsFunc.
    bool returnWString(tVariant* pvarRetValue, const std::string& utf8str);

    // Извлечь строку из параметра 1С (VTYPE_PWSTR) в std::string.
    std::string extractString(tVariant* param) const;

    // Извлечь целое число из параметра 1С (VTYPE_I4 / VTYPE_UI4).
    // Возвращает false, если тип не числовой.
    bool extractInt(tVariant* param, int& out) const;

    IAddInDefBase*  m_iConnect = nullptr;
    IMemoryManager* m_iMemory  = nullptr;
    BarcodeService  m_service;
};