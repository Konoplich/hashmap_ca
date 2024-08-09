#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libhashmap.h"

#define VALFMT          "%s"

void main()  /* таблица из имен и размеров файлов текущего каталога */
{
	iter_t ci;
	cell_t *cl;
	char key[40], value[40];
	cell_t *val;
	cell_t *hmap[10];
	size_t size = sizeof(hmap)/sizeof(cell_t*);
	printf("init size; %d, %d, %d %d\n", size, sizeof(hmap), sizeof(cell_t), sizeof(cell_t*));	
	memset(hmap,0,sizeof(hmap));
	for(int i=0; i < size;i++)
	    printf("%d %d\n",i, hmap[i]);

	extern FILE *popen();
	FILE *fp;
	char *s ;
	/* popen() читает вывод команды, заданной в 1-ом аргументе */
	fp = popen ("ls -s", "r");

	while (fscanf (fp, "%s%s", value, key) == 2)
	{
		printf("scann %s->%s\n", key, value);

		hashmap_set (hmap, size, key, value);
	}
	printf("scan end\n");
	hashmap_print(hmap, size);

	pclose (fp); /* popen() надо закрывать pclose(); */
	printf("- удалить\n");
	printf("p распечатать\n");
	printf("КЛЮЧ=ЗНАЧЕНИЕ  добавить\n");
	printf("КЛЮЧ :найти значение\n");
	printf("^d завершить\n");


	for (;;)
	{
		printf ("-> ");   /* приглашение */
		
		char* retval = fgets(key, sizeof(key), stdin);
		key[strlen(key)-1] = '\0';
		printf("echo (%s) [%s]\n", retval, key);
		//if (!gets(key))
		if(feof(stdin) || retval == NULL)
			break;   /* EOF */

		if (*key == '-')           /* -КЛЮЧ          :удалить     */
		{
			printf (hashmap_del (hmap, size, key+1) ? "OK\n" : "нет такого\n");
			continue;
		}

		if (!*key || !strcmp (key, "p")) /* = :распечатать таблицу*/
		{
			hashmap_print(hmap, size);
			continue;
		}

		if (s = strchr (key,
		                '=')) /* КЛЮЧ=ЗНАЧЕНИЕ  :добавить     */
		{
			*s++ = '\0';
			hashmap_set (hmap, size, key, s);
			continue;
		}

		if ((val = hashmap_get (hmap, size, key)) == NULL) /* КЛЮЧ :найти значение */
			printf ("нет такого ключа\n");
		else
		{
			printf ("значение ");
			printf (VALFMT, val->val);
			putchar ('\n');
		}
	}

	/* распечатка таблицы при помощи итератора */
	for (cl = hashmap_reset_iter (hmap, size, &ci) ; cl ; cl = hashmap_next_pair (hmap, size, &ci))
		hashmap_print_cell (cl), putchar ('\n');
}

