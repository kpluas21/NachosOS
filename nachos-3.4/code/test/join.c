#include "syscall.h"

int main() {
    int pid = Exec("../test/halt");
    int joinStatus;
    char *joinFailedMsg = "Join failed\n";
    char *joinSuccessMsg = "Join successful\n";
    char *execFailedMsg = "Exec failed\n";
    
    if (pid != -1) {
        joinStatus = Join(pid);
        if (joinStatus == -1) {
            Write(joinFailedMsg, 12, 1);
        } else {
            Write(joinSuccessMsg, 16, 1);
        }
    } else {
        Write(execFailedMsg, 12, 1);
    }
    
    Exit(0);
    return 0;
}