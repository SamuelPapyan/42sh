#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("input.txt", O_RDONLY);
    
    if (fd == -1) {
        perror("open");
        return 1;
    }

    dup2(fd, STDIN_FILENO); // Теперь scanf и другие читают из файла "input.txt"

    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    printf("Из файла: %s\n", buffer);
    fgets(buffer, sizeof(buffer), stdin);
    printf("Из файла: %s\n", buffer);
    fgets(buffer, sizeof(buffer), stdin);
    printf("Из файла: %s\n", buffer);
    fgets(buffer, sizeof(buffer), stdin);
    printf("Из файла: %s\n", buffer);
    

    close(fd);
    return 0;
}
