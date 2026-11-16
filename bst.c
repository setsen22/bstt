#include "DS.h"
#include <stdio.h>
#include <stdlib.h>
static int size(const Elm *x)
{
        if (x == NULL)
                return 0;
        return x->len;
}

/* NULL дэд модны өндөр 0 */
static int hgt(const Elm *x)
{
        if (x == NULL)
                return 0;
        return x->height;
}

/* Рекурсоос буцахдаа len, height-ийг шинэчилнэ (лекц: update counts after recursive calls) */
static void update(Elm *x)
{
        int hl = hgt(x->L);
        int hr = hgt(x->R);
        x->len = 1 + size(x->L) + size(x->R);
        x->height = 1 + (hl > hr ? hl : hr);
}

/* Лекцийн put(Node x, key): хоосон газар хүрвэл шинэ орой үүсгэнэ */
static Elm *put(Elm *x, int key)
{
        if (x == NULL) {
                Elm *e = malloc(sizeof(Elm));
                e->x = key;
                e->height = 1;
                e->len = 1;
                e->L = NULL;
                e->R = NULL;
                return e;
        }
        if (key < x->x)
                x->L = put(x->L, key);
        else if (key > x->x)
                x->R = put(x->R, key);
        update(x);
        return x;
}

/* Лекцийн min(): зүүн тийш null хүртэл явна */
static Elm *min(Elm *x)
{
        while (x->L != NULL)
                x = x->L;
        return x;
}

/* Лекцийн deleteMin(): зүүн холбоос null болтол явж, тэр оройг баруун хүүгээр нь орлуулна.
   Устгах оройг free хийнэ. */
static Elm *deleteMin(Elm *x)
{
        if (x->L == NULL) {
                Elm *r = x->R;
                free(x);
                return r;
        }
        x->L = deleteMin(x->L);
        update(x);
        return x;
}

/* Лекцийн Hibbard deletion */
static Elm *delete(Elm *x, int key)
{
        if (x == NULL)
                return NULL;
        if (key < x->x)
                x->L = delete(x->L, key);
        else if (key > x->x)
                x->R = delete(x->R, key);
        else {
                if (x->R == NULL) {             /* баруун хүүгүй */
                        Elm *l = x->L;
                        free(x);
                        return l;
                }
                if (x->L == NULL) {             /* зүүн хүүгүй */
                        Elm *r = x->R;
                        free(x);
                        return r;
                }
                /* 2 хүүтэй: successor-ийн утгыг авч, баруун дэд модноос successor-ийг устгана */
                int s = min(x->R)->x;
                x->R = deleteMin(x->R);
                x->x = s;
        }
        update(x);
        return x;
}

static void inorder(const Elm *x)
{
        if (x == NULL)
                return;
        inorder(x->L);
        printf("%d %d %d\n", x->x, x->len, x->height);
        inorder(x->R);
}

/* Лекцийн rank(). Лекцэд "key-ээс эрс бага" байсан бол энд "бага буюу тэнцүү"
   тул тэнцүү үед өөрийг нь мөн нэмнэ. */
static int rank(const Elm *x, int key)
{
        if (x == NULL)
                return 0;
        if (key < x->x)
                return rank(x->L, key);
        else if (key > x->x)
                return 1 + size(x->L) + rank(x->R, key);
        else
                return size(x->L) + 1;
}

/* Лекцийн floor() */
static Elm *floor_(Elm *x, int key)
{
        if (x == NULL)
                return NULL;
        if (key == x->x)
                return x;
        if (key < x->x)
                return floor_(x->L, key);
        Elm *t = floor_(x->R, key);
        if (t != NULL)
                return t;
        return x;
}

