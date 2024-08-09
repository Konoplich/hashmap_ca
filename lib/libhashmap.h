typedef unsigned char uchar;
typedef uchar *VAL;
typedef uchar *KEY;

typedef struct _cell
{
	struct _cell *next; /* ссылка на очередной элемент */
	KEY key;           /* ключ     */
	VAL val;           /* значение */
} cell_t;

/* итератор */
typedef struct _ititer
{
	int index;
	cell_t *ptr;
} iter_t;

cell_t *hashmap_get (cell_t **hashtable, size_t size, KEY key);
void hashmap_set (cell_t **hashtable,size_t size, KEY key, VAL val);
int hashmap_del (cell_t **hashtable,size_t size, KEY key);
void hashmap_print_cell (cell_t *ptr);
void hashmap_print(cell_t **hashtable, size_t size);
cell_t *hashmap_next_pair (cell_t **hashtable, size_t size, iter_t *ci);
cell_t *hashmap_reset_iter (cell_t **hashtable, size_t size, iter_t *ci);

