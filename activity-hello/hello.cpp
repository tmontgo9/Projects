#include <stdio.h>
#include <limits.h>
#include <unistd.h>
int main () {

    char hostname[HOST_NAME_MAX];
    gethostname (hostname, HOST_NAME_MAX);
    printf("The hostname is: " + hostname +"\n");
  return 0;
}
