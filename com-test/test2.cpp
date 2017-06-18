/* https://msdn.microsoft.com/en-us/library/windows/desktop/ff485839(v=vs.85).aspx */
#define UNICODE
#include <windows.h>
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"WindowsApp.lib")
#pragma comment(lib,"shell32.lib")
#pragma comment(lib,"mincore.lib")
#include <shobjidl.h>
#include <atlbase.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
     HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
				 COINIT_DISABLE_OLE1DDE);
     if (SUCCEEDED(hr))
     {
	  CComPtr<IFileOpenDialog> pFileOpen;

	  // Create the FileOpenDialog object.
	  hr = pFileOpen.CoCreateInstance(__uuidof(FileOpenDialog));
	  if (SUCCEEDED(hr))
	  {
	       // Show the Open dialog box.
	       hr = pFileOpen->Show(NULL);

	       // Get the file name from the dialog box.
	       if (SUCCEEDED(hr))
	       {
		    CComPtr<IShellItem> pItem;
		    hr = pFileOpen->GetResult(&pItem);
		    if (SUCCEEDED(hr))
		    {
			 LPWSTR pszFilePath;
			 hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

			 // Display the file name to the user.
			 if (SUCCEEDED(hr))
			 {
			      MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
			      CoTaskMemFree(pszFilePath);
			 }
		    }
	       }
	  }
	  CoUninitialize();
     }
     return 0;
}
