#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPTION_CREATE_BOOK 0
#define OPTION_LIST_BOOKS 2

#define BOOK_FLAG_SOLD 0b00000001

struct book_t
{
    char title[20];
    char description[200];
    unsigned char book_flag;
};

int handle_create(FILE *file);
int handle_list(FILE *file);

int handle_create(FILE *file)
{
    struct book_t book;
    memset(&book, 0, sizeof(book));
    printf("Enter the boooks name:\n");
    fgets(book.title, sizeof(book.title) - 1, stdin); //-1 to maintain the last 0(null terminaltor)
    printf("Book name: %s", book.title);

    printf("Enter the boooks description:\n");
    fgets(book.description, sizeof(book.description) - 1, stdin); //-1 to maintain the last 0(null terminaltor)
    printf("Book name: %s", book.description);

    printf("Has the book sold: Y/N?\n");
    char choose;
    scanf("%c", &choose);
    if(choose =='Y' || choose == 'y')
    {
        book.book_flag |= BOOK_FLAG_SOLD; //set the sold flag to book
    }


    fseek(file, 0, SEEK_END);
    fwrite(&book, sizeof(book), 1, file);

    return 0;
}

int handle_list(FILE *file)
{

    //calc. qtd. of books in data.bin file
    fseek(file, 0, SEEK_END);
    unsigned long total_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    int qtd_books = total_size / sizeof(struct book_t);

    //store all book of file inside this array
    struct book_t *list_of_books = (struct book_t *)malloc(sizeof(struct book_t) * qtd_books);
    int index = 0;
    while (fread(&list_of_books[index], sizeof(struct book_t), 1, file) == 1)
    {
        printf("%i - %s", index, list_of_books[index].title);
        index++;
    }

    //select book
    int option = 0;
    printf("Choose an option:");
    scanf("%i", &option);
    getchar(); // remover o \n após ler o número inteiro

    if (option < 0 || option >= qtd_books)
    {
        printf("Invalid book\n");
        return -1;
    }


    //show the selected book
    printf("\n----------------------------------------------\n");
    printf("title:%s\n" , list_of_books[option].title);
    printf("description:%s\n" , list_of_books[option].description);

    if(list_of_books[option].book_flag & BOOK_FLAG_SOLD)
    {
        printf("ThIS book was sold sorry\n");
    }

    printf("----------------------------------------------\n");

    free(list_of_books);

    return 0;
}

int main(int argc, char **argv)
{

    FILE *file = fopen("data.bin", "rb+");
    if (!file)
    {
        printf("Failed to open file!!!\n");
        return -1;
    }

    printf("Enter an option: ");
    int option = 0;
    scanf("%i", &option);
    getchar();

    int res = 0;
    switch (option)
    {

    case OPTION_CREATE_BOOK:
        res = handle_create(file);
        if (res < 0)
            return res;

        break;

    case OPTION_LIST_BOOKS:
        handle_list(file);
        break;

    default:
        printf("Invalid option chosen\n");
        return -1;
    }

    fclose(file);

    return 0;
}