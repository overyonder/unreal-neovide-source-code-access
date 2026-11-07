#include "NeovideSourceCodeAccessor.h"

#include "HAL/PlatformProcess.h"
#include "Internationalization/Internationalization.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "NeovideSourceCodeAccessor"

namespace
{
FString Quote(const FString& Value)
{
	return TEXT("\"") + Value.ReplaceQuotesWithEscapedQuotes() + TEXT("\"");
}

FString ResolveProjectDirectory()
{
	const FString ProjectDir = FPaths::ProjectDir();
	if (ProjectDir.IsEmpty())
	{
		return FString();
	}

	FString FullProjectDir = FPaths::ConvertRelativePathToFull(ProjectDir);
	FPaths::NormalizeDirectoryName(FullProjectDir);
	return FPaths::DirectoryExists(FullProjectDir) ? FullProjectDir : FString();
}
}

void FNeovideSourceCodeAccessor::RefreshAvailability()
{
	FString FoundPath;
	int32 ReturnCode = -1;
	FPlatformProcess::ExecProcess(
		TEXT("/usr/bin/env"),
		TEXT("sh -c \"command -v neovide\""),
		&ReturnCode,
		&FoundPath,
		nullptr);

	ExecutablePath = ReturnCode == 0 ? FoundPath.TrimStartAndEnd() : FString();
}

bool FNeovideSourceCodeAccessor::CanAccessSourceCode() const
{
	return !ExecutablePath.IsEmpty();
}

FName FNeovideSourceCodeAccessor::GetFName() const
{
	return FName(TEXT("Neovide"));
}

FText FNeovideSourceCodeAccessor::GetNameText() const
{
	return LOCTEXT("DisplayName", "Neovide");
}

FText FNeovideSourceCodeAccessor::GetDescriptionText() const
{
	return LOCTEXT("DisplayDescription", "Open source code files in Neovide");
}

bool FNeovideSourceCodeAccessor::OpenSolution()
{
	return OpenSolutionAtPath(FPaths::ProjectDir());
}

bool FNeovideSourceCodeAccessor::OpenSolutionAtPath(const FString& InSolutionPath)
{
	FString Directory = FPaths::DirectoryExists(InSolutionPath)
		? InSolutionPath
		: FPaths::GetPath(InSolutionPath);
	Directory = FPaths::ConvertRelativePathToFull(Directory);
	FPaths::NormalizeDirectoryName(Directory);
	if (Directory.IsEmpty() || !FPaths::DirectoryExists(Directory))
	{
		Directory = ResolveProjectDirectory();
	}

	const FString ChdirArg = Directory.IsEmpty() ? FString() : (TEXT(" --chdir ") + Quote(Directory));
	return Launch(TEXT("--fork") + ChdirArg, Directory);
}

bool FNeovideSourceCodeAccessor::DoesSolutionExist() const
{
	return FPaths::DirectoryExists(FPaths::ProjectDir() / TEXT("Source"));
}

bool FNeovideSourceCodeAccessor::OpenFileAtLine(const FString& FullPath, int32 LineNumber, int32 ColumnNumber)
{
	const int32 ValidLine = FMath::Max(LineNumber, 1);
	const int32 ValidColumn = FMath::Max(ColumnNumber, 1);
	const FString CursorCommand = FString::Printf(TEXT("+call cursor(%d,%d)"), ValidLine, ValidColumn);
	const FString ProjectDir = ResolveProjectDirectory();
	const FString ChdirArg = ProjectDir.IsEmpty() ? FString() : (TEXT(" --chdir ") + Quote(ProjectDir));

	return Launch(TEXT("--fork") + ChdirArg + TEXT(" ") + Quote(FullPath) + TEXT(" -- ") + Quote(CursorCommand), ProjectDir);
}

bool FNeovideSourceCodeAccessor::OpenSourceFiles(const TArray<FString>& AbsoluteSourcePaths)
{
	if (AbsoluteSourcePaths.IsEmpty())
	{
		return false;
	}

	const FString ProjectDir = ResolveProjectDirectory();
	FString Arguments = TEXT("--fork");
	if (!ProjectDir.IsEmpty())
	{
		Arguments += TEXT(" --chdir ") + Quote(ProjectDir);
	}
	for (const FString& SourcePath : AbsoluteSourcePaths)
	{
		Arguments += TEXT(" ") + Quote(SourcePath);
	}

	return Launch(Arguments, ProjectDir);
}

bool FNeovideSourceCodeAccessor::AddSourceFiles(
	const TArray<FString>& AbsoluteSourcePaths,
	const TArray<FString>& AvailableModules)
{
	return true;
}

bool FNeovideSourceCodeAccessor::SaveAllOpenDocuments() const
{
	return true;
}

void FNeovideSourceCodeAccessor::Tick(float DeltaTime)
{
}

bool FNeovideSourceCodeAccessor::Launch(const FString& Arguments, const FString& WorkingDirectory) const
{
	if (!CanAccessSourceCode())
	{
		return false;
	}

	uint32 ProcessId = 0;
	FProcHandle Process = FPlatformProcess::CreateProc(
		*ExecutablePath,
		*Arguments,
		true,
		false,
		false,
		&ProcessId,
		0,
		WorkingDirectory.IsEmpty() ? nullptr : *WorkingDirectory,
		nullptr);

	if (!Process.IsValid())
	{
		return false;
	}

	FPlatformProcess::CloseProc(Process);
	return true;
}

#undef LOCTEXT_NAMESPACE
