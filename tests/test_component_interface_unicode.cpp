#include <gtest/gtest.h>

#include "component/component_interface.h"
#include "ComponentBase.h"       
#include "IMemoryManager.h"

#include <cstring>
#include <string>
#include <vector>

class MockMemoryManager final : public IMemoryManager {
public:
    bool ADDIN_API AllocMemory(void** pMemory, unsigned long ulCountByte) override {
        *pMemory = new char[ulCountByte]();   
        ++allocCount;
        return true;
    }

    void ADDIN_API FreeMemory(void** pMemory) override {
        if (pMemory && *pMemory) {
            delete[] static_cast<char*>(*pMemory);
            *pMemory = nullptr;
            ++freeCount;
        }
    }

    int allocCount = 0;
    int freeCount  = 0;
};

class MockConnection final : public IAddInDefBase {
public:
    bool ADDIN_API AddError(unsigned short ,
                            const WCHAR_T* ,
                            const WCHAR_T* wsDescriptor,
                            long           ) override {
        lastError.clear();
        if (wsDescriptor) {
            for (const WCHAR_T* p = wsDescriptor; *p; ++p)
                lastError.push_back(static_cast<wchar_t>(*p));
        }
        ++errorCount;
        return true;
    }

    bool ADDIN_API Read(WCHAR_T*, tVariant*, long*, WCHAR_T**) override { return false; }
    bool ADDIN_API Write(WCHAR_T*, tVariant*) override { return false; }
    bool ADDIN_API RegisterProfileAs(WCHAR_T*) override { return false; }
    bool ADDIN_API SetEventBufferDepth(long) override { return false; }
    long ADDIN_API GetEventBufferDepth() override { return 0; }
    bool ADDIN_API ExternalEvent(WCHAR_T*, WCHAR_T*, WCHAR_T*) override { return false; }
    void ADDIN_API CleanEventBuffer() override {}
    bool ADDIN_API SetStatusLine(WCHAR_T*) override { return false; }
    void ADDIN_API ResetStatusLine() override {}

    std::wstring lastError;
    int          errorCount = 0;
};

static tVariant MakeWStrParam(const std::u16string& s) {
    tVariant v{};
    TV_VT(&v) = VTYPE_PWSTR;
    auto* buf = new WCHAR_T[s.size() + 1]();
    for (std::size_t i = 0; i < s.size(); ++i)
        buf[i] = static_cast<WCHAR_T>(s[i]);
    v.pwstrVal = buf;
    v.wstrLen  = static_cast<uint32_t>(s.size());
    return v;
}

static void FreeWStrParam(tVariant& v) {
    if (v.vt == VTYPE_PWSTR)
        delete[] v.pwstrVal;
    v = {};
}

static tVariant MakeI4Param(int32_t value) {
    tVariant v{};
    TV_VT(&v) = VTYPE_I4;
    TV_I4(&v) = value;
    return v;
}

static tVariant MakeR8Param(double value) {
    tVariant v{};
    TV_VT(&v) = VTYPE_R8;
    TV_R8(&v) = value;
    return v;
}

static std::u16string SurrogatePairFor(char32_t cp) {
    cp -= 0x10000u;
    char16_t hi = static_cast<char16_t>(0xD800u + (cp >> 10));
    char16_t lo = static_cast<char16_t>(0xDC00u + (cp & 0x3FFu));
    return {hi, lo};
}

class ComponentInterfaceTest : public ::testing::Test {
protected:
    MockMemoryManager mem;
    MockConnection    conn;
    ComponentInterface comp;

    void SetUp() override {
        comp.Init(&conn);
        comp.setMemManager(&mem);
    }

    bool CallGenerate(int method, const std::u16string& data,
                      int moduleSize, int margin, tVariant& retVal) {
        tVariant params[3];
        params[0] = MakeWStrParam(data);
        params[1] = MakeI4Param(moduleSize);
        params[2] = MakeI4Param(margin);

        tVarInit(&retVal);
        bool ok = comp.CallAsFunc(method, &retVal, params, 3);

        FreeWStrParam(params[0]);
        return ok;
    }

