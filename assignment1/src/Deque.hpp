#ifndef HALPERN_ZACH_DEQUE_H
#define HALPERN_ZACH_DEQUE_H

#include <cstring>

#define START_SIZE 100
#define START_ARRAY 50
#define MODULO(X,N)  (((X) < 0) ? (((X) % (N)) + (N)) : ((X) % (N)))

#define Deque_DEFINE(t)                                                                    \
    struct Deque_##t##_Iterator;                                                           \
    struct Deque_##t                                                                       \
    {                                                                                      \
        char type_name[strlen("Deque_"#t) + 1];                                            \
        t *elements;                                                                       \
        long head;                                                                         \
        long tail;                                                                         \
        size_t eles_count;                                                                 \
        long array_size;                                                                   \
        void (*dtor)(Deque_##t *);                                                         \
        bool (*empty)(Deque_##t *);                                                        \
        void (*pop_back)(Deque_##t *);                                                     \
        void (*pop_front)(Deque_##t *);                                                    \
        void (*push_back)(Deque_##t *, t);                                                 \
        void (*push_front)(Deque_##t *, t);                                                \
        size_t (*size)(Deque_##t *);                                                       \
        void (*clear)(Deque_##t *);                                                        \
        bool (*equal)(Deque_##t a, Deque_##t b);                                           \
        t &(*at)(Deque_##t *, int);                                                        \
        t &(*back)(Deque_##t *);                                                           \
        t &(*front)(Deque_##t *);                                                          \
        Deque_##t##_Iterator (*begin)(Deque_##t *);                                        \
        Deque_##t##_Iterator (*end)(Deque_##t *);                                          \
        void (*sort)(Deque_##t *, Deque_##t##_Iterator, Deque_##t##_Iterator);             \
        bool (*comparator)(const t &, const t &);                                          \
    };                                                                                     \
    struct Deque_##t##_Iterator                                                            \
    {                                                                                      \
        int current;                                                                       \
        int array_size;                                                                    \
        Deque_##t *deque;                                                                  \
        void (*inc)(Deque_##t##_Iterator *);                                               \
        void (*dec)(Deque_##t##_Iterator *);                                               \
        t &(*deref)(Deque_##t##_Iterator *);                                               \
        bool (*equal)(Deque_##t##_Iterator, Deque_##t##_Iterator);                         \
    };                                                                                     \
    void Deque_##t##_dtor(Deque_##t *dp)                                                   \
    {                                                                                      \
        free(dp->elements);                                                                \
    }                                                                                      \
    bool Deque_##t##_empty(Deque_##t *dp)                                                  \
    {                                                                                      \
        return (dp->size(dp) == 0);                                                        \
    }                                                                                      \
    void Deque_##t##_pop_back(Deque_##t *dp)                                               \
    {                                                                                      \
        if (dp->tail == 0)                                                                 \
        {                                                                                  \
            dp->tail = dp->array_size-1;                                                     \
        }                                                                                  \
        else                                                                               \
        {                                                                                  \
            dp->tail--;                                                                    \
        }                                                                                  \
        dp->eles_count--;                                                                  \
    }                                                                                      \
    void Deque_##t##_pop_front(Deque_##t *dp)                                              \
    {                                                                                      \
        if (dp->head == dp->array_size-1)                                                    \
        {                                                                                  \
            dp->head = 0;                                                                  \
        }                                                                                  \
        else                                                                               \
        {                                                                                  \
            dp->head++;                                                                    \
        }                                                                                  \
        dp->eles_count--;                                                                  \
    }                                                                                      \
    void Deque_##t##_push_back(Deque_##t *dp, t val)                                       \
    {                                                                                      \
        if (MODULO(dp->tail + 2, dp->array_size) == dp->head && dp->eles_count > 1)        \
        {                                                                                  \
            int new_size = dp->array_size * 2;                                             \
            t *new_array = (t *)realloc(dp->elements, new_size * sizeof(t));               \
                                                                                           \
            if (new_array)                                                                 \
            {                                                                              \
                for (long i = 0; i < dp->head; i++)                                        \
                {                                                                          \
                    new_array[dp->array_size] = new_array[i];                              \
                    dp->array_size++;                                                      \
                }                                                                          \
                dp->tail = MODULO(dp->array_size - 1, dp->array_size);                     \
            }                                                                              \
            else                                                                           \
            {                                                                              \
                printf("Realloc Failed!\n");                                               \
                exit(1);                                                                   \
            }                                                                              \
                                                                                           \
            new_array[dp->tail] = val;                                                     \
            dp->eles_count++;                                                              \
                                                                                           \
            dp->array_size = new_size;                                                     \
            dp->elements = new_array;                                                      \
        }                                                                                  \
        else                                                                               \
        {                                                                                  \
            int nct = MODULO(dp->tail + 1, dp->array_size);                                \
            dp->elements[nct] = val;                                                       \
            dp->tail = nct;                                                                \
            if (dp->eles_count++ == 0)                                                     \
            {                                                                              \
                dp->head = nct;                                                            \
            }                                                                              \
        }                                                                                  \
    }                                                                                      \
    void Deque_##t##_push_front(Deque_##t *dp, t val)                                      \
    {                                                                                      \
        if (MODULO(dp->tail + 2, dp->array_size) == dp->head && dp->eles_count > 1)        \
        {                                                                                  \
            int new_size = dp->array_size * 2;                                             \
            t *new_array = (t *)realloc(dp->elements, new_size * sizeof(t));               \
                                                                                           \
            if (new_array)                                                                 \
            {                                                                              \
                for (long i = 0; i < dp->head; i++)                                        \
                {                                                                          \
                    new_array[dp->array_size] = new_array[i];                              \
                    dp->array_size++;                                                      \
                }                                                                          \
                dp->tail = MODULO(dp->array_size - 2, dp->array_size);                     \
            }                                                                              \
            else                                                                           \
            {                                                                              \
                printf("Realloc Failed!\n");                                               \
                exit(1);                                                                   \
            }                                                                              \
                                                                                           \
            dp->head = MODULO(dp->head - 1, dp->array_size);                               \
            new_array[dp->head] = val;                                                     \
            dp->eles_count++;                                                              \
                                                                                           \
            dp->array_size = new_size;                                                     \
            dp->elements = new_array;                                                      \
        }                                                                                  \
        else                                                                               \
        {                                                                                  \
            int nch = MODULO(dp->head - 1, dp->array_size);                                \
            dp->elements[nch] = val;                                                       \
            dp->head = nch;                                                                \
            if (dp->eles_count++ == 0)                                                     \
            {                                                                              \
                dp->tail = nch;                                                            \
            }                                                                              \
        }                                                                                  \
    }                                                                                      \
    size_t Deque_##t##_size(Deque_##t *dp)                                                 \
    {                                                                                      \
        return dp->eles_count;                                                             \
    }                                                                                      \
    void Deque_##t##_clear(Deque_##t *dp)                                                  \
    {                                                                                      \
        dp->head = START_ARRAY;                                                            \
        dp->tail = START_ARRAY;                                                            \
        dp->eles_count = 0;                                                                \
    }                                                                                      \
    bool Deque_##t##_equal(Deque_##t dp1, Deque_##t dp2)                                   \
    {                                                                                      \
        if (dp1.size(&dp1) == dp2.size(&dp2))                                              \
        {                                                                                  \
            for (size_t i = 0; i < dp1.size(&dp1); i++)                                    \
            {                                                                              \
                auto f = dp1.at(&dp1, i);                                                  \
                auto g = dp2.at(&dp2, i);                                                  \
                if (dp1.comparator(f,g) || dp1.comparator(g,f))                            \
                {                                                                          \
                    return false;                                                          \
                }                                                                          \
            }                                                                              \
            return true;                                                                   \
        }                                                                                  \
        return false;                                                                      \
    }                                                                                      \
    t &Deque_##t##_at(Deque_##t *dp, int i)                                                \
    {                                                                                      \
        return dp->elements[MODULO(dp->head + i, dp->array_size)];                         \
    }                                                                                      \
    t &Deque_##t##_back(Deque_##t *dp)                                                     \
    {                                                                                      \
        return dp->elements[dp->tail];                                                     \
    }                                                                                      \
    t &Deque_##t##_front(Deque_##t *dp)                                                    \
    {                                                                                      \
        return dp->elements[dp->head];                                                     \
    }                                                                                      \
    int zach_##t##_comparator(const void *fir, const void *sec, void *dp)                  \
    {                                                                                      \
        t first = *(t *)fir;                                                               \
        t second = *(t *)sec;                                                              \
        Deque_##t dq = *(Deque_##t *)dp;                                                   \
        if (dq.comparator(first, second)) { return -1; }                                   \
        if (dq.comparator(second, first)) { return 1; }                                    \
        return 0;                                                                          \
    }                                                                                      \
    void Deque_##t##_sort(Deque_##t *dp, Deque_##t##_Iterator i1, Deque_##t##_Iterator i2) \
    {                                                                                      \
        int dist = MODULO(i2.current - i1.current, i1.array_size);                         \
        int k = MODULO(dp->array_size - i1.current, dp->array_size);                                                  \
        int count = 0;                                                                      \
        for (int start = 0; count < dp->array_size; start++)\
        {\
            int current = start;\
            t prev = dp->elements[start];\
            do {\
                int next = MODULO(current+k, dp->array_size);\
                t temp = dp->elements[next];\
                dp->elements[next] = prev;\
                prev = temp;\
                current = next;\
                count++;\
            } while (start != current);\
        }\
        i1.current = MODULO(dp->eles_count+k, dp->array_size);\
        dp->head = 0;\
        dp->tail = dp->eles_count;\
\
        qsort_r((void *)&i1.deref(&i1), dist, sizeof(t), zach_##t##_comparator, dp);       \
    }                                                                                      \
    void Deque_##t##_Iterator_inc(Deque_##t##_Iterator *dpi)                               \
    {                                                                                      \
        dpi->current = MODULO(dpi->current + 1, dpi->array_size);                          \
    }                                                                                      \
    void Deque_##t##_Iterator_dec(Deque_##t##_Iterator *dpi)                               \
    {                                                                                      \
        dpi->current = MODULO(dpi->current - 1, dpi->array_size);                          \
    }                                                                                      \
    t &Deque_##t##_Iterator_deref(Deque_##t##_Iterator *dpi)                               \
    {                                                                                      \
        return dpi->deque->elements[dpi->current];                                         \
    }                                                                                      \
    bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator it1, Deque_##t##_Iterator it2)    \
    {                                                                                      \
        return (it1.current == it2.current);                                               \
    }                                                                                      \
    Deque_##t##_Iterator Deque_##t##_begin(Deque_##t *dp)                                  \
    {                                                                                      \
        Deque_##t##_Iterator iter;                                                         \
        iter.inc = &Deque_##t##_Iterator_inc;                                              \
        iter.dec = &Deque_##t##_Iterator_dec;                                              \
        iter.deref = &Deque_##t##_Iterator_deref;                                          \
        iter.equal = &Deque_##t##_Iterator_equal;                                          \
        iter.current = dp->head;                                                           \
        iter.deque = dp;                                                                   \
        iter.array_size = dp->array_size;                                                  \
        return iter;                                                                       \
    }                                                                                      \
    Deque_##t##_Iterator Deque_##t##_end(Deque_##t *dp)                                    \
    {                                                                                      \
        Deque_##t##_Iterator iter;                                                         \
        iter.inc = &Deque_##t##_Iterator_inc;                                              \
        iter.dec = &Deque_##t##_Iterator_dec;                                              \
        iter.deref = &Deque_##t##_Iterator_deref;                                          \
        iter.equal = &Deque_##t##_Iterator_equal;                                          \
        iter.current = MODULO(dp->tail + 1, dp->array_size);                               \
        iter.deque = dp;                                                                   \
        iter.array_size = dp->array_size;                                                  \
        return iter;                                                                       \
    }                                                                                      \
    void Deque_##t##_ctor(Deque_##t *dp, bool (*comp)(const t &a1, const t &a2))           \
    {                                                                                      \
        Deque_##t rep;                                                                     \
        rep.empty = &Deque_##t##_empty;                                                    \
        rep.pop_back = &Deque_##t##_pop_back;                                              \
        rep.pop_front = &Deque_##t##_pop_front;                                            \
        rep.push_back = &Deque_##t##_push_back;                                            \
        rep.push_front = &Deque_##t##_push_front;                                          \
        rep.size = &Deque_##t##_size;                                                      \
        rep.clear = &Deque_##t##_clear;                                                    \
        rep.equal = &Deque_##t##_equal;                                                    \
        rep.at = &Deque_##t##_at;                                                          \
        rep.back = &Deque_##t##_back;                                                      \
        rep.front = &Deque_##t##_front;                                                    \
        rep.begin = &Deque_##t##_begin;                                                    \
        rep.end = &Deque_##t##_end;                                                        \
        rep.sort = &Deque_##t##_sort;                                                      \
        rep.dtor = &Deque_##t##_dtor;                                                      \
        rep.eles_count = 0;                                                                \
        rep.array_size = START_SIZE;                                                       \
        rep.head = START_ARRAY;                                                            \
        rep.tail = START_ARRAY;                                                            \
        rep.comparator = comp;                                                             \
        rep.elements = (t *)malloc(sizeof(t) * START_SIZE);                                \
        if (rep.elements == nullptr)                                                       \
        {                                                                                  \
            printf("Malloc Failed!\n");                                                    \
            exit(1);                                                                       \
        }                                                                                  \
        strcpy(rep.type_name, "Deque_" #t);                                                \
        *dp = rep;                                                                         \
    }

#endif
