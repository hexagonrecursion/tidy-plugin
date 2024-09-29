#include <string>
#include <filesystem>

void testDeclVar() {
    std::string s = "s";
    const std::string cs = "s";
    std::string &rs = s;
    const std::string &rcs = s;

    std::filesystem::path p = s;
    // CHECK: :[[# @LINE - 1 ]]:31: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    const std::filesystem::path cp = s;
    // CHECK: :[[# @LINE - 1 ]]:38: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    std::filesystem::path pc = cs;
    // CHECK: :[[# @LINE - 1 ]]:32: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    std::filesystem::path pp(s);
    // CHECK: :[[# @LINE - 1 ]]:30: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    std::filesystem::path pb{s};
    // CHECK: :[[# @LINE - 1 ]]:30: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    std::filesystem::path ap[] = {s, s};
    // CHECK: :[[# @LINE - 1 ]]:35: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    // CHECK: :[[# @LINE - 2 ]]:38: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    std::filesystem::path pr = rs;
    // CHECK: :[[# @LINE - 1 ]]:32: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    std::filesystem::path prc = rcs;
    // CHECK: :[[# @LINE - 1 ]]:33: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]

    std::string &&rrs = std::move(s);
    std::filesystem::path prr = rrs;
    // CHECK: :[[# @LINE - 1 ]]:33: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    s = "s";
    const std::string &&rrcs = std::move(s);
    std::filesystem::path prrc = rrcs;
    // CHECK: :[[# @LINE - 1 ]]:34: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
}

void callMe(std::filesystem::path p) {}

void testCall() {
    std::string s = "s";
    callMe(s);
    // CHECK: :[[# @LINE - 1 ]]:12: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
}

std::filesystem::path testRet() {
    std::string s = "s";
    return s;
    // CHECK: :[[# @LINE - 1 ]]:12: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
}

void testOperators() {
    std::string s = "s";
    std::filesystem::path p = "p";
    p / s;
    // CHECK: :[[# @LINE - 1 ]]:9: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    s / p;
    // CHECK: :[[# @LINE - 1 ]]:5: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    s == p;
    // CHECK: :[[# @LINE - 1 ]]:5: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    s != p;
    // CHECK: :[[# @LINE - 1 ]]:5: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    s < p;
    // CHECK: :[[# @LINE - 1 ]]:5: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    s <= p;
    // CHECK: :[[# @LINE - 1 ]]:5: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    s > p;
    // CHECK: :[[# @LINE - 1 ]]:5: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    s >= p;
    // CHECK: :[[# @LINE - 1 ]]:5: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    s <=> p;
    // CHECK: :[[# @LINE - 1 ]]:5: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
}

void testReplace() {
    std::string s = "s";
    std::filesystem::path p = "p";
    p.replace_filename(s);
    // CHECK: :[[# @LINE - 1 ]]:24: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
    p.replace_extension(s);
    // CHECK: :[[# @LINE - 1 ]]:25: warning: std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath() [colobot-str2path]
}

// TODO:
// - string view
// - std::filesystem::path::compare()
// - operator= and std::filesystem::path::assign()
// - std::filesystem::path::append() and std::filesystem::path::operator/=
// - std::filesystem::path::concat() and std::filesystem::path::operator+=
