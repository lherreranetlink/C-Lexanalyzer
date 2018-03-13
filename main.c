#include <stdio.h>
#include <stdlib.h>
#include "token.h"

int main(int argc, char *argv[])
{
    char filename[] = "example";
    FILE* fd;
    if ((fd = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "%s file not exists\n", filename);
        exit(1);
    }

    Token* aux;
    while((aux = getNextToken(fd))->type != EOF_SIGN)
    {
        printf("Symbol: %s\n", aux->symbol);
        printf("Type: %d\n", aux->type);
    }
    fclose(fd);
    return 0;
}
