#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

class AwesomePrefixCheck : public ClangTidyCheck
{
public:
    AwesomePrefixCheck(StringRef Name, ClangTidyContext* Context) : ClangTidyCheck(Name, Context)
    {
    }
    void registerMatchers(ast_matchers::MatchFinder* Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult& Result) override;
};

void AwesomePrefixCheck::registerMatchers(MatchFinder* Finder)
{
    auto hasTypeStr = hasType(type(hasUnqualifiedDesugaredType(recordType(hasDeclaration(recordDecl(hasName("::std::basic_string")))))));
    auto hasTypePath = hasType(type(hasUnqualifiedDesugaredType(recordType(hasDeclaration(recordDecl(hasName("::std::filesystem::path")))))));
    Finder->addMatcher(cxxConstructExpr(hasTypePath, hasArgument(0, expr(hasTypeStr).bind("str"))), this);
}

void AwesomePrefixCheck::check(const MatchFinder::MatchResult& Result)
{
    const auto *Str = Result.Nodes.getNodeAs<Expr>("str");
    if(!Str) return;
    const SourceManager &SM = Result.Context->getSourceManager();
    if(SM.isInSystemHeader(Str->getBeginLoc())) return;
    diag(Str->getBeginLoc(), "std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath()");
}

namespace {

class AwesomePrefixCheckModule : public ClangTidyModule
{
public:
    void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<AwesomePrefixCheck>("coveo-awesomeprefixcheck");
    }
};

}  // namespace

namespace clang::tidy {

// Register the module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<::AwesomePrefixCheckModule> awesomePrefixCheckInit("coveo-awesomeprefixcheck-module",
                                                                                       "Adds 'coveo-awesomeprefixcheck' checks.");

// This anchor is used to force the linker to link in the generated object file and thus register the module.
volatile int awesomePrefixCheckAnchorSource = 0;

}  // namespace clang::tidy
