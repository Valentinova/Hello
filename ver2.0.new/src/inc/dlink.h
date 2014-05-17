#ifndef _DOUBLY_LINKED_LIST
#define _DOUBLY_LINKED_LIST

typedef struct _dlink_entry_t
{
    struct _dlink_entry_t *prev;
    struct _dlink_entry_t *next;
} dlink_entry_t;

typedef struct _dlink_t
{
    dlink_entry_t head;
} dlink_t;

/*---------------------------------------------------------------------------*/
static __inline void
dlink_init(dlink_t *dlink)
{
    dlink->head.prev = dlink->head.next = &dlink->head;
}

static __inline int
dlink_is_empty(dlink_t *dlink)
{
    return (dlink->head.prev == &dlink->head) ? 1 : 0;
}

/*
 * dlink_move moves all the entry in dlink_src to dlink_dst
 */
static __inline void
dlink_move(dlink_t *dlink_dst, dlink_t *dlink_src)
{
    if (dlink_is_empty(dlink_src)) {
        dlink_init(dlink_dst);
    }
    else {
        /* list head points to the new head */
        dlink_src->head.next->prev = &dlink_dst->head;
        /* list tail points to the new head */
        dlink_src->head.prev->next = &dlink_dst->head;

        /* head points to the list head */
        dlink_dst->head.next = dlink_src->head.next;
        /* head points to the list tail */
        dlink_dst->head.prev = dlink_src->head.prev;

        dlink_init(dlink_src);
    }
}

static __inline void
dlink_insert_after(dlink_entry_t *t, dlink_entry_t *entry)
{
    dlink_entry_t *_plink = t;
    entry->prev = t;
    entry->next = t->next;
    _plink->next->prev = entry;
    _plink->next = entry;
}

static __inline void
dlink_push(dlink_t *dlink, dlink_entry_t *entry)
{
    dlink_insert_after(&dlink->head, entry);
}

static __inline void
dlink_push_tail(dlink_t *dlink, dlink_entry_t *entry)
{
    dlink_insert_after(dlink->head.prev, entry);
}

static __inline void
dlink_remove_entry(dlink_entry_t *entry)
{
    dlink_entry_t *_plink = entry->prev;
    dlink_entry_t *_nlink = entry->next;
    _plink->next = _nlink;
    _nlink->prev = _plink;
    entry->prev = 0;
    entry->next = 0;
}

static __inline dlink_entry_t *
dlink_pop(dlink_t *dlink)
{
    if (dlink_is_empty(dlink)) {
        return 0;
    }
    else {
        dlink_entry_t *entry = dlink->head.next;
        dlink_remove_entry(dlink->head.next);
        return entry;
    }
}

static __inline dlink_entry_t *
dlink_peek(dlink_t *dlink)
{
    if (dlink_is_empty(dlink)) {
        return 0;
    }
    return dlink->head.next;
}

static __inline dlink_entry_t *
dlink_peek_tail(dlink_t *dlink)
{
    if (dlink_is_empty(dlink)) {
        return 0;
    }
    return dlink->head.prev;
}

static __inline dlink_entry_t *
dlink_pop_tail(dlink_t *dlink)
{
    if (dlink_is_empty(dlink)) {
        return 0;
    }
    else {
        dlink_entry_t *entry = dlink->head.prev;

        dlink_remove_entry(dlink->head.prev);

        return entry;
    }
}

/*
 * Caller shall not change the list when it is walking through the list.
 */
#define dlink_foreach(dlink, entry)    \
    for ((entry) = (dlink)->head.next; \
         (entry) != &(dlink)->head;    \
         (entry) = (entry)->next)

#define dlink_foreach_safe(dlink, entry, entry2)    \
    for ((entry) = (dlink)->head.next, (entry2) = (entry)->next; \
         (entry) != &(dlink)->head;	\
         (entry) = (entry2), (entry2) = (entry)->next)

#define dlink_foreach_reverse(dlink, entry) \
    for ((entry) = (dlink)->head.prev;      \
         (entry) != &(dlink)->head;         \
         (entry) = (entry)->prev)

#ifndef field_offset
#define field_offset(type, field)   ((unsigned int) &(((type *) 0)->field))
#endif

#ifndef container_of
#define container_of(address, type, field) \
        ((type *) ((char *) (address) - (char *) (&((type *) 0)->field)))
#endif

#endif	/*_DOUBLY_LINKED_LIST*/