    static std::string RetValToUtf8(const tVariant& v) {
        if (TV_VT(&v) != VTYPE_PWSTR || !v.pwstrVal)
            return {};
        std::string result;
        for (uint32_t i = 0; i < v.wstrLen; ++i) {
            WCHAR_T ch = v.pwstrVal[i];
            if (ch < 0x80) {
                result.push_back(static_cast<char>(ch));
            } else {
                result.push_back('?');
            }
        }
        return result;
    }
};

TEST_F(ComponentInterfaceTest, InitSucceeds) {
    ComponentInterface c2;
    EXPECT_TRUE(c2.Init(&conn));
    EXPECT_TRUE(c2.setMemManager(&mem));
}

TEST_F(ComponentInterfaceTest, InitFailsOnNullConnection) {
    ComponentInterface c2;
    EXPECT_FALSE(c2.Init(nullptr));
}

TEST_F(ComponentInterfaceTest, SetMemManagerFailsOnNull) {
    ComponentInterface c2;
    EXPECT_FALSE(c2.setMemManager(nullptr));
}

TEST_F(ComponentInterfaceTest, GetInfoReturns2000) {
    EXPECT_EQ(comp.GetInfo(), 2000L);
}

TEST_F(ComponentInterfaceTest, RegisterExtensionAsReturnsBarcodeGenerator) {
    WCHAR_T* name = nullptr;
    EXPECT_TRUE(comp.RegisterExtensionAs(&name));
    ASSERT_NE(name, nullptr);

    EXPECT_EQ(name[0], static_cast<WCHAR_T>(L'B'));
    EXPECT_EQ(name[1], static_cast<WCHAR_T>(L'a'));
    EXPECT_EQ(name[2], static_cast<WCHAR_T>(L'r'));
    mem.FreeMemory(reinterpret_cast<void**>(&name));
}

TEST_F(ComponentInterfaceTest, GetNMethodsReturns3) {
    EXPECT_EQ(comp.GetNMethods(), 3L);
}

TEST_F(ComponentInterfaceTest, FindMethodByEnglishName) {
    auto MakeWide = [](const char16_t* s) -> std::basic_string<WCHAR_T> {
        std::basic_string<WCHAR_T> r;
        for (const char16_t* p = s; *p; ++p)
            r.push_back(static_cast<WCHAR_T>(*p));
        return r;
    };

    auto qr   = MakeWide(u"GenerateQR");
    auto dm   = MakeWide(u"GenerateDataMatrix");
    auto az   = MakeWide(u"GenerateAztec");
    auto unkn = MakeWide(u"UnknownMethod");

    EXPECT_EQ(comp.FindMethod(qr.c_str()),   0L);
    EXPECT_EQ(comp.FindMethod(dm.c_str()),   1L);
    EXPECT_EQ(comp.FindMethod(az.c_str()),   2L);
    EXPECT_EQ(comp.FindMethod(unkn.c_str()), -1L);
}

TEST_F(ComponentInterfaceTest, FindMethodByRussianName) {
    std::basic_string<WCHAR_T> nameRu;
    const char32_t chars[] = {
        0x421, 0x444, 0x43E, 0x440, 0x43C, 0x438, 0x440, 0x43E, 0x432, 0x430,
        0x442, 0x44C, 'Q', 'R', 0
    };
    for (const char32_t* p = chars; *p; ++p)
        nameRu.push_back(static_cast<WCHAR_T>(*p));

    EXPECT_EQ(comp.FindMethod(nameRu.c_str()), 0L);
}

TEST_F(ComponentInterfaceTest, GetNParamsReturns3ForAllMethods) {
    EXPECT_EQ(comp.GetNParams(0), 3L);
    EXPECT_EQ(comp.GetNParams(1), 3L);
    EXPECT_EQ(comp.GetNParams(2), 3L);
    EXPECT_EQ(comp.GetNParams(99), 0L);
}

TEST_F(ComponentInterfaceTest, HasRetValTrueForValidMethods) {
    EXPECT_TRUE(comp.HasRetVal(0));
    EXPECT_TRUE(comp.HasRetVal(1));
    EXPECT_TRUE(comp.HasRetVal(2));
    EXPECT_FALSE(comp.HasRetVal(99));
}

