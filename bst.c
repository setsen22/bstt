#include "DS.h"
#include <stdio.h>
#include <stdlib.h>
{
        return t == NULL ? 0 : t->height;
}

static int cnt(const Elm *t)
{
        return t == NULL ? 0 : t->len;
}
/*
  `ptree`-ийн зааж байгаа модонд `x` утгыг оруулна.
  Оруулахдаа хоёртын хайлтын модны зарчмаар оруулах бөгөөд оруулсан
  байрлалаас дээшхи өвөг эцгийн `len`, `height` утгууд өөрчлөгдөнө.
  Мод хоосон байсан бол `ptree->root` хаяг өөрчлөгдөж шинээр орсон оройг заана.
 */
static void update(Elm *t)
{
        int hl = hgt(t->L);
        int hr = hgt(t->R);
        t->height = 1 + (hl > hr ? hl : hr);
        t->len = 1 + cnt(t->L) + cnt(t->R);
}

static Elm *new_elm(int x)
{
        Elm *e = malloc(sizeof(Elm));
        if (e == NULL)
                return NULL;
        e->x = x;
        e->height = 1;
        e->len = 1;
        e->L = e->R = NULL;
        return e;
}

static Elm *put(Elm *t, int x)
{
        if (t == NULL)
                return new_elm(x);
        if (x < t->x)
                t->L = put(t->L, x);
        else if (x > t->x)
                t->R = put(t->R, x);
        update(t);
        return t;
}

/* t модны хамгийн бага оройг устгаад шинэ үндсийг буцаана.
   *minx-д устгасан утгыг хадгална */
static Elm *del_min(Elm *t, int *minx)
{
        if (t->L == NULL) {
                Elm *r = t->R;
                *minx = t->x;
                free(t);
                return r;
        }
        t->L = del_min(t->L, minx);
        update(t);
        return t;
}

static Elm *del(Elm *t, int x)
{
        if (t == NULL)
                return NULL;
        if (x < t->x)
                t->L = del(t->L, x);
        else if (x > t->x)
                t->R = del(t->R, x);
        else {
                if (t->L == NULL) {
                        Elm *r = t->R;
                        free(t);
                        return r;
                }
                if (t->R == NULL) {
                        Elm *l = t->L;
                        free(t);
                        return l;
                }
                /* Хоёр хүүтэй: баруун дэд модны хамгийн багаар орлуулна */
                int m;
                t->R = del_min(t->R, &m);
                t->x = m;
        }
        update(t);
        return t;
}

static void inorder(const Elm *t)
{
        if (t == NULL)
                return;
        inorder(t->L);
        printf("%d %d %d\n", t->x, t->len, t->height);
        inorder(t->R);
}
void bs_put(BST *ptree, int x)
{
ptree->root = put(ptree->root, x);        // Функцийг хэрэгжүүлнэ үү
}

/*
  `ptree`-ийн зааж байгаа модноос `x` утгыг хайн олдсон оройн `Elm*` хаягийг буцаана.
  Олдохгүй бол `NULL` хаягийг буцаана.
  Мод дандаа ялгаатай элементүүд хадгална гэж үзэж болно.
 */
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

/*
  Устгах функц: ХХМноос `x` утгыг хайж олоод устгана.
  Олдохгүй бол юу ч хийхгүй.
 */
void bs_del(BST *ptree, int x)
{
ptree->root = del(ptree->root, x);        // Функцийг хэрэгжүүлнэ үү
}

/*
  Хамгийн багыг устгах функц: ХХМноос хамгийг бага утгыг нь устгах функц.
  Устгасан утгыг буцаана.
 */
int bs_delMin(BST *ptree)
{
 int m = 0;
        if (ptree->root == NULL)
                return m;
        ptree->root = del_min(ptree->root, &m);
        return m;        // Функцийг хэрэгжүүлнэ үү
}

/*
  ХХМыг inorder дарааллаар, нэг мөрөнд нэг утга хэвлэнэ.
 */
void bs_inorder(const BST *ptree)
{
inorder(ptree->root);        // Функцийг хэрэгжүүлнэ үү
}

/*
  ХХМноос `x` утгаас эрс бага буюу тэнцүү байх хэдэн орой байгааг олж буцаана.
  Өөрөөр хэлбэл хоёртын хайлтын модны утгуудыг өсөх дарааллаар байрлуулбал
  `x`-ийг оролцуулаад өмнө хэдэн тоо байх вэ? гэсэн үг.
  `x` утга заавал модонд байх албагүй.
 */
int bs_rank(const BST *ptree, int x)
{
Elm *t = ptree->root;
        int r = 0;
        while (t != NULL) {
                if (x < t->x)
                        t = t->L;
                else if (x > t->x) {
                        r += 1 + cnt(t->L);
                        t = t->R;
                } else {
                        r += 1 + cnt(t->L);
                        break;
                }
        }
        return r;        // Функцийг хэрэгжүүлнэ үү
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
 Elm *t = ptree->root;
        Elm *best = NULL;
        while (t != NULL) {
                if (t->x == x)
                        return t;
                if (t->x < x) {
                        best = t;
                        t = t->R;
                } else
                        t = t->L;
        }
        return best;        // Функцийг хэрэгжүүлнэ үү
}

/*
  XXMноос `x`-ээс их буюу тэнцүү байх хамгийн бага утгын `Elm *` хаягийг олж буцаана.
  Олдохгүй бол `NULL` хаягийг буцаана.
 */
Elm *bs_ceiling(const BST *ptree, int x)
{
Elm *t = ptree->root;
        Elm *best = NULL;
        while (t != NULL) {
                if (t->x == x)
                        return t;
                if (t->x > x) {
                        best = t;
                        t = t->L;
                } else
                        t = t->R;
        }
        return best;        // Функцийг хэрэгжүүлнэ үү
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
