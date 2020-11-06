#include "linked-list.h"

int     print_error(const char *msg) {
    fprintf(stderr, msg, 255);
    return (0);
}

int main(void)
{
    list_t *l = NULL;

    l = list_create();
    if (l == NULL)
        return (print_error("la liste n'a pas pu être créée\n"));
    if (l->size_list != 0)
        return (print_error("Size incorrecte\n"));
    list_append(l, "Bonjour");
    list_append(l, "les");
    list_append(l, "amis");
    list_display(l);
    if (l->size_list != 3)
        return (print_error("Size incorrecte\n"));
    list_prepend(l, "Hey !");
    list_display(l);
    void *ele;
    list_pop(l, &ele);
    list_display(l);
    printf("%s\n", (char*)ele);
    // for (unsigned int i = 0; i < l->size_list; i++) {
    //     list_pop(l, NULL);
    //}
    if (list_is_empty(l))
        return (print_error("la liste n'est pas vide\n"));
    list_display(l);
    
    list_delete(l);

    list_t *l1 = NULL;
    list_t *l2 = NULL;
    l1 = list_create();
    l2 = list_create();

    for(int i = 0; i < 3; i++)
    {
        list_append(l1, "coucou");
        list_append(l2, "bonjour");
    }
    list_concatenation(l1, l2);
    list_display(l1);

    list_delete(l1);
    list_delete(l2);

    return 0;
}
