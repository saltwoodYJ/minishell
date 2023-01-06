#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h> 
char	**ft_split(char const *s, char c);

int ch_main(void) { 
    char *path = "../../desktop" ; 
    char dir[30] = "Current Directory: "; 
    int ch = chdir(path); 
    
    if( ch == 0 ) { 
        printf("Change directory\n") ; // 디렉토리 이동 성공 
    }else { 
        printf("Failed change directory!\n") ; // 디렉토리 이동 실패 
    }   
    
    write(1, dir, sizeof(char) * strlen(dir)) ; 

    // 현재 디렉토리 위치 확인  
    if ( system("pwd") == -1) { 
        perror("command failed!\n") ; 
    } 
    return 0;
}

#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int fd;

    fd = open("./here_doc/m_heredoc.tmp", O_RDWR | O_CREAT | O_TRUNC, 0644);
    return (0);
}