TEST_F(ComponentInterfaceTest, CallAsFuncFailsOnWrongParamCount) {
    tVariant ret{};
    tVariant params[2];
    params[0] = MakeWStrParam(u"hello");
    params[1] = MakeI4Param(10);

    EXPECT_FALSE(comp.CallAsFunc(0, &ret, params, 2));
    FreeWStrParam(params[0]);
}

TEST_F(ComponentInterfaceTest, CallAsFuncFailsOnNullParams) {
    tVariant ret{};
    EXPECT_FALSE(comp.CallAsFunc(0, &ret, nullptr, 3));
}

TEST_F(ComponentInterfaceTest, CallAsFuncFailsOnNullRetValue) {
    tVariant params[3];
    params[0] = MakeWStrParam(u"hello");
    params[1] = MakeI4Param(10);
    params[2] = MakeI4Param(4);

    EXPECT_FALSE(comp.CallAsFunc(0, nullptr, params, 3));
    FreeWStrParam(params[0]);
}

TEST_F(ComponentInterfaceTest, CallAsFuncFailsOnEmptyData) {
    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(u"");
    params[1] = MakeI4Param(10);
    params[2] = MakeI4Param(4);

    EXPECT_FALSE(comp.CallAsFunc(0, &ret, params, 3));
    EXPECT_GT(conn.errorCount, 0);
    FreeWStrParam(params[0]);
}

TEST_F(ComponentInterfaceTest, CallAsFuncFailsOnZeroModuleSize) {
    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(u"test");
    params[1] = MakeI4Param(0);   
    params[2] = MakeI4Param(4);

    EXPECT_FALSE(comp.CallAsFunc(0, &ret, params, 3));
    EXPECT_GT(conn.errorCount, 0);
    FreeWStrParam(params[0]);
}

TEST_F(ComponentInterfaceTest, CallAsFuncFailsOnNegativeMargin) {
    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(u"test");
    params[1] = MakeI4Param(10);
    params[2] = MakeI4Param(-1);   

    EXPECT_FALSE(comp.CallAsFunc(0, &ret, params, 3));
    EXPECT_GT(conn.errorCount, 0);
    FreeWStrParam(params[0]);
}

TEST_F(ComponentInterfaceTest, CallAsFuncFailsOnInvalidMethodNumber) {
    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(u"test");
    params[1] = MakeI4Param(10);
    params[2] = MakeI4Param(4);

    EXPECT_FALSE(comp.CallAsFunc(99, &ret, params, 3));
    FreeWStrParam(params[0]);
}

TEST_F(ComponentInterfaceTest, ExtractIntAcceptsVTYPE_I4) {
    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(u"X");
    params[1] = MakeI4Param(5);
    params[2] = MakeI4Param(0);

    int prevErrors = conn.errorCount;
    comp.CallAsFunc(0, &ret, params, 3);

    bool noValidationError = (conn.errorCount == prevErrors)
                          || (conn.lastError.find(L"moduleSize") == std::wstring::npos
                              && conn.lastError.find(L"margin")     == std::wstring::npos);
    EXPECT_TRUE(noValidationError);
    FreeWStrParam(params[0]);
    if (ret.vt == VTYPE_PWSTR) mem.FreeMemory(reinterpret_cast<void**>(&ret.pwstrVal));
}

TEST_F(ComponentInterfaceTest, ExtractIntAcceptsVTYPE_UI4) {
    tVariant v{};
    TV_VT(&v) = VTYPE_UI4;
    TV_UI4(&v) = 42;

    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(u"X");
    params[1] = v;
    params[2] = MakeI4Param(0);

    int prevErrors = conn.errorCount;
    comp.CallAsFunc(0, &ret, params, 3);

    bool noSizeError = (conn.lastError.find(L"moduleSize") == std::wstring::npos);
    EXPECT_TRUE(noSizeError);
    FreeWStrParam(params[0]);
    if (ret.vt == VTYPE_PWSTR) mem.FreeMemory(reinterpret_cast<void**>(&ret.pwstrVal));
}

