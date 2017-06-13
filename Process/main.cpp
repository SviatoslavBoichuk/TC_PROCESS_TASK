#include <QCoreApplication>
#include <cctype>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using std::cin;
using std::cout;
using std::endl;

/*[brief@]  Print menu into display
 *[param@]  none
 *[return@] none
 */
void PrintMenu()
{
    cout << "***********Menu***********" << endl;
    cout << "1. Create process fork()  " << endl;
    cout << "2. Create process execl() " << endl;
    cout << "3. Create process system()" << endl;
}

/*[brief@]  Print error message and finish program
 *[param@]  message to print on display
 *[return@] none
 */
void ErrorExit(const char * csz_error_message)
{
    if (csz_error_message != NULL)
    {
        cout << csz_error_message << endl;
    }
    exit(EXIT_FAILURE);
}

/*[brief@]  Create process using fork()
 *[param@]  none
 *[return@] none
 */
void CreateProcessFork()
{
    pid_t child_pid = fork();
    int status;

    if(child_pid == -1)
    {
        ErrorExit("fork() error!");
    }
    else if(child_pid == 0)
    {
        /* nothing to do */
        while (1) ;
    }
    else
    {
        if((child_pid = waitpid(child_pid, &status, 0)) && WIFEXITED(status))
        {
            cout << "Child process PID  = " << child_pid << " finished!" << endl;
            cout << "Return code = " << WEXITSTATUS(status) << endl;
        }

        setsid();
        daemon(1,1);

        // waint
        while (1) ;
    }
}

/*[brief@]  Create process using execl()
 *[param@]  none
 *[return@] none
 */
void CreateProcessExecl()
{
    cout << "Create process by execl()" << endl;

    pid_t Pid = fork();
    int status;

    if(Pid == -1)
    {
        ErrorExit("fork() error!");
    }
    else if(Pid == 0)
    {
        int exitStatus = execl("/bin/ls", "ls", "-A", static_cast<const char*>(NULL));
        cout << "execl() exit status = " << exitStatus << endl;
    }
    else
    {
        if((Pid = waitpid(Pid, &status, 0)) && WIFEXITED(status))
        {
            cout << "Child process PID  = " << Pid << " finished!" << endl;
            cout << "Return code = "        << WEXITSTATUS(status) << endl;
        }
    }
}

/*[brief@]  Create process using system()
 *[param@]  none
 *[return@] none
 */
void CreateProcessSystem()
{
    cout << "Create process by system()" << endl;
    int process_ret_val = system("ps");
    cout << "Process ps return " << process_ret_val << endl;
}


enum eMenuAction {eMenuAction_FORK = 1, eMenuAction_EXECL = 2, eMenuAction_SYSTEM = 3};


/*[brief@]  Create process using fork() or system() or execl()
 *[param@]  argc - args count, argv - arguments
 *[return@] EXIT_SUCCESS if main finish success, else return EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    char exitSymbol = 'n';

    do
    {
        system("clear");
        PrintMenu();

        cout << "Choose action(1-3): ";
        int action = 0;
        cin >> action;

        switch (action)
        {
        case eMenuAction::eMenuAction_FORK:
        {
            CreateProcessFork();
            break;
        }
        case eMenuAction::eMenuAction_EXECL:
        {
            CreateProcessExecl();
            break;
        }
        case eMenuAction::eMenuAction_SYSTEM:
        {
            CreateProcessSystem();
            break;
        }
        default:
            cout << "Wrong input! Try again!" << endl;
            break;
        }

        cout << "Exit (y/n)? ";
        cin >> exitSymbol;
    } while(toupper(exitSymbol) != 'Y');

    cout << "Goodbye!" << endl;

    return a.exec();
}
