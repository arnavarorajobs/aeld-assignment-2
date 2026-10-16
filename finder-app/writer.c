#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char *argv[]){

    openlog("writer", LOG_PID, LOG_USER);

    if (argc !=3){
        fprintf(stderr, "Usage: %s <writefile> <writestring>\n", argv[0]);
        syslog(LOG_ERR, "Expected 2 arguments; received %d", argc - 1);
        closelog();
        return 1;
    }


    const char *writefile = argv[1];
    const char *writestring = argv[2];

    // printf("File path: %s\n", writefile);
    // printf("Text: %s\n", writestring);

    FILE *output = fopen(writefile, "w");
    
    if (output == NULL) {
        int saved_errno = errno;
        fprintf(stderr, "writer: cannot open %s: %s\n", writefile, strerror(saved_errno)); 
        syslog(LOG_ERR, "Cannot open %s: %s", writefile, strerror(saved_errno));       
        closelog();
        return 1;
    }

    if (fputs(writestring, output) == EOF) {
        int saved_errno = errno;
        fprintf(stderr, "writer: cannot write to %s: %s\n", writefile, strerror(saved_errno));
        syslog(LOG_ERR, "Cannot write to %s: %s", writefile, strerror(saved_errno));
        fclose(output);
        closelog();
        return 1;
    }

    if (fclose(output) == EOF) {
        int saved_errno = errno;
        fprintf(stderr, "writer: cannot close %s: %s\n", writefile, strerror(saved_errno));
        syslog(LOG_ERR, "Cannot close %s: %s", writefile, strerror(saved_errno));
        closelog();
        return 1;
    }

    syslog(LOG_DEBUG, "Writing %s to %s", writestring, writefile);
    closelog();

    return 0;

}