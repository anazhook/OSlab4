#include <fstream>
#include <windows.h>
#include <iostream>
#include <string>
#include <conio.h>

int main(int argc, char* argv[]) {
    std::ofstream os;
    std::ifstream is;
    HANDLE hSemaphore1;
    HANDLE hSemaphore2;
    HANDLE sema;
    HANDLE hEvent_;

    hSemaphore1 = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"sema1");
    hSemaphore2 = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"sema2");
    sema = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"sema");
    hEvent_ = OpenEvent(EVENT_ALL_ACCESS, FALSE, std::to_wstring(atoi(argv[2])).c_str());

    SetEvent(hEvent_);

    int number, size_;
    char message[20];
    while (true) {
        std::cout << "enter '1' to send message or '2' to interrupt work: ";
        std::cin >> number;
        if (number == 1) {
            is.open(argv[1], std::ios::binary | std::ios::in);
            is.seekg(0, std::ios::end);
            size_ = is.tellg();
            is.close();
            if (size_ == atoi(argv[3]) * 20) {
                if (!ReleaseSemaphore(sema, 1, NULL)) {
                    CloseHandle(hSemaphore1);
                    CloseHandle(hSemaphore2);
                    CloseHandle(sema);
                    CloseHandle(hEvent_);
                    ExitProcess(0);
                }
                WaitForSingleObject(sema, INFINITE);
                WaitForSingleObject(hSemaphore2, INFINITE);
                ReleaseSemaphore(hSemaphore2, 1, NULL);
                continue;
            }
            os.open(argv[1], std::ios::binary | std::ios::in | std::ios::app);
            std::cout << "enter message: "; 
            std::cin >> message;
            os.write(reinterpret_cast<const char*>(&message), sizeof(char[20]));
            os.close();

            ReleaseSemaphore(hSemaphore1, 1, NULL);
            WaitForSingleObject(hSemaphore2, INFINITE);
        }
        else if (number == 2) {
            ReleaseSemaphore(sema, 1, NULL);
            CloseHandle(hSemaphore1);
            CloseHandle(hSemaphore2);
            CloseHandle(sema);
            CloseHandle(hEvent_);
            ExitProcess(0);
        }
    }
}