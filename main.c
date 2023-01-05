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

int main(void)
{
    char **s;
    char *ss;

    ss = "=123";
    s = ft_split(ss, '=');
    printf("%s, %s\n", s[0], s[1]);
}