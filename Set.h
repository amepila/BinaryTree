#ifndef SET_H_
#define SET_H_

typedef void * Type;
typedef struct strSet * Set;
typedef enum {FALSE, TRUE} bool;
typedef int (*CompareFunc) (Type, Type);
typedef void (*PrintFunc) (Type);
typedef void (*DestroyFunc) (Type);

Set set_create(CompareFunc cf, PrintFunc pf, DestroyFunc df);
int set_size(Set);
bool set_add(Set s, Type t);
bool set_contains(Set s, Type t);
void set_destroy(Set s);
bool set_remove(Set s, Type t);
void set_print(Set s);


#endif /* SET_H_ */