/* ceiling() нь floor()-ын толин тусгал */
static Elm *ceiling_(Elm *x, int key)
{
        if (x == NULL)
                return NULL;
        if (key == x->x)
                return x;
        if (key > x->x)
                return ceiling_(x->R, key);
        Elm *t = ceiling_(x->L, key);
        if (t != NULL)
                return t;
        return x;
}
/*
  `ptree`-ийн зааж байгаа модонд `x` утгыг оруулна.
  Оруулахдаа хоёртын хайлтын модны зарчмаар оруулах бөгөөд оруулсан
  байрлалаас дээшхи өвөг эцгийн `len`, `height` утгууд өөрчлөгдөнө.
  Мод хоосон байсан бол `ptree->root` хаяг өөрчлөгдөж шинээр орсон оройг заана.
 */
void bs_put(BST *ptree, int x)
{
        ptree->root = put(ptree->root, x);
}
Elm *bs_get(const BST *ptree, int x)
{
        Elm *t = ptree->root;
        while (t != NULL) {
                if (x < t->x)
                        t = t->L;
                else if (x > t->x)
                        t = t->R;
                else
                        return t;
        }
        return NULL;
}
void bs_del(BST *ptree, int x)
{
ptree->root = delete(ptree->root, x);       // Функцийг хэрэгжүүлнэ үү
}

/*
  Хамгийн багыг устгах функц: ХХМноос хамгийг бага утгыг нь устгах функц.
  Устгасан утгыг буцаана.
 */
int bs_delMin(BST *ptree)
{
 int m = min(ptree->root)->x;
        ptree->root = deleteMin(ptree->root);
        return m;        // Функцийг хэрэгжүүлнэ үү
}

/*
  ХХМыг inorder дарааллаар, нэг мөрөнд нэг утга хэвлэнэ.
 */
void bs_inorder(const BST *ptree)
{
inorder(ptree->root);       // Функцийг хэрэгжүүлнэ үү
}

/*
  ХХМноос `x` утгаас эрс бага буюу тэнцүү байх хэдэн орой байгааг олж буцаана.
  Өөрөөр хэлбэл хоёртын хайлтын модны утгуудыг өсөх дарааллаар байрлуулбал
  `x`-ийг оролцуулаад өмнө хэдэн тоо байх вэ? гэсэн үг.
  `x` утга заавал модонд байх албагүй.
 */
int bs_rank(const BST *ptree, int x)
{
return rank(ptree->root, x);       // Функцийг хэрэгжүүлнэ үү
}

/*
  ХХМноос `x` утгатай оройг хайж олоод, тухайн оройд суурилсан
  дэд модонд хэдэн орой байгааг олж буцаана.
  Олдохгүй бол -1-ийг буцаана.
 */
int bs_size(const BST *ptree, int x)
{
Elm *t = bs_get(ptree, x);
        if (t == NULL)
                return -1;
        return t->len;        // Функцийг хэрэгжүүлнэ үү
}

/*
  XXMноос `x`-ээс бага буюу тэнцүү байх хамгийн их утгын `Elm *` хаягийг олж буцаана.
  Олдохгүй бол `NULL` хаягийг буцаана.
 */
Elm *bs_floor(const BST *ptree, int x)
{
return floor_(ptree->root, x);      // Функцийг хэрэгжүүлнэ үү
}

/*
  XXMноос `x`-ээс их буюу тэнцүү байх хамгийн бага утгын `Elm *` хаягийг олж буцаана.
  Олдохгүй бол `NULL` хаягийг буцаана.
 */
Elm *bs_ceiling(const BST *ptree, int x)
{
return ceiling_(ptree->root, x);        // Функцийг хэрэгжүүлнэ үү
}

/*
  ХХМноос `x` утгатай оройг хайж олоод, тухайн оройд суурилсан
  дэд модны өндөр хэд байгааг олж буцаана. Олдохгүй бол -1-ийг буцаана.
 */
int bs_height(const BST *ptree, int x)
{
Elm *t = bs_get(ptree, x);
        if (t == NULL)
                return -1;
        return t->height;        // Функцийг хэрэгжүүлнэ үү
}
