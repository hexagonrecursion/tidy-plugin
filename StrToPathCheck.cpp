#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

class StringToPathCheck : public ClangTidyCheck
{
public:
    StringToPathCheck(StringRef Name, ClangTidyContext* Context) : ClangTidyCheck(Name, Context)
    {
    }
    void registerMatchers(ast_matchers::MatchFinder* Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult& Result) override;
};

void StringToPathCheck::registerMatchers(MatchFinder* Finder)
{
    auto hasTypeStr = hasType(type(hasUnqualifiedDesugaredType(recordType(hasDeclaration(recordDecl(anyOf(hasName("::std::basic_string"), hasName("::std::basic_string_view"))))))));
    auto hasTypePath = hasType(type(hasUnqualifiedDesugaredType(recordType(hasDeclaration(recordDecl(hasName("::std::filesystem::path")))))));
    Finder->addMatcher(
        cxxConstructExpr(
            hasTypePath,
            hasArgument(0, expr(hasTypeStr).bind("str"))
        ),
        this
    );
    Finder->addMatcher(
        cxxMemberCallExpr(
            on(expr(hasTypePath)),
            callee(namedDecl(anyOf(
                    hasName("compare"),
                    hasName("assign"),
                    hasName("append"),
                    hasName("concat")
            ))),
            hasArgument(0, expr(hasTypeStr).bind("str"))
        ),
        this
    );
    Finder->addMatcher(binaryOperation(isAssignmentOperator(), hasLHS(expr(hasTypePath)), hasRHS(expr(hasTypeStr).bind("str"))), this);
}

void StringToPathCheck::check(const MatchFinder::MatchResult& Result)
{
    const auto *Str = Result.Nodes.getNodeAs<Expr>("str");
    if(!Str) return;
    const SourceManager &SM = Result.Context->getSourceManager();
    if(SM.isInSystemHeader(Str->getBeginLoc())) return;
    diag(Str->getBeginLoc(), "std::string should not be converted to std::filesystem::path without using TempToPath() or StrUtils::ToPath()");
}

namespace {

class StrToPathModule : public ClangTidyModule
{
public:
    void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<StringToPathCheck>("colobot-str2path");
    }
};

}  // namespace

namespace clang::tidy {

// Register the module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<::StrToPathModule> strToPathModuleInit("colobot-str2path-module", "Adds 'colobot-str2path' checks.");

}  // namespace clang::tidy
