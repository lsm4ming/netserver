#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

constexpr const int FAIL_EXIT_CODE = -1;

void startProcess(char *const argv[])
{
    pid_t pid = fork();

    if (pid == -1)
    {
        std::cerr << "Failed to fork process." << std::endl;
        exit(FAIL_EXIT_CODE);
    }

    if (pid == 0)
    {
        if (execvp(argv[0], argv) == -1)
        {
            std::cerr << "Failed to start the target process." << std::endl;
            exit(FAIL_EXIT_CODE);
        }
    }
    else
    {
        std::cout << "Started the target process with PID: " << pid << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <target_command>" << std::endl;
        return FAIL_EXIT_CODE;
    }
    while (true)
    {
        startProcess(argv + 1);

        int status;
        wait(&status);
        if (WIFEXITED(status))
        {
            std::cout << "目标进程已退出，状态为: " << WEXITSTATUS(status) << std::endl;
        }
        else
        {
            std::cerr << "目标进程异常退出." << std::endl;
        }
        sleep(1);
    }
    return 0;
}