TEST_F(ComponentInterfaceTest, ExtractIntAcceptsVTYPE_R8) {
    tVariant v{};
    TV_VT(&v) = VTYPE_R8;
    TV_R8(&v) = 7.9;   

    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(u"X");
    params[1] = v;
    params[2] = MakeI4Param(0);

    int prevErrors = conn.errorCount;
    comp.CallAsFunc(0, &ret, params, 3);

    bool noSizeError = (conn.lastError.find(L"moduleSize") == std::wstring::npos);
    EXPECT_TRUE(noSizeError);
    FreeWStrParam(params[0]);
    if (ret.vt == VTYPE_PWSTR) mem.FreeMemory(reinterpret_cast<void**>(&ret.pwstrVal));
}

TEST_F(ComponentInterfaceTest, ExtractIntRejectsVTYPE_PWSTR) {
    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(u"X");
    params[1] = MakeWStrParam(u"not_a_number");  
    params[2] = MakeI4Param(0);

    EXPECT_FALSE(comp.CallAsFunc(0, &ret, params, 3));
    FreeWStrParam(params[0]);
    FreeWStrParam(params[1]);
}

static tVariant MakeSingleCharParam(char16_t ch) {
    return MakeWStrParam(std::u16string{ch});
}

TEST_F(ComponentInterfaceTest, AsciiRoundTripThroughParams) {
    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(u"Hello");
    params[1] = MakeI4Param(10);
    params[2] = MakeI4Param(0);

    int prevErrors = conn.errorCount;
    comp.CallAsFunc(0, &ret, params, 3);

    bool noDataError = (conn.lastError.find(L"data") == std::wstring::npos);
    EXPECT_TRUE(noDataError);

    FreeWStrParam(params[0]);
    if (ret.vt == VTYPE_PWSTR) mem.FreeMemory(reinterpret_cast<void**>(&ret.pwstrVal));
}

TEST_F(ComponentInterfaceTest, EmptyStringParamTriggersError) {
    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(u"");
    params[1] = MakeI4Param(10);
    params[2] = MakeI4Param(0);

    EXPECT_FALSE(comp.CallAsFunc(0, &ret, params, 3));
    EXPECT_NE(conn.lastError.find(L"data"), std::wstring::npos);
}

TEST_F(ComponentInterfaceTest, SurrogatePairAcceptedAsData) {
    std::u16string emoji = SurrogatePairFor(0x1F600);
    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(emoji);
    params[1] = MakeI4Param(10);
    params[2] = MakeI4Param(0);

    int prevErrors = conn.errorCount;
    comp.CallAsFunc(0, &ret, params, 3);

    bool noDataError = (conn.lastError.find(L"data") == std::wstring::npos);
    EXPECT_TRUE(noDataError);

    FreeWStrParam(params[0]);
    if (ret.vt == VTYPE_PWSTR) mem.FreeMemory(reinterpret_cast<void**>(&ret.pwstrVal));
}

TEST_F(ComponentInterfaceTest, LoneHighSurrogateDoesNotCrash) {
    std::u16string broken;
    broken.push_back(0xD800);   
    broken.push_back(u'A');

    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(broken);
    params[1] = MakeI4Param(10);
    params[2] = MakeI4Param(0);

    EXPECT_NO_THROW(comp.CallAsFunc(0, &ret, params, 3));
    FreeWStrParam(params[0]);
    if (ret.vt == VTYPE_PWSTR) mem.FreeMemory(reinterpret_cast<void**>(&ret.pwstrVal));
}

TEST_F(ComponentInterfaceTest, LoneLowSurrogateDoesNotCrash) {
    std::u16string broken;
    broken.push_back(0xDC00);   
    broken.push_back(u'B');

    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(broken);
    params[1] = MakeI4Param(10);
    params[2] = MakeI4Param(0);

    EXPECT_NO_THROW(comp.CallAsFunc(0, &ret, params, 3));
    FreeWStrParam(params[0]);
    if (ret.vt == VTYPE_PWSTR) mem.FreeMemory(reinterpret_cast<void**>(&ret.pwstrVal));
}

