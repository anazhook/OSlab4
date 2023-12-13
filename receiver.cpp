#include <iostream>
#include <windows.h>
#include <fstream>
#include <conio.h>
#include <string>

int main() {
    char bin[10]; int size, n;
    std::cout << "binary file name, number of notes and quantity of sender processes: "; 
    std::cin >> bin >> size >> n;
    std::ifstream is;
    std::ofstream os(bin, std::ios::binary | std::ios::out | std::ios::trunc);
    os.close();
    HANDLE hSemaphore1;
    HANDLE hSemaphore2;
    HANDLE sema;
    HANDLE* hEvent_ = new HANDLE[n];

    STARTUPINFO* si = new STARTUPINFO[n];
    PROCESS_INFORMATION* pi = new PROCESS_INFORMATION[n];

    hSemaphore1 = CreateSemaphore(NULL, 0, size, L"sema1");
    hSemaphore2 = CreateSemaphore(NULL, size, size, L"sema2");
    sema = CreateSemaphore(NULL, 0, n, L"sema");

    char for_senders[100];
    for (int i = 0; i < n; i++) {
        wsprintfA(for_senders, "Sender.exe %s %d %d", bin, i, size);
        hEvent_[i] = CreateEvent(NULL, TRUE, FALSE, std::to_wstring(i).c_str());
        ZeroMemory(&si[i], sizeof(STARTUPINFO));
        si[i].cb = sizeof(STARTUPINFO);
        ZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));
        CreateProcessA(nullptr, for_senders, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, (LPSTARTUPINFOA)&si[i], &pi[i]);
    }
    WaitForMultipleObjects(n, hEvent_, TRUE, INFINITE);

    int number, size_;
    char message[20];
    char* str;
    while (true) {
        std::cout << "enter '1' to read a message or '2' to interrupt work: "; 
        std::cin >> number;
        if (number == 1) {
            is.open(bin, std::ios::binary | std::ios::in);
            is.seekg(0, std::ios::end);
            size_ = is.tellg();
            if (!size_) {
                is.close();
                if (!ReleaseSemaphore(sema, 1, NULL)) {
                    CloseHandle(hSemaphore1);
                    CloseHandle(hSemaphore2);
                    CloseHandle(sema);
                    CloseHandle(*hEvent_);
                    CloseHandle((*pi).hThread);
                    CloseHandle((*pi).hProcess);
                    return 0;
                }
                WaitForSingleObject(sema, INFINITE);
                WaitForSingleObject(hSemaphore1, INFINITE);
                ReleaseSemaphore(hSemaphore1, 1, NULL);
                continue;
            }
            is.seekg(0, std::ios::beg);
            is.read(reinterpret_cast<char*>(&message), sizeof(char[20]));
            std::cout << message << "\n";

            str = new char[size_ - 20];
            is.read(str, size_ - 20);
            is.close();
            os.open(bin, std::ios::binary | std::ios::out);
            os.write(str, size_ - 20);
            os.close();
            delete str;

            WaitForSingleObject(hSemaphore1, INFINITE);
            ReleaseSemaphore(hSemaphore2, 1, NULL);
        }
        else if (number == 2) {
            ReleaseSemaphore(sema, 1, NULL);
            CloseHandle(*hEvent_);
            CloseHandle(hSemaphore1);
            CloseHandle(hSemaphore2);
            CloseHandle(sema);
            CloseHandle((*pi).hThread);
            CloseHandle((*pi).hProcess);
            return 0;
        }
    }
}