void RegisterProgram();
BOOL RegisterMyProgramForStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args);
BOOL IsMyProgramRegisteredForStartup(PCWSTR pszAppName);
void RegisterIfDoesntExist(PCWSTR pszAppName);