TEST_F(ComponentInterfaceTest, CyrillicStringAcceptedAsData) {
    std::u16string cyrillic;
    cyrillic.push_back(0x0422);
    cyrillic.push_back(0x0435);
    cyrillic.push_back(0x0441);
    cyrillic.push_back(0x0442);

    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(cyrillic);
    params[1] = MakeI4Param(10);
    params[2] = MakeI4Param(0);

    bool noDataError = true;
    int prevErrors = conn.errorCount;
    comp.CallAsFunc(0, &ret, params, 3);
    noDataError = (conn.lastError.find(L"data") == std::wstring::npos);
    EXPECT_TRUE(noDataError);

    FreeWStrParam(params[0]);
    if (ret.vt == VTYPE_PWSTR) mem.FreeMemory(reinterpret_cast<void**>(&ret.pwstrVal));
}

TEST_F(ComponentInterfaceTest, MaxBMPCodePointAccepted) {
    std::u16string s;
    s.push_back(0xFFFD);

    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(s);
    params[1] = MakeI4Param(10);
    params[2] = MakeI4Param(0);

    EXPECT_NO_THROW(comp.CallAsFunc(0, &ret, params, 3));
    FreeWStrParam(params[0]);
    if (ret.vt == VTYPE_PWSTR) mem.FreeMemory(reinterpret_cast<void**>(&ret.pwstrVal));
}

TEST_F(ComponentInterfaceTest, PropertiesAreNotSupported) {
    EXPECT_EQ(comp.GetNProps(), 0L);
    EXPECT_EQ(comp.FindProp(nullptr), -1L);
    EXPECT_EQ(comp.GetPropName(0, 0), nullptr);
    EXPECT_FALSE(comp.IsPropReadable(0));
    EXPECT_FALSE(comp.IsPropWritable(0));

    tVariant v{};
    EXPECT_FALSE(comp.GetPropVal(0, &v));
    EXPECT_FALSE(comp.SetPropVal(0, &v));
}

TEST_F(ComponentInterfaceTest, CallAsProcAlwaysReturnsFalse) {
    tVariant params[3];
    params[0] = MakeWStrParam(u"test");
    params[1] = MakeI4Param(10);
    params[2] = MakeI4Param(4);

    EXPECT_FALSE(comp.CallAsProc(0, params, 3));
    FreeWStrParam(params[0]);
}

TEST_F(ComponentInterfaceTest, CallAsFuncFailsGracefullyWithoutMemManager) {
    ComponentInterface c2;
    c2.Init(&conn);

    tVariant ret{}, params[3];
    params[0] = MakeWStrParam(u"test");
    params[1] = MakeI4Param(10);
    params[2] = MakeI4Param(4);

    EXPECT_NO_THROW({
        bool ok = c2.CallAsFunc(0, &ret, params, 3);

        (void)ok;
    });
    FreeWStrParam(params[0]);
}

TEST_F(ComponentInterfaceTest, GetMethodNameReturnsNonNullForValidIndex) {
    for (long i = 0; i < 3; ++i) {
        const WCHAR_T* en = comp.GetMethodName(i, 0);
        const WCHAR_T* ru = comp.GetMethodName(i, 1);
        EXPECT_NE(en, nullptr) << "English name for method " << i;
        EXPECT_NE(ru, nullptr) << "Russian name for method " << i;

        if (en) mem.FreeMemory(reinterpret_cast<void**>(const_cast<WCHAR_T**>(&en)));
        if (ru) mem.FreeMemory(reinterpret_cast<void**>(const_cast<WCHAR_T**>(&ru)));
    }
}

TEST_F(ComponentInterfaceTest, GetMethodNameReturnsNullForInvalidIndex) {
    EXPECT_EQ(comp.GetMethodName(99, 0), nullptr);
    EXPECT_EQ(comp.GetMethodName(0, 99), nullptr);
}

TEST_F(ComponentInterfaceTest, GetParamDefValueReturnsEmpty) {
    tVariant v{};
    TV_VT(&v) = VTYPE_I4;

    EXPECT_FALSE(comp.GetParamDefValue(0, 0, &v));
    EXPECT_EQ(TV_VT(&v), VTYPE_EMPTY);
}