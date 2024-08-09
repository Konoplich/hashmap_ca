#include <stdlib.h>
#include <stdio.h>
#include "libhashmap.h"
#include <string.h>     /* prototype for strchr() */

/* типы ключа и значения: в нашем случае это строки */

uchar *strudup (const uchar
                *s) /* создание копии строки в "куче" */
{
	uchar *p = (uchar *) malloc (strlen (s)+1);
	strcpy (p, s);
	return p;
}

/* одна из возможных хэш-функций */
//unsigned int
//hash; /* последнее вычисленное значение хэш-функции */

int HASHFUNC (size_t size, KEY key)
{
	unsigned int i = 0;
	uchar *keysrc = key;

	while (*key)
	{
		i = (i << 1)| (i >> 15); /* ROL */
		i ^= *key++;
	}

	unsigned int hash = i % size;
	printf ("hash(%s)=%d\n", keysrc, hash);  /* отладка */
	return hash;
}
#define EQKEY(s1, s2)   (strcmp(s1, s2) == 0)
#define FREEKEY(s)      free(s)
#define FREEVAL(s)      free(s)
#define SETVAL(at,s)    (at = strudup(s))
#define SETKEY(at,s)    (at = strudup(s))
#define KEYFMT          "%s"
#define VALFMT          "%s"

/* получение значения по ключу */
cell_t *hashmap_get (cell_t **hashtable, size_t size, KEY key)
{
	cell_t *p;

	for (p = hashtable[HASHFUNC (size, key)]; p; p = p->next)
		if (EQKEY (p->key, key))
			return p;

	printf("get %s\n", key);
	return NULL;    /* отсутствует */
}

/* занести пару ключ:значение в таблицу */
void hashmap_set (cell_t **hashtable, size_t size, KEY key, VAL val)
{
	cell_t *p;
	unsigned int hash = HASHFUNC(size, key);

	/* проверить - не было ли звена с таким ключом */
	if ((p = hashmap_get (hashtable, size, key)) == NULL)      /* не было   */
	{
		if (! (p = (cell_t *) malloc (sizeof (*p))))
			return;

		SETKEY (p->key, key);
		p->next = hashtable[hash]; /* hash вычислено в get() */
		hashtable[hash] = p;
		printf("set\n");
	}
	else{   /* уже было: изменить значение */
		FREEVAL (p->val);
	    printf("free\n");
	}

	SETVAL (p->val, val);
}

/* удаление по ключу */
int hashmap_del (cell_t **hashtable, size_t size, KEY key)
{
	int indx = HASHFUNC (size, key);
	cell_t *p, *prev = NULL;

	if ((p=hashtable[indx]) == NULL)
		return 0;

	for (; p ; prev = p, p=p->next)
		if (EQKEY (p->key, key))
		{
			FREEVAL (p->val);
			FREEKEY (p->key);

			if (p == hashtable[indx])  /* голова списка */
				hashtable[indx] = p->next;
			else
				prev->next = p->next;

			free ((void *) p);
			return 1; /* удален */
		}

	return 0;  /* не было такого */
}

/* распечатать пару ключ:значение */
void hashmap_print_cell (cell_t *ptr)
{
	putchar ('(');
	printf (KEYFMT, ptr->key);
	putchar (',');
	printf (VALFMT, ptr->val);
	putchar (')');
}

/* распечатка таблицы (для отладки) */
void hashmap_print(cell_t **hashtable, size_t size)
{
	register i;
	cell_t *p;
	printf ("----TABLE CONTENTS----\n");

	for (i=0; i < size; i++)
		if ((p = hashtable[i]) != NULL)
		{
			printf ("%d: ", i);
			for (; p; p=p->next)
				hashmap_print_cell(p), putchar (' ');
			putchar ('\n');
		}
}

/* выдать очередное значение */
cell_t *hashmap_next_pair (cell_t **hashtable, size_t size, iter_t *ci)
{
	cell_t *result;

	while ((result = ci->ptr) == NULL)
	{
		if (++ (ci->index) >= size)
			return NULL;    /* больше нет */

		ci->ptr = hashtable[ci->index];
	}

	ci->ptr = result->next;
	return result;
}

/* инициализация итератора */
cell_t *hashmap_reset_iter (cell_t **hashtable, size_t size, iter_t *ci)
{
	ci->index = (-1);
	ci->ptr = NULL;
	return hashmap_next_pair (hashtable, size, ci); /* первое значение */
}
